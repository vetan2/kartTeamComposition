#include "kartTeam.h"

int main() {
	Player* player;
	Track* track;
	int playerCnt, trackCnt;
	int i;
	
	scanf("%d %d", &playerCnt, &trackCnt);

	player = new Player[playerCnt];
	for (i = 0; i < playerCnt; i++) {
		player[i].trackRec = new double[trackCnt];
		player[i].trackZ = new double[trackCnt];
	}

	track = new Track[trackCnt];

	for (i = 0; i < trackCnt; i++)
		scanf("%s", track[i].name);

	for (i = 0; i < playerCnt; i++)
		player[i].input(trackCnt);

	for (i = 0; i < trackCnt; i++)
		track[i].getTrackStat(player, playerCnt, i);

	for (i = 0; i < playerCnt; i++)
		player[i].calcZ(track, trackCnt);
	sortPlayer(player, playerCnt);

	printResult(player, playerCnt, track, trackCnt, getOptComp(player, playerCnt));
}

void Player::input(int trackCnt) {
	int i, n;
	
	scanf("%s", name);
	for (i = 0; i < trackCnt; i++) {
		scanf("%d", &n);
		trackRec[i] = (n / 10000) * 60 + n % 10000 / 100 + (double)(n % 100) * 0.01;
	}
}

void Player::calcZ(Track* track, int trackCnt) {
	int i;

	Z = 0;
	for (i = 0; i < trackCnt; i++) {
		trackZ[i] = -20.0 * (trackRec[i] - track[i].avg) / track[i].std + 100;
		Z += trackZ[i];
	}
	Z /= trackCnt;
}

/*
	트랙의 기록 평균과 표준편차를 구한다.
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
	oppTeam과의 경기를 matchCnt번 시뮬레이션한다.
	한 번의 경기에서 선수들은 각자의 Z에 random하게 컨디션 하락(-20 ~ 0)을 받는다.
	Output : |(상대 팀에 대한 우리 팀 점수 차이의 평균| (낮을수록 밸런스가 좋음)
			   ex) 우리팀 20점 상대팀 19점이면 +1)
*/
double Team::matchA(Team oppTeam, int matchCnt) {
	int rank[8], score[8] = { 10, 8, 6, 5, 4, 3, 2, 1 };
	int i, j, k;
	double resScore = 0;
	Player player[8];	// player[0 ~ 3] : 아군, player[4 ~ 7] : 상대

	srand(time(NULL));
	for (i = 0; i < matchCnt; i++) {
		// rank 초기화
		for (j = 0; j < 8; j++)
			rank[j] = j;

		// player[]에 선수 정보를 넣는다.
		// 선수들은 각자의 Z에 random한 컨디션 하락(-20 ~ 0)을 받는다.
		for (j = 0; j < 4; j++) {
			player[j] = this->member[j];
			player[j].Z -= rand() % 20 - 1;
			
			player[j + 4] = oppTeam.member[j];
			player[j + 4].Z -= rand() % 20 - 1;
		}

		// 순위 정렬 (Bubble Sort)
		// rank[n]에는 (n + 1)등을 한 선수의 번호가 들어가게 된다.
		for (j = 7; j > 0; j--)
			for (k = 0; k < j; k++)
				if (player[rank[k]].Z < player[rank[k + 1]].Z) {
					int tmp = rank[k];
					rank[k] = rank[k + 1];
					rank[k + 1] = tmp;
				}

		// 점수 계산
		for (j = 0; j < 8; j++) {
			if (rank[j] < 4)
				resScore += score[j];
			else
				resScore -= score[j];
		}
	}

	return resScore / matchCnt;
}

double Team::matchB(Team oppTeam) {
	int rank[8] = { 0, 1, 2, 3, 4, 5, 6, 7 }, score[8] = { 10, 8, 6, 5, 4, 3, 2, 1 };
	int i, j, k;
	double resScore = 0, ZGapAvg = 0, ZGapStd = 0;
	Player player[8];	// player[0 ~ 3] : 아군, player[4 ~ 7] : 상대

	// player[]에 선수 정보를 넣는다.
	for (j = 0; j < 4; j++) {
		player[j] = this->member[j];
		player[j + 4] = oppTeam.member[j];
	}

	// 순위 정렬 (Bubble Sort)
	// rank[n]에는 (n + 1)등을 한 선수의 번호가 들어가게 된다.
	for (j = 7; j > 0; j--)
		for (k = 0; k < j; k++)
			if (player[rank[k]].Z < player[rank[k + 1]].Z) {
				int tmp = rank[k];
				rank[k] = rank[k + 1];
				rank[k + 1] = tmp;
			}

	// ZGapAvg 계산
	ZGapAvg = (player[rank[0]].Z - player[rank[7]].Z) / 7;

	// 점수 계산
	for (j = 0; j < 8; j++) {
		if (rank[j] < 4)
			resScore += score[j];
		else
			resScore -= score[j];
	}

	for (i = 0; i < 7; i++)
		ZGapStd += pow(player[rank[i]].Z - player[rank[i + 1]].Z - ZGapAvg, 2);
	ZGapStd /= 7, ZGapStd = sqrt(ZGapStd);

	return ZGapStd + fabs(resScore);
}

void sortPlayer(Player* player, int playerCnt) {
	int i, j;
	Player tmp;

	for (i = playerCnt - 1; i > 0; i--)
		for (j = 0; j < i; j++)
			if (player[j].Z < player[j + 1].Z) {
				tmp = player[j];
				player[j] = player[j + 1];
				player[j + 1] = tmp;
			}
}

Composition* getAllComp(Player* player, int playerCnt) {
	Composition* comp = new Composition [11];
	int i, j;
	int teamCnt;
	teamCnt = playerCnt / 4;

	for (i = 0; i < 11; i++) {
		comp[i].team = new Team[teamCnt];	// 각 조합에는 teamCnt개의 팀이 있다.

		// 최상위권 teamCnt명을 각각 다른 팀에 배치
		for (j = 0; j < teamCnt; j++)
			comp[i].team[j].member[0] = player[j];
	}

	/*
		group 1 조합
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
		group 2 조합
		comp[7] ~ comp[10]
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

	return comp;
}

double calcCompScore(Composition comp, int playerCnt) {
	Team* team = comp.team;
	int i, j, teamCnt;
	double res = 0;

	teamCnt = playerCnt / 4;
	for (i = 0; i < teamCnt - 1; i++)
		for (j = i + 1; j < teamCnt; j++) {
			res += team[i].matchA(team[j], 100000);
			// res += team[i].matchB(team[j]);
		}

	return res / ((teamCnt) * (teamCnt - 1) / 2);
}

Composition getOptComp(Player* player, int playerCnt) {
	Composition* comp, res;
	int i, teamCnt;
	double compScore[11], minScore;

	teamCnt = playerCnt / 4;
	comp = getAllComp(player, playerCnt);

	for (i = 0; i < 11; i++)
		compScore[i] = calcCompScore(comp[i], playerCnt);

	minScore = fabs(compScore[0]);
	res = comp[0];
	for (i = 1; i < 11; i++)
		if (fabs(compScore[i]) < minScore) {
			minScore = fabs(compScore[i]);
			res = comp[i];
		}

	return res;
}

void printRec(double rec) {
	int min = (int)rec / 60;
	double csec = floor(100 * (rec - (int)rec) + 0.5);	// 센티초(centi second)
	printf("0%d분 ", min);

	if (rec - min * 60.0 < 10)
		printf("0");
	printf("%d초 ", (int)(rec - min * 60.0));

	if (csec < 10)
		printf("0");
	printf("%d", (int)csec);
}

void printResult(Player* player, int playerCnt, Track* track, int trackCnt, Composition comp) {
	int i, j, teamCnt;
	teamCnt = playerCnt / 4;
	
	printf(",");
	for (i = 0; i < trackCnt; i++)
		printf("%s,,", track[i].name);
	printf("\n");

	printf(",");
	for (i = 0; i < trackCnt; i++)
		printf("%s,%s,", "기록", "표준점수");
	printf("Z");
	printf("\n");

	for (i = 0; i < playerCnt; i++) {
		printf("%s,", player[i].name);

		for (j = 0; j < trackCnt; j++) {
			printRec(player[i].trackRec[j]);
			printf(",");
			printf("%lf,", player[i].trackZ[j]);
		}
		
		printf("%lf", player[i].Z);
		printf("\n");
	}

	printf("%s,", "평균");
	for (i = 0; i < trackCnt; i++) {
		printRec(track[i].avg);
		printf(",,");
	}
	printf("\n");
	printf("%s,", "표준편차(초)");
	for (i = 0; i < trackCnt; i++)
		printf("%lf,,", track[i].std);
	printf("\n\n");

	for (i = 0; i < teamCnt; i++)
		printf("팀 %c,", 'A' + i);
	printf("\n");
	for (i = 0; i < 4; i++) {
		for (j = 0; j < teamCnt; j++)
			printf("%s,", comp.team[j].member[i].name);
		printf("\n");
	}
}
