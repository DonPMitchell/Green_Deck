#include "card.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NV 11
#define NCONST 2
#define LIMIT 4

extern int bDebugging;

static int nTrials = 1000;
static int vec[NV];
static int gradient[NV];
static Card rgcd[NV];

CardList
GreenDeckSetup(int deckVector[NV])
{
	CardList cl;
	Game g;
	int i, j, nCards;

	rgcd[0] = Card("Forest");
	rgcd[1] = Card("Armageddon");
	rgcd[2] = Card("Black Lotus");
	rgcd[3] = Card("Sol Ring");
	rgcd[4] = Card("Juggernaut");
	rgcd[5] = Card("Mana Vault");
	rgcd[6] = Card("Clockwork Beast");
	rgcd[7] = Card("Llanowar Elf");
	rgcd[8] = Card("Wild Growth");
	rgcd[9] = Card("Orcish Lumberjack");
	rgcd[10] = Card("Kaysa");
	rgcd[11] = Card("Concordant Crossroads");
	rgcd[12] = Card("Grizzly Bear");
	rgcd[13] = Card("Erhnam Djinn");
	rgcd[14] = Card("Aspect Of Wolf");
	rgcd[15] = Card("Woolly Mammoths");
	rgcd[16] = Card("Balduvian Bears");
	rgcd[17] = Card("Balduvian Hordes");
	rgcd[18] = Card("War Mammoth");
	rgcd[19] = Card("Fyndhorn Elf");
	rgcd[20] = Card("Elvish Archer");
	rgcd[21] = Card("Timber Wolves");
	rgcd[22] = Card("Craw Wurm");

	for (nCards = 0, j = 1; j < NV; j++)
		nCards += deckVector[j];
	deckVector[0] = 60 - nCards;
	for (j = 0; j < NV; j++) {
		for (i = 0; i < deckVector[j]; i++)
			cl.Insert(rgcd[j]);
	}
	return cl;
}

CardList
RedBlackDeckSetup(int deckVector[NV])
{
	CardList cl;
	Game g;
	int i, j, nCards;

	rgcd[0] = Card("Forest");
	rgcd[1] = Card("Armageddon");	// various spells
	rgcd[10] = Card("Crusade");		// bad moon
	rgcd[2] = Card("White Knight");	// black knight
	rgcd[3] = Card("War Mammoth");	// bog wraith
	rgcd[4] = Card("Dark Ritual");
	rgcd[5] = Card("Mesa Pegasus");	// drudge skeletons
	rgcd[6] = Card("Hypnotic Spector");
	rgcd[7] = Card("Serra Angel");	// sengir vampire
	rgcd[8] = Card("Unholy Strength");
	rgcd[9] = Card("Will O The Wisp");

	for (nCards = 0, j = 1; j < NV; j++)
		nCards += deckVector[j];
	deckVector[0] = 60 - nCards;
	for (j = 0; j < NV; j++) {
		for (i = 0; i < deckVector[j]; i++)
			cl.Insert(rgcd[j]);
	}
	return cl;
}

CardList
AlphaDeckSetup(int deckVector[NV])
{
	CardList cl;
	Game g;
	int i, j, nCards;

	rgcd[0] = Card("Forest");
	rgcd[1] = Card("Mox");
	rgcd[2] = Card("Black Lotus");
	rgcd[3] = Card("Sol Ring");
	rgcd[4] = Card("Berserk");
	rgcd[5] = Card("Mana Vault");
	rgcd[6] = Card("Clockwork Beast");
	rgcd[7] = Card("Llanowar Elf");
	rgcd[8] = Card("Wild Growth");
	rgcd[9] = Card("Craw Wurm");
	rgcd[10] = Card("Giant Growth");
	rgcd[11] = Card("Giant Spider");
	rgcd[12] = Card("Grizzly Bear");
	rgcd[13] = Card("Instill Energy");
	rgcd[14] = Card("Hurricane");
	rgcd[15] = Card("War Mammoth");
	rgcd[16] = Card("Force Of Nature");
	rgcd[17] = Card("Scryb Sprites");
	rgcd[18] = Card("Aspect Of Wolf");
	rgcd[19] = Card("Timber Wolves");
	rgcd[20] = Card("Shanodin Dryads");
	rgcd[21] = Card("Jade Statue");
	rgcd[22] = Card("Birds Of Paradise");
	rgcd[23] = Card("Juggernaut");

	for (nCards = 0, j = 1; j < NV; j++)
		nCards += deckVector[j];
	deckVector[0] = 60 - nCards;
	for (j = 0; j < NV; j++) {
		for (i = 0; i < deckVector[j]; i++)
			cl.Insert(rgcd[j]);
	}
	return cl;
}

CardList
WhiteDeckSetup(int deckVector[NV])
{
	CardList cl;
	Game g;
	int i, j, nCards;

	rgcd[0] = Card("Plains");
	rgcd[1] = Card("Mox");
	rgcd[2] = Card("Black Lotus");
	rgcd[3] = Card("Sol Ring");
	rgcd[4] = Card("Armageddon");
	rgcd[5] = Card("Mana Vault");
	rgcd[6] = Card("Clockwork Beast");
	rgcd[7] = Card("Serra Angel");
	rgcd[8] = Card("Crusade");
	rgcd[9] = Card("Savannah Lions");
	rgcd[10] = Card("Benalish Hero");
	rgcd[11] = Card("Mesa Pegasus");
	rgcd[12] = Card("White Knight");
	rgcd[13] = Card("Pearled Unicorn");
	rgcd[14] = Card("Holy Strength");
	rgcd[15] = Card("Howling Mine");
	rgcd[16] = Card("Call To Arms");
	rgcd[17] = Card("Kjeldoran Warrior");
	rgcd[18] = Card("Icatian Infantry");
	rgcd[19] = Card("Icatian Javelineers");
	rgcd[20] = Card("Tundra Wolves");
	rgcd[21] = Card("Order Of Leitbur");
	rgcd[22] = Card("Order Of The White Shield");
	rgcd[23] = Card("Angelic Voices");

	for (nCards = 0, j = 1; j < NV; j++)
		nCards += deckVector[j];
	deckVector[0] = 60 - nCards;
	for (j = 0; j < NV; j++) {
		for (i = 0; i < deckVector[j]; i++)
			cl.Insert(rgcd[j]);
	}
	return cl;
}

int
KillSpeed(int deckVector[NV])
{
	CardList cl;
	Game g;
	int i, nTurns, nTotalTurns;

	cl = RedBlackDeckSetup(deckVector);
	bDebugging = 0;
	nTotalTurns = 0;
	for (i = 0; i < nTrials; i++) {
		nTurns = 0;
		g.NewGame(cl);
		while (g.TakeTurn())
			nTurns++;
		nTotalTurns += nTurns*nTurns;	// minimize square kill speed
	}
	return nTotalTurns;
}

void
Optimize()
{
	int i, nGrad, nBase;
	double fSpeed;

	nBase = KillSpeed(vec);
	fSpeed = sqrt((double)nBase / (double)nTrials);
	printf("RMS KILL SPEED: %f\n", fSpeed);
	for (i = 0; i < NCONST; i++)
		printf("%2d %s\n", vec[i], rgcd[i].szName);
	for (i = NCONST; i < NV; i++) {
		vec[i] += 1;
		nGrad = KillSpeed(vec);
		vec[i] -= 1;
		gradient[i] = nGrad - nBase;
		if (gradient[i] > 0 && vec[i] > 0) {
			vec[i] -= 1;
			nGrad = KillSpeed(vec);
			vec[i] += 1;
			gradient[i] = nBase - nGrad;
		}
		printf("%2d %s, (grad = %d)\n", vec[i], rgcd[i].szName, gradient[i]);
	}
}

void
Descend()
{
	int i, iMax, nMaxGrad;

	nMaxGrad = 0;
	iMax = -1;
	for (i = NCONST; i < NV; i++) {
		if (gradient[i] < 0) {
			if (vec[i] >= LIMIT)
				continue;
			if (-gradient[i] > nMaxGrad) {
				nMaxGrad = -gradient[i];
				iMax = i;
			}
		} else {
			if (vec[i] <= 0)
				continue;
			if (gradient[i] > nMaxGrad) {
				nMaxGrad = gradient[i];
				iMax = i;
			}
		}
	}
	// tweek the most effective card count
	if (iMax == -1)
		exit(0);
	if (gradient[iMax] < 0)
		vec[iMax]++;
	else
		vec[iMax]--;
	printf("Changing %s to %d\n", rgcd[iMax].szName, vec[iMax]);
}

void
Initialize()
{
	int i;
	static unsigned seed;

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

	vec[1] = 12;		// Mox Emerald (Armagedon)
	vec[2] = 0;		// Black Lotus
	vec[3] = 0;		// Sol Ring
	vec[4] = 0;		// Juggernaut
	vec[5] = 0;		// Mana Vault
	for (i = NCONST; i < NV; i++) {
		seed = 1103515245*seed + 12345;
		vec[i] = 1 + ((seed >> 16) + (seed << 16)) % 3;
	}
}

void
Respond()
{
}

int
main(int argc, char *argv[])
{
	CardList cl;
	Card cd;
	Game g;
	int i, nMana, nDamage, nTurns;

	Initialize();
	if (argc > 1) {
		nTrials = 200;
		for (i = 0; i < 40; i++) {
			Optimize();
			Descend();
		}
		nTrials = 1000;
		for (i = 0; i < 30; i++) {
			Optimize();
			Descend();
		}
		nTrials = 10000;
		for (i = 0; i < 20; i++) {
			Optimize();
			Descend();
		}
		nTrials = 100000;
		for (;;) {
			Optimize();
			Descend();
		}
		return 0;
	}
	bDebugging = 1;
	nMana = 0;
	nDamage = 0;
	nTurns = 0;
	cl = RedBlackDeckSetup(vec);
	// printf("%d\n", cl.LoadCards("deck.txt"));
	for (i = 0; i < 1; i++) {
		g.NewGame(cl);
		if (bDebugging) g.clHand.Print();
		while (g.TakeTurn())
			nTurns++;

	}
	printf("%d turns to kill out of %d trials\n", nTurns, i);
	return 0;
}
