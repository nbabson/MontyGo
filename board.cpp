/*
   Copyright (c) 2015 Neil Babson and Gabe Stauth
   This source file is protected under the MIT License. 
   Please see the file LICENSE in the distribution for license terms.
*/

/*
   This file contains a 'draw' function which takes a 2D int array as
   argument to represent a game position, and draws the board.  The 
   array represents empty spaces with 0s, black pieces with 1s, and white 
   pieces with 2s.

   The 'main' function in this file is used to test the board, and
   records game positions on a stack to allow undos.
*/
#include "stack.h"
using namespace std;

void draw(int board[][9]);
bool again();

int main()
{
   char move[3];
   int color = 0;
   int board[9][9];
   stack history;
   bool undoFlag = true;

   for (int i = 0; i < 9; ++i)
      for(int j = 0; j < 9; ++j) 
	 board[i][j] = 0;
   history.push(board);
   do
   {
      draw(board);
      cout << "Enter a move (i.e. g3, p=pass, u=undo, q=quit) : ";
      cin.get(move, 10, '\n');
      cin.ignore(10, '\n');
      if (move[0] == 'u')
      {
         history.pop(board);
         if (undoFlag)
	    (history.pop(board));
         undoFlag = false;
      }
      else
      {
	 if (move[0] != 'p' && move[0] != 'q')
	 {
            board[((int)move[0]) - 97][((int)move[1]) - 49] = color + 1;
            undoFlag = true;
	 }
	 history.push(board);
      }
      color = (color + 1) % 2;
   }
   while (move[0] != 'q');
   return 0;
}

bool again()
{
   char ans;
   cout << "Again? (y or n) ";
   cin >> ans;
   cin.ignore(100, '\n');   
   if (ans == 'y')
      return true;
   return false;
}

void draw(int board[][9])
{
   cout << "\033[35;7m";
   cout << "  1 2 3 4 5 6 7 8 9  \n ";
   for (int i = 0; i < 20; ++i)
      cout << (char) 128;
   for (int i = 0; i < 9; ++i)
   {

      cout << endl << (char) (i + 97) << (char) 128;
      for (int j = 0; j < 9; ++j)
      {
	 if (board[i][j] == 0)
	    cout << ". ";
	 else if (board[i][j] == 1)
	    cout << "\033[40m0 ";
	 else 
	    cout << "\033[47m0\033[40;7m ";
      }
      cout << (char) 128;
   }
   cout << "\n ";
   for (int i = 0; i < 20; ++i)
      cout << (char) 128;
   cout << endl << endl << "\033[0m";
}

