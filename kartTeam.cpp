#include "kartTeam.h"

int main() {
	FILE* fp;
	Player* player;
	Track* track;
	int playerCnt, trackCnt;
	int i;

	scanf("%d %d", &playerCnt, &trackCnt);
	if (playerCnt % 4 != 0)
		errorExit("������ ���� 4�� ����� �Ǿ�� �մϴ�.");

	fp = fopen("result.csv", "w");

	player = new Player[playerCnt];
	for (i = 0; i < playerCnt; i++) {
		player[i].trackRec = new double[trackCnt];
		player[i].trackT = new double[trackCnt];
	}

	track = new Track[trackCnt];

	for (i = 0; i < trackCnt; i++)
		scanf("%s", track[i].name);

	for (i = 0; i < playerCnt; i++)
		player[i].input(trackCnt);
	printf("\n");

	for (i = 0; i < trackCnt; i++)
		track[i].getTrackStat(player, playerCnt, i);

	for (i = 0; i < playerCnt; i++)
		player[i].calcT(track, trackCnt);
	sortPlayer(player, playerCnt);

	writeResult(fp, player, playerCnt, track, trackCnt, getOptComp(player, playerCnt));
	fclose(fp);
	printf("\n�� �� �Ϸ�! result.csv ������ Ȯ���ϼ���.\n\n");
	system("pause");
}

void errorExit(const char* s) {
	printf("%s\n\n", s);
	system("pause");
	exit(-1);
}

void Player::input(int trackCnt) {
	int i, n;
	
	scanf("%s", name);
	for (i = 0; i < trackCnt; i++) {
		scanf("%d", &n);
		trackRec[i] = (n / 10000) * 60 + n % 10000 / 100 + (double)(n % 100) * 0.01;
	}
}

void Player::calcT(Track* track, int trackCnt) {
	int i;

	T = 0;
	for (i = 0; i < trackCnt; i++) {
		trackT[i] = -20.0 * (trackRec[i] - track[i].avg) / track[i].std + 100;
		T += trackT[i];
	}
	T /= trackCnt;
}

/*
	Ʈ���� ��� ��հ� ǥ�������� ���Ѵ�.
	Update : avg, std
*/
void Track::getTrackStat(Player* player, int playerCnt, int trackNum) {
	int i;

	avg = 0, std = 0;

	for (i = 0; i < playerCnt; i++)
		avg += player[i].trackRec[trackNum];
	avg /= playerCnt;

	for (i = 0; i < playerCnt; i++)
		std += pow(player[i].trackRec[trackNum] - avg, 2);
	std /= playerCnt;
	std = sqrt(std);
}

/*
	oppTeam���� ��⸦ matchCnt�� �ùķ��̼��Ѵ�.

	Output : ��� ���� ���� �츮 �� ���� ������ ��� (������ �������� �뷱���� ����)
*/
double Team::matchA(Team oppTeam, int matchCnt) {
	int rank[8], score[8] = { 10, 8, 6, 5, 4, 3, 2, 1 };
	int i, j, k;
	double resScore = 0;
	Player player[8];	// player[0 ~ 3] : �Ʊ�, player[4 ~ 7] : ���

	srand(time(NULL));
	for (i = 0; i < matchCnt; i++) {
		// rank �ʱ�ȭ
		for (j = 0; j < 8; j++)
			rank[j] = j;

		// player[]�� ���� ������ �ִ´�.
		// �������� ������ T�� random�� ����� �϶�(-20 ~ 0)�� �޴´�.
		for (j = 0; j < 4; j++) {
			player[j] = this->member[j];
			player[j].T -= rand() % 20 - 1;
			
			player[j + 4] = oppTeam.member[j];
			player[j + 4].T -= rand() % 20 - 1;
		}

		// ���� ���� (Bubble Sort)
		// rank[n] : (n + 1)���� �� ������ index
		for (j = 7; j > 0; j--)
			for (k = 0; k < j; k++)
				if (player[rank[k]].T < player[rank[k + 1]].T) {
					int tmp = rank[k];
					rank[k] = rank[k + 1];
					rank[k + 1] = tmp;
				}

		// ���� ���
		for (j = 0; j < 8; j++) {
			if (rank[j] < 4)
				resScore += score[j];
			else
				resScore -= score[j];
		}
	}

	// resScore���� matchCnt���� ����� ���� ������ ��� �����Ƿ�, matchCnt�� ���� ����� �����Ѵ�.
	return resScore / matchCnt;
}

/*
	�Ʊ� ���� oppTeam�� �������� T������ �������� �뷱�� ������ �����Ѵ�.

	Output: �뷱�� ����(�̿ϼ�)
*/
double Team::matchB(Team oppTeam) {
	int rank[8] = { 0, 1, 2, 3, 4, 5, 6, 7 }, score[8] = { 10, 8, 6, 5, 4, 3, 2, 1 };
	int i, j, k;
	double resScore = 0, TGapAvg = 0, TGapStd = 0;
	Player player[8];	// player[0 ~ 3] : �Ʊ�, player[4 ~ 7] : ���

	// player[]�� ���� ������ �ִ´�.
	for (j = 0; j < 4; j++) {
		player[j] = this->member[j];
		player[j + 4] = oppTeam.member[j];
	}

	// ���� ���� (Bubble Sort)
	// rank[n]���� (n + 1)���� �� ������ ��ȣ�� ���� �ȴ�.
	for (j = 7; j > 0; j--)
		for (k = 0; k < j; k++)
			if (player[rank[k]].T < player[rank[k + 1]].T) {
				int tmp = rank[k];
				rank[k] = rank[k + 1];
				rank[k + 1] = tmp;
			}

	// TGapAvg ���
	TGapAvg = (player[rank[0]].T - player[rank[7]].T) / 7;

	// ���� ���
	for (j = 0; j < 8; j++) {
		if (rank[j] < 4)
			resScore += score[j];
		else
			resScore -= score[j];
	}

	for (i = 0; i < 7; i++)
		TGapStd += pow(player[rank[i]].T - player[rank[i + 1]].T - TGapAvg, 2);
	TGapStd /= 7, TGapStd = sqrt(TGapStd);

	// TO DO
	// TGapAvg, TGapStd, resScore�� ���� ����
	return TGapAvg + TGapStd + fabs(resScore);
}

double Composition::calcScore(int playerCnt) {
	int i, j, teamCnt;
	double res = 0;

	teamCnt = playerCnt / 4;
	for (i = 0; i < teamCnt - 1; i++)
		for (j = i + 1; j < teamCnt; j++) {
			res += team[i].matchA(team[j], 100000);
			// res += team[i].matchB(team[j]);
			
			printf("������ : %.2f%%\n", (float)(++matchCmpl) / matchTotal * 100);
		}

	return res / ((teamCnt) * (teamCnt - 1) / 2);
}

void sortPlayer(Player* player, int playerCnt) {
	int i, j;
	Player tmp;

	for (i = playerCnt - 1; i > 0; i--)
		for (j = 0; j < i; j++)
			if (player[j].T < player[j + 1].T) {
				tmp = player[j];
				player[j] = player[j + 1];
				player[j + 1] = tmp;
			}
}

Composition* getAllComp(Player* player, int playerCnt) {
	Composition* comp = new Composition [12];
	int i, j;
	int teamCnt;
	teamCnt = playerCnt / 4;

	for (i = 0; i < 12; i++) {
		comp[i].team = new Team[teamCnt];	// �� ���տ��� teamCnt���� ���� �ִ�.

		// �ֻ����� teamCnt���� ���� �ٸ� ���� ��ġ (comp[11] ����)
		if (i != 11) {
			for (j = 0; j < teamCnt; j++)
				comp[i].team[j].member[0] = player[j];
		}	
	}

	/*
		group 1 ����
		comp[0] ~ comp[6]
	*/
	for (i = 0; i < 7; i++) {
		if (i / 4 == 0)
			for (j = 0; j < teamCnt; j++)
				comp[i].team[j].member[1] = player[-j - 1 + 2 * teamCnt];
		else
			for (j = 0; j < teamCnt; j++)
				comp[i].team[j].member[1] = player[j + teamCnt];
	}

	for (i = 0; i < 7; i++) {
		if (i % 4 <= 1)
			for (j = 0; j < teamCnt; j++)
				comp[i].team[j].member[2] = player[-j - 1 + 3 * teamCnt];
		else
			for (j = 0; j < teamCnt; j++)
				comp[i].team[j].member[2] = player[j + 2 * teamCnt];
	}

	for (i = 0; i < 7; i++) {
		if (i % 2 == 0)
			for (j = 0; j < teamCnt; j++)
				comp[i].team[j].member[3] = player[-j - 1 + 4 * teamCnt];
		else
			for (j = 0; j < teamCnt; j++)
				comp[i].team[j].member[3] = player[j + 3 * teamCnt];
	}

	/*
		group 2 ����
		comp[7] ~ comp[11]
	*/
	// comp[7]
	for (i = 0; i < teamCnt; i++) {
		comp[7].team[i].member[1] = player[i + teamCnt];
		comp[7].team[i].member[2] = player[-i * 2 - 2 + 4 * teamCnt];
		comp[7].team[i].member[3] = player[-i * 2 - 1 + 4 * teamCnt];
	}

	// comp[8]
	for (i = 0; i < teamCnt; i++) {
		comp[8].team[i].member[1] = player[-i * 2 - 2 + 3 * teamCnt];
		comp[8].team[i].member[2] = player[-i * 2 - 1 + 3 * teamCnt];
		comp[8].team[i].member[3] = player[i + 3 * teamCnt];
	}
		
	// comp[9]
	for (i = 0; i < teamCnt; i++) {
		comp[9].team[i].member[1] = player[-i - 1 + 2 * teamCnt];
		comp[9].team[i].member[2] = player[-i * 2 - 2 + 4 * teamCnt];
		comp[9].team[i].member[3] = player[-i * 2 - 1 + 4 * teamCnt];
	}

	// comp[10]
	for (i = 0; i < teamCnt; i++) {
		comp[10].team[i].member[1] = player[-i * 2 - 2 + 3 * teamCnt];
		comp[10].team[i].member[2] = player[-i * 2 - 1 + 3 * teamCnt];
		comp[10].team[i].member[3] = player[-i - 1 + 4 * teamCnt];
	}

	// comp[11]
	for (i = 0; i < teamCnt; i++) {
		comp[11].team[i].member[0] = player[i * 2];
		comp[11].team[i].member[1] = player[i * 2 + 1];
		comp[11].team[i].member[2] = player[-i * 2 - 2 + 4 * teamCnt];
		comp[11].team[i].member[3] = player[-i * 2 - 1 + 4 * teamCnt];
	}

	matchTotal = (teamCnt * (teamCnt - 1)) / 2 * 12;

	return comp;
}

Composition getOptComp(Player* player, int playerCnt) {
	Composition* comp, res;
	int i, teamCnt;
	double compScore[12], minScore;

	teamCnt = playerCnt / 4;
	comp = getAllComp(player, playerCnt);

	for (i = 0; i < 12; i++)
		compScore[i] = comp[i].calcScore(playerCnt);

	minScore = fabs(compScore[0]);
	res = comp[0];
	for (i = 1; i < 11; i++)
		if (fabs(compScore[i]) < minScore) {
			minScore = fabs(compScore[i]);
			res = comp[i];
		}

	return res;
}

void writeRec(FILE* fp, double rec) {
	int min = (int)rec / 60;
	double csec = floor(100 * (rec - (int)rec) + 0.5);	// ��Ƽ��(centi second)
	fprintf(fp, "0%d�� ", min);

	if (rec - min * 60.0 < 10)
		fprintf(fp, "0");
	fprintf(fp, "%d�� ", (int)(rec - min * 60.0));

	if (csec < 10)
		fprintf(fp, "0");
	fprintf(fp, "%d", (int)csec);
}

void writeResult(FILE* fp, Player* player, int playerCnt, Track* track, int trackCnt, Composition comp) {
	int i, j, teamCnt;
	teamCnt = playerCnt / 4;
	
	fprintf(fp, ",");
	for (i = 0; i < trackCnt; i++)
		fprintf(fp, "%s,,", track[i].name);
	fprintf(fp, "\n");

	fprintf(fp, ",");
	for (i = 0; i < trackCnt; i++)
		fprintf(fp, "%s,%s,", "���", "T����");
	fprintf(fp, "T����(����)");
	fprintf(fp, "\n");

	for (i = 0; i < playerCnt; i++) {
		fprintf(fp, "%s,", player[i].name);

		for (j = 0; j < trackCnt; j++) {
			writeRec(fp, player[i].trackRec[j]);
			fprintf(fp, ",");
			fprintf(fp, "%lf,", player[i].trackT[j]);
		}
		
		fprintf(fp, "%lf", player[i].T);
		fprintf(fp, "\n");
	}

	fprintf(fp, "%s,", "���");
	for (i = 0; i < trackCnt; i++) {
		writeRec(fp, track[i].avg);
		fprintf(fp, ",,");
	}
	fprintf(fp, "\n");
	fprintf(fp, "%s,", "ǥ������(��)");
	for (i = 0; i < trackCnt; i++)
		fprintf(fp, "%lf,,", track[i].std);
	fprintf(fp, "\n\n");

	for (i = 0; i < teamCnt; i++)
		fprintf(fp, "�� %c,", 'A' + i);
	fprintf(fp, "\n");
	for (i = 0; i < 4; i++) {
		for (j = 0; j < teamCnt; j++)
			fprintf(fp, "%s,", comp.team[j].member[i].name);
		fprintf(fp, "\n");
	}
}