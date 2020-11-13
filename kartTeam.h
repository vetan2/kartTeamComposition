#pragma once
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

class Player;
class Track;

int matchTotal;		// match 함수가 호출되는 총 횟수
int matchCmpl = 0;	// matchCompletion : match 함수가 호출된 횟수

class Player {
	public:
		char name[20];
		double* trackRec;	// 초 단위
		double* trackT;		// trackT[i] : 트랙 i의 T점수
		double T;			// trackT의 평균

		void input(int trackCnt);
		void calcT(Track* track, int trackCnt);
};

class Track {
	public:
		char name[20];	// 약칭
		double avg;		// 선수들의 기록 평균
		double std;		// 선수들의 기록 표준편차

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

		double calcScore(int playerCnt);
};

void errorExit(const char* s);
void sortPlayer(Player* player, int playerCnt);
Composition* getAllComp(Player* player, int playerCnt);
Composition getOptComp(Player* player, int playerCnt);
void writeRec(FILE* fp, double rec);
void writeResult(FILE* fp, Player* player, int playerCnt, Track* track, int trackCnt, Composition comp);