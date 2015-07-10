
/*
   Copyright (c) 2015 Neil Babson and Gabe Stauth
   This source file is protected under the MIT License. 
   Please see the file LICENSE in the distribution for license terms.
*/

/*
   This file contains the header file for the gameBoard class which
   implements a Go board, allows legal moves, and remembers captures.
*/
#ifndef board_h
#define board_h

#include "stack.h"




class gameBoard
{
   public:
      gameBoard();
      int move(int x, int y, int &color);
      void draw();
      void undo();
   private:      
      int addGroup(int x, int y, int color);
      int checkNeighbor(int x, int y, int color);
      int checkLiberties(int x, int y, int color);
      void killGroup(int groupNum, int color);
      int numberGroup(int x, int y, int color);
      void resetFlags();
      
      stack history;           // Stack containing a history of moves played
      piece pieces[11][11];    // 9 x 9 board with an edge frame. 0=empty, 1=black,2=white,3=edge
      int undoFlag;            // true if the last board operation was not an undo
      int numString;           // The number that will be assigned  to the next string (connected group) formed
      bool koFlag;             // true if the next move is restricted due to ko
      int koMove[2];  // 0 = x, 1 = y
      int dead[2];   // 0 = black, 1=white
};

#endif
