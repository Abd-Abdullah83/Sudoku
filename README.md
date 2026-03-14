# 🔢 Sudoku

A fully-featured Sudoku application built in **C++** with **SFML** — play interactively, generate puzzles, and solve any board instantly.

---

## 📖 Overview

**Sudoku** is a C++17 Windows desktop application that combines an interactive puzzle game with a built-in backtracking solver, all rendered through the SFML graphics library. Players can generate new puzzles at varying difficulty levels, fill in answers through a responsive GUI, and invoke the solver at any point to complete the board automatically.

The project demonstrates constraint-satisfaction algorithms, clean object-oriented design in C++, and real-time 2D rendering using SFML.

---

## ✨ Features

- **Puzzle Generator** — Creates valid, uniquely-solvable puzzles across four difficulty levels: Easy, Medium, Hard, and Expert.
- **Backtracking Solver** — Solves any legal Sudoku board in milliseconds using a recursive constraint-based algorithm.
- **SFML GUI** — Smooth, responsive 2D interface with full keyboard and mouse input support.
- **Real-time Validation** — Highlights conflicting cells in rows, columns, and 3×3 subgrids as you type.
- **Hint System** — Reveals a single correct cell on demand without exposing the full solution.
- **Auto-solve Visualiser** — Animates the backtracking algorithm step-by-step for educational purposes.

---

## 🛠️ Prerequisites

Before building the project, ensure the following are installed on your Windows machine:

- [Visual Studio 2019 or later](https://visualstudio.microsoft.com/) with the **Desktop development with C++** workload, or MinGW-w64 (GCC 9+)
- [CMake 3.16+](https://cmake.org/download/)
- [SFML 2.6 for Windows](https://www.sfml-dev.org/download.php)

### Installing SFML on Windows

1. Download the SFML 2.6 package matching your compiler (Visual C++ or MinGW) from the [official SFML download page](https://www.sfml-dev.org/download.php).
2. Extract the archive to a known location, for example `C:\SFML`.
3. Set the `SFML_DIR` variable in `CMakeLists.txt` to point to that location, or pass it during the CMake configure step:

```bash
cmake .. -DSFML_DIR="C:/SFML/lib/cmake/SFML"
```

---

## 🚀 Building & Running

Open **Developer Command Prompt for Visual Studio** (or a terminal with MinGW on your `PATH`) and run the following:

```bash
# 1. Clone the repository
git clone https://github.com/Abd-Abdullah83/sudoku.git
cd sudoku

# 2. Create a build directory
mkdir build
cd build

# 3. Configure with CMake
cmake .. -DSFML_DIR="C:/SFML/lib/cmake/SFML"

# 4. Build in Release mode
cmake --build . --config Release

# 5. Run the executable
.\Release\sudoku.exe
```

> **Note:** The SFML `.dll` files (`sfml-graphics-2.dll`, `sfml-window-2.dll`, `sfml-system-2.dll`) must be present in the same directory as `sudoku.exe`. Copy them from `C:\SFML\bin\` if they are not placed there automatically by the build.

---

## 🎮 How to Play

Once launched, the SFML window presents the main menu with the following options.

**New Game** — Choose a difficulty level and a freshly generated puzzle is displayed on the board.

**Solve** — Automatically fills in the current board using the backtracking solver. You may also enter a custom puzzle and solve it instantly.

**Hint** — Press `H` at any point during gameplay to reveal one correct cell.

**Reset** — Clears all user-entered values and restores the original puzzle state.


---



---

## 🧪 Running Tests

Unit tests for the solver and generator can be built and run separately:

```bash
cmake .. -DSFML_DIR="C:/SFML/lib/cmake/SFML" -DBUILD_TESTS=ON
cmake --build . --config Release
.\Release\sudoku_tests.exe
```

All tests should pass before any pull request is submitted.

---


## 📄 License

This project is licensed under the [MIT License](LICENSE). You are free to use, modify, and distribute it with attribution.

---

## 👤 Author

**Your Name**
- GitHub: [@your-username](https://github.com/Abd-Abdullah83)
- LinkedIn: [your-linkedin](https://linkedin.com/in/abdullah-tahir-ds)

---

> *If you find this project useful, consider giving it a ⭐ on GitHub.*
