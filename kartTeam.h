#pragma once
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

class Player {
	public:
		char name[20];
		double* trackRec;	// �� ����
		double* trackT;		// trackT[i] : Ʈ�� i�� T����
		double T;			// trackT�� ���

		void input();
		void calcT();
};

class Track {
	public:
		int trackID;
		char name[20];	// ��Ī
		double avg;		// �������� ��� ���
		double std;		// �������� ��� ǥ������

		// ���� ����� Ÿ�Ӿ��� ��Ϻ��� �ִ� 1�д� 3�� ���̳��ٰ� �������� ��,
		// �̿� �����Ǵ� T������ �ִ� �϶� ��ġ
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

int matchCnt;		// match �Լ��� ȣ��Ǵ� �� Ƚ��
int matchCmpl = 0;	// matchCompletion : match �Լ��� ȣ��� Ƚ��

void errorExit(const char* s);
void sortPlayer();
Composition* getAllComp();
Composition getOptComp();
void writeRec(double rec);
void writeResult(Composition comp);