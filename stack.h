/*
   Copyright (c) 2015 Neil Babson and Gabe Stauth
   This source file is protected under the MIT License. 
   Please see the file LICENSE in the distribution for license terms.
*/

/*
   This file contains the header file for a stack which is used
   to hold board positions so that the progress of a 9x9 go game can
   be recorded.
*/
#include <time.h>        // time, clock
#include <stdlib.h>      // abs()
#include <iostream>
using namespace std;


struct piece
{
   int color; // 0 = empty, 1 = black, 2 = white, 3 = border
   int groupNum;
   bool visited;
};

struct node
{
   piece board[11][11];
   node* next;
   bool koFlag;
   int koMove[2];
};

class stack
{
   public:
      stack();
      ~stack();
      void push(piece board[][11], bool koFlag, int koMove[]);
      void pop(piece board[][11], bool &koFlag, int koMove[]);
   private:
      node* top;
};




