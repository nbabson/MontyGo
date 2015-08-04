
/*
   Copyright (c) 2015 Neil Babson and Gabe Stauth
   This source file is protected under the MIT License. 
   Please see the file LICENSE in the distribution for license terms.
*/

/*
   This file contains the implementation of the Monte Carlo Search Tree
   which controls the computer AI.
*/

#include "board.h"


//Recursive function that builds the Monte Carlo tree
void gameBoard::MCcheckMoves(MCnode* n, int depth)
{
   move_t temp, bestMoves[4];
   int score, tempi, tempj; 
   if (depth > 3) 
      return;            // Perform tree search to depth of 3
   // Try passing
   bestMoves[3].wins = MCtryMove(0, 0, n -> color);
   
   for (int i = 1; i < 10; ++i)
   {
      for (int j = 1; j < 10; ++j)
      {
	 score = MCtryMove(i, j, n -> color);
	 for (int k = 0; k < 4; ++k)
	 {
	    if (score > bestMoves[k].wins)  // Bubble good moves through array
	    {
	       if (k == 0)
	       {
		  bestMoves[0].wins = score;
		  bestMoves[0].x = i;
		  bestMoves[0].y = j;
	       }
	       else 
	       {
		  temp.wins = bestMoves[k].wins;
		  temp.x = bestMoves[k].x;
		  temp.y = bestMoves[k].y;
		  bestMoves[k].wins = bestMoves[k-1].wins;;
		  bestMoves[k].x = bestMoves[k-1].x;
		  bestMoves[k].y = bestMoves[k-1].y;;
		  bestMoves[k-1].wins = temp.wins;
		  bestMoves[k-1].x = temp.x;
		  bestMoves[k-1].y = temp.y;
	       }
	    }
	 }
/*	 
         cout << "BestMoves\n";
	 for (int i = 0; i < 4; ++i)
	    cout <<bestMoves[i].wins<< "   ";
	 cout << endl;
*/

      }
   }

   // Put four best moves on the tree
   for (int i = 0; i < 4; ++i)
   {
      n -> next[i] = new MCnode;
      n -> next[i] -> move.wins = bestMoves[i].wins;
      n -> next[i] -> move.x = bestMoves[i].x;
      n -> next[i] -> move.y = bestMoves[i].y;
      n -> next[i] -> board -> copy(n -> board);
      n -> next[i] -> board -> move(bestMoves[i].x, bestMoves[i].y, n -> color);
      n -> next[i] -> parent = n;
      n -> next[i] -> games = PLAYOUTS;
      n -> next[i] -> color = -(n -> color);
      n -> games += PLAYOUTS;
      // Keep total of black wins
      if (n -> color == 1)
         n -> move.wins += bestMoves[i].wins;
      if (DEBUG) 
      {
         cout << "Parent's move: " << n->move.x << n->move.y << "      Parents color: " << n->color << "      Parent's wins: " << n->move.wins << endl ;
         cout << "Depth: " << depth << "        Color: " << n->next[i]->color << "      " <<n->next[i]->move.x<<n->next[i]->move.y<< "   My wins: " << n->next[i]->move.wins<<endl;
      }
   }

   // Recursively search children
   ++depth;
   for (int i = 0; i < 4; ++i)
      MCcheckMoves(n -> next[i], depth);

   // Update parent
   if (n -> parent)
   {
      n -> parent -> games += n -> games;
      n -> parent -> move.wins += n -> move.wins;
   }
}
   

//Find and play the most successful move from the second row of the tree
int gameBoard::MCmove()
{
   int bestMove, maxWins = -1;
   MCnode *root = new MCnode(this);
   root -> color = 1;

   undoOn = false;
   MCcheckMoves(root, 1);
   for (int i = 0; i < 4; ++i)
   {
      if (root -> next[i] -> move.wins > maxWins)    
      {
	 maxWins = root -> next[i] -> move.wins;           
         bestMove = i;
      }
      if (DEBUG)
         cout << "Children: " << root -> next[i] -> move.wins << "    " << root->next[i]->move.x<<root->next[i]->move.y<<endl;
   }
  
   undoOn = true;
   if (root -> next[bestMove] -> move.x)  // Best move isn't passing
   {
      this -> move(root->next[bestMove]->move.x, root->next[bestMove]->move.y, 1);   // color = 1
      cout << (char) (root->next[bestMove]->move.x + 96) << root->next[bestMove]->move.y << endl;
      deleteTree(root);
      return 1;
   }
   else                                // Pass
   {
      deleteTree(root);
      koFlag = false;
   }
   return 0;
}


//Delete the search tree after a move is selected
void deleteTree(MCnode * root)
{
   if (!root)
      return;
   for (int i = 0; i < 4; ++i)
      deleteTree(root -> next[i]);
   if (root -> board)
   {
      delete root -> board;
      root -> board = NULL;
   }
   delete root;
   root = NULL;
}


//Perform PLAYOUTS ranndom games from a given starting move and return 
//the total won by the color of the argument 'color'
int gameBoard::MCtryMove(int firstx, int firsty, int color)
{
   int wins = 0;
   gameBoard testBoard;

   testBoard.copy(this);
   if (firstx && !testBoard.move(firstx, firsty, color))  // If the first move is illegal return -1
	 return -1;
   for (int i = 0; i < PLAYOUTS; ++i)
      if (MCrandomGame(testBoard, -color, false))            
         ++wins; 
   return wins;

}


//Plays a random game between two computer players.  If a player attempts to
//make an illegal move 6 times in a row she passes.  If both players pass
//in succession the game ends.
bool gameBoard::MCrandomGame(gameBoard board, int color, bool display)
{                        
   int startColor = color;   
   int x, y;
   int misses, move = 0;
   bool passFlag = false; 
   bool done = false;

   do
   {
      if (display)
      {
         cout << "Move " << move << endl;
         board.draw();
      }
      misses = 0;
      do
      {
	 x = (rand() % 9) + 1; 
	 y = (rand() % 9) + 1;
	 if (board.move(x, y, color))
	 {
	    passFlag = false;
	    ++move;
	    break;
	 }
	 else
	    misses += 1;
	 if (misses == 6)
	 {
	    if (passFlag)
	       done = true;
	    else 
	       passFlag = true;
	    break;
	 }
         ++move;
      }
      while (misses < 6);
      color *= -1;
   }
   while (!done);
   bool  result = board.score(display);
   // If the first move in the random playout was black then the white player's move is
   // being evaluated and we return the opposite of the result from score, which 
   // returns true if black won.
   if (startColor == 1)
      return !result;
   else 
      return result;
}


//This function orchestrates a game between a human and the Monte Carlo tree algorithm
void MCgame()
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
	 if (!board.MCmove())
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

