//
//	State of game
//
#ifndef GAME_H
#define GAME_H

#include "card.h"

class Game {
public:
				Game()	{}
	
	void		NewGame(CardList clDeck);
	int			TakeTurn();
	void		UnTapPhase();
	void		UpKeepPhase();
	void		DrawPhase();
	void		MainPhase();
	void		AttackSubPhase();
	void		DiscardPhase();
	int			PlayPerminant(char *szCard);
	int			PlayInstant(char *szCard);
	int			PlayInstill();
	int			AvailableMana();
	int			AllAvailableMana();
	int			TapForMana(char *szCard);
	int			GatherMana(int nNeeded);
	void		IncreaseMana();
	void		SummonCreatures();
	void		Hurricane(int bDoIt);

	int			nDamageDealt;
	int			nAvailableMana;	// what could be tapped
	int			nManaPool;		// what's actually in the pool now
	int			nLife;
	int			bConcordant;	// Concordant Crossroads in play
	int			bFastbond;		// Fastbond in play
	int			nHowling;		// Howling Mine
	int			nAspect;		// Aspect of Wolf
	int			nHoly;			// Holy Strength
	int			nCrusade;		// Crusade
	int			nTurn;			// which turn
	CardList	clLibrary;
	CardList	clGraveYard;
	CardList	clHand;
	CardList	clInPlay;
};

#endif

