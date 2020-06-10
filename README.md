<h1 align="center">Lem-in - Intelligent graph traversal.</h1>
<p align="center">
    <img src="https://github.com/salahadawi/lem-in/blob/master/images/lem-in_visualizer_demo.gif">
</p>

Lem-in is a project in which a set number of ants must find the shortest path through a graph from a start point to an end point. The ants must strategically work together to minimize the amount of moves done.

Running the program with a valid map as input will print each move by each ant.

<h2 align="center">Rules</h2>

* Each room (except the starting room ##start and ending room ##end) can only contain one ant at a time.

* Each ant can move one room at a time, to a room that is connected to their current room.

<h2 align="center">How does it work?</h2>
Here is a valid graph in text:

```
#number of ants
10
#rooms
##start
1 23 3
2 16 7
3 16 3
4 16 5
5 9 3
6 1 5
7 4 8
##end
0 9 5
#links
0-4
0-6
1-3
4-3
5-2
3-5
4-2
2-1
7-6
7-2
7-4
6-5
```

Here's what the room looks like visually:
<p align="center">
    <img src="https://github.com/salahadawi/lem-in/blob/master/images/lem-in_graph.png">
</p>

And in my visualizer:
<p align="center">
    <img src="https://github.com/salahadawi/lem-in/blob/master/images/lem-in_graph_visualizer.png">
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
