#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 3
#define VICTORY_COUNT 3

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

/**
 * Verifica se há um vencedor no jogo e o retorna.
 */
Player has_winner(Board b) 
{
	int i, j, k, m, n;
	int has_won_col, has_won_row, has_won_d, has_won_ad;
	Player lc_p, mid_p;

	// Espaço disponível para buscar do meio do jogo
	int disp = VICTORY_COUNT - 2;
	// Tamnanho do jogo nXn
	int m_size = BOARD_SIZE - (VICTORY_COUNT - 1);

	// Percorre o jogo a matriz dentro do jogo, isto é, eliminando as
	// bordas da tabela, com uma matriz de mXm, onde m é o número de
	// espaços consecutivos necessários para ganhar o jogo
	for (i = 1, j = 1; i <= m_size; j += 1) 
	{
		// Jogador na posição central da matriz de busca
		mid_p = b.places[i][j];
		has_won_d = 1;

		for (k = 1; k <= disp; k += 1) 
		{
			// Checa as diagonais
			if (b.places[i - k][j - k] != mid_p
			 || b.places[i + k][j + k] != mid_p) 
			{
				has_won_d = 0;
			}
		}

		if (has_won_d) 
		{
			return mid_p;
		}

		has_won_ad = 1;

		for (k = 1; k <= disp; k += 1) 
		{
			// Checa as antidiagonais
			if (b.places[i + k][j - k] != mid_p
			 || b.places[i - k][j + k] != mid_p) 
			{
				has_won_ad = 0;
			}
		}

		if (has_won_ad) 
		{
			return mid_p;
		}

		for (k = i - disp, m = j - disp; k <= i + disp; k += 1, m += 1) 
		{
			// Jogador no canto da matriz 
			lc_p = b.places[k][m];

			has_won_row = 1;
			has_won_col = 1;

			for (n = 1; n <= disp + 1; n += 1) 
			{
				if (lc_p != b.places[k][m + n])
				{
					has_won_row = 0;
				}

				if (lc_p != b.places[k + n][m]) 
				{
					has_won_col = 0;
				}
			}

			if (has_won_row) 
			{
				return lc_p;
			}

			if (has_won_col) 
			{
				return lc_p;
			}
		}


		if (j == BOARD_SIZE - 1) 
		{
			i += 1;
			j = 1;
		}
	}

	return E;
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