<h1 align="center">Lem-in - Intelligent graph traversal.</h1>
<p align="center">
    <img src="https://github.com/salahadawi/lem-in/blob/master/images/lem-in_visualizer_demo.gif">
</p>

Lem-in is a project in which a set number of ants must find the shortest path through a graph from a start point to an end point. The ants must strategically work together to minimize the amount of moves done.

Running the program with a valid map as input will print each move by each ant.

To ease use and testing, i coded a graphical visualizer with SDL2 to simulate the graph and each ants moves.

<h2 align="center">Rules</h2>

All ants must reach the end room while following some rules:

* Each room (except the starting room ##start and ending room ##end) can only contain one ant at a time.

* Each ant can move one room at a time, to a room that is connected to their current room.

<h2 align="center">How does it work?</h2>

### Here is a valid graph in text:

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
First the amount of ants inside the graph is listed. 

Then, each room is listed in the format `name x_pos y_pos`. 

Then, all links between the rooms is listed in the format `room1_name-room2_name`

### Here's what the room looks like visually:

<p align="center">
    <img src="https://github.com/salahadawi/lem-in/blob/master/images/lem-in_graph.png">
</p>

### And in my visualizer:

<p align="center">
    <img src="https://github.com/salahadawi/lem-in/blob/master/images/lem-in_graph_visualizer.png">
</p>

<h2 align="center">Visualizer</h2>

The visualizer will read the output of the lem_in program, and use it to simulate the graph. Each ant and their moves are saved, and can be viewed one step at a time.

### Controls
```
* Mousewheel to zoom in/out

* Hold mouse1/mouse2 and drag to move map

* Right arrow key for next move

* Left arrow key for previous move

* Up/down arrow keys to control ant animation speed
```

<h2 align="center">Algorithm</h2>

I decided to attempt creating my own algorithm to solve this exercise. 

Moving the ants from a starting point to an ending point while following the necessary rules is easy. The challenge comes with making the ants work as a team, with some ants having to occasionally take less optimal paths to avoid blocking other ants.

### Preprocessing
First, each room in the graph is given a value. This value corresponds to how many moves it will take to reach the end room from that room. The end room always has a value of 0, rooms directly connected to it have a value of 1... and so on.

### Ant Movement
Then, the ants will start moving. Each ant will check the available paths, do some calculations, and choose the move it thinks is best.

To calculate the best move, the ant will check how many ants are behind it. With this information, the ant can take a longer path, and leave a shorter path free for the next ants to use, effectively helping other ants reach the end quicker.

<h2 align="center">Usage</h2>

1. Clone the repo:
```
git clone https://github.com/salahadawi/lem-in.git
```
2. Compile the project and the visualizer:
```
make && make -C visualizer
```

3. Run the project with a map:
```
./lem-in < maps/mapgrid
```

4. Run the project with a map and visualize it:
```
./lem-in < maps/mapgrid | ./visualizer/visualizer
```
