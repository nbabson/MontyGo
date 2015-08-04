
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

const int PLAYOUTS = 100;
const bool DEBUG = false;

//Not currently used
struct moves_t
{
   int x[4];
   int y[4];
   bool win;
};


struct move_t
{
   long wins;
   int x;
   int y;
   move_t():x(0), y(0), wins(0){}
};


struct MCnode;

class gameBoard
{
   public:
      gameBoard();
      gameBoard(gameBoard*);
      int move(int x, int y, int color);
      void draw();
      void undoAgainstAI(int &color);  //Undo computer and human player's last moves
      void undo();
      bool score(bool display);
      int tryMove(int firstx, int firsty, moves_t moves[]);
      int checkMoves();
      void removeDead();
      int MCmove();
   private:      
      int addGroup(int x, int y, int color);
      int checkNeighbor(int x, int y, int color);
      int checkLiberties(int x, int y, int color);
      void killGroup(int groupNum, int color);
      int numberGroup(int x, int y, int color);
      void resetFlags();
      void drawScored();
      void copy(gameBoard * to_copy);
      void displayGroup(int groupNum);
      void MCcheckMoves(MCnode* n, int depth);
      int MCtryMove(int firstx, int firsty, int color);
      bool MCrandomGame(gameBoard board, int color, bool display);

      bool undoOn;
      stack history;           // Stack containing a history of moves played
      piece pieces[11][11];    // 9 x 9 board with an edge frame. 0=empty, 1=black,-1=white,3=edge
      int numString;           // The number that will be assigned  to the next string (connected group) formed
      bool koFlag;             // true if the next move is restricted due to ko
      int koMove[2];  // 0 = x, 1 = y
      int dead[2];   // 0 = black, 1=white
};


struct MCnode
{
   MCnode * parent;
   long games;
   MCnode * next[4];  // Best four moves are children nodes 
   gameBoard * board; // Current board
   int color;
   move_t  move; 
   MCnode():parent(0)
   {
      next[0] = next[1] = next[2] = next[3] = 0;
      board = new gameBoard;
   }
   MCnode(gameBoard* to_copy):parent(0)
   {
      next[0] = next[1] = next[2] = next[3] = 0;
      board = new gameBoard(to_copy);
   }
};


void MCgame();
void deleteTree(MCnode * n);
bool randomGame(gameBoard board, moves_t move, int color, bool display);
void dumbGame();

#endif
