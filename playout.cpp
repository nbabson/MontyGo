
/*
   Copyright (c) 2015 Neil Babson and Gabe Stauth
   This source file is protected under the MIT License. 
   Please see the file LICENSE in the distribution for license terms.
*/

/* This file contains the following functions:

   int gameBoard::tryMove(int firstx, int firsty, moves_t moves[])
   tryMoves takes the x and y coordinates of a possible move for 
   black and returns -1 if this move is an illegal play.  Otherwise
   randomGame() is called 1000 times to perform random playouts
   from this starting move, recording the win or loss information in
   the moves array argument.  The number of wins for black from the
   starting move is returned.

   int gameBoard::checkMoves()
   chceckMoves() calls tryMove() to perform a playout on each board
   position as well as passing.  The most successfull move is then
   made.  If a move is made 1 is returned. -1 is returned if the 
   computer chose to pass.

   void dumbGame()
   This function manages a game between a human player and the brute
   force algorithm that plays 1000 random playouts at each open
   board position every turn to find a move.

   void gameBoard::copy(gameBoard * to_copy)
   Copies the position on one game board onto another one,

   void gameBoard::removeDead()
   This function enlists the human players help to decied if each group
   of contiguous pieces is alive or dead.

   void gameBoard::displayGroup(int groupNum)
   This display function takes a group number as argument and displays
   the board with all pieces in that group shown as *s.
*/


#include "board.h"

int gameBoard::tryMove(int firstx, int firsty, moves_t moves[])
{
   int wins = 0;
   int color = 1;
   gameBoard testBoard;

   testBoard.copy(this);
   if (firstx && !testBoard.move(firstx, firsty, color))
	 return -1;
   for (int i = 0; i < 1000; ++i)
   {
      if (randomGame(testBoard, moves[i], -1, false))
      {
         ++wins;
	 moves[i].win = true;
      }
      else
	 moves[i].win = false;
   }
   return wins;

}


int gameBoard::checkMoves()
{
   moves_t moves[1000];
   int score, bestScore = 0;
   int besty, bestx;
   int color = 1;

   // Try passing
   bestScore = tryMove(0, 0, moves);
   bestx = 0;
   besty = 0;
   
   undoOn = false;
   for (int i = 1; i < 10; ++i)
   {
      for (int j = 1; j < 10; ++j)
      {
	 score = tryMove(i, j, moves);
	 if (score > bestScore)
	 {
	    bestScore = score;
	    bestx = i;
	    besty = j;
	 }
      }
   }
   if (bestx)
   {
      undoOn = true;
      this -> move(bestx, besty, color);
      cout << (char) (bestx + 96) << besty << endl;
      return 1;
   }
   else 
      koFlag = false;
   return 0;
}




void dumbGame()
{
   gameBoard board;
   char move[3];
   int color = 1;
   bool passFlag = false; 
   int choice;
   
   move[0] = '0';
   do
   {
      board.draw();
      if (color == 1)
      {
	 if (!board.checkMoves())
	 {
	    if (passFlag)
	       move[0] = 'q';
	    cout << "Computer pass\n";
	    passFlag = true;
	 }
      }
      else
      {
	 cout << "Enter a move (i.e. g3, p=pass, u=undo, q=quit) : ";
	 cin.get(move, 10, '\n');
	 if (cin.fail())
	    cin.clear();
	 cin.ignore(10, '\n');
	 if (move[0] == 'u')    
	    board.undoAgainstAI(color);
	 else if (move[0] == 'p')
	 {
	    if (passFlag)
	       move[0] = 'q';
	    passFlag = true;
	 }
	 else if (move[0] != 'q')
	 {
	    if (!board.move((int)move[0] - 96,(int)move[1] - 48, color))
	    {
	       cout << "Invalid move\n";
	       color *= -1;
	    }
	    else
	       passFlag = false;
	 }
      }
      color *= -1;
   }
   while (move[0] != 'q');
   cout << "Game over\n";
   board.removeDead();
   board.score(true);
}


void gameBoard::undoAgainstAI(int &color)
{
   history.pop(pieces, koFlag, koMove); 
   history.pop(pieces, koFlag, koMove); 
   color *= -1;
}

void gameBoard::copy(gameBoard * to_copy)
{
   for (int i = 1; i < 10; ++i)
      for (int j = 1; j < 10; ++j)
	 this -> pieces[i][j] = to_copy -> pieces[i][j];
   koFlag = to_copy -> koFlag;
   koMove[0] = to_copy -> koMove[0];
   koMove[1] = to_copy -> koMove[1];
   undoOn = to_copy -> undoOn;
}


void gameBoard::removeDead()
{
   bool groups[numString + 1];
   char ans;

   for (int i = 1; i <=numString; ++i)
      groups[numString] = false;
   
   for (int i = 1 ; i < 10; ++i)
   {
      for (int j = 1; j < 10; ++j)
      {
         if (pieces[i][j].color && !groups[pieces[i][j].groupNum])
	 {
            groups[pieces[i][j].groupNum] = true;
	    displayGroup(pieces[i][j].groupNum);
	    do
	    {
	       cout << "Human, is the marked group alive? (y or n) : ";
	       cin >> ans;
	       if (cin.fail())
	          cin.clear();
	       cin.ignore(100, '\n');
	    }
	    while (ans != 'y' && ans != 'n');
	    if (ans == 'n')
               killGroup(pieces[i][j].groupNum, pieces[i][j].color);
	 }
      }
   }
}


// Displays a game position.
void gameBoard::displayGroup(int groupNum)
{
   cout << "\033[35;7m";
   cout << "  1 2 3 4 5 6 7 8 9  \n ";
   for (int i = 0; i < 20; ++i)
      cout << (char) 128;
   for (int i = 1; i < 10; ++i)
   {
      cout << endl << (char) (i + 96) << (char) 128;
      for (int j = 1; j < 10; ++j)
      {
	 if (pieces[i][j].color == 0)
	    cout << ". ";
	 else if (pieces[i][j].color == 1)
	 {
	    if (pieces[i][j].groupNum == groupNum)
	       cout << "\033[40m* ";
	    else
	       cout << "\033[40m0 ";
	 }
	 else
	 {
	    if (pieces[i][j].groupNum == groupNum)
	       cout << "\033[47m*\033[40;7m ";
	    else
	       cout << "\033[47m0\033[40;7m ";
	 }
      }
      cout << (char) 128;
   }
   cout << "\n ";
   for (int i = 0; i < 20; ++i)
      cout << (char) 128;
   cout << endl << endl << "\033[0m";
}

