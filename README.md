<h1 align="center">Lem-in - Intelligent graph traversal.</h1>
<p align="center">
    <img src="https://github.com/salahadawi/filler/blob/master/images/filler_demo.gif">
</p>

Lem-in is a project in which a set number of ants must find the shortest path through a graph from a start point to an end point. The ants must strategically work together to minimize the amount of moves done.

Running the program with a valid map as input will print each move by each ant.

<h2 align="center">Rules</h2>
* Each room (except the starting room ##start and ending room ##end) can only contain one ant at a time.
* Each ant can move one room at a time, to a room that is connected to their current room.

<h2 align="center">How does it work?</h2>
<p align="center">
    <img src="https://github.com/salahadawi/filler/blob/master/images/filler_map.png">
    <img src="https://github.com/salahadawi/filler/blob/master/images/filler_heatmap.png">
</p>

The above image on the left shows a board state given to the AI. Playing as 'X',
the AI will generate a heatmap of the grid, with each coordinate getting a value equal to it's distance to the nearest opposing players block.  
  
  
The strategy used is to place blocks as near to the opponent as possible,
with the intention of blocking the opponents paths and limiting their following moves.

<h2 align="center">Algorithm</h2>

<h2 align="center">Usage</h2>

1. Clone the repo:
```
git clone https://github.com/salahadawi/filler.git
```
2. Compile the project:
```
make
```

3. Run the AI against a simple practice AI:
```
./filler_vm -f maps/map00 -p1 ./sadawi.filler -p2 ./players/random.filler
```
The game will be output to the terminal.
