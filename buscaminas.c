/*
 * Minesweeper.c
 *
 *  Created on: 17 may 2023
 *      Author: nakal
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

typedef struct {
	char value, visible;
}Cell_t;

typedef struct {
	Cell_t **mat;
	unsigned int dim1, dim2, mines;
}Board_t;

Cell_t* Cell_c1(char value, char visible) {

	//Creates a cell, sets its values and returns a pointer to it.

	Cell_t *self;

	self = malloc(sizeof(Cell_t));
	self->value = value;
	self->visible = visible;

	return self;
}

void Cell_d1(Cell_t *self) {

	//Destroys a cell.

	free(self);
}

Board_t* Board_c1(unsigned int dim1, unsigned int dim2, unsigned int mines, int seed) {

	//Creates a board, sets its values to zero, then randomly sets the mines and returns a pointer to the board.

	Board_t *self;
	unsigned int i, j, k, l;

	self = (Board_t*)malloc(sizeof(Board_t));
	if (self == NULL) {
		return NULL;
	}

	self->dim1 = dim1;
	self->dim2 = dim2;
	self->mines = mines;
	self->mat = malloc(dim1 * sizeof(Cell_t*));
	if (self->mat == NULL) {
		return NULL;
	}

	for (i = 0; i < self->dim1; i++) {
		self->mat[i] = malloc(self->dim2 * sizeof(Cell_t));
		if (self->mat[i] == NULL) {
			for (j = 0; j < i; j++) {
				free(self->mat[j]);
			}
			return NULL;
		}
		for (j = 0; j < self->dim2; j++) {
			self->mat[i][j].value = '0';
			self->mat[i][j].visible = '0';
		}
	}

	for (i = 0; i < self->mines; i++) {
		srand(i * seed);
		k = rand() % self->dim1;
		l = rand() % self->dim2;;
		while (self->mat[k][l].value == '9') {
			k = rand() % self->dim1;;
			l = rand() % self->dim2;;
		}
		self->mat[k][l].value = '9';
	}

	return self;

}

void Board_d1(Board_t *self){

	//Destroys a board.

	unsigned int i;

	if (self != NULL) {
		for (i = 0; i < self->dim1; i++) {
			free(self->mat[i]);
		}
		free(self->mat);
	}

	free(self);
}

int errorsInBoard(Board_t *self) {

	//Checks if there are NULL pointers in Board.

	unsigned int i;

	if (self == NULL) {
		return -1;
	}
	else {
		if (self->mat == NULL) {
			return -2;
		}
		else {
			for (i = 0; i < self->dim1; i++) {
				if (self->mat[i] == NULL) {
					return -3;
				}
			}
			return 0;
		}
	}
}

int checkMine(Board_t *self, unsigned int i, unsigned int j) {

	//Check if there is a mine in the selected cell. Return 0 if not, 1 if yes and a negative value if there are any errors.

	if (!errorsInBoard(self)) {
		if (i < self->dim1 && j < self->dim2){
			if (self->mat[i][j].value == '9') {
				return 1;
			}
			else {
				return 0;
			}
		}
		else {
			return -2;
		}
	}
	else {
		return -1;
	}
}

void setCell(Board_t *self, unsigned int i, unsigned int j, char value, char visible) {

	//Sets the values of a cell inside a board.

	if (!errorsInBoard(self)) {
		if (i < self->dim1 && j < self->dim2) {
			self->mat[i][j].value = value;
			self->mat[i][j].visible = visible;
		}
	}
}

char getCellValue(Cell_t *self) {

	//Returns cell's value.

	//if (self != NULL) {
		return self->value;

}

char getCellVisible(Cell_t *self) {

	//Return cell's visible' value.

	return self->visible;
}

void printBoard(Board_t *self) {

	//Prints a board.

	unsigned int i, j;

	if (!errorsInBoard(self)) {
		printf("  ");
		for (i = 0; i < self->dim1; i++) {
			printf("%d ", i);
		}
		printf("\n");
		for (i = 0; i < self->dim1; i++) {
			printf("%d ", i);
			for (j = 0; j < self->dim2; j++) {
				printf("%c ", self->mat[i][j].value);
			}
			printf("\n");
		}
	}
}

void setBoard(Board_t *self) {

	//Sets the values of cells which have a mine nearby.

	unsigned int i, j;

	if (!errorsInBoard(self)) {
		for (i = 0; i < self->dim1; i++) {
			for (j = 0; j < self->dim2; j++) {
				if (self->mat[i][j].value == '9') {
					if (i - 1 < self->dim1 && j - 1 < self->dim2 && !checkMine(self, i - 1, j - 1)) {
						setCell(self, i - 1, j - 1, (char)((int)(self->mat[i - 1][j - 1].value) + 1), '0');
					}
					if (i - 1 < self->dim1 && !checkMine(self, i - 1, j)) {
						setCell(self, i - 1, j, (char)((int)(self->mat[i - 1][j].value) + 1), '0');
					}
					if (i - 1 < self->dim1 && j + 1 < self->dim2 && !checkMine(self, i - 1, j + 1)) {
						setCell(self, i - 1, j + 1, (char)((int)(self->mat[i - 1][j + 1].value) + 1), '0');
					}
					if (j - 1 < self->dim2 && !checkMine(self, i, j - 1)) {
						setCell(self, i, j - 1, (char)((int)(self->mat[i][j - 1].value) + 1), '0');
					}
					if (j + 1 < self->dim2 && !checkMine(self, i, j + 1)) {
						setCell(self, i, j + 1, (char)((int)(self->mat[i][j + 1].value) + 1), '0');
					}
					if (i + 1 < self->dim1 && j - 1 < self->dim2 && !checkMine(self, i + 1, j - 1)) {
						setCell(self, i + 1, j - 1, (char)((int)(self->mat[i + 1][j - 1].value) + 1), '0');
					}
					if (i + 1 < self->dim1 && !checkMine(self, i + 1, j)) {
						setCell(self, i + 1, j, (char)((int)(self->mat[i + 1][j].value) + 1), '0');
					}
					if (i + 1 < self->dim1 && j + 1 < self->dim2 && !checkMine(self, i + 1, j + 1)) {
						setCell(self, i + 1, j + 1, (char)((int)(self->mat[i + 1][j + 1].value) + 1), '0');
					}
				}
			}
		}
	}
}

void discoverSurroundingCells(Board_t *self, unsigned int i, unsigned int j) {

	//Discover a cell and its surrounding cells until it hits a cell whose value is not 0.

	if (!errorsInBoard(self)) {
		if (i < self->dim1 && j < self->dim2) {
			if (self->mat[i][j].visible == '0') {
				self->mat[i][j].visible = '1';
				if (self->mat[i][j].value == '0') {
					discoverSurroundingCells(self, i - 1, j - 1);
					discoverSurroundingCells(self, i - 1, j);
					discoverSurroundingCells(self, i - 1, j + 1);
					discoverSurroundingCells(self, i, j - 1);
					discoverSurroundingCells(self, i, j + 1);
					discoverSurroundingCells(self, i + 1, j - 1);
					discoverSurroundingCells(self, i + 1, j);
					discoverSurroundingCells(self, i + 1, j + 1);
				}
			}
		}
	}
}

void printBoardVisible(Board_t *self) {

	//Prints a board.

	unsigned int i, j;

	if (!errorsInBoard(self)) {
		printf("  ");
		for (i = 0; i < self->dim1; i++) {
			printf("%d ", i);
		}
		printf("\n");
		for (i = 0; i < self->dim1; i++) {
			printf("%d ", i);
			for (j = 0; j < self->dim2; j++) {
				if (self->mat[i][j].visible == '1') {
					printf("%c ", self->mat[i][j].value);
				}
				else {
					printf("  ");
				}
			}
			printf("\n");
		}
	}
}

int youWon(Board_t *self) {

	//Returns 1 if all cells that are not visible are mines and 0 otherwise.

	unsigned int i, j;

	if (!errorsInBoard(self)) {
		for (i = 0; i < self->dim1; i++) {
			for (j = 0; j < self->dim2; j++) {
				if (self->mat[i][j].visible == '0' && self->mat[i][j].value != '9') {
					return 0;
				}
			}
		}
		return 1;
	}
	else {
		return -1;
	}
}

int getMove(Board_t *self, unsigned int *x, unsigned int *y) {

	//Reads values from keyboard and returns them in x and y.

	int inputsMatched = 0;

	if (!errorsInBoard(self)) {
		do {
			printf("Select a cell [row].[column]: ");
			fflush(0);
			inputsMatched = scanf("%u.%u", x, y);
		}
		while (inputsMatched != 2 || *x >= self->dim1 || *y >= self->dim2);
		return 0;
	}
	else {
		return -1;
	}

}

int writeBoard(Board_t *self) {

	//Writes the board in the path board.txt

	int fd;

	if (errorsInBoard(self)) {
		return -2;
	}

	fd = open("board.txt", O_WRONLY | O_CREAT | S_IWUSR);
	if (fd == -1) {
		printf("Error opening file.\n");
		return -1;
	}
	write(fd, &self->dim1, sizeof(unsigned int));
	write(fd, &self->dim2, sizeof(unsigned int));
	write(fd, &self->mines, sizeof(unsigned int));
	write(fd, self->mat, self->dim1 * self->dim2 * sizeof(Cell_t));

	if (close(fd) == -1) {
		printf("Error closing file.\n");
		return -3;
	}
	return 0;

}

int readBoard(Board_t *self) {

	//Reads the file board.txt and returns a pointer to the board.

	int fd;

	fd = open("board.txt", O_RDONLY);
	if (fd == -1) {
		printf("Error opening file.\n");
		return -1;
	}

	read(fd, &self->dim1, sizeof(unsigned int));
	read(fd, &self->dim2, sizeof(unsigned int));
	read(fd, &self->mines, sizeof(unsigned int));
	read(fd, self->mat, self->dim1 * self->dim2 * sizeof(Cell_t));

	if (close(fd) == -1) {
		printf("Error closing file.\n");
		return -2;
	}
	return 0;
}


int main() {

	Board_t *self = NULL;
	unsigned int i = 0, j = 0;
	char answer = '\0';

	do {
		printf("Would you like to load the existing game (y) or start a new game (n)? ");
		fflush(0);
	}
	while (scanf("%c", &answer) != 1 || !(answer == 'y' || answer == 'n'));

	if (answer == 'y') {
		if (readBoard(self)) {
			return -2;
		}
		if (errorsInBoard(self)) {
			return -1;
		}
	}
	else {
		self = Board_c1(10, 10, 15, 2000);
		if (errorsInBoard(self)) {
			return -1;
		}
		setBoard(self);
	}


	do {
		printBoardVisible(self);
		getMove(self, &i, &j);
		if (checkMine(self, i, j) == 1) {
			printf("********GAME OVER!********\n");
			break;
		}
		discoverSurroundingCells(self, i, j);
		writeBoard(self);
	}
	while (youWon(self) == 0);

	if (youWon(self) == 1) {
		printf("********YOU WON!********\n");
	}

	printBoard(self);
	Board_d1(self);


	return 0;
}

