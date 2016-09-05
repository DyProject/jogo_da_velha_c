#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 5
#define VICTORY_COUNT 3
#define MAX_COMP_REC 50

typedef enum {
	X, // Jogador 1 
	O, // Jogador 2 ou IA
	E  // Vazio (Empty)
} Player;

typedef struct 
{
	Player places[BOARD_SIZE][BOARD_SIZE];
	unsigned int round;
	unsigned int weight;
	unsigned int is_win_scenario;
} Board;

typedef struct 
{
	int row;
	int col;
} Move;

Board new_board() 
{
	Board b;
	int i, j;

	b.round = 1;
	b.weight = 0;

	for (i = 0; i < BOARD_SIZE; i += 1) 
	{
		for (j = 0; j < BOARD_SIZE; j += 1) 
		{
			b.places[i][j] = E;
		}
	}

	return b;
}

Board new_board_from(Board b) 
{
	Board new;
	int i, j;

	new.round = b.round;
	new.weight = b.weight;

	for (i = 0; i < BOARD_SIZE; i += 1) 
	{
		for (j = 0; j < BOARD_SIZE; j += 1) 
		{
			new.places[i][j] = b.places[i][j];
		}
	}

	return new;
}

void h(Board* b) 
{
	int i, j, count_row, count_col, count_d;

	b->weight = 0;

	for (i = 0; i < BOARD_SIZE; i += 1) 
	{
		count_row = 0;
		count_col = 0;

		for (j = 0; j < BOARD_SIZE; j += 1) 
		{
			if (b->places[i][j] == O) 
			{
				count_row += 1;
			}

			if (b->places[j][i] == O) 
			{
				count_col += 1;
			}
		}

		if (count_row >= VICTORY_COUNT || count_col >= VICTORY_COUNT) 
		{
			b->is_win_scenario = 1;
			return;
		}

		b->weight += count_row + count_col;
	}

	count_d = 0;

	for (i = 0; i < BOARD_SIZE; i += 1) 
	{
		if (b->places[i][i] == O) 
		{
			count_d += 1;
		}
		else 
		{
			count_d = 0;
		}

		if (count_d >= VICTORY_COUNT) 
		{
			b->is_win_scenario = 1;
			return;
		}

		b->weight += count_d;
	}

	count_d = 0;

	for (i = 1; i <= BOARD_SIZE; i += 1) 
	{
		if (b->places[i - 1][BOARD_SIZE - i] == O) 
		{
			count_d += 1;
		}
		else 
		{
			count_d = 0;
		}

		if (count_d >= VICTORY_COUNT) 
		{
			b->is_win_scenario = 1;
			return;
		}

		b->weight += count_d;
	}
}

char player_to_str(Player p) 
{
	if (p == X) return 'X';
	if (p == O) return 'O';
	if (p == E) return ' ';
}

void print_board(Board b) 
{
	int i, j, k;

	printf("\n");

	printf("Round: %d\n", b.round);
	printf("Peso: %d\n\n", b.weight);

	for (i = 0; i < BOARD_SIZE; i += 1) 
	{
		printf(" %d  ", i);
	}

	printf("\n");

	for (i = 0; i < BOARD_SIZE; i += 1) 
	{
		for (j = 0; j < BOARD_SIZE; j += 1) 
		{
			printf(" %c ", player_to_str(b.places[i][j]));

			if (j != BOARD_SIZE - 1) 
			{
				printf("|");
			}
			else 
			{
				printf(" %d", i);
			}
		}

		if (i != BOARD_SIZE - 1) 
		{
			printf("\n");

			for (k = 0; k < BOARD_SIZE * 4; k += 1) 
			{
				printf("-");
			}
		}

		printf("\n");
	}

	printf("\n");
}

int comp_char(char that, char with) 
{
	return (int)that == (int)with;
}

int check_move(Board* b, int row, int col) 
{
	if (row >= BOARD_SIZE || col >= BOARD_SIZE) 
	{
		return 0;
	}

	return b->places[row][col] == E;
}

int player_has_won(Player p, Board b) 
{
	int i, j, count_row, count_col, count_d;

	for (i = 0; i < BOARD_SIZE; i += 1) 
	{
		count_row = 0;
		count_col = 0;

		for (j = 0; j < BOARD_SIZE; j += 1) 
		{
			if (b.places[i][j] == p) 
			{
				count_row += 1;
			}

			if (b.places[j][i] == p) 
			{
				count_col += 1;
			}
		}

		if (count_row >= VICTORY_COUNT || count_col >= VICTORY_COUNT) 
		{
			return 1;
		}
	}

	count_d = 0;

	for (i = 0; i < BOARD_SIZE; i += 1) 
	{
		if (b.places[i][i] == p) 
		{
			count_d += 1;
		}
		else 
		{
			count_d = 0;
		}

		if (count_d >= VICTORY_COUNT) 
		{
			return 1;
		}
	}

	count_d = 0;

	for (i = 1; i <= BOARD_SIZE; i += 1) 
	{
		if (b.places[i - 1][BOARD_SIZE - i] == p) 
		{
			count_d += 1;
		}
		else 
		{
			count_d = 0;
		}

		if (count_d >= VICTORY_COUNT) 
		{
			return 1;
		}	
	}

	return 0;
}

/*Player has_winner(Board b) 
{
	if (player_has_won(X, b)) return X;
	if (player_has_won(O, b)) return O;
	return E;
}*/

Player has_winner(Board b) 
{
	int i, j, k, m, n, p;
	int has_won_row, has_won_col, has_won_d;
	Player row_p, col_p, mid_p;

	int disp = VICTORY_COUNT - 2;
	int m_size = BOARD_SIZE - (VICTORY_COUNT - 1);

	for (i = 1, j = 1; i < m_size; i += 1) 
	{
		mid_p = b.places[i][j];
		has_won_d = 0;

		for (k = i, m = j; k < i + disp; k += 1, j += 1) 
		{
			if (b.places[k - 1][j - 1] != mid_p
			 || b.places[k + 1][j + 1] != mid_p) 
			{
				has_won_d = 0;
				break;
			}

			if (b.places[k + 1][j - 1] != mid_p
			 || b.places[k - 1][j + 1] != mid_p) 
			{
				has_won_d = 0;
				break;
			}
		}

		if (has_won_d) 
		{
			return mid_p;
		}

		for (k = i - disp, m = j - disp; k <= i + disp; k += 1, m += 1) 
		{
			row_p = b.places[k][i - disp];
			col_p = b.places[j - disp][m];

			has_won_row = 1;
			has_won_col = 1;

			for (n = i - (disp - 1), p = j - (disp - 1); n <= i + disp; n += 1, p += 1) 
			{
				if (row_p != b.places[k][n]) 
				{
					has_won_row = 0;
				}

				if (col_p != b.places[p][m]) 
				{
					has_won_col = 0;
				}
			}

			if (has_won_row) 
			{
				return row_p;
			}

			if (has_won_col) 
			{
				return col_p;
			}
		}


		if (j == BOARD_SIZE - 1) 
		{
			i += 1;
			j = 1;
		}
	}
}

int is_velha(Board b) 
{
	int i, j;

	for (i = 0; i < BOARD_SIZE; i += 1) 
	{
		for (j = 0; j < BOARD_SIZE; j += 1) 
		{
			if (b.places[i][j] != E) 
			{
				return 0;
			}
		}
	}

	return 1;
}

Move random_move(Board b) 
{
	int r_col, r_row, is_valid = 0;
	Move res;

	srand(time(NULL));

	while ( ! is_valid) 
	{
		r_row = rand() % BOARD_SIZE;
		r_col = rand() % BOARD_SIZE;

		if (check_move(&b, r_row, r_col)) 
		{
			is_valid = 1;
		}
	}

	res.row = r_row;
	res.col = r_col;

	return res;
}

Move next(Board b) 
{
	Board best;
	Move res;
	int limit = 0;

	while (best.weight < b.weight && limit < MAX_COMP_REC) 
	{
		best = new_board_from(b);
		res = random_move(b);

		best.places[res.row][res.col] = O;

		h(&best);

		limit += 1;
	}

	return res;
}

int main(void) 
{
	Board b = new_board();
	Player who_is = X;
	Player winner = E;
	Move comp_move;
	int has_end = 0;
	int has_choose = 0;
	int is_against_machine = 0;
	int is_machine_turn = 0;
	int is_valid_move = 0;
	char choice;
	int col, row;

	while ( ! has_choose) 
	{
		printf("Você gostaria de jogar contra a máquina? [Y/N]\n");
		scanf(" %c", &choice);

		if (comp_char(choice, 'Y') || comp_char(choice, 'y')) 
		{
			is_against_machine = 1;
			has_choose = 1;
		}
		else if (comp_char(choice, 'N') || comp_char(choice, 'n')) 
		{
			has_choose = 1;
		}
	}
	
	while ( ! has_end) 
	{
		h(&b);
		print_board(b);

		if ( ! is_against_machine && ! is_machine_turn) 
		{
			if (who_is == X) 
			{
				printf("Vez do jogador X:\n");
			}
			else 
			{
				printf("Vez do jogador O:\n");
			}

			while ( ! is_valid_move) 
			{			
				printf("linha: ");
				scanf(" %d", &row);

				printf("Coluna: ");
				scanf(" %d", &col);

				is_valid_move = check_move(&b, row, col);

				if ( ! is_valid_move) 
				{
					printf("O movimento %d,%d não é válido.\n", row, col);
				}
			}

			is_valid_move = 0;

			b.places[row][col] = who_is;


			if (is_against_machine) 
			{
				is_machine_turn = 1;
			}
		} 
		else 
		{
			comp_move = next(b);

			b.places[comp_move.row][comp_move.col] = O;

			is_machine_turn = 0;
		}

		winner = has_winner(b);

		if (winner != E) 
		{
			print_board(b);

			if (winner == X) 
			{
				printf("O jogador X venceu!\n");
			}
			else 
			{
				printf("O jogador O venceu!\n");
			}

			has_end = 1;
		}

		if (who_is == X) 
		{
			who_is = O;
		}
		else 
		{
			who_is = X;
		}

		b.round += 1;

		if ( ! has_end && is_velha(b)) 
		{
			has_choose = 0;

			while ( ! has_choose) 
			{
				printf("Deu Velha! Deseja repetir? [Y/N]\n");

				scanf(" %c", &choice);

				if (comp_char(choice, 'Y') || comp_char(choice, 'y')) 
				{
					b = new_board();
					has_choose = 1;
				}
				else if (comp_char(choice, 'N') || comp_char(choice, 'n')) 
				{
					has_end = 1;
					has_choose = 1;
				}
			}
		}

	}

	printf("Fim!\n");
}