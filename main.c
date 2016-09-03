#include <stdio.h>

#define BOARD_SIZE 3

typedef enum {
	X, // Jogador 1 
	O, // Jogador 2 ou IA
	E  // Vazio (Empty)
} Player;

typedef struct 
{
	Player places[BOARD_SIZE][BOARD_SIZE];
	unsigned int round;
} Board;

Board new_board() 
{
	Board b;
	int i, j;

	b.round = 1;

	for (i = 0; i < BOARD_SIZE; i += 1) 
	{
		for (j = 0; j < BOARD_SIZE; j += 1) 
		{
			b.places[i][j] = E;
		}
	}

	return b;
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

	printf("Round: %d\n\n", b.round);

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

Player has_winner(Board b) 
{
	int i, j, has_won;
	Player aux;
	Player* row;

	for (i = 0; i < BOARD_SIZE; i += 1) 
	{
		row = b.places[i];
		aux = row[0];
		has_won = 1;

		for (j = 1; j < BOARD_SIZE; j += 1) 
		{
			if (row[j] != aux) 
			{
				has_won = 0;
			}
		}

		if (has_won) 
		{
			return aux;
		}
	}

	for (i = 1; i < BOARD_SIZE; i += 1) 
	{
		aux = b.places[0][i];
		has_won = 1;

		for (j = 1; j < BOARD_SIZE; j += 1) 
		{
			if (b.places[i][j] != aux) 
			{
				has_won = 0;
			}
		}

		if (has_won) 
		{
			return aux;
		}
	}


	return E;
}

int main(void) 
{
	Board b = new_board();
	Player who_is = X;
	Player winner = E;
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

			if (is_against_machine) 
			{
				is_machine_turn = 1;
			}
		} 
		else 
		{
			// Machinesss!!

			is_machine_turn = 0;
		}

		b.round += 1; 
	}

	printf("Fim!\n");
}