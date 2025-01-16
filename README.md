# Conway's Game of Life

A C implementation of Conway's Game of Life using SDL2.

## Requirements

- GCC compiler
- SDL2 development libraries

## Compilation

Compile the game using the following command:

```bash
gcc -o game_of_life main.c $(sdl2-config --cflags --libs)
```

## Running the Game

After compilation, run the game with:

```bash
./game_of_life
```

## Controls

The game starts in a paused state with a random initial pattern. Use the
following controls:

- **Space**: Pause/unpause the simulation
- **R**: Reset with a new random pattern
- **C**: Clear the grid
- **Q**: Quit the game
- **Left Click**: Toggle cells on/off

## Game Rules

Conway's Game of Life follows these rules:

1. Any live cell with fewer than two live neighbors dies (underpopulation)
2. Any live cell with two or three live neighbors lives on to the next
generation
3. Any live cell with more than three live neighbors dies (overpopulation)
4. Any dead cell with exactly three live neighbors becomes a live cell
(reproduction)

## Technical Details

- Window Size: 800x600 pixels
- Cell Size: 5x5 pixels
- Grid Dimensions: 160x120 cells
- Frame Rate: 10 updates per second when running

## Notes

- The game features edge wrapping (cells on edges connect to the opposite edge)
- Random patterns are generated with approximately 45% live cells
- The simulation can be paused at any time to edit the pattern
