/*
 * Copyright (C) 2014  Elias Rodrigues <https://github.com/elias19r>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>

/**
 * Inicializa os discos da torre 0
 * Valores n, n-1, n-2, ..., 2, 1 para enumerar os discos de acordo com o seu tamanho
 * Valor 0 significa nenhum disco (ja eh definido nas torres pois usa-se calloc)
 */
void init(int **towers, int n)
{
	int i;

	for (i = 0; i < n; i++)
		towers[0][i] = n - i;
}

/**
 * Retorna o indice da posicao do disco do topo de uma torre i
 */
int top_pos(int **towers, int n, int i)
{
	int j;

	for (j = n - 1; j >= 0; j--)
		if (towers[i][j] != 0)
			return j;

	return 0;
}

/**
 * Retorna o numero do disco que esta no topo de uma torre i
 */
int top(int **towers, int n, int i)
{
	return towers[i][top_pos(towers, n, i)];
}

/**
 * Retorna o indice da torre faltante
 * Exemplos: torres 1, 2 => faltante eh a torre 0
 *           torres 0, 2 => faltante eh a torre 1
 *           torres 1, 0 => faltante eh a torre 2
 */
int missing(int tower_a, int tower_b)
{
	return 3 - (tower_a + tower_b);
}

/**
 * Retorna a letra de uma torre i
 */
char letter(int i)
{
	if (i == 0)
		return 'A';

	if (i == 1)
		return 'B';

	return 'C';
}

/**
 * Desenha na tela a resolucao de um movimento
 */
void draw(int disk, int src, int dest)
{
	printf("Move disk (%d) from tower %c to tower %c\n", disk, letter(src),
	       letter(dest));
}

/**
 * Move o disco do topo da tower_a para o topo da tower_b
 * e imprime resolucao na tela
 */
void move(int **towers, int n, int tower_a, int tower_b)
{
	int top_a, top_b, pos_a, pos_b;

	pos_a = top_pos(towers, n, tower_a);
	pos_b = top_pos(towers, n, tower_b);

	top_a = towers[tower_a][pos_a];
	top_b = towers[tower_b][pos_b];

	if (top_b == 0)
		pos_b = -1;	// Esta linha eh para o caso de tower_b estar vazia

	// Troca disco
	towers[tower_b][pos_b + 1] = top_a;
	towers[tower_a][pos_a] = 0;

	// Desenha a solucao na tela
	draw(top_a, tower_a, tower_b);
}

/**
 * Funcao principal do programa
 */
int main(int argc, char **argv)
{
	int n;			// a quantidade de discos
	int *towers[3];		// as tres torres
	int src, dest;		// torre de origem e destino
	int tower_a, tower_b;	// torres candidatas para serem origem e destino em um movimento
	int top_a, top_b;	// discos das torres candidatas a origem e destino
	int current, recent;	// current: torre que esta o disco 1; recent: torre em que o disco 1 estava antes
	int i;			// variavel auxiliar
	unsigned long long turn, total = 0;	// turn: numero da jogada atual; total: total de jogadas

	/**
	 * IMPORTANTE: o disco 1 funciona como pivo.
	 * Sua posicao eh sempre referencia para os movimentos!
	 */

	// Recebe a quantidade de discos
	printf("Tower of Hanoi\n\nAmount of disks: ");
	if (argc > 1) {
		n = atoi(argv[1]);
		printf("%d\n", n);
	} else
		scanf("%d", &n);

	// Aloca as tres torres
	for (i = 0; i < 3; i++)
		towers[i] = (int *)calloc(n, sizeof(int));

	// Inicializa a torre 0 com os discos
	init(towers, n);

	// Calcula o total de jogadas
	if (n > 0 && n < 65) {
		for (total = 1, i = 0; i < n - 1; i++)
			total *= 2;
		total += total - 1;
	} else if (n >= 65)
		printf("ERROR: Unable to calculate\n\n");

	printf("Total moves: %llu\n\n", total);

	// Comecando as jogadas
	for (turn = 1; turn <= total; turn++) {

		// Se eh a primeira jogada: tratamento especial
		if (turn == 1) {

			// Confere se a quantidade de disco eh par, pois
			//      toda primeira jogada segue este padrao em funcao da quantidade
			//      de discos ser par ou impar
			if (n % 2 == 0) {
				move(towers, n, 0, 1);
				current = 1;
				recent = 0;
			} else {
				move(towers, n, 0, 2);
				current = 2;
				recent = 0;
			}
		}
		// Demais jogadas
		else {

			// Confere se o numero da jogada eh par, pois
			//      para as demais jogadas, ocorre um padrao
			//      de acordo com ela ser par ou impar
			if (turn % 2 == 0) {

				// Pega as torres candidatas para serem a origem e destino:
				//      as unicas candidatas possiveis sao as torres que nao tem o disco 1
				tower_a = recent;
				tower_b = missing(current, recent);;
				top_a = top(towers, n, tower_a);
				top_b = top(towers, n, tower_b);

				// Define a torre de origem e destino
				// Essas condicoes seguem a regra do jogo:
				//      nenhum disco maior pode sobrepor um disco menor
				if (top_a != 0 && top_b == 0) {
					src = tower_a;
					dest = tower_b;
				} else if (top_b != 0 && top_a == 0) {
					src = tower_b;
					dest = tower_a;
				} else if (top_a < top_b) {
					src = tower_a;
					dest = tower_b;
				} else {
					src = tower_b;
					dest = tower_a;
				}

				// Realiza o movimento
				move(towers, n, src, dest);

				// Movimento padrao para as jogadas impares
			} else {
				move(towers, n, current,
				     missing(current, recent));
				i = recent;
				recent = current;
				current = missing(current, i);
			}
		}

	}

	// Liberando as tres torres
	for (i = 0; i < 3; i++)
		free(towers[i]);
	printf("\nEnd.\n");

	return 0;
}
