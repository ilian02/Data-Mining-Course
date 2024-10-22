# Sliding Puzzle Solver using IDA\*

### Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [How it works](#how-it-works)


## Introduction

This project implements a **Sliding Puzzle Solver** using the **Iterative Deepening A\*** (IDA\*) search algorithm. The sliding puzzle is a popular game that consists of a grid of numbered tiles with one tile missing, where the objective is to rearrange the tiles into a particular configuration, typically by sliding tiles into the empty space.

The IDA\* algorithm efficiently finds an optimal solution by combining depth-first search with heuristic guidance, making it well-suited for this type of puzzle.

## Features

- Solves **N x N** sliding puzzles.
- Uses the **Manhattan Distance** heuristic for efficient searching.
- Ensures optimal solution using the **IDA\*** algorithm.
- Supports custom puzzle sizes and initial configurations.
- Visualizes each step of the solution process.

## Usage

To solve a sliding puzzle, first compile the program. For best efficiency use release or -O3 version.
g++ slider_solver.cpp -O3 -o slider_solver
After running the .slider_solver.exe:
1) On the first line input the count of blocks - 8, 15, 25 and so on. Be careful the problem is NP complete.
2) On the second line input where the empty place should be in your final state. 
    -1 is default right angle
    starting from 0 and going up to piece count -1 to the right.
3) After that input all of the numbers according to their possition
```
    5 4 3
    1 2 0
    7 8 0
```

Your input should look like this:
```
    8
    6
    1 2 3
    4 5 6
    0 7 8
```

Extpected output for the given example is: 
```
    2
    left
    left
```

- You can enable a timer by changing the USE_CLOCK variable to true.

## How it works

The **Iterative Deepening A\*** (IDA\*) algorithm is an extension of the A\* search that combines depth-first search with a heuristic to reduce memory consumption while ensuring an optimal solution. It iteratively deepens the search while keeping track of the estimated cost of reaching the goal, using a heuristic to guide the search.

### Step-by-step Breakdown

1. **Input Parsing**: 
   The program reads the size of the puzzle and the initial configuration from the user input. The puzzle is represented as a 1D list where each number corresponds to a tile, and `0` represents the empty space.

2. **State Generation**: 
   From the current state, the algorithm generates possible new states by sliding tiles into the empty space. Each valid move creates a new state, which is then evaluated.

3. **Heuristic Evaluation**: 
   The solver uses the **Manhattan Distance** heuristic. This measures the sum of the distances of each tile from its goal position (ignoring obstacles). The Manhattan Distance helps the solver estimate how close the current state is to the goal state.

4. **Iterative Deepening**: 
   IDA\* performs a series of depth-first searches, where each search has a bound on the cost. This bound is increased gradually in each iteration until the goal state is found. 
   
   - During each iteration, the current state’s cost (sum of actual cost and heuristic cost) is checked.
   - If the current state’s cost exceeds the current bound, the search terminates and the bound is updated for the next iteration.
   - If the current state’s cost is within the bound, the search continues deeper.

5. **Solution or Failure**: 
   The algorithm continues to deepen iteratively until the goal configuration is found. Once the goal is reached, the sequence of moves is printed or displayed, showing how to solve the puzzle.
