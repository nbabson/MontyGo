/*
   Copyright (c) 2015 Neil Babson and Gabe Stauth
   This source file is protected under the MIT License. 
   Please see the file LICENSE in the distribution for license terms.
*/

/*
   This file implements a stack to hold board positions so that the 
   progress of a 9x9 go game can be recorded.
*/
#include "stack.h"

stack::stack()
{
   top = NULL;
}

stack::~stack()
{
   node* temp = top;
   while (top)
   {
      top = top -> next;
      delete temp;
      temp = top;
   }
}

void stack::push(int board[][9])
{
   node* temp = new node;
   for (int i = 0; i < 9; ++i)
      for (int j = 0; j < 9; ++j)
	 temp -> board[i][j] = board[i][j];
   temp -> next = top;
   top = temp;
}


void  stack::pop(int board[][9])
{
   node* temp = top;
   
   if (!top)
      return;
   for (int i = 0; i < 9; ++i)
      for (int j = 0; j < 9; ++j)
	 board[i][j] = temp -> board[i][j];
  top = temp -> next;
  delete temp;
}

      

