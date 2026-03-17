<div align="center">

# 🔢 SUDOKU

### *Two complete C++ implementations — Console & Graphical SFML*

<br>

![C++](https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![SFML](https://img.shields.io/badge/SFML-2.x-8CC445?style=for-the-badge&logo=sfml&logoColor=white)
![Console](https://img.shields.io/badge/Console-Version-4A90D9?style=for-the-badge)
![Difficulty](https://img.shields.io/badge/Difficulty-Easy_|_Medium_|_Hard-E67E22?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-lightgrey?style=for-the-badge)

<br>

> *Procedurally generated puzzles · Mistake tracking · Hint system · Backtracking solver*

<br>

---

</div>

## 📸 Screenshots

<div align="center">

| Menu | Gameplay | Mistake Highlight |
|:---:|:---:|:---:|
| ![Menu](screenshots/menu.png) | ![Board](screenshots/board.png) | ![Mistake](screenshots/mistake.png) |

| Multiple Mistakes | Console Version |
|:---:|:---:|
| ![Mistakes](screenshots/mistakes_many.png) | ![Console](screenshots/console.png) |

</div>

---

## 🎮 Two Versions

<div align="center">

| | Version | Description |
|:---:|:---|:---|
| 🖥️ | **SudokuSFML.cpp** | Full graphical window · mouse input · colour feedback |
| ⌨️ | **SudokuConsole.cpp** | Terminal-based · ASCII grid · keyboard row/col/value input |

</div>

---

## 🌟 Features

### 🖼️ SFML Graphical Version
- 🎯 **Difficulty select screen** — Easy, Medium, Hard with colour-coded buttons
- 🎲 **Procedurally generated** unique puzzles every game
- 🔴 **Real-time mistake highlighting** — wrong cells flash red instantly
- 💡 **Hint system** — 3 hints per game to reveal a correct cell
- 📊 **Live HUD** — Mistakes counter + Hints remaining displayed at top
- 🔢 **Number pad** — clickable 1–9 bar at the bottom of the board
- 🖱️ **Mouse-click cell selection** — click any cell to select it

### ⌨️ Console Version
- 📐 **ASCII 9×9 grid** with 3×3 box dividers
- 🔢 **Row / col / value** input format (`row col value` or `0` to exit)
- ✅ **Validation** — rejects invalid placements immediately
- 🔀 **Shuffled puzzle generation** for variety each run
- 🧠 **Backtracking solver** used internally for generation

---

## 🧩 How the Puzzle Engine Works

```
1. Fill diagonal 3×3 boxes with shuffled 1–9
2. Solve the rest using recursive backtracking
3. Remove cells based on difficulty level
4. Validate every player input against the solution
```

### Core Functions

```cpp
void  alot(int**& a)               // Allocate 9×9 dynamic grid
void  del(int**& a)                // Deallocate and nullify grid
bool  valid(int** p, int r, int c, int x)  // Check placement validity
void  shuffle(int* arr, int n)     // Fisher-Yates shuffle for randomness
bool  solve(int** p)               // Recursive backtracking solver
void  generate(int** p)            // Build a full valid board
void  print(int** p)               // Render ASCII board (console)
```

---

## 🎯 Difficulty Levels

<div align="center">

| 🟢 Easy | 🔵 Medium | 🔴 Hard |
|:---:|:---:|:---:|
| More clues revealed | Balanced reveal | Fewer clues — harder deduction |
| Great for beginners | Moderate challenge | For experienced solvers |

</div>

---

## 🎨 SFML Visual Design

| Element | Detail |
|:---|:---|
| **Background** | Dark navy (`#0d1b2a`) for reduced eye strain |
| **Grid lines** | Blue-tinted cell borders with bold 3×3 box outlines |
| **Given numbers** | White — placed at puzzle generation |
| **Player numbers** | Light blue — numbers entered by the player |
| **Hint numbers** | Cyan — cells revealed by hint |
| **Wrong cells** | 🔴 Red background — immediate mistake feedback |
| **Number pad** | 1–9 buttons along the bottom edge |

---

## 📁 Project Structure

```
Sudoku/
│
├── 📄 SudokuSFML.cpp        ← Graphical version (SFML window + mouse)
├── 📄 SudokuConsole.cpp     ← Console version (terminal + keyboard)
│
├── 📄 README.md
└── 📄 LICENSE
```

---

## 🚀 How to Build & Run

### 🖼️ SFML Version

**🐧 Linux / 🍎 macOS**
```bash
# Clone
git clone https://github.com/Abd-Abdullah83/Sudoku.git
cd Sudoku

# Compile
g++ -std=c++17 SudokuSFML.cpp -o sudoku \
    -lsfml-graphics -lsfml-window -lsfml-system

# Run
./sudoku
```

**🪟 Windows (MinGW)**
```bash
g++ -std=c++17 SudokuSFML.cpp -o sudoku.exe ^
    -lsfml-graphics -lsfml-window -lsfml-system

sudoku.exe
```

### ⌨️ Console Version

```bash
# No dependencies — just compile and run
g++ -std=c++17 SudokuConsole.cpp -o sudoku_console
./sudoku_console
```

> [!NOTE]
> The SFML version requires **SFML 2.x**.
> - Ubuntu → `sudo apt install libsfml-dev`
> - macOS → `brew install sfml`
> - Windows → [sfml-dev.org](https://www.sfml-dev.org/download.php)

---

## 🕹️ How to Play

### 🖼️ SFML Version
```
1. Select difficulty — Easy / Medium / Hard
2. Click a cell on the board to select it
3. Click a number (1–9) from the bottom pad to place it
4. Wrong placements highlight red and add to mistake count
5. Use hints (💡 3 available) to reveal a correct cell
```

### ⌨️ Console Version
```
1. The board is displayed as an ASCII grid
2. Enter:  row(1-9)  col(1-9)  value(1-9)
   Example: 1 3 7  →  places 7 at row 1, column 3
3. Enter 0 to exit
4. Invalid moves are rejected with an error message
```

---

## 🧠 Algorithm — Backtracking Solver

The puzzle generator uses a **recursive backtracking** algorithm:

```
• Try placing digits 1–9 in each empty cell
• Check row, column, and 3×3 box constraints via valid()
• If no digit fits → backtrack to previous cell
• Repeat until the board is fully solved
• Remove cells to create the player's puzzle
```

This guarantees every generated puzzle has a **unique valid solution**.

---

<div align="center">

---

### Built by **Abd-Abdullah83**
*FAST National University of Computer & Emerging Sciences (NUCES)*

<br>

*Fill every row, column, and box — no repeats.*

</div>
