#pragma once
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

class Player;
class Track;
class Team;

class Player {
	public:
		char name[20];
		double* trackRec;
		double* trackZ;
		double Z;

		void input(int trackCnt);
		void calcZ(Track* track, int trackCnt);
};

class Track {
	public:
		char name[20];	// ¾àÄª
		double avg;
		double std;

		void getTrackStat(Player* player, int playerCnt, int trackNum);
};

class Team {
	public:
		Player member[4];

		double matchA(Team oppTeam, int matchCnt);
		double matchB(Team oppTeam);
};

class Composition {
	public:
		Team* team;
};

void sortPlayer(Player* player, int playerCnt);		// Bubble Sort
Composition* getAllComp(Player* player, int playerCnt);
double calcCompScore(Composition comp, int playerCnt);
Composition getOptComp(Player* player, int playerCnt);
void printRec(double rec);
void printResult(Player* player, int playerCnt, Track* track, int trackCnt, Composition comp);