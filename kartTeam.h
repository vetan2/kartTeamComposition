#pragma once
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

class Player;
class Track;

int matchTotal;		// match �Լ��� ȣ��Ǵ� �� Ƚ��
int matchCmpl = 0;	// matchCompletion : match �Լ��� ȣ��� Ƚ��

class Player {
	public:
		char name[20];
		double* trackRec;	// �� ����
		double* trackT;		// trackT[i] : Ʈ�� i�� T����
		double T;			// trackT�� ���

		void input(int trackCnt);
		void calcT(Track* track, int trackCnt);
};

class Track {
	public:
		char name[20];	// ��Ī
		double avg;		// �������� ��� ���
		double std;		// �������� ��� ǥ������

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