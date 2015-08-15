
/*
   Copyright (c) 2015 Neil Babson and Gabe Stauth
   This source file is protected under the MIT License. 
   Please see the file LICENSE in the distribution for license terms.
*/

/*
   This file contains:
   void score() - Algorithm that scores partial or completed games, adding each players points of territory
             to their captive count
   void drawScored() - Draws the board of a completed game, marking each players territorial points with
                       x's and outputs the final score.
   void randomGame() - Plays a complete game between two computer players making random moves.
*/

#include "board.h"


/* This function makes repeated passes over the board looking for empty points to assign to a player as 
   territory.  Counting ends if the algorithm finds no new points on a complete pass over the board.
   On each pass empty sapces that are adjacent to a single player are marked as belonging to that 
   player and empty spaces adjacent to both players (which are not territory) are marked as visited
   so that their status won't be checked again.  Spaces marked as territory in previous completed passes 
   are treated as pieces.  Each players territorial points are added to their captured pieces count and a 
   final score is displayed.
   */
bool gameBoard::score(bool display)
{
   bool done;           // No more territory to be found
   int neighborColor;   // Color of a group adjoining an empty space
   int generation = 2;  // Prevents spaces marked as territory in the same pass over the board
                                  // from interfering  

   resetFlags();
   do
   {
      done = true;
      for (int i = 1; i < 10; ++i)
      {
	 for (int j = 1; j < 10; ++j)
	 {
	    if (!pieces[i][j].visited && !pieces[i][j].color) 
	    //If a space is empty and the point hasn't been marked as shared
	    {
	       neighborColor = 0;

	       //Check the four neighbors of an empty space, unless that space is on an edge.
	       //If exactly one neighboring color from a previous generation of the board is
	       //found the space will be marked as territory for that color.
	       //Black territory > 0, white territory < 0.  If a space is found to have 
	       //neighbors of both colors the visited flag is set.
	       if (i > 1 && pieces[i-1][j].color && abs(pieces[i-1][j].color) < generation)
	       {            
		  if (pieces[i-1][j].color > 0)
		     neighborColor = generation;
		  else
		     neighborColor = -1 * generation;                 
	       }
	       if (i < 9 && pieces[i+1][j].color && abs(pieces[i+1][j].color) < generation)
	       {           
		  if (neighborColor)
		  {
		     if ((neighborColor > 0 && pieces[i+1][j].color < 0) || (neighborColor < 0 && pieces[i+1][j].color > 0))
			pieces[i][j].visited = true;
		  }
		  else 
		  {
		     if (pieces[i+1][j].color > 0)
			neighborColor = generation;
		     else
			neighborColor = -1 * generation;
		  }
	       }
	       if (j > 1 && pieces[i][j-1].color && !pieces[i][j].visited && abs(pieces[i][j-1].color) < generation)
	       {          
		  if (neighborColor)
		  {
		     if ((neighborColor > 0 && pieces[i][j-1].color < 0) || (neighborColor < 0 && pieces[i][j-1].color > 0))
			pieces[i][j].visited = true;
		  }
		  else 
		  {
		     if (pieces[i][j-1].color > 0)
			neighborColor = generation;
		     else 
			neighborColor = -1 * generation;
		  }
	       }
	       if (j < 9 && pieces[i][j+1].color && !pieces[i][j].visited && abs(pieces[i][j+1].color) < generation) 
	       {         
		  if (neighborColor)
		  {
		     if ((neighborColor > 0 && pieces[i][j+1].color < 0) || (neighborColor < 0 && pieces[i][j+1].color > 0))
			pieces[i][j].visited = true;
		  }
		  else 
		  {
		     if (pieces[i][j+1].color > 0)
			neighborColor = generation;
		     else neighborColor = -1 * generation;
		  }
	       }
	       
	       if (neighborColor && !pieces[i][j].visited)
	       {
		  // Territory is added to the count of captured opponents.
		  if (neighborColor < 0)
		     ++dead[0];
		  else
		     ++dead[1];
		  pieces[i][j].color = neighborColor;
		  done = false;
	       }
	    }
	 }
      }
      ++generation;
   }
   while (!done);
   if (display)
      drawScored();
   if (dead[1] - dead[0] > 0)
      return true;
   return false;
}





// Displays a game position and the final score.
void gameBoard::drawScored()
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
	    cout << "\033[40m0 ";
	 else if (pieces[i][j].color == -1)
	    cout << "\033[47m0\033[40;7m ";
	 else if (pieces[i][j].color > 0)
	    cout << "\033[40mx ";
	 else if (pieces[i][j].color < 0)
	    cout << "\033[47mx\033[40;7m ";
      }
      cout << (char) 128;
   }
   cout << "\n ";
   for (int i = 0; i < 20; ++i)
      cout << (char) 128;
   cout << endl << endl << "\033[0m";
   cout << "\nBlack points: " << dead[1];
   cout << "\nWhite points: " << dead[0] << endl;
}


//Plays a random game between two computer players.  If a player attempts to
//make an illegal move 6 times in a row she passes.  If both players pass
//in succession the game ends.
bool randomGame(gameBoard board, moves_t moves, int color, bool display)
{
   int x, y;
   int misses, move = 0;
//   gameBoard board;
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
/*	    if (move < 5)
	    {
	       moves.x[move] = x;
	       moves.y[move] = y;
	    }    */
	    passFlag = false;
	    ++move;
	    break;
	 }
	 else
	 {
/*	    if (move < 5)
	    {
	       moves.x[move] = 0;
	       moves.y[move] = 0;
	    }   */
	    misses += 1;
//	    cout << "Invalid move\n";
	 }
	 if (misses == 10)
	 {
	    if (passFlag)
	       done = true;
	    else 
	       passFlag = true;
	    break;
	 }
//         ++move;
      }
      while (misses < 10);
      color *= -1;
   }
   while (!done);
   return board.score(display);
}

