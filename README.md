# Game of Life

This is a simple implementation of Conway's Game of Life in C++.
It uses the olcPixelGameEngine by Javidx9 (https://github.com/OneLoneCoder/olcPixelGameEngine) for displaying the grid.
The game controls are:

- Space: Pause/Resume
- C: Clear the grid
- R: Randomize the grid
- N: Next generation
- Left Click: Add a living cell
- Right Click: Remove a living cell
- Tab: Show/Hide Console (help or. h for help)
- F: Make game faster **
- S: Make game slower **

The game allows you to set the mode to either classic (where the simulation runs normally) or to stepped (where you have
to press N to go to the temp generation).
You can add cells to the grid by clicking on them. Right click will remove the cells.
You can also randomize the grid by pressing R or. You can clear the grid by pressing C.
You can pause the simulation by pressing Space. You can show/hide the console by pressing Tab.
You can also press h or help to show the help in the console.
You can also define custom rules for generating the next generation. See the table below for usage. These rules can be
set through the console or the config file.
<br>

**
If you decide to change the speed, the game FPS will drop and the input will become way less
responsive. The game thread was put to sleep and will ignore any inputs until it wakes up again.
To account for this, better pause the game or press and hold the buttons for longer to have them detected.
Input might be changed in the future.

### Commands

| Command                         | Description                                                                 | Aliases |
|---------------------------------|-----------------------------------------------------------------------------|---------|
| help                            | Show help                                                                   | h       |
| clear                           | Clear the grid                                                              | c       |
| randomize                       | Randomize the grid                                                          | r, rand |
| temp                            | Go to the temp generation                                                   | n       |
| pause                           | Pause the simulation                                                        | p       |
| next                            | Go to the next generation                                                   | n       |
| set mode \<mode>                | Set the mode to either classic or stepped                                   |         |
| set rand \<value>               | Set the randomization value (1-100)                                         |         |
| set border \<bool>              | Set the border to either true or false                                      |         |
| set cell \<alive/dead> \<color> | Set the color of the cell                                                   |         |
| set algo \<algorithm>           | Set the algorithm to use                                                    |         |
| set speed \<speed>              | Set the speed of the simulation (0-9) 0 is slowest, 9 is fastest (realtime) |         |
| save \<filename>                | Save the current state                                                      | s       |
| load \<filename>                | Load a state                                                                | l       |

#### Algorithms

Custom algorithms follow the following format:

```text
B<numbers>/S<numbers>
```

Where B is the birth rule and S is the survival rule.
The numbers are the number of neighbors that the cell needs to be born or survive.
These numbers can range from 0-8.
For example, B3/S23 is the standard Conway's Game of Life algorithm.
Another example is B36/S23 which is the HighLife automaton.
Experiment with different algorithms to see what they do!

## Building

To build the project, you need to have the olcPixelGameEngine installed -
just [download the header file](https://github.com/OneLoneCoder/olcPixelGameEngine) and put it in the same directory as
the source files.
Then, you can build the project with your favorite C++ compiler (I would recommend g++ if you are on linux).
You can also use the provided CMakeLists.txt file to build the project with CMake (in CLion for example).

### Linux

To build the project on linux, you can use the provided Makefile or run the following command:

```bash
g++ -o GameOfLife main.cpp game.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17
```

If you need more information on how to build the project, you can check out
the [olcPixelGameEngine header file](https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/olcPixelGameEngine.h),
which has a section on building the project.

### Windows

To build in windows, I recommend using CLion, which has a built-in CMake support.
Otherwise, you can use the provided CMakeLists.txt file to build the project with CMake.
For more information on how to build the project, you can check out
the [olcPixelGameEngine header file](https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/olcPixelGameEngine.h),
which has a section on building the project.

### Gif

![Game of Life](https://davidblog.si/wp-content/uploads/2023/01/Peek-2023-01-04-16-26.gif)

## TODO

- [x] Add a way to change the randomization chance
- [x] Add a way to change cell states with the mouse
- ~~[ ] Add a way to change the grid size (currently set in constants.h)~~
- ~~[ ] Add a way to change the cell size (currently set in constants.h)~~
- [x] Add color customization
- [x] Make it so that individual steps can be taken instead of only pausing/resuming the game
- [x] Add different algorithms for calculating the next generation
- [x] Add a way to change the speed of the simulation
- [x] Save/load the game state
