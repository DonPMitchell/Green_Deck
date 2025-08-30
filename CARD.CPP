#include "card.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>

Card
CardList::Draw(int nthCard)
{
	int i;
	Card cd;

	--nthCard;	// base 1 indexing
	cd = rgcdCards[nthCard];
	--cCards;
	for (i = nthCard; i < cCards; i++)
		rgcdCards[i] = rgcdCards[i+1];
	return cd;
}

void
CardList::Shuffle()
{
    static unsigned long seed = 0;
    int i, j;
    Card cd;

    if (seed == 0) {
		seed = time(0);
        seed = 1103515245*seed + 12345;
        seed = 1103515245*seed + 12345;
        seed = 1103515245*seed + 12345;
        seed = ((seed >> 16) + (seed << 16));
        seed = 1103515245*seed + 12345;
        seed = 1103515245*seed + 12345;
        seed = 1103515245*seed + 12345;
	}
    for (i = cCards; i > 1; --i) {
        seed = 1103515245*seed + 12345;
        j = ((seed >> 16) + (seed << 16)) % i;
        cd = rgcdCards[i-1];
        rgcdCards[i-1] = rgcdCards[j];
        rgcdCards[j] = cd;
    }
}

//
//	return base 1 index or 0 if not a member
//
int
CardList::Member(char *szCard)
{
	int i;

	for (i = 0; i < cCards; i++)
		if (strcmp(szCard, rgcdCards[i].szName) == 0)
			return i + 1;
	return 0;
}

int
CardList::Count(char *szCard)
{
	int i, c;

	c = 0;
	for (i = 0; i < cCards; i++)
		if (strcmp(szCard, rgcdCards[i].szName) == 0)
			c++;
	return c;
}

int
CardList::CountType(int nType)
{
	int i, c;

	c = 0;
	for (i = 0; i < cCards; i++)
		if (rgcdCards[i].nType == nType)
			c++;
	return c;
}

int
CardList::UntappedMember(char *szCard)
{
	int i;

	for (i = 0; i < cCards; i++)
		if (!rgcdCards[i].bTapped && strcmp(szCard, rgcdCards[i].szName) == 0)
			return i + 1;
	return 0;
}
#define ISA(SZ) strcmp(sz, SZ) == 0

Card::Card(char *sz)
{
	nType = nPower = nToughness = nCost = nMana = 0;
	bTapped = 0;
	nInstill = 0;
	nUseInstill = 0;
	bCanAttack = 0;	// summoning sickness
	szName = sz;

	//
	//	green cards
	//
	if (ISA("Forest")) {
		nMana = 1;
		nType = LAND;
	} else if (ISA("Llanowar Elf")) {
		nMana = 1;
		nPower = nToughness = 1;
		nCost = 1;
		nType = CREATURE;
	} else if (ISA("Wild Growth")) {
		nMana = 0;
		nCost = 1;
		nType = ENCHANTMENT;
	} else if (ISA("Sol Ring")) {
		nMana = 2;
		nCost = 1;
		nType = ARTIFACT;
	} else if (ISA("Mana Vault")) {
		nMana = 3;
		nCost = 1;
		nType = ARTIFACT;
	} else if (ISA("Fellwar Stone")) {
		nCost = 2;
		nMana = 1;
		nType = ARTIFACT;
	} else if (ISA("Sol Grail")) {
		nCost = 3;
		nMana = 1;
		nType = ARTIFACT;
	} else if (ISA("Mox")) {
		nMana = 1;
		nType = ARTIFACT;
	} else if (ISA("Juggernaut")) {
		nCost = 4;
		nPower = 5;
		nToughness = 3;
		nType = CREATURE;
	} else if (ISA("Giant Growth")) {
		nCost = 1;
		nPower = 3;
		nType = INSTANT;
	} else if (ISA("Giant Spider")) {
		nCost = 4;
		nPower = 2;
		nToughness = 4;
		nType = CREATURE;
	} else if (ISA("Ironroot Treefolk")) {
		nCost = 5;
		nPower = 3;
		nToughness = 5;
		nType = CREATURE;
	} else if (ISA("Force Of Nature")) {
		nCost = 6;
		nPower = 8;
		nToughness = 8;
		nType = CREATURE;
	} else if (ISA("Clockwork Beast")) {
		nCost = 6;
		nPower = 7;
		nToughness = 4;
		nType = CREATURE;
	} else if (ISA("Erhnam Djinn")) {
		nCost = 4;
		nPower = 4;
		nToughness = 5;
		nType = CREATURE;
	} else if (ISA("Grizzly Bear")) {
		nCost = 2;
		nPower = 2;
		nToughness = 2;
		nType = CREATURE;
	} else if (ISA("Scryb Sprites")) {
		nCost = 1;
		nPower = 1;
		nToughness = 1;
		nType = CREATURE;
	} else if (ISA("War Mammoth")) {
		nCost = 4;
		nPower = 3;
		nToughness = 3;
		nType = CREATURE;
	} else if (ISA("Jade Statue")) {
		nCost = 4;
		nPower = 3;
		nToughness = 6;
		nType = CREATURE;
	} else if (ISA("Craw Wurm")) {
		nCost = 6;
		nPower = 6;
		nToughness = 4;
		nType = CREATURE;
	} else if (ISA("Elvish Archer")) {
		nCost = 2;
		nPower = 2;
		nToughness = 1;
		nType = CREATURE;
	} else if (ISA("Fyndhorn Elf")) {
		nMana = 1;
		nCost = 1;
		nPower = nToughness = 1;
		nType = CREATURE;
	} else if (ISA("Timber Wolves")) {
		nCost = nPower = nToughness = 1;
		nType = CREATURE;
	} else if (ISA("Shanodin Dryads")) {
		nCost = nPower = nToughness = 1;
		nType = CREATURE;
	} else if (ISA("Hungry Mist")) {
		nCost = 4;
		nPower = 6;
		nToughness = 2;
		nType = CREATURE;
	} else if (ISA("Balduvian Bears")) {
		nCost = 2;
		nPower = nToughness = 2;
		nType = CREATURE;
	} else if (ISA("Fyndhorn Elder")) {
		nCost = 3;
		nPower = nToughness = 1;
		nMana = 2;
		nType = CREATURE;
	} else if (ISA("Ghazban Ogre")) {
		nCost = 1;
		nPower = nToughness = 2;
		nType = CREATURE;
	} else if (ISA("Durkwood Boars")) {
		nCost = 5;
		nPower = nToughness = 4;
		nType = CREATURE;
	} else if (ISA("Argothian Pixies")) {
		nCost = 2;
		nPower = 2;
		nToughness = 1;
		nType = CREATURE;
	} else if (ISA("Chub Toad")) {
		nCost = 3;
		nPower = 3;		// only if blocked
		nToughness = 3;
		nType = CREATURE;
	} else if (ISA("Woolly Mammoths")) {
		nCost = 3;
		nPower = 3;
		nToughness = 2;
		nType = CREATURE;
	} else if (ISA("Instill Energy")) {
		nCost = 1;
		nType = ENCHANTMENT;
	} else if (ISA("Aspect Of Wolf")) {
		nCost = 2;
		nType = ENCHANTMENT;
	} else if (ISA("Concordant Crossroads")) {
		nCost = 1;
		nType = ENCHANTMENT;
	} else if (ISA("Fastbond")) {
		nCost = 1;
		nType = ENCHANTMENT;
	} else if (ISA("Stampede")) {
		nCost = 3;
		nType = INSTANT;
	} else if (ISA("Hurricane")) {
		nCost = 1;
		nType = INSTANT;
	} else if (ISA("Howling Mine")) {
		nCost = 2;
		nType = ARTIFACT;
	} else if (ISA("Birds Of Paradise")) {
		nCost = 1;
		nMana = 1;
		nPower = 0;
		nToughness = 1;
		nType = CREATURE;
	} else if (ISA("Kaysa")) {
		nCost = 5;
		nPower = 2 + 1;
		nToughness = 3 + 1;	// enchantes itself
		nType = CREATURE;
	} else if (ISA("Black Lotus")) {
		nCost = 0;
		nMana = 3;
		nType = ARTIFACT;
	} else if (ISA("Berserk")) {
		nCost = 1;
		nType = INSTANT;
	//
	//	white cards
	//
	} else if (ISA("Plains")) {
		nMana = 1;
		nType = LAND;
	} else if (ISA("Armageddon")) {
		nCost = 4;
		nType = INSTANT;
	} else if (ISA("Blessing")) {
		nCost = 2;
		nType = ENCHANTMENT;
	} else if (ISA("Crusade")) {
		nCost = 2;
		nType = ENCHANTMENT;
	} else if (ISA("Call To Arms")) {
		nCost = 2;
		nType = ENCHANTMENT;
	} else if (ISA("Angelic Voices")) {
		nCost = 4;
		nType = ENCHANTMENT;
	} else if (ISA("Savannah Lions")) {
		nCost = 1;
		nPower = 2;
		nToughness = 1;
		nType = CREATURE;
	} else if (ISA("Serra Angel")) {
		nCost = 5;
		nPower = 4;
		nToughness = 4;
		nType = CREATURE;
	} else if (ISA("White Knight")) {
		nCost = 2;
		nPower = nToughness = 2;
		nType = CREATURE;
	} else if (ISA("Benalish Hero")) {
		nCost = 1;
		nPower = nToughness = 1;
		nType = CREATURE;
	} else if (ISA("Kjeldoran Warrior")) {
		nCost = 1;
		nPower = nToughness = 1;
		nType = CREATURE;
	} else if (ISA("Icatian Infantry")) {
		nCost = 1;
		nPower = nToughness = 1;
		nType = CREATURE;
	} else if (ISA("Icatian Javelineers")) {
		nCost = 1;
		nPower = nToughness = 1;
		nType = CREATURE;
	} else if (ISA("Tundra Wolves")) {
		nCost = 1;
		nPower = nToughness = 1;
		nType = CREATURE;
	} else if (ISA("Order Of Leitbur")) {
		nCost = 2;
		nPower = 2;
		nToughness = 1;
		nType = CREATURE;
	} else if (ISA("Order Of The White Shield")) {
		nCost = 2;
		nPower = 2;
		nToughness = 1;
		nType = CREATURE;
	} else if (ISA("Holy Strength")) {
		nCost = 1;
		nType = ENCHANTMENT;
	} else if (ISA("Mesa Pegasus")) {
		nCost = 2;
		nPower = nToughness = 1;
		nType = CREATURE;
	} else if (ISA("Pearled Unicorn")) {
		nCost = 3;
		nPower = nToughness = 2;
		nType = CREATURE;
	//
	//	Red/Black Cards
	//
	} else if (ISA("Sedge Troll")) {
		nCost = 3;
		nPower = nToughness = 3;
		nType = CREATURE;
	} else if (ISA("Lightning Bolt")) {
		nCost = 1;
		nType = INSTANT;
	} else if (ISA("Dark Ritual")) {
		nCost = 1;
		nMana = 3;
		nType = INSTANT;
	} else if (ISA("Unholy Strength")) {
		nCost = 1;
		nType = ENCHANTMENT;
	} else if (ISA("Hypnotic Spector")) {
		nCost = 3;
		nPower = nToughness = 2;
		nType = CREATURE;
	} else if (ISA("Fire Elemental")) {
		nCost = 5;
		nPower = 5;
		nToughness = 4;
		nType = CREATURE;
	} else if (ISA("Balduvian Hordes")) {
		nCost = 4;
		nPower = 5;
		nToughness = 5;
		nType = CREATURE;		
	} else if (ISA("Will O The Wisp")) {
		nCost = 1;
		nPower = 0;
		nToughness = 1;
		nType = CREATURE;
	} else if (ISA("Orcish Lumberjack")) {
		nCost = 1;
		nMana = 3;
		nPower = nToughness = 1;
		nType = CREATURE;
	//
	//	Blue Creatures
	//
	} else if (ISA("Lord Of Atlantis")) {
		nCost = 2;
		nPower = nToughness = 2;
		nType = CREATURE;
	} else if (ISA("Merfolk")) {
		nCost = 1;
		nPower = nToughness = 1;
		nType = CREATURE;
	} else if (ISA("Vodalian Soldiers")) {
		nCost = 2;
		nPower = 1;
		nToughness = 2;
		nType = CREATURE;
	} else if (ISA("Vodalian Knight")) {
		nCost = 3;
		nPower = nToughness = 2;
		nType = CREATURE;
	} else if (ISA("River Merfolk")) {
		nCost = 2;
		nPower = 2;
		nToughness = 1;
		nType = CREATURE;
	} else {
		printf("Illegal card %s\n", szName);
		assert(0);
	}
}

int
CardList::LoadCards(char *szFileName)
{
	FILE *pf;
	char buffer[128], *psz;
	int nCopies, iStart, nLen, i;
	Card cd;

	cCards = 0;
	pf = fopen(szFileName, "r");
	if (!pf)
		return 0;
	while (fgets(buffer, sizeof(buffer), pf)) {
		if (buffer[0] == '/')
			continue;
		iStart = 0;
		nCopies = 0;
		while (isdigit(buffer[iStart])) {
			nCopies = 10*nCopies + buffer[iStart] - '0';
			iStart++;
		}
		if (nCopies == 0)
			nCopies = 1;
		while (isspace(buffer[iStart]))
			iStart++;
		nLen = strlen(buffer);
		while (isspace(buffer[--nLen]))
			buffer[nLen] = 0;
		nLen = strlen(buffer + iStart);
		if (nLen == 0)
			continue;
		psz = new char[nLen];
		strcpy(psz, buffer + iStart);
		cd = Card(psz);
		for (i = 0; i < nCopies; i++)
			Insert(cd);
	}
	fclose(pf);
	return cCards;
}

void
CardList::Print()
{
	int i;

	for (i = cCards - 1; i >= 0; --i)
		puts(rgcdCards[i].szName);
}



