# Game of Life
This is a simple implementation of Conway's Game of Life in C++.
It uses the olcPixelGameEngine by Javidx9 (https://github.com/OneLoneCoder/olcPixelGameEngine) for displaying the grid.
The game controls are:
- Space: Pause/Resume
- C: Clear the grid
- R: Randomize the grid
- Left Click: Toggle cell state

For now, you can only randomize the grid with a 50% chance of a cell being alive.
You cannot yet change the randomization chance or change cell states with the mouse.
In the future I will add these features.

## Building
To build the project, you need to have the olcPixelGameEngine installed - just [download the header file](https://github.com/OneLoneCoder/olcPixelGameEngine) and put it in the same directory as the source files.
Then, you can build the project with your favorite C++ compiler (I would recommend g++ if you are on linux).
You can also use the provided CMakeLists.txt file to build the project with CMake (in CLion for example).

### Linux 
To build the project on linux, you can use the provided Makefile or run the following command:
```bash
g++ -o GameOfLife main.cpp game.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17
```
If you need more information on how to build the project, you can check out the [olcPixelGameEngine header file](https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/olcPixelGameEngine.h), which has a section on building the project.

### Gif
![Game of Life](https://davidblog.si/wp-content/uploads/2023/01/Peek-2023-01-04-16-26.gif)

## TODO
- [ ] Add a way to change the randomization chance
- [ ] Add a way to change cell states with the mouse
- [ ] Add a way to change the grid size (currently set in constants.h)
- [ ] Add a way to change the cell size (currently set in constants.h)
- [ ] Add color customization
- [ ] Make it so that individual steps can be taken instead of only pausing/resuming the game
