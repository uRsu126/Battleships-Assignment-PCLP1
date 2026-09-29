//Urse Andrei 312CB
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char type;
	int length;
	int start_X, start_Y;
	int end_X, end_Y;
	char orient;
} ship;

typedef struct {
	int hits;
	int total_attacks;
	int ship_number;
} game_stats;

//FUNCTII PENTUR TABLA
int **alloc_board(int N, int M)
{
	int **a = calloc((N + 1), sizeof(int *));
	if (!a) {
		perror("Eroare la alocarea memoriei.");
		exit(1);
	}
	for (int i = 1; i <= N; i++) {
		a[i] = calloc((M + 1), sizeof(int));
		if (!a[i]) {
			perror("Eroare la alocarea memoriei.");
			exit(1);
		}
	}
	return a;
}

void print_board(int **board, int N, int M)
{
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= M; j++) {
			printf("%d ", board[i][j]);
		}
		printf("\n");
	}
}

void free_board(int **a, int N)
{
	for (int i = 1; i <= N; i++) {
		free(a[i]);
	}
	free(a);
}

//FUNCTII PENTRU NAVE
int ship_length(char type)
{
	if (type == 'A') {
		return 1;
	}
	if (type == 'L') {
		return 2;
	}
	if (type == 'B') {
		return 3;
	}
	if (type == 'Y') {
		return 4;
	}
	if (type == 'S') {
		return 5;
	}
	return 0;
}

int ship_total_number(int N, int M)
{
	long surface = (long)N * M;
	int maxA = surface / 20;
	int maxL = surface / 30;
	int maxB = surface / 40;
	int maxY = surface / 55;
	int maxS = surface / 70;
	return maxA + maxL + maxB + maxY + maxS;
}

const char *ship_name(char letter)
{
	if (letter == 'A') {
		return "Albacore";
	}
	if (letter == 'L') {
		return "Laffey";
	}
	if (letter == 'B') {
		return "Belfast";
	}
	if (letter == 'Y') {
		return "Yamato";
	}
	if (letter == 'S') {
		return "Shinano";
	}
	return 0;
}

int verify_ship(int **board, int N, int M, char type,
				char orient, int X, int Y, int length)
{
	if (orient == 'H') {
		if (Y < 1 || Y + length - 1 > M || X < 1 || X > N) {
			printf("Nava %s nu poate fi amplasata orizontal "
					"la coordonatele (%d, %d).\n", ship_name(type), X, Y);
			return 0;
		}
		for (int k = 0; k < length; k++) {
			if (board[X][Y + k] != 0) {
				printf("Nava %s nu poate fi amplasata orizontal "
						"la coordonatele (%d, %d).\n", ship_name(type), X, Y);
				return 0;
			}
		}
	} else if (orient == 'V') {
		if (X > N || X - length + 1 < 1 || Y < 1 || Y > M) {
			printf("Nava %s nu poate fi amplasata vertical "
					"la coordonatele (%d, %d).\n", ship_name(type), X, Y);
			return 0;
		}
		for (int k = 0; k < length; k++) {
			if (board[X - k][Y] != 0) {
				printf("Nava %s nu poate fi amplasata vertical "
						"la coordonatele (%d, %d).\n", ship_name(type), X, Y);
				return 0;
			}
		}
	}
	return 1;
}

void place_ship(int **board, int **logic_board, ship *ships,
				int *ships_alive, char type, char orient,
				int X, int Y, int length)
{
	int index = *ships_alive;
	ships[index].type = type;
	ships[index].length = length;
	ships[index].orient = orient;
	ships[index].start_X = X;
	ships[index].start_Y = Y;

	if (orient == 'H') {
		for (int k = 0; k < length; k++) {
			board[X][Y + k] = length;
			logic_board[X][Y + k] = index + 1;
		}
		ships[index].end_X = X;
		ships[index].end_Y = Y + length - 1;
	} else {
		for (int k = 0; k < length; k++) {
			board[X - k][Y] = length;
			logic_board[X - k][Y] = index + 1;
		}
		ships[index].end_X = X - length + 1;
		ships[index].end_Y = Y;
	}
	(*ships_alive)++;
}

//CITIRE NAVE
void read_ships(int **boards[2], int **logic_board[2], ship *ships[2],
				int ships_alive[2], int max_ships, int N, int M)
{
	char type, orient;
	int X, Y, length, player = 0;

	while (ships_alive[0] < max_ships || ships_alive[1] < max_ships) {

		if (ships_alive[player] >= max_ships) {
			player = 1 - player;
		}

		scanf(" %c %c %d %d", &type, &orient, &X, &Y);
		length = ship_length(type);
		if (length > 0 &&
			verify_ship(boards[player], N, M, type, orient, X, Y, length)) {
			//Plasam nava
			place_ship(boards[player], logic_board[player],
					   ships[player], &ships_alive[player],
					   type, orient, X, Y, length);

			player = 1 - player;
		}
	}
}

//EXECUTARE ATAC
void attack(int id, int **target_board, int **target_attack_board,
			int **logic_board, ship *ships, int *ships_alive, int X, int Y)
{
	int ship_id = logic_board[X][Y] - 1;
	ship *s = &ships[ship_id];
	if (ship_id < 0) {
		return;
	}
	if (X == s->start_X && Y == s->start_Y) {
		target_attack_board[X][Y] = 1;
		(*ships_alive)--;
		if (s->orient == 'H') {
			for (int j = 0; j < s->length; j++) {
				target_board[X][Y + j] = 0;
			}
		} else {
			for (int j = 0; j < s->length; j++) {
				target_board[X - j][Y] = 0;
			}
		}
		printf("Jucatorul %d a distrus o nava %s plasata intre coordonatele "
				"(%d, %d) si (%d, %d).\n", id, ship_name(s->type),
				s->start_X, s->start_Y, s->end_X, s->end_Y);
	} else {
		target_attack_board[X][Y] = 1;
		printf("Jucatorul %d a lovit o nava %s la coordonatele (%d, %d).\n",
			   id, ship_name(s->type), X, Y);
	}
}

//RULARE JOCURI
void battleships(int J, game_stats **total_stats)
{
	for (int game = 1; game <= J; game++) {
		int N, M;
		scanf("%d%d", &N, &M);
		int **boards[2];
		int **logic_boards[2];
		int **attack_boards[2];
		ship *ships[2];
		int *ships_alive = calloc(2, sizeof(int));
		int max_ships = ship_total_number(N, M);

		for (int p = 0; p < 2; p++) {
			boards[p] = alloc_board(N, M);
			attack_boards[p] = alloc_board(N, M);
			logic_boards[p] = alloc_board(N, M);
			ships[p] = (ship *)calloc(max_ships, sizeof(ship));
			ships_alive[p] = 0;
			total_stats[p][game - 1].ship_number = max_ships;//Stats
		}
		//Citire si plasare nave
		read_ships(boards, logic_boards, ships, ships_alive,
				   max_ships, N, M);

		//Afisare table jucatori
		print_board(boards[0], N, M);
		printf("\n");
		print_board(boards[1], N, M);

		//Faza de atacuri
		int turn = 0;
		int end = 1;
		int attack_X, attack_Y;
		while (end) {
			scanf("%d%d", &attack_X, &attack_Y);
			int attacker_id = turn + 1;
			int target_id = 1 - turn;
			total_stats[turn][game - 1].total_attacks++;//Stats

			int **target_board = boards[target_id];
			int **target_attack_board = attack_boards[target_id];
			int **logic_board = logic_boards[target_id];

			if (target_board[attack_X][attack_Y] == 0 &&
				target_attack_board[attack_X][attack_Y] == 0) {
				target_attack_board[attack_X][attack_Y] = 1;

				printf("Jucatorul %d a ratat o lovitura la coordonatele "
					   "(%d, %d).\n", attacker_id, attack_X, attack_Y);

			} else if (target_attack_board[attack_X][attack_Y] == 1) {

				printf("Coordonatele (%d, %d) au fost deja atacate de "
					   "jucatorul %d.\n",
						attack_X, attack_Y, attacker_id);

			} else if (target_board[attack_X][attack_Y] != 0 &&
					   target_attack_board[attack_X][attack_Y] == 0 &&
					   logic_board[attack_X][attack_Y] != 0) {

				total_stats[turn][game - 1].hits++;
				attack(attacker_id, target_board, target_attack_board,
					   logic_board, ships[target_id],
					   &ships_alive[target_id], attack_X, attack_Y);

			}
			if (ships_alive[target_id] == 0) {
				end = 0;
				printf("Jucatorul %d a castigat.\n", attacker_id);
			}
			turn = 1 - turn;
		}
		//Eliberare memorie
		for (int p = 0; p < 2; p++) {
			free_board(boards[p], N);
			free_board(attack_boards[p], N);
			free_board(logic_boards[p], N);
			free(ships[p]);
		}
		free(ships_alive);
	}
}

//FUNCTII STATISTICI
void first_cond(int J, double sum1, int sum2, float rap_acc,
				float total_acc, double *game_accs, int *game_ships)
{
	int count = 0, id = -1;
	while ((sum1 / sum2) < rap_acc && count < J) {
		double max_gain = -1;
		for (int i = 0; i < J; i++) {
			if (game_accs[i] < 100) {
				double gain = (100 - game_accs[i]) * game_ships[i];
				if (gain > max_gain) {
					max_gain = gain;
					id = i;
				}
			}
		}
		if (id != -1) {
			sum1 += max_gain;
			game_accs[id] = 100;
			count++;
		} else {
			break;
		}
	}
	printf("%06.2f.%d\n", total_acc, count);
}

void second_cond(int total_attacks, float rap_acc,
				 float game_acc, game_stats current_game)
{
	double min_hits = (total_attacks * rap_acc + 99) / 100;
	min_hits = floor(min_hits) - current_game.hits;
	if (min_hits < 0) {
		min_hits = 0;
	}
	printf("%06.2f.%0.f\n", game_acc, min_hits);
}

void third_cond(int J, double sum1, int sum2, float rap_acc,
				float total_acc, double *game_accs, int *game_ships)
{
	int count = 0, id = -1;
	while (count < J) {
		double min_loss = 999999;
		for (int i = 0; i < J; i++) {
			if (game_accs[i] > 0.0) {
				double loss = game_accs[i] * game_ships[i];
				if (loss < min_loss) {
					min_loss = loss;
					id = i;
				}
			}
		}
		if (id != -1) {
			double new_total_acc = (sum1 - min_loss) / sum2;
			if (new_total_acc >= rap_acc) {
				sum1 -= min_loss;
				game_accs[id] = 0;
				count++;
			} else {
				break;
			}
		} else {
			break;
		}
	}
	printf("%06.2f.%d\n", total_acc, count);
}

void fourth_cond(int total_attacks, float rap_acc,
				 float game_acc, game_stats current_game)
{
	double max_hits = (total_attacks * rap_acc) / 100;
	max_hits = current_game.hits - ceil(max_hits);
	if (max_hits < 0) {
		max_hits = 0;
	}
	printf("%06.2f.%0.f\n", game_acc, max_hits);
}

void game_statistics(int J, int game_id, char *type2, float rap_acc,
					 game_stats *player_stats)
{
	if (game_id < 0 || game_id >= J) {
		//Verif daca exista jocul
		printf("Nu exista date pentru meciul cerut.\n");
		return;
	}
	double game_acc = 0;
	int total_attacks = player_stats[game_id].total_attacks;
	game_acc = (double)player_stats[game_id].hits * 100.0  / total_attacks;
	game_acc = floor(game_acc * 100) / 100;
	if (strcmp(type2, "MIN") == 0) {
		//Cerinta 2: Nr min de lovituri pe care NU ar fi trebuit sa le rateze
		second_cond(total_attacks, rap_acc, game_acc, player_stats[game_id]);
	} else if (strcmp(type2, "MAX") == 0) {
		//Cerinta 4: Nr max de lovituri pe care le ar fi putut rata
		fourth_cond(total_attacks, rap_acc, game_acc, player_stats[game_id]);
	}
}

void all_games_statistics(int J, char *type2, float rap_acc,
						  game_stats *player_stats)
{
	double sum1 = 0;
	int sum2 = 0;

	double *game_accs = calloc(J, sizeof(double));
	int *game_ships = calloc(J, sizeof(int));

	for (int i = 0; i < J; i++) {
		int attacks = player_stats[i].total_attacks;

		game_accs[i] = (double)player_stats[i].hits * 100 / attacks;
		game_ships[i] = player_stats[i].ship_number;

		sum1 += game_accs[i] * game_ships[i];
		sum2 += game_ships[i];
	}

	double total_acc = sum1 / sum2;
	total_acc = floor(total_acc * 100) / 100;

	if (strcmp(type2, "MIN") == 0) {
		//Cerinta 1: Nr minim de jocuri in care ar fi trebuit
		//sa aiba 100% acuratete
		first_cond(J, sum1, sum2, rap_acc, total_acc, game_accs, game_ships);

	} else if (strcmp(type2, "MAX") == 0) {
		//Cerinta 3: Nr maxim de jocuri in care ar fi putut
		//avea 0% acuratete
		third_cond(J, sum1, sum2, rap_acc, total_acc, game_accs, game_ships);
	}
	free(game_accs);
	free(game_ships);
}

void calc_statistics(int J, game_stats **total_stats)
{
	char *key = calloc(20, sizeof(char));
	scanf("%s", key);
	char *command = calloc(20, sizeof(char));
	scanf("%s", command);

	if (strcmp(key, "-") == 0) {
		while (strcmp(command, "Q") != 0) {

			char player = command[0];//O / T

			char type1 = command[1];//U / T

			char temp_id[4];//AAA
			strncpy(temp_id, command + 2, 3);
			temp_id[3] = '\0';
			int game_id = atoi(temp_id) - 1;

			char temp_acc[7];//BBB.BB
			strncpy(temp_acc, command + 5, 6);
			temp_acc[6] = '\0';
			float rap_acc = atof(temp_acc);

			char type2[4];//MIN / MAX
			strcpy(type2, command + 11);
			type2[3] = '\0';

			int p;
			if (player == 'O') {
				p = 0;
			} else if (player == 'T') {
				p = 1;
			}

			if (type1 == 'U') {
				//Statistici pentru un joc
				game_statistics(J, game_id, type2, rap_acc, total_stats[p]);
				//Se returneaza output (format din game_acc si min/max_hits)

			} else if (type1 == 'T') {

				//Statistici pentru toate jocurile
				all_games_statistics(J, type2, rap_acc, total_stats[p]);
			}
			scanf("%s", command);
		}
	}
	free(key);
	free(command);
}

int main(void)
{
	int J;
	scanf("%d", &J);

	//Statisticile celor 2 jucatori
	game_stats **total_stats = (game_stats **)calloc(2, sizeof(game_stats *));
	for (int p = 0; p < 2; p++) {
		total_stats[p] = (game_stats *)calloc(J + 1, sizeof(game_stats));
	}

	//Rulare Jocuri
	battleships(J, total_stats);

	//Calculare Statistici
	calc_statistics(J, total_stats);

	//Eliberare Memorie Statistici
	free(total_stats[0]);
	free(total_stats[1]);
	free(total_stats);
	return 0;
}
