/*
   Copyright (c) 2015 Neil Babson and Gabe Stauth
   This source file is protected under the MIT License. 
   Please see the file LICENSE in the distribution for license terms.
*/

/*
   This file contains the implementation of the boardGame class functions
   which control a 9 x 9 Go board.  The main function provides an interface
   to allow a 2 person game to be played.
*/
#include "board.h"
using namespace std;

bool buildGroup;     // This flag is used to distinguish snapbacks from kos.
const int GAMES = 100000;

int main()
{
   char move[3];
   int color = 1;
   gameBoard board;
   bool passFlag = false; 
   int choice;
   moves_t moves[1000];
   int wins;

   srand(time(NULL));
   cout << "1. Random computer game\n";    //Time GAMES random games
   cout << "2. Human two player game\n";
   cout << "3. Try out a move\n";          //Try 1000 playouts from an initial move
   cout << "4. Play against dumb game\n";  //Play against brute force algorithm
   cout << "5. Play Monte Carlo go\n";
   cout << "Choose: ";
   cin >> choice;
   if (cin.fail())
      cin.clear();
   cin.ignore(10, '\n');
   if (choice == 2)
   {
      do
      {
	 board.draw();
	 cout << "Enter a move (i.e. g3, p=pass, u=undo, q=quit) : ";
	 cin.get(move, 10, '\n');
	 if (cin.fail())
	    cin.clear();
	 cin.ignore(10, '\n');
	 if (move[0] == 'u')    
	    board.undo();
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
	 color *= -1;
      }
      while (move[0] != 'q');
      board.score(true);
   }
   else if (choice == 1)
   {
      clock_t t;
      t = clock();
      for (int i = 0; i < GAMES; ++i)
         randomGame(board, moves[0], 1, false);
      t = clock() - t;
      cout << "Time elapsed: " << ((float) t) / CLOCKS_PER_SEC << " seconds to play " << GAMES << " games.\n";
   }
   else if (choice == 3) 
   {
      wins = board.tryMove(5, 5, moves);    
      cout << "Number of wins: " << wins << endl;

   }
   else if (choice == 4)
      dumbGame();
   else 
      MCgame();
   return 0;
}


// Constructor
gameBoard::gameBoard()
{
   for (int i = 1; i < 10; ++i)
      for(int j = 1; j < 10; ++j) 
      {
	 pieces[i][j].color = 0;
	 pieces[i][j].groupNum = 0;
      }
   for (int i = 0; i < 11; ++i)
   {
      pieces[0][i].color = 3;
      pieces[10][i].color = 3;
      pieces[i][0].color = 3;
      pieces[i][10].color = 3;
      pieces[0][i].groupNum = -1;
      pieces[10][i].groupNum = -1;
      pieces[i][0].groupNum = -1;
      pieces[i][10].groupNum = -1;
   }
   numString = 0;
   dead[0] = dead[1] = 0;
   koFlag = false;
   undoOn = true;
}

gameBoard::gameBoard(gameBoard *to_copy) 
{
   for (int i = 1; i < 10; ++i)
      for (int j = 1; j < 10; ++j)
	 this -> pieces[i][j] = to_copy -> pieces[i][j];
   koFlag = to_copy -> koFlag;
   koMove[0] = to_copy -> koMove[0];
   koMove[1] = to_copy -> koMove[1];
   undoOn = to_copy -> undoOn;
}


void gameBoard::undo()
{
   history.pop(pieces, koFlag, koMove); 
}


// Returns 0 if the player attempts to make an illegal move.
int gameBoard::move(int x, int y, int color)
{
    if (x < 1 || x > 9 || y < 1 || y > 9)
       return 0;
    if (pieces[x][y].color)
       return 0;
    if (koFlag)
    {
       if (x == koMove[0] && y == koMove[1])
	  return 0;    
       else 
	  koFlag = false;
    }
    if (!addGroup(x, y, color))
       return 0;
    if (undoOn)	  
       history.push(pieces, koFlag, koMove);
    pieces[x][y].color = color;
    return 1;
}



// Displays a game position.
void gameBoard::draw()
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


// Reset the visited flags
void gameBoard::resetFlags()
{
   for (int i = 1; i < 10; ++i)
      for (int j = 1; j < 10; ++j)
	 pieces[i][j].visited = false;
}



// Checks the neighbors of a stone that has been played to find out if any groups have died and if the move is legal.
int gameBoard::addGroup(int x, int y, int color)
{
   int g1, g2, g3, g4;
   int total;

   g1 = pieces[x+1][y].groupNum;   
   g2 = pieces[x-1][y].groupNum;  
   g3 = pieces[x][y-1].groupNum;   
   g4 = pieces[x][y+1].groupNum;  
   pieces[x][y].groupNum = ++numString;
   resetFlags();
   buildGroup = false;
   pieces[x][y].visited = true;
   total = checkNeighbor(x+1, y, color);
   if (pieces[x-1][y].color == color || pieces[x-1][y].color == 0 || g1 != g2)
      total += checkNeighbor(x-1, y, color);
   if (pieces[x][y-1].color == color || pieces[x][y-1].color == 0 || (g3 != g1 && g3 != g2))
      total += checkNeighbor(x, y-1, color);
   if (pieces[x][y+1].color == color || pieces[x][y+1].color == 0 || (g4 != g1 && g4 != g2 && g4 != g3))
      total += checkNeighbor(x, y+1, color);
   return total;
//   return checkNeighbor(x+1, y, color) + checkNeighbor(x-1, y, color) + checkNeighbor(x, y-1, color) + checkNeighbor(x, y+1, color);
}



// Eliminates a dead group.  The location of the first removed piece is saved in the koMove array.
// If only a single stone is killed the goFlag is set to true, and the location stored in koMove 
// is an illegal play.
void gameBoard::killGroup(int groupNum, int color)
{
   int deadStones = 0;

   for (int i = 1; i < 10; ++i)
      for (int j = 1; j < 10; ++j)
	 if (pieces[i][j].groupNum == groupNum)
	 {
	    if (!deadStones)
	    {
	       koMove[0] = i;
	       koMove[1] = j;
	    }
	    ++deadStones;
	    pieces[i][j].color = 0;
	    pieces[i][j].groupNum = 0;
	 }

   if (deadStones == 1 && !buildGroup)
      koFlag = true;
   dead[(color - 1) / -2 ] += deadStones;
}



// This function searches until it finds a liberty for a group,
// returning 0 if the group is dead.
int gameBoard::checkLiberties(int x, int y, int color)
{
   int liberties;

   if (pieces[x][y].color == 0 && !pieces[x][y].visited)
      return 1;
   if (pieces[x][y].color != color || pieces[x][y].visited)
   {
     // pieces[x][y].visited = true;
      return 0;
   }
   pieces[x][y].visited = true;
   liberties = checkLiberties(x-1, y, color);
   if (!liberties)
      liberties = checkLiberties(x+1, y, color);
   if (!liberties)
      liberties = checkLiberties(x, y-1, color);
   if (!liberties)
      liberties = checkLiberties(x, y+1, color);
   return liberties;
}



// Calls checkLiberties to check whether a move killed any of its adjacent groups
// and then calls numberGroup to assign the same group number to all
// connected stones of the same color.
int gameBoard::checkNeighbor(int x, int y, int color)
{
   if (pieces[x][y].color == 0)
      return 1;
   if (!pieces[x][y].visited && color != pieces[x][y].color && pieces[x][y].color != 3)
   {  
      if (!checkLiberties(x, y, pieces[x][y].color))
      {
	 killGroup(pieces[x][y].groupNum, pieces[x][y].color);
	 return 1;
      }
      return 0;
   }
   else if (color == pieces[x][y].color)
      return numberGroup(x, y, color);
   return 0;
}



// Recursive function assigns new group number to all stones of a connected group. 
// 0 is returned if the group has no liberties, indicating an illegal move.
int gameBoard::numberGroup(int x, int y, int color)
{
   if (pieces[x][y].color == 3 || pieces[x][y].visited) 
      return 0;
   if (pieces[x][y].color == 0)
      return 1;
   if (pieces[x][y].color == color)
   {
      buildGroup = true;
      koFlag = false;   // If the move adds to a group than it doesn't start a ko
      pieces[x][y].visited = true;
      pieces[x][y].groupNum = numString;
      return numberGroup(x-1, y, color) + numberGroup(x+1, y, color) + numberGroup(x, y-1, color) + numberGroup(x, y+1, color);
   }
   return 0;
}




