#include "brain.h"
#include "board.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include <cmath>

using namespace std;


const int w_ = 7, h_ = 6;
const int MAX_DEPTH = 4;


int main()
{
	bool cont = true, validMove, newGame = true; //determines whether the game should continue
	char whoFirst; //who will go first
	int moveChoice; //human player's move choice
	char yn; //takes input from player as to whether they want to play again
	int rowPlayed; //stores the row in which the last disc was played
	int colPlayed; //stores the column in which the last disc was played
	int turn; //keeps track of which turn # it is
	int tempScore = 0;

	Board board(h_, w_);
	Brain brain(h_, w_, MAX_DEPTH);

	const int BLACK = 0;
	const int BLUE = 1;
	const int GREEN = 2;
	const int CYAN = 3;
	const int RED = 4;
	const int MAGENTA = 5;
	const int BROWN = 6;
	const int LIGHTGRAY = 7;
	const int DARKGRAY = 8;
	const int LIGHTBLUE = 9;
	const int LIGHTGREEN = 10;
	const int LIGHTCYAN = 11;
	const int LIGHTRED = 12;
	const int LIGHTMAGENTA = 13;
	const int YELLOW = 14;
	const int WHITE = 15;

	while (newGame)
	{
		turn = 0;


		board.boardInit();
		brain.scoreReset();

		//First output of the program:
		system("cls");

		cout << "CONNECT 4!\n\nWho should go first? (m for me, c for computer): ";
		cin >> whoFirst;

		if (whoFirst == 'm' || whoFirst == 'M')
		{
			board.printScreen();
			board.playerMove(turn, rowPlayed, colPlayed);
		}

		//MAIN GAME LOOP

		while (cont)
		{
			board.printScreen(brain);

			brain.scoreReset();
			brain.rankedScoreReset();

			cout << "Now its' the computer's turn\n\n";
			cout << "thinking ...\n\n";

			brain.minimax(board, tempScore, board.getDisc('c'), board.getDisc('c'), 0, turn);

			brain.rankScores();


			cout << endl << endl;


			//now the computer will make its move
			for (int i = 0; i < w_; i++)
			{
				rowPlayed = board.playMove(brain.getRankedScore(i), board.getDisc('c'), turn);
				if (rowPlayed != -1)
				{
					colPlayed = brain.getRankedScore(i);
					break;
				}
			}

			board.printScreen(brain);

			cout << "The computer played in column ";
			board.printColor(colPlayed + 1, GREEN);
			cout << "\n";


			if (brain.winDetect(board, rowPlayed, colPlayed, board.getDisc('c')))
			{
				cout << "\nThe computer wins!\n";
				break;
			}


			//now the player's turn
			board.playerMove(turn, rowPlayed, colPlayed);

			board.printScreen(brain);

			//detecting a player win	
			if (brain.winDetect(board, rowPlayed, colPlayed - 1, board.getDisc('p')))
			{
				cout << "\nYou win!\n";
				break;
			}

		}

		cout << "Would you like to play again? (y/n): ";
		cin >> yn;
		if (yn != 'y' && yn != 'Y')
			newGame = false;
	}

	return 0;
}