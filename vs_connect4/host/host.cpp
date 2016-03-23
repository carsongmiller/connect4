#include "stdafx.h"
#include "minimax_AI.h"
#include "con4_util.h"

using namespace std;

int main()
{
	srand(time(0));

	int board[h_][w_];		//the main board
	bool newGame = true;	//whether to start a new game
	int endOfGame;			//6 = yes, 7 = no
	int rowPlayed = 0;		//stores the row in which the last disc was played
	int colPlayed = 0;		//stores the column in which the last disc was played
	int whosTurn;			//keeps track of who's turn it is (1 = player1/human, 2 = player2/computer)
	int turn;				//keeps track of how mant discs have been played
	int MAX_DEPTH;			//max depth of recursion for minimax
	int winner = 0;			//winner at end of game
	/*
	int board[h_][w_] = {
		{ 0, 0, 2, 0, 0 },
		{ 0, 1, 1, 0, 0 },
		{ 0, 2, 1, 0, 0 },
		{ 0, 2, 1, 2, 0 }
	};
	*/

	while (newGame)
	{
		boardInit(board);
		system("cls");
		whosTurn = 1;
		printScreen(board);

		turn = 0;

		while (turn < w_*h_)
		{
			if (turn < 2) MAX_DEPTH = 4;
			else if (MAX_DEPTH > w_*h_ - turn) MAX_DEPTH = w_*h_ - turn;
			else MAX_DEPTH = 7;

			cout << "Press enter to continue";
			//cin.ignore();
			
			if (whosTurn == 1) //player 1, corresponds to pDisc
				minimaxTurn(board, rowPlayed, colPlayed, turn, MAX_DEPTH, whosTurn);

			else if (whosTurn == 2) //player 2, corresponds to cDisc
				minimaxTurn(board, rowPlayed, colPlayed, turn, MAX_DEPTH, whosTurn);

			printScreen(board);
			cout << "PLAYED: ";
			printColor(colPlayed + 1, GAME_COLOR);
			cout << "\n\n";

			if (winDetect(board, rowPlayed, colPlayed, whosTurn))
			{
				winner = whosTurn;
				break;
			}
			//Sleep(1000);

			if (whosTurn == 1) whosTurn = 2;
			else whosTurn = 1;

			turn++;
			if (turn >= w_*h_) break;
		}

		//Displaying an end game message box
		switch (winner)
		{
		case 0:
			endOfGame = MessageBox(NULL, TEXT("Cat's Game!\n\nPlay again?"), TEXT("End of Game"), MB_YESNO);
			break;

		case 1:
			endOfGame = MessageBox(NULL, TEXT("Player 1 Wins!\n\nPlay again?"), TEXT("End of Game"), MB_YESNO);
			break;

		case 2:
			endOfGame = MessageBox(NULL, TEXT("Player 2 Wins!\n\nPlay again?"), TEXT("End of Game"), MB_YESNO);
			break;
		}

		if (endOfGame == 6) newGame = true;
		else if (endOfGame == 7) newGame = false;
	}
}