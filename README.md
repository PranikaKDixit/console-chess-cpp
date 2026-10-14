# Console Chess in C++

A simple console-based chess game written in C++ using a custom board and piece system.

## Features

- Full chess move validation
- Legal-move generation
- Check, checkmate, and stalemate detection
- Draw detection
- Undo support
- Move history
- Console UI for two-player gameplay

## Project Structure

- `main.cpp` - application entry point
- `include/` - header files for board, pieces, game logic, and player state
- `src/` - implementation files
- `build/` - compiled object files

## How to Run

Open the x64 Native Tools Command Prompt for VS and then type the following commands:

```bat
cd path\to\console-chess-cpp
cl /EHsc /std:c++17 /utf-8 /Iinclude main.cpp src\*.cpp /Fe:chess.exe
chess.exe
```

If you are already in the project folder, you can skip the `cd` step and just run:

```bat
cl /EHsc /std:c++17 /utf-8 /Iinclude main.cpp src\*.cpp /Fe:chess.exe
chess.exe
```

## Controls

- Enter moves in the format `e2e4`
- Type `quit` to exit the game
- Type `undo` to undo the last full move
- Type `list` to display legal moves

## Notes

This project is designed for Windows and uses the Microsoft C++ compiler from Visual Studio. The `x64 Native Tools Command Prompt for VS` ensures the correct include paths and compiler environment are available.
