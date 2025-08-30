#include "game.h"
#include <string.h>
#include <stdio.h>

#define PRINTF if (bDebugging) printf
int bDebugging;

void
Game::NewGame(CardList clDeck)
{
	int i;
muligan:
	clLibrary = clDeck;
	clLibrary.Shuffle();
	clHand.cCards = 0;
	for (i = 0; i < 7 && clLibrary.cCards; i++)
		clHand.Insert(clLibrary.Draw());
	if (!clHand.CountType(LAND)) {
		PRINTF("Muligan\n");
		goto muligan;				// no land
	}
	clGraveYard.cCards = 0;
	clInPlay.cCards = 0;
	nLife = 20;
	nDamageDealt = 0;
	bConcordant = 0;
	nAvailableMana = 0;
	bFastbond = 0;
	nHowling = 0;
	nAspect = 0;
	nHoly = 0;
	nCrusade = 0;
	nTurn = 0;
	nManaPool = 0;
	PRINTF("NEW GAME\n");
}

int
Game::TakeTurn()
{
	PRINTF("NEXT TURN:\n");
	nTurn++;
	if (clLibrary.cCards < 1 || nDamageDealt >= 30 || nLife <= 0) {
		PRINTF("End of game\n");
		return 0;
	}
	nManaPool = 0;
	UnTapPhase();
	UpKeepPhase();
	DrawPhase();
	MainPhase();
	DiscardPhase();
	return 1;
}

void
Game::UnTapPhase()
{
	int i;

	// Mana Vaults don't untap unless 4 is paid during upkeep
	nManaPool = 0;
	for (i = 1; i <= clInPlay.cCards; i++) {
		if (clInPlay[i].bTapped
		&& strcmp(clInPlay[i].szName, "Mana Vault") != 0) {
			PRINTF("Untapping %s\n", clInPlay[i].szName);
			clInPlay[i].bTapped = 0;
			clInPlay[i].nUseInstill = clInPlay[i].nInstill;
		}
	}
}

void
Game::UpKeepPhase()
{
	nManaPool = 0;
	//couldn't summon them if there wasn't enough for upkeep,
	//but should really destroy Mist if cannot upkeep
	GatherMana(4*clInPlay.Count("Force Of Nature"));
	GatherMana(2*clInPlay.Count("Hungry Mist"));
}

void
Game::DrawPhase()
{
	Card cd;
	int i;

	// nManaPool = 0;
	for (i = 0; i < nHowling + 1; i++)
		if (clLibrary.cCards) {
			clHand.Insert(cd = clLibrary.Draw());
			PRINTF("Drawing %s\n", cd.szName);
		}
}

void
Game::DiscardPhase()
{
	Card cd;
	int i;

	nManaPool = 0;
	if (clHand.cCards > 7) {
		for (i = 1; i <= clHand.cCards; i++)
			if (clHand[i].nType == INSTANT) {
				clGraveYard.Insert(cd = clHand.Draw(i));
				PRINTF("Discarding %s\n", cd.szName);
			}
	}	
	if (clHand.cCards > 7) {
		for (i = 1; i <= clHand.cCards; i++)
			if (clHand[i].nType == INSTANT) {
				clGraveYard.Insert(cd = clHand.Draw(i));
				PRINTF("Discarding %s\n", cd.szName);
			}
	}	
	while (clHand.cCards > 7) {
		clGraveYard.Insert(cd = clHand.Draw());
		PRINTF("Discarding %s\n", cd.szName);
	}
}

//
//	support functions for main phase
//

int
Game::PlayPerminant(char *szCard)
{
	int iCard, nCost, iTarget, i;
	Card cd;

	iCard = clHand.Member(szCard);
	if (iCard)
		nCost = clHand[iCard].nCost;
	else
		return 0;
	if (nCost >= 4)
		AllAvailableMana();	// count Mana Vaults too
	else
		AvailableMana();
	if (iCard && nAvailableMana >= nCost) {
		if (clHand[iCard].nType == CREATURE && !bConcordant)
			clHand[iCard].bTapped = 1;	// simulate summoning sickness
		if (clHand[iCard].nType == CREATURE) {
			clHand[iCard].nPower += nCrusade;
			clHand[iCard].nToughness += nCrusade;
		}
		if (clHand[iCard] == "Force Of Nature" && AvailableMana() < 4)
			return 0;	// not enough to pay its upkeep
		if (clHand[iCard] == "Balduvian Hordes" && clHand.cCards <= 1)
			return 0;	// cannot pay discard cost
		if (clHand[iCard] == "Concordant Crossroads")
			bConcordant = 1;
		if (clHand[iCard] == "Fastbond")
			bFastbond = 1;
		if (clHand[iCard] == "Howling Mine")
			nHowling++;
		if (clHand[iCard] == "Crusade" || clHand[iCard] == "Call To Arms"
			|| clHand[iCard] == "Angelic Voices" || clHand[iCard] == "Kaysa") {
			nCrusade++;
			for (i = 1; i <= clInPlay.cCards; i++)
				if (clInPlay[i].nType == CREATURE) {
					clInPlay[i].nPower++;
					clInPlay[i].nToughness++;
			}
		}
		//REVIEW: Do Instill Energy like this too
		if (clHand[iCard] == "Wild Growth") {
			if (!clInPlay.Member("Forest"))
				return 0;		// no target forest
		}
		if (clHand[iCard] == "Instill Energy")
			if (!PlayInstill())
				return 0;
		if (clHand[iCard] == "Aspect Of Wolf") {
			if (clInPlay.CountType(CREATURE) == 0)
				return 0;
			else
				nAspect++;
		}
		if (clHand[iCard] == "Holy Strength") {
			if (clInPlay.CountType(CREATURE) == 0)
				return 0;
			else
				nHoly++;
		}
		if (clHand[iCard] == "Unholy Strength") {
			if (clInPlay.CountType(CREATURE) == 0)
				return 0;
			else
				nHoly += 2;
		}
		GatherMana(nCost);
		if (strcmp(szCard, "Wild Growth") == 0) {
			// growth an untapped forest preferably
			if (!(iTarget = clInPlay.UntappedMember("Forest")))
				iTarget = clInPlay.Member("Forest");
			clInPlay[iTarget].nMana += 1;
		}
		clInPlay.Insert(clHand.Draw(iCard));
		if (strcmp(szCard, "Balduvian Hordes") == 0) {
			clHand.Shuffle();
			clGraveYard.Insert(cd = clHand.Draw());
			PRINTF("Discarding %s to pay for Hordes\n", cd.szName);
		}
		// AvailableMana();
		PRINTF("Playing %s\n", szCard);
		PRINTF("%d in mana pool, %d available\n", nManaPool, nAvailableMana);
		return iCard;
	} else {
		AvailableMana();	// don't leave Vaults in nAvailableMana
		return 0;
	}
}

int
Game::PlayInstant(char *szCard)
{
	int iCard, nCost;

	AvailableMana();
	if ((iCard = clHand.Member(szCard))
	&& nAvailableMana >= (nCost = clHand[iCard].nCost)) {
		if (clHand[iCard] == "Lightning Bolt")
			nDamageDealt += 3;
		GatherMana(nCost);
		clGraveYard.Insert(clHand.Draw(iCard));
		// AvailableMana();
		PRINTF("Casting %s\n", szCard);
		return iCard;
	} else
		return 0;
}

//
//	card will be put in play.  just look for target to
//	bump nInstill and nUseInstill;
//
int
Game::PlayInstill()
{
	int iTarget, iTappedTarget, nPower, nTappedPower, i;

	PRINTF("Trying Instill Energy\n");
	iTarget = iTappedTarget = 0;
	nPower = nTappedPower = 0;
	for (i = 1; i <= clInPlay.cCards; i++) {
		if (clInPlay[i].nType == CREATURE) {
			if (!clInPlay[i].bTapped && clInPlay[i].nMana > nPower) {
				nPower = clInPlay[i].nMana;
				iTarget = i;
			}
			if (clInPlay[i].bTapped && clInPlay[i].nPower > nTappedPower) {
				nTappedPower = clInPlay[i].nPower;
				iTappedTarget = i;
			}
		}
	}
	if (iTappedTarget) {
		clInPlay[iTappedTarget].bTapped = 0;
		clInPlay[iTappedTarget].nInstill++;
		PRINTF("Instilling %s\n", clInPlay[iTappedTarget].szName);
		return 1;
	}
	if (iTarget && nPower) {
		clInPlay[iTarget].nInstill++;
		clInPlay[iTarget].nUseInstill++;
		PRINTF("Instilling mana generator %s\n", clInPlay[iTarget].szName);
		return 1;
	}
	return 0;
}

int
Game::AvailableMana()
{
	int i, cForests;
	Card cd;

	nAvailableMana = nManaPool;
	cForests = clInPlay.Count("Forest");
	for (i = 1; i <= clInPlay.cCards; i++) {
		cd = clInPlay[i];
		if (cd == "Orcish Lumberjack")
			if (cForests == 0)
				continue;
			else {
				cForests--;
				nAvailableMana--;	//REVIEW: just for untapped
			}
		if (!cd.bTapped && !(cd == "Mana Vault") && !(cd == "Dark Ritual"))
			nAvailableMana += cd.nMana * (1 + cd.nUseInstill);
	}
	return nAvailableMana;
}

int
Game::AllAvailableMana()
{
	int i, cForests;
	Card cd;

	nAvailableMana = nManaPool;
	cForests = clInPlay.CountType(LAND);
	for (i = 1; i <= clInPlay.cCards; i++) {
		cd = clInPlay[i];
		if (cd == "Orcish Lumberjack")
			if (cForests == 0)
				continue;
			else {
				cForests--;	// have to sacrifice one to get it
				nAvailableMana--;	//REVIEW: just for untapped
			}
		if (!cd.bTapped)
			nAvailableMana += cd.nMana * (1 + cd.nUseInstill);
	}
	return nAvailableMana;
}

int
Game::TapForMana(char *szCard)
{
	int iCard, nMana;

	if (!(iCard = clInPlay.UntappedMember(szCard)))
		return 0;	
	clInPlay[iCard].bTapped = 1;
	if (clInPlay[iCard].nUseInstill) {	// Instill Energy
		clInPlay[iCard].bTapped = 0;
		clInPlay[iCard].nUseInstill--;
	}
	if (strcmp(szCard, "Orcish Lumberjack") == 0 && clInPlay.CountType(LAND) == 0)
		return 0;
	nMana = clInPlay[iCard].nMana;
	PRINTF("Tapping %s for %d mana (%d in pool)\n", szCard, nMana, nManaPool);
	if (strcmp(szCard, "Black Lotus") == 0 || strcmp(szCard, "Dark Ritual") == 0)
		clGraveYard.Insert(clInPlay.Draw(iCard));	// discard Lotus
	if (strcmp(szCard, "Orcish Lumberjack") == 0) {
		if (!(iCard = clInPlay.UntappedMember("Forest")))
			iCard = clInPlay.Member("Forest");
		clGraveYard.Insert(clInPlay.Draw(iCard));	// discard a forest
		PRINTF("Sacrificing a forest to lumberjack\n");
	}
	nManaPool += nMana;
	return nMana;
}

//
//	generate mana for pool wisely, spare Lotus or Vaults if possible
//
int
Game::GatherMana(int nNeeded)
{
	int nMana, nWasNeeded;

	AvailableMana();
	if (nManaPool > nNeeded) {
		nManaPool -= nNeeded;
		return 1;
	}
	if (nManaPool <= nNeeded) {
		nNeeded -= nManaPool;
		nManaPool = 0;
	}
	if (nNeeded <= 0)
		return 1;
	nWasNeeded = nNeeded;
	//REVIEW: won't tape vault now if there was some mana in pool
	while (nNeeded >= 3 && TapForMana("Dark Ritual"))
		nNeeded -= 3;
	while (nNeeded >= 4 && TapForMana("Mana Vault"))
		nNeeded -= 3;
	while (nNeeded >= 3 && TapForMana("Orcish Lumberjack"))
		nNeeded -= 3;
	if (nNeeded >= 3)
		nNeeded -= TapForMana("Black Lotus");
	if (nNeeded >= 2)
		nNeeded -= TapForMana("Sol Ring");
	while (nNeeded > 0 && (nMana = TapForMana("Forest"))) 
		nNeeded -= nMana;
	while (nNeeded > 0 && (nMana = TapForMana("Plains"))) 
		nNeeded -= nMana;
	while (nNeeded > 0 && TapForMana("Mox"))
		nNeeded -= 1;
	while (nNeeded > 0 && TapForMana("Birds Of Paradise"))
		nNeeded -= 1;
	while (nNeeded > 0 && TapForMana("Llanowar Elf"))
		nNeeded -= 1;
	while (nNeeded > 0 && TapForMana("Fyndhorn Elf"))
		nNeeded -= 1;
//	while (nNeeded > 0 && TapForMana("Mana Vault"))
//		nNeeded -= 3;
	if (nNeeded > 0)
		nNeeded -= TapForMana("Black Lotus");
	// at least enough mana should be in the pool now
	nManaPool -= nWasNeeded;
	return nNeeded <= 0;
}

//
//	try to cast perminants that increase mana supply
//
void
Game::IncreaseMana()
{
	AvailableMana();
	if (!bFastbond)
		PlayPerminant("Fastbond");
	if (!bConcordant)
		PlayPerminant("Concordant Crossroads");
	if (bConcordant || nAvailableMana == 1) {
		while (PlayPerminant("Llanowar Elf"))
			;
		while (PlayPerminant("Fyndhorn Elf"))
			;
		while (PlayPerminant("Orcish Lumberjack"))
			;
		while (PlayPerminant("Birds Of Paradise"))
			;
	}
	while (PlayPerminant("Wild Growth"))
		;
	PlayPerminant("Sol Ring");
	while (PlayPerminant("Mana Vault"))
		;
	while (PlayPerminant("Dark Ritual"))	// approximate by Mana Vault
		;
}

//
//	try to cast the most efficient creatures
//
void
Game::SummonCreatures()
{
	// AvailableMana();
	while (PlayPerminant("Force Of Nature"))
		;
	while (PlayPerminant("Clockwork Beast"))
		;
	while (PlayPerminant("Craw Wurm"))
		;
	while (PlayPerminant("Kaysa"))
		;
	while (PlayPerminant("Fire Elemental"))
		;
	while (PlayPerminant("Hungry Mist"))
		;
	while (PlayPerminant("Balduvian Hordes"))
		;
	while (PlayPerminant("Juggernaut"))
		;
	while (PlayPerminant("Erhnam Djinn"))
		;
	PlayPerminant("Howling Mine");
	while (PlayPerminant("Serra Angel"))
		;
	while (PlayPerminant("Angelic Voices"))
		;
	while (PlayPerminant("Durkwood Boars"))
		;
	while (PlayPerminant("War Mammoth"))
		;
	while (PlayPerminant("Jade Statue"))
		;
	while (PlayPerminant("Sedge Troll"))
		;
	while (PlayPerminant("Hypnotic Spector"))
		;
	while (PlayPerminant("Woolly Mammoths"))
		;
	while (PlayPerminant("Chub Toad"))
		;
	while (PlayPerminant("Ironroot Treefolk"))
		;
	while (PlayPerminant("Fyndhorn Elder"))
		;
	if (clInPlay.CountType(LAND) >= 3)
		while (PlayPerminant("Aspect Of Wolf"))
			;
	while (PlayPerminant("Balduvian Bears"))
		;
	while (PlayPerminant("Grizzly Bear"))
		;
	while (PlayPerminant("Elvish Archer"))
		;
	while (PlayPerminant("Argothian Pixies"))
		;
	while (PlayPerminant("Crusade") || PlayPerminant("Call To Arms"))
		;
	while (PlayPerminant("White Knight"))
		;
	while (PlayPerminant("Order Of Leitbur"))
		;
	while (PlayPerminant("Order Of The White Shield"))
		;
	while (PlayPerminant("Ghazban Ogre"))
		;
	while (AvailableMana() > 1 && PlayPerminant("Instill Energy"))
		;
	while (PlayPerminant("Unholy Strength"))
		;
	while (PlayPerminant("Savannah Lions"))
		;
	while (PlayPerminant("Benalish Hero"))
		;
	while (PlayPerminant("Kjeldoran Warrior"))
		;
	while (PlayPerminant("Icatian Infantry"))
		;
	while (PlayPerminant("Icatian Javelineers"))
		;
	while (PlayPerminant("Tundra Wolves"))
		;
	while (PlayPerminant("Holy Strength"))
		;
	while (PlayPerminant("Pearled Unicorn"))
		;
	while (PlayPerminant("Mesa Pegasus"))
		;
	while (PlayPerminant("Orcish Lumberjack"))
		;
	while (PlayPerminant("Fyndhorn Elf"))
		;
	while (PlayPerminant("Llanowar Elf"))
		;
	while (PlayPerminant("Birds Of Paradise"))
		;
	while (PlayPerminant("Scryb Sprites"))
		;
	while (PlayPerminant("Timber Wolves"))
		;
	while (PlayPerminant("Shanodin Dryads"))
		;
	while (PlayPerminant("Will O The Wisp"))
		;
	while (PlayPerminant("Aspect Of Wolf"))
		;
	while (PlayPerminant("Instill Energy"))
		;
	while (PlayInstant("Lightning Bolt"))
		;
}

void
Game::AttackSubPhase()
{
	int i, nDamage, cAttackers, nForests;

	nDamage = 0;
	// nManaPool = 0;
	cAttackers = 0;
	for (i = 1; i <= clInPlay.cCards; i++)
		if (clInPlay[i].nType == CREATURE && !clInPlay[i].bTapped) {
			if (clInPlay[i] == "Jade Statue") {
				if (AvailableMana() < 2)
					continue;	// must activate to make it a creature
				else
					GatherMana(2);
			}
			clInPlay[i].bTapped = 1;
			if (clInPlay[i].nUseInstill) {
				clInPlay[i].bTapped = 0;	//REVIEW: after attack
				clInPlay[i].nUseInstill--;
			}
			nDamage += clInPlay[i].nPower;
			cAttackers++;
			PRINTF("Attacking with %s\n", clInPlay[i].szName);
			if (clInPlay[i] == "Clockwork Beast") {
				clInPlay[i].nPower -= 1;
				if (clInPlay[i].nPower < 0)
					clInPlay[i].nPower = 0;
			}
		}
	if (cAttackers >= 3 && PlayInstant("Stampede"))
		nDamage += cAttackers;
	if (cAttackers)
		while (PlayInstant("Giant Growth"))
			nDamage += 3;
	nForests = clInPlay.Count("Forest")/2;
	PRINTF("%d damage from Aspect Of Wolf\n", (nForests)*nAspect);
	nDamage += (nForests)*nAspect;		// aspect of wolf
	nDamage += nHoly;					// holy strength
	nDamageDealt += nDamage;
	PRINTF("%d damage from attack. Total %d\n", nDamage, nDamageDealt);

}

//
//	after a few experimental heuristics, this only casts a
//	hurricane if it can do a coup de grace this turn
//
void
Game::Hurricane(int bDoIt)
{
	int nLifeRemaining;

	AvailableMana();
	nLifeRemaining = 20 - nDamageDealt;
	if ((bDoIt || nAvailableMana + 1 >= nLifeRemaining)
	  && PlayInstant("Hurricane")) {
		nDamageDealt += AvailableMana();
		PRINTF("Hurricane for %d damage\n", nAvailableMana);
		GatherMana(nAvailableMana);
	}
}

void
Game::MainPhase()
{
	//
	//	play free cards
	//
	// AvailableMana();
	// nManaPool = 0;
	PlayPerminant("Forest");		// one per turn
	if (bFastbond)
		while (PlayPerminant("Forest"))
			;
	PlayPerminant("Plains");		// one per turn
	if (bFastbond)
		while (PlayPerminant("Plains"))
			;
	PlayPerminant("Black Lotus");	// only one in deck
	while (PlayPerminant("Mox"))	// dump as many as you are holding
		;
	IncreaseMana();
	if (bConcordant) {
		SummonCreatures();	// can do immediate creature damage
		AttackSubPhase();
		if (nTurn >= 3 && AvailableMana() > 3)
			Hurricane(1);
	} else {
		// if (clHand.Member("Instill Energy"))
			SummonCreatures();
		AttackSubPhase();
		Hurricane(0);		// try for an immediate kill
		// SummonCreatures();
	}
	PRINTF("%d mana available\n", AvailableMana());
}