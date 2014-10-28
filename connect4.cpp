// Connect 4 Game
// Ryan Saladin
// 10/27/14
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <unistd.h>
using namespace std;

void play(char board[][7], int (*player1)(char board[][7], char player), int (*player2)(char board[][7], char player));
int p1(const char board[][7], char player);
int p2(const char board[][7], char player);
int human(const char board[][7], char player);
bool notFinished(char board[][7]);
bool validMove(char board[][7], int index, char player);
void printBoard(char board[][7]);
char winner(char board[][7]);

int main()
{
	srand(time(0));
	char board[6][7];
	for(int i=0; i<6; i++)
		for(int j=0; j<7; j++)
			board[i][j] = '-';
	if(rand()%1)
		play(board, &human, &human); // change this to play(board, &human, &p1) in addition to below to play against your AI
	else
		play(board, &human, &human); // change this to play(board, &p1, &human) in addition to above to play against your AI

	printBoard(board);
	cout << "Player " << winner(board) << " wins!\n";
	return 0;
}

void play(char board[][7], int (*player1)(const char board[][7], char player), int (*player2)(const char board[][7], char player))
{	char player_turn = 'x', move;
	while(notFinished(board))
	{
		printBoard(board);
		if(player_turn == 'x')
		{
			cout << "Player turn " << player_turn << endl;
			move = (*player1)(board, player_turn);
			if(validMove(board, move-1, player_turn))
				player_turn = 'o';
			else
				cout << "Invalid move\n";
		}
		else
		{
			cout << "Player turn " << player_turn << endl;
			move = (*player2)(board, player_turn);
			if(validMove(board, move-1, player_turn))
				player_turn = 'x';
			else
				cout << "Invalid move\n";
		}
	}
}

int p1(const char board[][7], char player)
{
	// board is the current layout of the board
	// player is the character token of your player (either 'x' or 'o')
	// return the number of the column your AI wants to play in [1-7] and that move will be made
}

int human(const char board[][7], char player)
{
	int move;
	cout << "Enter column you wish to play in [1...7]: ";
	cin >> move;
	return move;
}

// Returns true if the board is not yet full and there are no winners
bool notFinished(char board[][7])
{
	bool boardNotFull = false;
	for(int j=0; j<7; j++)
		if(board[0][j] == '-')
		{
			boardNotFull = true;;
			break;
		}
	return (winner(board) == '-') && boardNotFull;
}

// Attempt to make move for given column, make the move and return true if possible, return false otherwise
bool validMove(char board[][7], int index, char player)
{
	for(int i=5; i>=0; i--)
		if(board[i][index] == '-')
		{
			board[i][index] = player;
			return true;
		}
	return false;
}

// Checks all the possible 4-in-a-row spots for winner, capitalizes winning set and returns that player if winner, returns '-' otherwise
char winner(char board[][7])
{
	char win = '-';
	// Check all horizontals
	for(int i=0; i<6; i++)
		for(int j=0; j<4; j++)
			if(board[i][j] != '-' && board[i][j] == board[i][j+1] &&
									board[i][j] == board[i][j+2] &&
									board[i][j] == board[i][j+3])
			{
				win = board[i][j];
				for(int k=0; k<4; k++)
					board[i][j+k] = toupper(win);	// Capitalize the winning moves
			}
			
	// Check all verticals
	for(int i=0; i<3; i++)
		for(int j=0; j<7; j++)
			if(board[i][j] != '-' && board[i][j] == board[i+1][j] &&
									board[i][j] == board[i+2][j] &&
									board[i][j] == board[i+3][j])
			{
				win = board[i][j];
				for(int k=0; k<4; k++)
					board[i+k][j] = toupper(win);	// Capitalize the winning moves
			}
			
	// Check all back-slash diagonals '\'
	for(int i=0; i<3; i++)
		for(int j=0; j<4; j++)
			if(board[i][j] != '-' && board[i][j] == board[i+1][j+1] &&
									board[i][j] == board[i+2][j+2] &&
									board[i][j] == board[i+3][j+3])
			{
				win = board[i][j];
				for(int k=0; k<4; k++)
					board[i+k][j+k] = toupper(win);	// Capitalize the winning moves
			}
			
	// Check all forward-slash diagonals '/'
	for(int i=0; i<3; i++)
		for(int j=6; j>2; j--)
			if(board[i][j] != '-' && board[i][j] == board[i-1][j-1] &&
									board[i][j] == board[i-2][j-2] &&
									board[i][j] == board[i-3][j-3])
			{
				win = board[i][j];
				for(int k=0; k<4; k++)
					board[i+k][j+k] = toupper(win);	// Capitalize the winning moves
			}
	
	return win;	
}

// Print the board out on the screen, delay added to allow time to see moves when playing AI's
void printBoard(char board[][7])
{
	usleep(100000);
	cout << "\t1 2 3 4 5 6 7\n";
	for(int i=0; i<6; i++)
	{
		for(int j=0; j<7; j++)
		{
			if(!j)
				cout << "\t";
			cout << board[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}
