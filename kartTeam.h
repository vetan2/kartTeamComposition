#pragma once
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

class Player {
	public:
		char name[20];
		double* trackRec;	// 초 단위
		double* trackT;		// trackT[i] : 트랙 i의 T점수
		double T;			// trackT의 평균

		void input();
		void calcT();
};

class Track {
	public:
		int trackID;
		char name[20];	// 약칭
		double avg;		// 선수들의 기록 평균
		double std;		// 선수들의 기록 표준편차

		// 실전 기록이 타임어택 기록보다 최대 1분당 3초 차이난다고 가정했을 때,
		// 이에 대응되는 T점수의 최대 하락 수치
		double T_decline;

		void getTrackStat();
};

class Team {
	public:
		Player member[4];

		double matchA(Team oppTeam, int matchCnt, double T_decline);
		double matchB(Team oppTeam);
};

class Composition {
	public:
		Team* team;

		double calcScore();
};

FILE* fp;

Player* player;
int playerCnt, teamCnt;

Track* track;
int trackCnt;

int matchCnt;		// match 함수가 호출되는 총 횟수
int matchCmpl = 0;	// matchCompletion : match 함수가 호출된 횟수

void errorExit(const char* s);
void sortPlayer();
Composition* getAllComp();
Composition getOptComp();
void writeRec(double rec);
void writeResult(Composition comp);