# MontyGo
Open source 9x9 Go program using Monte Carlo search algorithm


Copyright (c) 2015 Neil Babson and Gabe Stauth
This source file is protected under the MIT License. 
Please see the file LICENSE in the distribution for license terms.

Contact information:
nbabson@pdx.edu
gstauth@pdx.edu

Compile with:
g++ *cpp

This program uses a Monte Carlo tree algorithm to play 9x9 games of Go.
The file board.cpp contains the implementation of a board on which to
human players can play.  The board recognizes illegal moves, performs 
and keeps track of captures, and saves previous board positions on a 
stack (stack.cpp) to allow undo.


The functions in the file score.cpp score partial or completed games,
display a completed game with territory marked, and play out a complete 
random game.

playout.cpp enlists the users help to identify and remove dead groups at
the end of a game and implements a brute force Go game in which the
computer player (black) tries each available move 1000 times, playing 
out the remaining game randomly, and selects the move that resulted in
the most wins.

The Monte Carlo algorithm is contained in the file monte.cpp.  From 
the available moves the computer player selects four candidates and 
builds a tree with four promising responses at each level.  The results
of random playouts are propogated up the tree to determine which of
the original four moves to make.  The depth of the tree can be set in
the recursive function MCcheckmoves(), and the number of playouts 
performed at each step is set by the constant PLAYOUTS in the board.h
file.

At present the Monte Carlo algorithm does not select good moves, and 
further testing is required.  The brute force game has some ability
to capture enemy pieces and may be able to beat a human beginner.


8/14/2015
Latest tweak to the algorithm significantly improved the play of the brute
force algorithm.  It now can beat human beginners.  Unfortunately this
development happened the day after the program was demonstrated to
the open source development class.
