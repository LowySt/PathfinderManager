//-------------------------//
//    FUNGAL ARCHETYPE     //
//-------------------------//

b32 fungalIsCompatible(CachedPageEntry *page)
{
    AssertNonNull(page);
    
    if(ls_utf32AreEqual(page->type, U"Non Morto"_W))     { return FALSE; }
    if(ls_utf32Contains(page->subtype, U"Incorporeo"_W)) { return FALSE; }
    
    //TODO: Must have CON be damageable (So probably constructs can't be turned into fungal?)
    
    return FALSE;
}

b32 fungalGS(s32 hitDice, s32 *gsDiff, s32 *rmDiff)
{ *gsDiff = 1; *rmDiff = 0; return FALSE; }

void fungalAS(s32 as[AS_COUNT])
{ 
    as[AS_STR] += 4;
    if(as[AS_DEX] < 3) { as[AS_DEX] = 1; } else { as[AS_DEX] -= 2; }
    as[AS_CON] += 4;
    
    return;
}

b32 fungalAC(utf32 *size, s32 ac[AC_TYPES_COUNT])
{
    ac[AC_NATURAL] += 2;
    return FALSE;
}

void fungalSenses(utf32 *old)
{ 
    AssertMsg(old, "Null senses string pointer\n");
    
    CompendiumPrependStringIfMissing(old, U"Scurovisione 18 m"_W, U"Scurovisione 18 m, "_W);
    
    return;
}

void fungalType(utf32 *type)
{
    AssertMsg(type, "Null type string pointer\n");
    
    ls_utf32Clear(type);
    ls_utf32Append(type, U"Vegetale");
    return;
}

void fungalSubType(utf32 *subtype)
{
    AssertMsg(subtype, "Null subtype string pointer\n");
    
    //TODO: This requires more work. It's shit.
    CompendiumPrependStringIfMissing(subtype, U"Potenziato"_W, U"Potenziato, "_W);
    
    return;
}

void fungalDV(CachedPageEntry *page, utf32 *oldType, u64 *DV)
{
    u64 newDV = ChangeRacialDVType(*DV, HP_Die_Face::HP_Die_D8);
    *DV = newDV;
    return;
}

void fungalDefCap(utf32 *old)
{ 
    AssertNonNull(old);
    CompendiumPrependStringIfMissing(old, U"Tratti dei Vegetali"_W, U"Tratti dei Vegetali, "_W);
    return;
}

void fungalSpeed(utf32 *old)
{
    AssertNonNull(old);
    
    //TODO: Decent way to change speed not done yet.
    
    return;
}

void fungalImmunities(utf32 *old)
{
    AssertMsg(old, "Null immunities string pointer\n");
    CompendiumPrependStringIfMissing(old, U"Malattie"_W, U"Malattie, "_W);
    return;
}

void fungalMelee(CachedPageEntry *page, utf32 *melee)
{
    AssertNonNull(page);
    AssertNonNull(melee);
    
    //TODO: No decent way to check if there's natural attacks, and add slam if there aren't.
    
    return;
}

b32 fungalSpecAtk(utf32 *spec)
{
    AssertNonNull(spec);
    
    CompendiumPrependStringIfMissing(spec, U"Creare Progenie"_W, U"Creare Progenie, "_W);
    CompendiumPrependStringIfMissing(spec, U"Nube di Spore Velenose"_W, U"Nube di Spore Velenose, "_W);
    CompendiumPrependStringIfMissing(spec, U"Spore Micotiche"_W, U"Spore Micotiche, "_W);
    CompendiumPrependStringIfMissing(spec, U"Sangue Velenoso"_W, U"Sangue Velenoso, "_W);
    CompendiumPrependStringIfMissing(spec, U"Sangue o Carne Micotica"_W, U"Sangue o Carne Micotica, "_W);
    
    return FALSE;
}

void fungalSpecQual(utf32 *old)
{
    AssertNonNull(old);
    
    CompendiumPrependStringIfMissing(old, U"Metabolismo Micotico"_W, U"Metabolismo Micotico, "_W);
    CompendiumPrependStringIfMissing(old, U"Ringiovanimento"_W, U"Ringiovanimento, "_W);
    
    return;
}

void fungalSpecCap(utf32 *old)
{
    AssertNonNull(old);
    
    const utf32 createChildren = U"Creare Progenie (Str)\nUna creatura uccisa dai danni "
        "a Costituzione causati dalla Nube di Spore Velenose della creatura micotica si "
        "trasforma in una Progenie Micotica nel giro di 24 ore. L'incantesimo Crescita "
        "Vegetale dimezza il tempo di trasformazione, mentre l'incantesimo Rimpicciolire "
        "Vegetali lo raddoppia. "
        "L'incantesimo Inaridire distrugge le spore e impedisce la trasformazione del "
        "cadavere, mentre gli incantesimi che rimuovono le malattie sono inefficaci contro "
        "le spore in crescita. Una volta che la creatura si \U000000E8 trasformata in una progenie "
        "micotica, il suo cadavere viene distrutto. Una progenie micotica riceve "
        "l'Archetipo Creatura Micotica, ma perde tutti i livelli di classe e i ricordi della "
        "creatura base da cui \U000000E8 stata generata. Se la creatura base ha 1 o meno Dadi Vita razziali "
        "e di norma ha dei livelli di classe, si usa la versione Combattente di livello 1 della "
        "creatura base. Una Progenie Micotica nasce come essere indipendente dalla creatura base "
        "e in possesso di tutte le conoscenze necessarie (incluso il linguaggio) per "
        "utilizzare le proprie capacit\U000000E0 e soppravvivere. Anche se non ha alcun legame "
        "particolare con la Creatura Micotica che l'ha creata, la nuova Creatura Micotica "
        "riconosce immediatamente altre Creature Micotiche come appartenenti alla sua stessa razza.\n\n"_W;
    
    const utf32 poisonSporeCloud = U"Nube di Spore Velenose (Str)\nUna volta al giorno, una "
        "Creatura Micotica pu\U000000F2 emettere una nube di spore soffocanti in un'area del raggio "
        "di 4,5 metri che permane nell'aria per 10 round. La nube funziona come un veleno a inalazione. "
        "Tutte le creature che respirano, quando entrano nell'area della nube, devono superare "
        "un Tiro Salvezza su Tempra per non inalare le spore. Le creature che restano nell'area "
        "della nube di spore devono continuare a effettuare Tiri Salvezza su Tempra. Due o pi\U000000F9 "
        "nubi di spore emesse da Creature Micotiche diverse impongono alle creature che si "
        "trovano nei punti in cui si sovrappongono di effettuare un Tiro Salvezza separato per "
        "ciascuna delle nubi che occupano l'area.\n\n"_W;
    
    const utf32 fungalSpores = U"Spore micotiche\nVeleno-inalazione;\nTS Tempra CD 10 + 1/2 "
        "dei Dadi Vita razziali della creatura micotica + il suo modificatore di Costituzione;\n"
        "frequenza 1/round per 6 round;\n"
        "effetti 1d2 danni a Costituzione e Affaticato per 1 minuto;\n"
        "cura 2 TS.\n\n"_W;
    
    const utf32 poisonBlood = U"Sangue Velenoso (Str)\nIl sangue e la carne di una Creatura Micotica "
        "sono veleni a ingestione. Tutte le creature che effettuano un attacco con morso contro "
        "una creatura micotica, ne divorano una o ne ingeriscono per qualsiasi ragione delle parti "
        "devono superare un Tiro Salvezza su Tempra per non essere avvelenate. Una Creatura Micotica "
        "pu\U0000000F2 prelevare il suo stesso sangue per ricavarne un veleno a ingestione che funziona nel "
        "modo descritto in precedenza. Il sangue della Creatura Micotica ha per\U000000F2 odore e sapore "
        "particolari ed estremamente sgradevoli, quindi la maggior parte delle creature intelligenti "
        "non manger\U000000E0 il cibo che lo contiene, a meno che il suo sapore non venga mascherato a dovere "
        "(con una prova di abilit\U000000E0 in Professione [Cuoco] con CD 15). La Creatura Micotica deve "
        "infliggersi almeno 1 danno per ottenere una dose piena di veleno e il sangue prelevato "
        "(o le parti del corpo amputate) mantengono il loro effetto tossico solo per 24 ore, a meno "
        "che non vengano mescolati con altro sangue micotico fresco.\n\n"_W;
    
    const utf32 fungalBloodOrFlesh = U"Sangue o carne micotica\nVeleno-ingestione;\n"
        "TS Tempra CD 10 + 1/2 dei Dadi Vita razziali della Creatura Micotica + il suo "
        "modificatore di Costituzione;\n"
        "frequenza 1/minuto per 6 minuti;\n"
        "effetti 1 danno a Forza e 1 danno a Destrezza e Nauseato per 1 minuto;\n"
        "cura 2 TS.\n\n"_W;
    
    const utf32 fungalMetabolism = U"Metabolismo Micotico (Str)\nLe Creature Micotiche respirano, "
        "ma non mangiano e non dormono nel senso convenzionale dei termini.\n\n"_W;
    
    const utf32 rejuvenation = U"Ringiovanimento (Str)\nUna Creatura Micotica ottiene tutto il "
        "nutrimento di cui ha bisogno dal contatto con la terra umida, ma deve ringiovanire con "
        "la stessa frequenza con cui gli umani dormono, e per altrettanto tempo. Finch\U000000E8 resta in "
        "contatto con terra umida all'aperto, una Creatura Micotica che riposa recupera punti "
        "ferita come se effettuasse un riposo completo a letto con cure a lungo termine "
        "(4 danni per Dado Vita per ogni giorno di riposo). Durante il Ringiovanimento, la "
        "Creatura Micotica pu\U000000F2 svolgere attivit\U000000E0 leggere, ma le attivit\U000000E0 faticose "
        "(combattere, correre o lanciare incantesimi) impediscono il recupero di punti ferita per "
        "quel giorno. Il riposo completo a letto non aumenta l'ammontare di punti ferita recuperati "
        "dalla Creatura Micotica grazie al Ringiovanimento.\n\n"_W;
    
    CompendiumPrependStringIfMissing(old, createChildren, createChildren);
    CompendiumPrependStringIfMissing(old, poisonSporeCloud, poisonSporeCloud);
    CompendiumPrependStringIfMissing(old, fungalSpores, fungalSpores);
    CompendiumPrependStringIfMissing(old, poisonBlood, poisonBlood);
    CompendiumPrependStringIfMissing(old, fungalBloodOrFlesh, fungalBloodOrFlesh);
    CompendiumPrependStringIfMissing(old, fungalMetabolism, fungalMetabolism);
    CompendiumPrependStringIfMissing(old, rejuvenation, rejuvenation);
    
    ls_utf32TrimWhitespaceRight(old);
    
    return;
}

void fungalLang(utf32 *old)
{
    AssertNonNull(old);
    
    //NOTE: Needs to be able to talk, before it receives new languages.
    if(old->len > 0) { CompendiumPrependStringIfMissing(old, U"Silvano"_W, U"Silvano, "_W); }
    
    return;
}

ArchetypeDiff FungalCreature = {
    U"Micotico"_W, fungalIsCompatible, 
    fungalGS, fungalAS, fungalAC, fungalSenses,
    archetypeRDStub, archetypeResistanceStub, archetypeRIStub, fungalSpecAtk,
    archetypeSizeStub, fungalMelee, archetypeAlignStub, fungalType,
    fungalSubType, fungalDV, archetypeSTStub, fungalDefCap, fungalSpeed,
    fungalImmunities, archetypeBABStub, archetypeSkillsStub, archetypeTalentsStub, 
    archetypeEnvStub, archetypeOrgStub, archetypeTreasureStub, fungalSpecQual, fungalSpecCap,
    fungalLang, archetypeAuraStub, archetypeWeakStub,
};
