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
#include <iostream>
using namespace std;

struct node
{
   int board[9][9];
   node* next;
};

class stack
{
   public:
      stack();
      ~stack();
      void push(int board[][9]);
      void pop(int board[][9]);
   private:
      node* top;
};



