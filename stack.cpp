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

void stack::push(piece board[][11], bool koFlag, int koMove[])
{
   node* temp = new node;
   for (int i = 0; i < 11; ++i)
      for (int j = 0; j < 11; ++j)
      {
	 temp -> board[i][j].color = board[i][j].color;
	 temp -> board[i][j].groupNum = board[i][j].groupNum;
      }
   temp -> next = top;
   top = temp;
   temp -> koFlag = koFlag;
   if (koFlag)
   {
      temp -> koMove[0] = koMove[0];
      temp -> koMove[1] = koMove[1];
   }
}


void  stack::pop(piece board[][11], bool &koFlag, int koMove[])
{
   node* temp = top;
   
   if (!top)
      return;
   for (int i = 0; i < 11; ++i)
      for (int j = 0; j < 11; ++j)
      {
	 board[i][j].color = temp -> board[i][j].color;
	 board[i][j].groupNum = temp -> board[i][j].groupNum;
      }
   koFlag = temp -> koFlag;
   if (koFlag)
   {
      koMove[0] = temp -> koMove[0];
      koMove[1] = temp -> koMove[1];
   }
   top = temp -> next;
   delete temp;
}

      

