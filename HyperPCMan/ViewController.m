//
//  ViewController.m
//  HyperPCMan
//
//  Created by Lowy on 13/11/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import "ViewController.h"

@implementation ViewController

- (void)resetMobs {
    for(int i = 0; i < MOB_SIZE; i++) {
        [Mobs[i] hide];
        [Mobs[i] setName:EnemyNames[i]];
        [Mobs[i]->Box->Box setStringValue:@"0"];
        [Mobs[i]->Init setStringValue:@""];
    }
}

- (void)resetAllies {
    for(int i = 0; i < ALLY_SIZE; i++) {
        [Allies[i] hide];
        [Allies[i] setName:AllyNames[i]];
        [Allies[i]->Box->Box setStringValue:@"0"];
        [Allies[i]->Init setStringValue:@""];
    }
}

- (void)resetOrder {
    orderNum = PARTY_SIZE;
    for(int i = 0; i < ORDER_SIZE; i++) {
        if(i < PARTY_SIZE) { [Order[i] show]; }
        else { [Order[i] hide]; }
        [Order[i]->Name setStringValue:@""];
    }
}

- (void)showNOrder:(NSInteger)n {
    for(int i = 0; i < ORDER_SIZE; i++) {
        if(i < n) { [Order[i] show]; }
        else { [Order[i] hide]; }
    }
}

- (void)changePos:(NSInteger)p newPos:(NSInteger)newP {
    NSInteger num = orderNum;
    
    if(p == newP) { return; }
    if(newP > num) { return; }

    NSInteger currIdx = p - 1;
    NSInteger newIdx = currIdx - 1;
    NSInteger range;
    NSString *moved = [Order[currIdx]->Name stringValue];
    
    if(newP > p) {
        NSInteger untouched = newP - p - 1;
        range = num - untouched;
        if(currentTurnIdx <= (p-1) || currentTurnIdx >= (newP - 1)) {
            currentTurnIdx += 1;
            if(currentTurnIdx > num) { currentTurnIdx = 1; }
        }
    } else {
        range = p - newP + 1;
        if(currentTurnIdx <= (p-1) && currentTurnIdx >= (newP - 1)) {
            currentTurnIdx += 1;
            if(currentTurnIdx > num) { currentTurnIdx = 1; }
        }
    }
    
    for(NSInteger i = range; i > 0; i--) {
        if(newIdx < 0) { newIdx = num - 1; }
        [Order[currIdx]->Name setStringValue:[Order[newIdx]->Name stringValue]];
        if(currIdx - 1 < 0) { currIdx = num - 1; }
        else { currIdx -= 1; }
        newIdx = currIdx - 1;
    }
    [Order[newP-1]->Name setStringValue:moved];
   
    [CurrentInTurn setStringValue:[Order[currentTurnIdx]->Name stringValue]];
}

bool isToken(char c) {
    switch(c)
    {
        case '(': return true;
        case ')': return true;
        case 'd': return true;
        case '+': return true;
        case '-': return true;
        case '*': return true;
    }
    
    return false;
}

// ---------------------------------------- //
// -------------- DICE THROW -------------- //
// --------------            -------------- //

//#include <stdio.h>
//#include <string.h>
//#include <stdbool.h>
//#include <stdlib.h>

#define ASSERT(expr) \
    if (!(expr)) \
        aFailed(__FILE__, __LINE__)

void aFailed(char *file, int line) {
    printf("Assertion fired in file:%s line:%d\n", file, line);
}

#define OP_ARR_LEN 4
const char operators[OP_ARR_LEN] = {'d', '+', '*', '-'};

#define PA_ARR_LEN 2
const char parens[PA_ARR_LEN] = { '(', ')' };

enum TokenType
{
    TOKEN_BLOCK_START = 0x0000000,
    TOKEN_VAL         = 0x0000001,
    TOKEN_OPEN_PAREN  = 0x0000010,
    TOKEN_CLOSE_PAREN = 0x0000100,
    TOKEN_PLUS        = 0x0001000,
    TOKEN_MINUS       = 0x0010000,
    TOKEN_TIMES       = 0x0100000,
    TOKEN_DICE_THROW  = 0x1000000
};

char *tokenStr[] = {"BLOCK_START", "VAL", "OPEN_PAREN", "CLOSE_PAREN", "PLUS", "MINUS", "TIMES", "DICE_THROW"};

struct Token
{
    enum TokenType t;
    float val;
};

struct TokenList
{
    struct TokenList *next;
    struct TokenList *prev;
    struct Token t;
};

bool isOperator(char c)
{
    for(int i = 0; i < OP_ARR_LEN; i++)
    { if(c == operators[i]) { return true; } }

    return false;
}

bool isNum(char c)
{
    if((c <= '9') && (c >= '0')) { return true; }
    if(c == '.' ) { return true; }
    return false;
}

// 0x0000011 &
// 0x0000001 =
// 0x0000001

bool expectTok(char *v, int typeMask)
{
    if((TOKEN_VAL & typeMask) == TOKEN_VAL) {
        if(isNum(*v)) { return true; }
    }
    
    if((TOKEN_OPEN_PAREN & typeMask) == TOKEN_OPEN_PAREN) {
        if(*v == '(') { return true; }
    }
    
    return false;
}

struct TokenList *tokenize(const char *in, int len)
{
    struct TokenList *Result = calloc(1, sizeof(struct TokenList));
    Result->t.t = TOKEN_BLOCK_START;

    struct TokenList *curr = Result;
    char *At = (char *)in;
    while(len > 0)
    {
        //A Stream is made up of: values, operators, parens
        if(*At == '(')
        {
            int parenLen = 0;
            char *Bt = At+1;
            while(*Bt != ')') { if(parenLen >= (len-1)) { return 0x0; } Bt += 1; parenLen += 1; }
            struct Token open = {TOKEN_OPEN_PAREN, 0};
            struct Token close = {TOKEN_CLOSE_PAREN, 0};
            struct TokenList *ParenList = tokenize(At+1, parenLen);
            if(ParenList == 0x0) { return 0x0; }
            
            curr->next = calloc(1, sizeof(struct TokenList));
            curr->next->prev = curr;
            curr = curr->next;
            curr->t = open;
            ParenList->prev = curr; //NEED?
            curr->next = ParenList;
            while(curr->next) { curr = curr->next; }
            curr->next = calloc(1, sizeof(struct TokenList));
            curr->next->prev = curr;
            curr = curr->next;
            curr->t = close;
            At += (2 + parenLen);
            len -= (2 + parenLen);
        }
        else if(isOperator(*At))
        {
            struct Token op = {};
            bool res = true;
            switch(*At)
            {
                case 'd': { op.t = TOKEN_DICE_THROW; res = expectTok(At+1, TOKEN_VAL);               } break;
                case '+': { op.t = TOKEN_PLUS; res = expectTok(At+1, TOKEN_VAL | TOKEN_OPEN_PAREN);  } break;
                case '-': { op.t = TOKEN_MINUS; res = expectTok(At+1, TOKEN_VAL | TOKEN_OPEN_PAREN); } break;
                case '*': { op.t = TOKEN_TIMES; res = expectTok(At+1, TOKEN_VAL | TOKEN_OPEN_PAREN); } break;
            }
            
            if(res == false) { return 0x0; }
            
            curr->next = calloc(1, sizeof(struct TokenList));
            curr->next->prev = curr;
            curr = curr->next;
            curr->t = op;
            At += 1;
            len -= 1;
        }
        else
        {
            struct Token tok = {TOKEN_VAL, 0};
            int valLen = 0;
            char *Bt = At;
            while(isNum(*Bt)) { Bt += 1; valLen += 1; }
            if(valLen == 0) { return 0x0; } //Note this should be good.
            
            float v = atof(At);
            tok.val = v;
            curr->next = calloc(1, sizeof(struct TokenList));
            curr->next->prev = curr;
            curr = curr->next;
            curr->t = tok;
            At += valLen;
            len -= valLen;
        }
    }

    return Result;
}

struct TokenList *throwDie(struct TokenList *head)
{
    struct TokenList *prev = head->prev;
    struct TokenList *next = head->next;
    struct TokenList *curr = head;

    float newVal = 0;
    int numThrows = prev->t.val;
    int faces = next->t.val;
    for(int i = 0; i < numThrows; i++)
    {
        int throwVal = arc4random_uniform(faces)+1;
        newVal += throwVal;
        
    } //NOTE: This should be a random throw

    struct TokenList *pp = prev->prev;
    struct TokenList *dT = curr;

    struct Token newTok = {TOKEN_VAL, newVal};
    if(next->next != NULL)
    {
        curr = next->next;
        free(dT);
        free(prev);
        free(next);
    
        curr->prev = calloc(1, sizeof(struct TokenList));
        curr->prev->next = curr;
        curr->prev->t = newTok;
        if(pp != NULL) { pp->next = curr->prev; curr->prev->prev = pp; }
    }
    else
    {
        curr = next;
        free(dT);
        free(prev);

        curr->t = newTok;
        curr->prev = pp;
        if(pp != NULL) { pp->next = curr; }
    }
    
    return curr;
}

enum OpType
{
    OP_PLUS,
    OP_MINUS,
    OP_TIMES
};

float calc(struct TokenList *, struct TokenList *);

struct TokenList *doOp(struct TokenList *head, struct TokenList *tail, enum OpType opType)
{
    struct TokenList *curr = head;
    struct TokenList *prev = curr->prev;
    struct TokenList *next = curr->next;

    float nextVal = 0;
    float prevVal = prev->t.val;
    if(next->t.t == TOKEN_OPEN_PAREN)
    {
        nextVal = calc(next->next, tail);

        struct TokenList *newPrevLink = prev->prev;
        free(prev);
        while(curr->t.t != TOKEN_CLOSE_PAREN) { curr = curr->next; free(curr->prev); }
        if(curr->next != NULL) { curr = curr->next; free(curr->prev); }
        
        float resultVal = 0;
        switch(opType)
        {
            case OP_PLUS:  resultVal = prevVal + nextVal; break;
            case OP_MINUS: resultVal = prevVal - nextVal; break;
            case OP_TIMES: resultVal = prevVal * nextVal; break;
        }

        struct Token newTok = {TOKEN_VAL, resultVal};
        curr->prev = calloc(1, sizeof(struct TokenList));
        curr->prev->t = newTok;
        curr->prev->next = curr;
        curr->prev->prev = newPrevLink;
        newPrevLink->next = curr->prev;
        return curr;
    }
    else
    {
        ASSERT(next->t.t == TOKEN_VAL);
        if(next->next != NULL)
        {
            if(next->next->t.t == TOKEN_DICE_THROW)
            {
                //Throw dice and then sum.
                throwDie(next->next);
                next = curr->next;
                float newVal = next->t.val;
                float resultVal = 0;
                switch(opType)
                {
                    case OP_PLUS:  resultVal = prev->t.val + newVal; break;
                    case OP_MINUS: resultVal = prev->t.val - newVal; break;
                    case OP_TIMES: resultVal = prev->t.val * newVal; break;
                }
            
                struct TokenList *newPrevLink = prev->prev;

                struct Token newTok = {TOKEN_VAL, resultVal};
                if(next->next != NULL)
                {
                    curr = next->next;
                    free(prev->next);
                    free(prev);
                    free(next);

                    curr->prev = calloc(1, sizeof(struct TokenList));
                    curr->prev->t = newTok;
                    curr->prev->next = curr;
                    curr->prev->prev = newPrevLink;
                    if(newPrevLink != NULL) { newPrevLink->next = curr->prev; }
                }
                else
                {
                    curr = next;
                    free(prev->next);
                    free(prev);

                    curr->t = newTok;
                    curr->prev = newPrevLink;
                    if(newPrevLink != NULL) { newPrevLink->next = curr; }
                }

                return curr;
            }
        }
        
        //It's an operator.
        float resultVal = 0;
        switch(opType)
        {
            case OP_PLUS:  resultVal = prev->t.val + next->t.val; break;
            case OP_MINUS: resultVal = prev->t.val - next->t.val; break;
            case OP_TIMES: resultVal = prev->t.val * next->t.val; break;
        }

        struct Token newTok = {TOKEN_VAL, resultVal};
        struct TokenList *newPrevLink = prev->prev;
        curr = curr->next;
        free(prev->next);
        free(prev);
        curr->t = newTok;
        curr->prev = newPrevLink;
        newPrevLink->next = curr;
        return curr;
    }

    ASSERT(false);
    return 0;
}

void preCalcMult(struct TokenList *head, struct TokenList *tail)
{
    struct TokenList *curr = head;

    for(;;)
    {
        if(curr == NULL) { break; }
        if(curr->prev == tail) { break; }

        struct TokenList *next = curr->next;
        struct TokenList *prev = curr->prev;
        //@NOTE Should these be a thing?
        // ASSERT(next); ASSERT(prev);

        //Skip all parenthesis
        if(curr->t.t == TOKEN_OPEN_PAREN)
        {
            while(curr->t.t != TOKEN_CLOSE_PAREN)
            { curr = curr->next; ASSERT(curr != NULL); }
            curr = curr->next;
            continue;
        }

        if(curr->t.t == TOKEN_TIMES)
        {
            if(prev->t.t == TOKEN_VAL && next->t.t == TOKEN_VAL)
            {
                struct TokenList *times = curr;
                
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

                float newVal = prev->t.val * next->t.val;
                struct Token newTok = {TOKEN_VAL, newVal};
                struct TokenList *pp = prev->prev;
                curr = next;
                free(curr->prev);
                free(prev);
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

float calc(struct TokenList *head, struct TokenList *tail)
{
    struct TokenList *curr = head;

    //NOTE: First a Multiplication pass, because it has precedence.
    preCalcMult(head, tail);
        
    //NOTE: Second a Parenthesis pass, so that I can perform multiplication
    // Between blocks is doable.
    for(;;)
    {
        if(curr == NULL) {break;}
        struct TokenList *next = curr->next;
        struct TokenList *prev = curr->prev;

        if(curr->t.t == TOKEN_OPEN_PAREN)
        {
            struct TokenList *end = next;
            while(end->t.t != TOKEN_CLOSE_PAREN) { end = end->next; }

            float resultVal = calc(next, end);
            
            struct TokenList *newPrevLink = prev;
            while(curr->t.t != TOKEN_CLOSE_PAREN) { curr = curr->next; free(curr->prev); }

            struct Token newTok = {TOKEN_VAL, resultVal};
            if(curr->next != NULL) {
                curr = curr->next; free(curr->prev);
                curr->prev = calloc(1, sizeof(struct TokenList));
                curr->prev->t = newTok;
                curr->prev->next = curr;
                curr->prev->prev = newPrevLink;
                if(newPrevLink) { newPrevLink->next = curr->prev; }
            }
            else {
                curr->prev = calloc(1, sizeof(struct TokenList));
                curr->prev->t = newTok;
                curr->prev->next = curr->next;
                curr->prev->prev = newPrevLink;
                if(newPrevLink) { newPrevLink->next = curr->prev; }
                free(curr);
                
                //TODO: NOTE: Should I set curr to NULL or break out of the loop?
                break;
            }
        }
        else { curr = curr->next; }
    }

    curr = head;

    //NOTE: Third, another multiplication pass, to take into consideration the multiplications
    // between the values in the parentheses (The reason why the first mult pass can't be avoided
    // is because htere may be pure multiplications between elements inside parenthesis and
    // these have to be performed BEFORE other calculations inside a parenthesis.
    preCalcMult(head, tail);

    //NOTE: Finally complete the calculations.
    for(;;)
    {
        if(curr == NULL) { break; }
        struct TokenList *next = curr->next;
        struct TokenList *prev = curr->prev;

        switch(curr->t.t)
        {
            case TOKEN_DICE_THROW:
            {
                ASSERT(prev->t.t == TOKEN_VAL);
                ASSERT(next->t.t == TOKEN_VAL);

                curr = throwDie(curr);
            } break;
        
            case TOKEN_PLUS:  { curr = doOp(curr, tail, OP_PLUS); } break;
            case TOKEN_MINUS: { curr = doOp(curr, tail, OP_MINUS); } break;
            case TOKEN_TIMES: { curr = doOp(curr, tail, OP_TIMES); } break;
            
            case TOKEN_CLOSE_PAREN:    { goto END; } break;
            case TOKEN_BLOCK_START:    { curr = curr->next; } break;
            case TOKEN_VAL:    { curr = curr->next; } break;

            default: { ASSERT(false); }
        }
    }

    END:
    curr = head->next; //Skip BLOCK_START token;
    return curr->t.val;
}

- (float)calcThrow:(NSString *)s {
    
    const char *input = [s UTF8String];
    int inputLen = (int)[s length];
    
    struct TokenList *head = tokenize(input, inputLen);
    
    if(head == 0x0) { return 0.0f; }
    
    struct TokenList *tail = head;
    while(tail->next != NULL) { tail = tail->next; }

    float result = calc(head, tail);
    
    while(head != NULL) {
        struct TokenList *last = head;
        head = head->next;
        free(last);
    }
    
    return result;
}

- (void)viewDidLoad {
    [super viewDidLoad];

    // Do any additional setup after loading the view.
    
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}


@end
