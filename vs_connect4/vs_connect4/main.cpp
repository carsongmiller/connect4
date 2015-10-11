//connect4
//Carson Miller
//10-7-2015


/*

Things to modify:
-fix winDetect() (I think it's working now)
-shallower recursion beginning, allow deeper as game goes on
-weigh certain situations at certain situations in the game more heavily
-when the board is symmetrical, the scores for each column should be symmetrical
-consider adding a small bit of randomness into minimax
-like if two options are both very good and within %20 of eachother or something
-add a chance that the "worse" one is chosen
-weigh scores better
-consider: run through minimax until the first instance of a win is found, then go some certain number of iterations deeper
-this would be overwritten by logic that makes minimax go shallower at the beginning

*/

#include "con4.h"

using namespace std;

int main()
{
	bool cont = true, validMove, newGame = true; //determines whether the game should continue
	char whoFirst; //who will go first
	int moveChoice; //human player's move choice
	int score[w_]; //stores the scores of each column (handed to minimax())
	int bestScore; //stores the number of the column with the best score
	char yn; //takes input from player as to whether they want to play again

	int w_ = 7, h_ = 6;

	Board board(w_, h_);

	while (newGame)
	{
		//First output of the program:
		system("cls");

		cout << "CONNECT 4!\n\nWho should go first? (m for me, c for computer): ";
		cin >> whoFirst;

		if (whoFirst == 'm' || whoFirst == 'M')
		{
			board.printBoard();

			validMove = false;
			while (!validMove)
			{
				cout << "Where would you like to go? (enter column number): ";
				cin >> moveChoice;

				if (!board.playMove(moveChoice - 1, pDisc))
					cout << "There is no space in that column, choose a different one\n\n";

				if (moveChoice > w_)
					cout << "There is no column " << moveChoice << ". Please choose a column between 1 and " << w_ << "\n\n";

				else
					validMove = true;
			}
		}

		system("cls");
		cout << "CONNECT 4!\n\n\n";
		board.printBoard();

		//MAIN GAME LOOP

		while (cont)
		{
			//reset the score of each column to 0 at the beginning of each turn
			for (int i = 0; i < w_; i++)
				score[i] = 0;

			cout << "Now its' the computer's turn\n";
			cout << "\nthinking ...\n\n";

			minimax(board, score, cDisc, cDisc, 0);


			cout << endl << endl;

			bestScore = 0;

			for (int i = 1; i < w_; i++)
			{
				if (score[i] > score[bestScore])
					bestScore = i;
			}

			//now the computer will make its move

			playMove(board, bestScore, cDisc);

			system("cls");
			cout << "CONNECT 4!\n\n\n";

			//printing the score array for debugging
			for (int i = 0; i < w_; i++)
				cout << score[i] << "\t";

			cout << "\n";

			printBoard(board);

			if (winDetect(board, cDisc))
			{
				cout << "\nThe computer wins!\n";
				break;
			}

			//now the player's turn
			//Player chosing his move
			validMove = false;
			while (!validMove)
			{
				cout << "Where would you like to go? (enter column number): ";
				cin >> moveChoice;

				if (!playMove(board, moveChoice - 1, pDisc))
					cout << "There is no space in that column, choose a different one\n";
				else
					validMove = true;
			}

			system("cls");
			cout << "CONNECT 4!\n\n\n";
			printBoard(board);

			//detecting a player win	
			if (winDetect(board, pDisc))
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