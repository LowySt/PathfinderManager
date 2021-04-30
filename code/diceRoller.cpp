#include "lsCRT.h"

const u32 OP_ARR_LEN = 4;
const char operators[OP_ARR_LEN] = {'d', '+', '*', '-'};

const u32 PA_ARR_LEN = 2;
const char parens[PA_ARR_LEN] = { '(', ')' };

enum DiceRoller_TokenType
{
	TOKEN_BLOCK_START = 0,
	TOKEN_VAL,
	TOKEN_OPEN_PAREN,
	TOKEN_CLOSE_PAREN,
	TOKEN_PLUS,
	TOKEN_MINUS,
	TOKEN_TIMES,
	TOKEN_DICE_THROW
};

const char *tokenStr[] = {"BLOCK_START", "VAL", "OPEN_PAREN", "CLOSE_PAREN", "PLUS", "MINUS", "TIMES", "DICE_THROW"}; 

struct DiceRoller_Token
{
	enum DiceRoller_TokenType t;
    f32 val;
};

struct DiceRoller_TokenList
{
    DiceRoller_TokenList *next;
    DiceRoller_TokenList *prev;
    DiceRoller_Token t;
};

b32 isOperator(char c)
{
	for(int i = 0; i < OP_ARR_LEN; i++)
	{ if(c == operators[i]) { return TRUE; } }
    
	return FALSE;
}

b32 isNum(char c)
{
	if((c <= '9') && (c >= '0')) { return TRUE; }
	if(c == '.' ) { return TRUE; }
	return FALSE;
}

DiceRoller_TokenList *tokenize(const char *in, int len)
{
    DiceRoller_TokenList *Result = (DiceRoller_TokenList *)ls_alloc(sizeof(DiceRoller_TokenList));
    Result->t.t = TOKEN_BLOCK_START;
    
    DiceRoller_TokenList *curr = Result;
    char *At = (char *)in;
    while(len > 0)
    {
        //A Stream is made up of: values, operators, parens
        if(*At == '(')
        {
            int parenLen = 0;
            char *Bt = At+1;
            while(*Bt != ')') { if(parenLen >= (len-1)) { return 0x0; } Bt += 1; parenLen += 1; }
            DiceRoller_Token open = {TOKEN_OPEN_PAREN, 0};
            DiceRoller_Token close = {TOKEN_CLOSE_PAREN, 0};
            DiceRoller_TokenList *ParenList = tokenize(At+1, parenLen);
            if(ParenList == 0x0) { return 0x0; }
            
            curr->next = (DiceRoller_TokenList *)ls_alloc(sizeof(DiceRoller_TokenList));
            curr->next->prev = curr;
            curr = curr->next;
            curr->t = open;
            ParenList->prev = curr; //NEED?
            curr->next = ParenList;
            while(curr->next) { curr = curr->next; }
            curr->next = (DiceRoller_TokenList *)ls_alloc(sizeof(DiceRoller_TokenList));
            curr->next->prev = curr;
            curr = curr->next;
            curr->t = close;
            At += (2 + parenLen);
            len -= (2 + parenLen);
        }
        else if(isOperator(*At))
        {
            DiceRoller_Token op = {};
            switch(*At)
            {
                case 'd': { op.t = TOKEN_DICE_THROW; } break;
                case '+': { op.t = TOKEN_PLUS;       } break;
                case '-': { op.t = TOKEN_MINUS;      } break;
                case '*': { op.t = TOKEN_TIMES;      } break;
            }
            curr->next = (DiceRoller_TokenList *)ls_alloc(sizeof(DiceRoller_TokenList));
            curr->next->prev = curr;
            curr = curr->next;
            curr->t = op;
            At += 1;
            len -= 1;
        }
        else
        {
            DiceRoller_Token tok = {TOKEN_VAL, 0};
            u32 valLen = 0;
            char *Bt = At; 
            while(isNum(*Bt)) { Bt += 1; valLen += 1; }
            if(valLen == 0) { return 0x0; }
            
            f32 v = ls_atof(At, valLen);
            tok.val = v;
            curr->next = (DiceRoller_TokenList *)ls_alloc(sizeof(DiceRoller_TokenList));
            curr->next->prev = curr;
            curr = curr->next;
            curr->t = tok;
            At += valLen;
            len -= valLen;
        }
    }
    
    return Result;
}

DiceRoller_TokenList *throwDie(DiceRoller_TokenList *head)
{
    DiceRoller_TokenList *prev = head->prev;
    DiceRoller_TokenList *next = head->next;
    DiceRoller_TokenList *curr = head;
    
    f32 newVal = 0;
    s32 numThrows = prev->t.val;
    s32 faces = next->t.val;
	for(u32 i = 0; i < numThrows; i++) 
    { newVal += pcg32_bounded(&pcg32_global, faces) + 1; } 
    
	DiceRoller_TokenList *pp = prev->prev;
    DiceRoller_TokenList *dT = curr;
    
    DiceRoller_Token newTok = {TOKEN_VAL, newVal};
	if(next->next != NULL)
	{
		curr = next->next;
		ls_free(dT);
		ls_free(prev);
		ls_free(next);
        
		curr->prev = (DiceRoller_TokenList *)ls_alloc(sizeof(DiceRoller_TokenList));
		curr->prev->next = curr;
		curr->prev->t = newTok;
		if(pp != NULL) { pp->next = curr->prev; curr->prev->prev = pp; }
	}
	else 
	{
		curr = next;
		ls_free(dT);
		ls_free(prev);
        
		curr->t = newTok;
		curr->prev = pp;
		if(pp != NULL) { pp->next = curr; }
	}
	
	return curr;
}

enum DiceRoller_OpType
{
	OP_PLUS,
	OP_MINUS,
	OP_TIMES
};

f32 calc(DiceRoller_TokenList *, DiceRoller_TokenList *);

DiceRoller_TokenList *doOp(DiceRoller_TokenList *head, DiceRoller_TokenList *tail, DiceRoller_OpType opType)
{
    DiceRoller_TokenList *curr = head;
    DiceRoller_TokenList *prev = curr->prev;
    DiceRoller_TokenList *next = curr->next;
    
    f32 nextVal = 0;
    f32 prevVal = prev->t.val;
	if(next->t.t == TOKEN_OPEN_PAREN) 
	{
		nextVal = calc(next->next, tail);
        
        DiceRoller_TokenList *newPrevLink = prev->prev;
		ls_free(prev);
        
		while(curr->t.t != TOKEN_CLOSE_PAREN) { curr = curr->next; ls_free(curr->prev); }
		if(curr->next != NULL) { curr = curr->next; ls_free(curr->prev); }
		
        f32 resultVal = 0;
		switch(opType)
		{
			case OP_PLUS:  resultVal = prevVal + nextVal; break;
			case OP_MINUS: resultVal = prevVal - nextVal; break;
			case OP_TIMES: resultVal = prevVal * nextVal; break;
		}
        
        DiceRoller_Token newTok = {TOKEN_VAL, resultVal};
		curr->prev = (DiceRoller_TokenList *)ls_alloc(sizeof(DiceRoller_TokenList));
		curr->prev->t = newTok;
		curr->prev->next = curr;
		curr->prev->prev = newPrevLink;
		newPrevLink->next = curr->prev;
		return curr;
	}
	else 
	{
		Assert(next->t.t == TOKEN_VAL);
		if(next->next != NULL) 
		{
			if(next->next->t.t == TOKEN_DICE_THROW)
			{
				//Throw dice and then sum.
				throwDie(next->next);
				next = curr->next;
                f32 newVal = next->t.val;
                f32 resultVal = 0; 
				switch(opType) 
				{
					case OP_PLUS:  resultVal = prev->t.val + newVal; break;
					case OP_MINUS: resultVal = prev->t.val - newVal; break;
					case OP_TIMES: resultVal = prev->t.val * newVal; break;
				}
                
				DiceRoller_TokenList *newPrevLink = prev->prev;
                
				DiceRoller_Token newTok = {TOKEN_VAL, resultVal};
				if(next->next != NULL)
				{
					curr = next->next;
					ls_free(prev->next);
					ls_free(prev);
					ls_free(next);
                    
					curr->prev = (DiceRoller_TokenList *)ls_alloc(sizeof(DiceRoller_TokenList));
					curr->prev->t = newTok;
					curr->prev->next = curr;
					curr->prev->prev = newPrevLink;
					if(newPrevLink != NULL) { newPrevLink->next = curr->prev; }
				}
				else
				{
					curr = next;
					ls_free(prev->next);
					ls_free(prev);
                    
					curr->t = newTok;
					curr->prev = newPrevLink;
					if(newPrevLink != NULL) { newPrevLink->next = curr; }
				}
                
				return curr;
			}
		}
		
		//It's an operator.
        f32 resultVal = 0;
		switch(opType)
		{
			case OP_PLUS:  resultVal = prev->t.val + next->t.val; break;
			case OP_MINUS: resultVal = prev->t.val - next->t.val; break;
			case OP_TIMES: resultVal = prev->t.val * next->t.val; break;
		}
        
        DiceRoller_Token newTok = {TOKEN_VAL, resultVal};
        DiceRoller_TokenList *newPrevLink = prev->prev;
		curr = curr->next;
		ls_free(prev->next);
		ls_free(prev);
		curr->t = newTok;
		curr->prev = newPrevLink;
		newPrevLink->next = curr;
		return curr;
	}
    
	Assert(FALSE);
	return 0;
}

void preCalcMult(DiceRoller_TokenList *head, DiceRoller_TokenList *tail)
{
    DiceRoller_TokenList *curr = head;
    
	for(;;)
	{
		if(curr == NULL) { break; }
		if(curr->prev == tail) { break; }
        
        DiceRoller_TokenList *next = curr->next;
        DiceRoller_TokenList *prev = curr->prev;
		//@NOTE Should these be a thing?
		// Assert(next); Assert(prev);
        
		//Skip all parenthesis
		if(curr->t.t == TOKEN_OPEN_PAREN)
		{
			while(curr->t.t != TOKEN_CLOSE_PAREN)
			{ curr = curr->next; Assert(curr != NULL); }
			curr = curr->next;
			continue;
		}
        
		if(curr->t.t == TOKEN_TIMES)
		{
			if(prev->t.t == TOKEN_VAL && next->t.t == TOKEN_VAL)
			{
                DiceRoller_TokenList *times = curr;
				
				if(prev->prev)
				{
					if(prev->prev->t.t == TOKEN_DICE_THROW)
					{
						curr = throwDie(prev->prev);
						curr = times;
						prev = curr->prev;
						next = curr->next;
					}
				}
				if(next->next)
				{
					if(next->next->t.t == TOKEN_DICE_THROW)
					{
						curr = throwDie(next->next);
						curr = times;
						prev = curr->prev;
						next = curr->next;
					}
				}
                
                f32 newVal = prev->t.val * next->t.val;
                DiceRoller_Token newTok = {TOKEN_VAL, newVal};
                DiceRoller_TokenList *pp = prev->prev;
				curr = next;
				ls_free(curr->prev);
				ls_free(prev);
				curr->prev = pp;
				curr->t = newTok;
				if(pp) { pp->next = curr; }
				continue;
			}
			else { curr = curr->next; }
		}
		else { curr = curr->next; }
	}
}

f32 calc(DiceRoller_TokenList *head, DiceRoller_TokenList *tail)
{
	int Result = 0;
    DiceRoller_TokenList *curr = head;
    
	//NOTE: First a Multiplication pass, because it has precedence.
	preCalcMult(head, tail);
    
	//NOTE: Second a Parenthesis pass, so that multiplication
	// Between blocks is doable.
	for(;;)
	{
		if(curr == NULL) {break;}
        DiceRoller_TokenList *next = curr->next;
        DiceRoller_TokenList *prev = curr->prev;
        
		if(curr->t.t == TOKEN_OPEN_PAREN)
		{
            DiceRoller_TokenList *end = next;
			while(end->t.t != TOKEN_CLOSE_PAREN) { end = end->next; }
            
            f32 resultVal = calc(next, end);
			
            DiceRoller_TokenList *newPrevLink = prev;
            while(curr->t.t != TOKEN_CLOSE_PAREN) { curr = curr->next; ls_free(curr->prev); }
            
            DiceRoller_Token newTok = {TOKEN_VAL, resultVal};
            if(curr->next != NULL) {
                curr = curr->next; ls_free(curr->prev);
				curr->prev = (DiceRoller_TokenList *)ls_alloc(sizeof(DiceRoller_TokenList));
				curr->prev->t = newTok;
				curr->prev->next = curr;
				curr->prev->prev = newPrevLink;
				if(newPrevLink) { newPrevLink->next = curr->prev; }
            }
            else {
				curr->prev = (DiceRoller_TokenList *)ls_alloc(sizeof(DiceRoller_TokenList));
				curr->prev->t = newTok;
				curr->prev->next = curr->next;
				curr->prev->prev = newPrevLink;
				if(newPrevLink) { newPrevLink->next = curr->prev; }
				ls_free(curr);
                
				//TODO: NOTE: Should I set curr to NULL or just break?
				break;
			}
		}
		else { curr = curr->next; }
	}
    
	curr = head;
    
	//NOTE: Third, another multiplication pass, to take into consideration the multiplications
	// between the values in the parentheses (The reason why the first mult pass can't be avoided
	// is because there may be pure multiplications between elements inside parenthesis and
	// these have to be performed BEFORE other calculations inside a parenthesis.
	preCalcMult(head, tail);
    
	//NOTE: Finally complete the calculations.
	for(;;)
	{
		if(curr == NULL) { break; }
        DiceRoller_TokenList *next = curr->next;
        DiceRoller_TokenList *prev = curr->prev;
        
		switch(curr->t.t)
		{
			case TOKEN_DICE_THROW:
			{
				Assert(prev->t.t == TOKEN_VAL);
				Assert(next->t.t == TOKEN_VAL);
                
				curr = throwDie(curr);
			} break;
            
			case TOKEN_PLUS:        { curr = doOp(curr, tail, OP_PLUS); } break;
			case TOKEN_MINUS:       { curr = doOp(curr, tail, OP_MINUS); } break;
			case TOKEN_TIMES:       { curr = doOp(curr, tail, OP_TIMES); } break;
			
			case TOKEN_CLOSE_PAREN: { goto END; }         break;
			case TOKEN_BLOCK_START: { curr = curr->next; } break;
			case TOKEN_VAL:         { curr = curr->next; } break;
            
			default: { Assert(FALSE); }
		}
	}
    
	END:
	curr = head->next; //Skip BLOCK_START token;
	return curr->t.val;
}

f32 diceRoll(const char *input, u32 inputLen)
{
	//const char *input = "2*2d8+8d6+20*2+(1d8+4d6+11)*1.5";
	//const char *input = "(5d20+7)+1+3d8+2+(2d6+1)-(1*2)";
	//const char *input = "2d6+(1+3d6)";
	//const char *input = "(2d6+17)";
	if(inputLen == 0) { return 0.0f; }
    
    DiceRoller_TokenList *head = tokenize(input, inputLen);
    DiceRoller_TokenList *tail = head;
	while(tail->next != NULL) { tail = tail->next; }
    
    f32 result = calc(head, tail);
    return result;
    
#if 0
	printf("Result: %.1f\n", result);
    
	printf("\n");
	struct TokenList *curr = head;
	
	while(curr != NULL)
	{
		printf("%s -> ", tokenStr[curr->t.t]);
		curr = curr->next;
	}
	printf("\n\n");
	
	return 0;
#endif
}
