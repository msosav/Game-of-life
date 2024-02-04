# Conway's Game of Life

1. Any live cell with fewer than two live neighbors dies, as if by underpopulation.
2. Any live cell with two or three live neighbors lives on to the next generation.
3. Any live cell with more than three live neighbors dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.

## How to compile it

```bash
g++ -std=c++20 game.cpp -0 game `sdl2-config --cflags --libs`
```

## How to run it

```
./game
```
