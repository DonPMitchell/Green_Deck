//
//	cards
//
#ifndef CARD_H
#define CARD_H

#include <string.h>
#define LAND		1
#define CREATURE	2
#define ENCHANTMENT	3
#define INSTANT		4
#define ARTIFACT	5

class Card {
public:
				Card()		{}
				Card(char *sz);

	int			operator ==(char *szCard) { return strcmp(szName, szCard) == 0; }
	char		bTapped;
	char		bCanAttack;
	char		nType;
	char		nPower;
	char		nMana;
	char		nToughness;
	char		nCost;
	char		nInstill;
	char		nUseInstill;
	char		*szName;
};

class CardList {
public:
				CardList()	{ cCards = 0; }

	int			cCards;
	Card		rgcdCards[128];
	int			Member(char *szCard);	// return base 1 index or 0
	int			UntappedMember(char *szCard);
	int			Count(char *szCard);
	int			CountType(int nType);
	Card&		operator [](int i)	{ return rgcdCards[i - 1]; }	// base 1
	void		Shuffle();
	Card		Draw()				{ return rgcdCards[--cCards]; }
	Card		Draw(int nthCard);		// base 1 indexing
	Card		Insert(Card cd)		{ return rgcdCards[cCards++] = cd; }
	int			LoadCards(char *szFileName);
	void		Print();
};

#endif

