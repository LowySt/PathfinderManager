//-------------------------//
//    FUNGAL ARCHETYPE     //
//-------------------------//

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
    U"Micotico"_W, fungalGS, fungalAS, fungalAC, fungalSenses,
    archetypeRDStub, archetypeResistanceStub, archetypeRIStub, fungalSpecAtk,
    archetypeSizeStub, fungalMelee, archetypeAlignStub, fungalType,
    fungalSubType, fungalDV, archetypeSTStub, fungalDefCap, fungalSpeed,
    fungalImmunities, archetypeBABStub, archetypeSkillsStub, archetypeTalentsStub, 
    archetypeEnvStub, archetypeOrgStub, archetypeTreasureStub, fungalSpecQual, fungalSpecCap,
    fungalLang, archetypeAuraStub, archetypeWeakStub,
};



//-------------------------//
//  EXOSKELETON ARCHETYPE  //
//-------------------------//

b32 exoskeletonGS(s32 hitDice, s32 *gsDiff, s32 *rmDiff)
{
    //NOTE: Because this creature sets the gs, rather than modify it, we report the index in the gsSet
    switch(hitDice)
    {
        case 1:  { *gsDiff = 2; } break;
        
        case 2:  { *gsDiff = 4; } break;
        
        case 3:
        case 4:  { *gsDiff = 5; } break;
        
        case 5:
        case 6:  { *gsDiff = 6; } break;
        
        case 7:
        case 8:  { *gsDiff = 7; } break;
        
        case 9:
        case 10: { *gsDiff = 8; } break;
        
        case 11:
        case 12: { *gsDiff = 9; } break;
        
        case 13:
        case 14:
        case 15: { *gsDiff = 10; } break;
        
        case 16:
        case 17: { *gsDiff = 11; } break;
        
        case 18:
        case 19:
        case 20: { *gsDiff = 12; } break;
        
        case 21:
        case 22:
        case 23:
        case 24: { *gsDiff = 13; } break;
        
        case 25:
        case 26:
        case 27:
        case 28: { *gsDiff = 14; } break;
    }
    
    return TRUE;
}

void exoskeletonAS(s32 as[AS_COUNT])
{
    as[AS_STR] += 2;
    as[AS_CON] = AS_NO_VALUE;
    as[AS_INT] = AS_NO_VALUE;
    as[AS_WIS] = 10;
    as[AS_CHA] = 10;
    return;
}

b32 exoskeletonAC(utf32 *size, s32 ac[AC_TYPES_COUNT])
{
    AssertNonNull(size);
    
    for(s32 i = 0; i < AC_TYPES_COUNT; i++)
    { ac[i] = -99; }
    
    if(ls_utf32AreEqual(*size, U"Colossale"_W))         { ac[AC_NATURAL] = 11; }
    else if(ls_utf32AreEqual(*size, U"Mastodontica"_W)) { ac[AC_NATURAL] = 7; }
    else if(ls_utf32AreEqual(*size, U"Enorme"_W))       { ac[AC_NATURAL] = 4; }
    else if(ls_utf32AreEqual(*size, U"Grande"_W))       { ac[AC_NATURAL] = 3; }
    else if(ls_utf32AreEqual(*size, U"Media"_W))        { ac[AC_NATURAL] = 2; }
    else if(ls_utf32AreEqual(*size, U"Piccola"_W))      { ac[AC_NATURAL] = 1; }
    else                                                { ac[AC_NATURAL] = 0; }
    
    return TRUE;
}

void exoskeletonRD(s32 hitDice, utf32 *old)
{
    AssertNonNull(old);
    
    CompendiumPrependStringIfMissing(old, U"RD 5/contundente"_W, U"RD 5/contundente, "_W);
    return;
}

b32 exoskeletonSpecAtk(utf32 *spec)
{
    AssertNonNull(spec);
    
    const utf32 note = U"N.B. Perde tutti gli attacchi speciali associati alla biologia vivente"_W;
    const utf32 note2 = U"N.B. Perde tutti gli attacchi speciali associati alla biologia vivente, "_W;
    
    CompendiumPrependStringIfMissing(spec, note, note2);
    
    return FALSE;
}

void exoskeletonMelee(CachedPageEntry *page, utf32 *melee)
{
    AssertNonNull(page);
    AssertNonNull(melee);
    
    //TODO: Only obtains slam attack if it doesn't have any natural attacks
    
    s32 attackBonus = page->BABval + page->modAS[AS_STR] - 10;
    s32 damageBonus = page->modAS[AS_STR] - 10;
    if(melee->len == 0) { damageBonus *= 1.5; } //NOTE: If only attack
    
    if(melee->len > 0) { ls_utf32Append(melee, U", schianto "_W); }
    else               { ls_utf32Append(melee, U"schianto  "_W); }
    
    if(attackBonus >= 0)      { ls_utf32AppendChar(melee, '+'); }
    ls_utf32AppendInt(melee, attackBonus);
    
    ls_utf32Append(melee, U" ("_W);
    
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Minuta")))       { ls_utf32Append(melee, U"1d2"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Minuscola")))    { ls_utf32Append(melee, U"1d3"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Piccola")))      { ls_utf32Append(melee, U"1d4"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Media")))        { ls_utf32Append(melee, U"1d6"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Grande")))       { ls_utf32Append(melee, U"1d8"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Enorme")))       { ls_utf32Append(melee, U"2d8"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Mastodontica"))) { ls_utf32Append(melee, U"2d8"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Colossale")))    { ls_utf32Append(melee, U"3d6"_W); }
    
    if(damageBonus >= 0)      { ls_utf32AppendChar(melee, '+'); }
    ls_utf32AppendInt(melee, damageBonus);
    
    ls_utf32AppendChar(melee, ')');
    
    return;
}

b32 exoskeletonAlign(utf32 *align)
{
    AssertNonNull(align);
    
    ls_utf32Clear(align);
    ls_utf32Append(align, U"NM"_W);
    
    return FALSE;
}

void exoskeletonType(utf32 *type)
{
    AssertNonNull(type);
    
    ls_utf32Clear(type);
    ls_utf32Append(type, U"Non Morto");
    
    return;
}

void exoskeletonDV(CachedPageEntry *page, utf32 *oldType, u64 *DV)
{
    s32 dvDiff = 0;
    
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Piccola")))      { dvDiff += 1;  }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Media")))        { dvDiff += 1;  }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Grande")))       { dvDiff += 2;  }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Enorme")))       { dvDiff += 4;  }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Mastodontica"))) { dvDiff += 6;  }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Colossale")))    { dvDiff += 10; }
    
    u64 newDV = ChangeRacialDVCount(*DV, dvDiff);
    *DV = newDV;
    
    return;
}

b32 exoskeletonST(s32 totalDV, s32 st[ST_COUNT])
{
    st[ST_CON] = totalDV / 3;
    st[ST_DEX] = totalDV / 3;
    st[ST_WIS] = (totalDV / 2) + 2;
    return TRUE;
}

void exoskeletonDefCap(utf32 *old)
{
    AssertNonNull(old);
    
    ls_utf32Clear(old);
    ls_utf32Append(old, U"Tratti dei Non Morti"_W);
    
    return;
}

void exoskeletonSpeed(utf32 *old)
{
    AssertNonNull(old);
    
    //TODO: Because maneuverability goes down, the bonus to the flight ability changes.
    //      Zombies loose all abilities, so I'll leave it to the GM to calculate it if necessary
    //      But showing it properly would be better.
    const s32 manuvCount = 8;
    const utf32 patterns[manuvCount] = { U"(scarsa)"_W, U"(media)"_W, U"(buona)"_W, U"(perfetta)"_W,
        U"(Scarsa)"_W, U"(Media)"_W, U"(Buona)"_W, U"(Perfetta)"_W,
    };
    utf32 replacement = U"(maldestra)"_W;
    
    for(s32 i = 0; i < manuvCount; i++)
    {
        if(old->len - patterns[i].len + replacement.len >= old->size)
        { AssertMsg(FALSE, "speed string not large enough\n"); return; }
        
        if(ls_utf32Replace(old, patterns[i], replacement) == TRUE) { return; }
    }
    
    return;
}

void exoskeletonBAB(utf32 *old, s32 dv)
{
    AssertNonNull(old);
    
    ls_utf32Clear(old);
    
    s32 bab = (s32)(((f64)dv)*0.75);
    if(bab > 0) { ls_utf32Append(old, U"+"); }
    ls_utf32AppendInt(old, bab);
    
    return;
}

u32 exoskeletonSkills(u32 skillEntry)
{
    //NOTE: Exoskeletons loose all abilities.
    return 0;
}

void exoskeletonTalents(CachedPageEntry *page)
{
    AssertNonNull(page);
    
    for(s32 i = 0; i < 24; i++)
    { ls_utf32Clear(page->talents + i); }
    
    u32 packed = GetTalentPackedFromName(U"Robustezza"_W);
    
    if(packed != TALENT_NOT_FOUND)
    {
        BuildTalentFromPacked_t(&compendium.codex, packed, &page->talents[0]);
        page->talentEntry[0] = packed;
    }
    
    return;
}

void exoskeletonSpecQual(utf32 *old)
{
    AssertNonNull(old);
    
    CompendiumPrependStringIfMissing(old, U"Esplosione"_W, U"Esplosione, "_W);
    
    return;
}

void exoskeletonSpecCap(utf32 *old)
{
    AssertNonNull(old);
    
    const utf32 note = U"N.B. Perde tutti gli attacchi speciali associati alla biologia vivente\n\n"_W;
    
    const utf32 explosion = U"Esplosione (Str)\nQuando un esoscheletro viene distrutto, "
        "il suo guscio dissecato esplode rilasciando i resti polverosi delle visceri "
        "dell'esoscheletro nell'aria circostante. Qualsiasi creatura adiacente ad un esoscheletro "
        "quando questi scoppia deve superare un Tiro Salvezza su Tempra o diventa Barcollante per "
        "1 round mentre tossisce ed annaspa. Creature che non hanno bisogno di respirare sono "
        "immuni a questo effetto. Se l'esoscheletro possiede 10 o più Dadi Vita, la vittima \U000000E8 "
        "invece Nauseata per 1 round. La CD del Tiro Salvezza \U000000E8 pari a 10 + met\U000000E0 dei Dadi Vita "
        "dell'esoscheletro + modificatore di Carisma.\n\n"_W;
    
    CompendiumPrependStringIfMissing(old, explosion, explosion);
    
    CompendiumPrependStringIfMissing(old, note, note);
    
    return;
}


ArchetypeDiff ExoskeletonCreature = {
    U"Esoscheletro"_W, exoskeletonGS, exoskeletonAS, exoskeletonAC, archetypeSensesStub,
    exoskeletonRD, archetypeResistanceStub, archetypeRIStub, exoskeletonSpecAtk,
    archetypeSizeStub, exoskeletonMelee, exoskeletonAlign, exoskeletonType,
    archetypeSubTypeStub, exoskeletonDV, exoskeletonST, exoskeletonDefCap, exoskeletonSpeed,
    archetypeImmunitiesStub, exoskeletonBAB, exoskeletonSkills, exoskeletonTalents, 
    archetypeEnvStub, archetypeOrgStub, archetypeTreasureStub, exoskeletonSpecQual, exoskeletonSpecCap,
    archetypeLangStub, archetypeAuraStub, archetypeWeakStub,
};