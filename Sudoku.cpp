/*
 ██████  ██    ██ ██████   ██████  ██   ██ ██    ██
██       ██    ██ ██   ██ ██    ██ ██  ██  ██    ██
 █████   ██    ██ ██   ██ ██    ██ █████   ██    ██
      ██ ██    ██ ██   ██ ██    ██ ██  ██  ██    ██
 ██████   ██████  ██████   ██████  ██   ██  ██████

  SFML Sudoku — Compile with:
  g++ sudoku_sfml.cpp -o sudoku -lsfml-graphics -lsfml-window -lsfml-system
  or with MSVC + vcpkg sfml
*/

#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include <string>
#include <sstream>
#include <algorithm>

// ─── Palette ────────────────────────────────────────────────────────────────
const sf::Color BG{ 15,  17,  26, 255 };   // deep navy
const sf::Color GRID_LINE{ 50,  55,  80, 255 };   // muted blue-grey
const sf::Color BOX_LINE{ 100, 120, 200, 255 };  // accent blue
const sf::Color CELL_BG{ 22,  26,  42, 255 };   // dark cell
const sf::Color CELL_SEL{ 35,  50,  95, 255 };   // selected cell
const sf::Color CELL_SAME{ 28,  40,  70, 255 };   // same-digit highlight
const sf::Color CELL_FIXED{ 18,  22,  36, 255 };   // fixed/given cell
const sf::Color CELL_WRONG{ 80,  20,  30, 255 };   // wrong answer
const sf::Color CELL_RIGHT{ 20,  60,  40, 255 };   // correct flash
const sf::Color TXT_FIXED{ 200, 210, 255, 255 };   // given number
const sf::Color TXT_USER{ 90, 180, 255, 255 };   // user-entered number
const sf::Color TXT_WRONG{ 255,  80,  80, 255 };   // wrong number
const sf::Color TXT_HINT{ 120, 220, 130, 255 };   // hint text
const sf::Color ACCENT{ 90, 130, 255, 255 };   // highlight accent
const sf::Color BTN_BG{ 30,  38,  70, 255 };   // button bg
const sf::Color BTN_HOV{ 50,  65, 120, 255 };   // button hover

// ─── Layout constants ───────────────────────────────────────────────────────
const int WINDOW_W = 700;
const int WINDOW_H = 780;
const int BOARD_X = 40;
const int BOARD_Y = 80;
const int BOARD_SIZE = 540;
const int CELL_SIZE = BOARD_SIZE / 9;   // 60

// ─── Sudoku Logic ───────────────────────────────────────────────────────────
void alot(int**& a) {
    a = new int* [9];
    for (int i = 0; i < 9; i++) a[i] = new int[9];
}
void del(int**& a) {
    for (int i = 0; i < 9; i++) delete[] a[i];
    delete[] a;
    a = nullptr;
}
bool valid(int** p, int r, int c, int x) {
    for (int i = 0; i < 9; i++) {
        if (x == p[r][i] || x == p[i][c]) return false;
    }
    int rr = (r / 3) * 3, rc = (c / 3) * 3;
    for (int k = 0; k < 3; k++)
        for (int m = 0; m < 3; m++)
            if (p[rr + k][rc + m] == x) return false;
    return true;
}
void shuffle(int* d) {
    for (int i = 8; i > 0; i--) {
        int j = rand() % (i + 1);
        std::swap(d[i], d[j]);
    }
}
bool solve(int** p) {
    for (int r = 0; r < 9; r++)
        for (int c = 0; c < 9; c++)
            if (p[r][c] == 0) {
                int digits[9] = { 1,2,3,4,5,6,7,8,9 };
                shuffle(digits);
                for (int i = 0; i < 9; i++) {
                    if (valid(p, r, c, digits[i])) {
                        p[r][c] = digits[i];
                        if (solve(p)) return true;
                        p[r][c] = 0;
                    }
                }
                return false;
            }
    return true;
}
void generate(int** solution, int** puzzle, int**& empty, int removals = 45) {
    alot(empty);
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++) empty[i][j] = 0;
    solve(empty);
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++) {
            solution[i][j] = empty[i][j];
            puzzle[i][j] = empty[i][j];
        }
    int rem = removals;
    while (rem--) {
        int r = rand() % 9, c = rand() % 9;
        if (puzzle[r][c] != 0) puzzle[r][c] = 0;
        else rem++;
    }
}

// ─── State ──────────────────────────────────────────────────────────────────
enum class GameState { PLAYING, WON, MENU };

struct Game {
    int** solution = nullptr;
    int** puzzle = nullptr;
    int** empty = nullptr;
    bool  fixed[9][9]{};
    bool  wrong[9][9]{};
    int   selR = -1, selC = -1;
    int   mistakes = 0;
    int   hints = 3;
    GameState state = GameState::MENU;
    int   difficulty = 1; // 0=easy 1=medium 2=hard

    void newGame() {
        if (solution) del(solution);
        if (puzzle)   del(puzzle);
        if (empty)    del(empty);
        alot(solution); alot(puzzle);
        int removals = 35 + difficulty * 10; // easy=35 med=45 hard=55
        generate(solution, puzzle, empty, removals);
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++) {
                fixed[i][j] = (puzzle[i][j] != 0);
                wrong[i][j] = false;
            }
        selR = selC = -1;
        mistakes = 0;
        hints = 3;
        state = GameState::PLAYING;
    }
    bool isSolved() const {
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                if (puzzle[i][j] == 0) return false;
        return true;
    }
    void useHint() {
        if (hints <= 0 || selR < 0 || selC < 0) return;
        if (fixed[selR][selC]) return;
        puzzle[selR][selC] = solution[selR][selC];
        wrong[selR][selC] = false;
        hints--;
    }
    void enterDigit(int val) {
        if (selR < 0 || selC < 0 || fixed[selR][selC]) return;
        bool wasWrong = wrong[selR][selC];
        puzzle[selR][selC] = val;
        wrong[selR][selC] = (solution[selR][selC] != val);
        // Only count a new mistake when transitioning from correct/empty -> wrong.
        // Overwriting one wrong answer with another wrong answer does NOT add another mistake.
        if (wrong[selR][selC] && !wasWrong) mistakes++;
    }
    ~Game() {
        if (solution) del(solution);
        if (puzzle)   del(puzzle);
        if (empty)    del(empty);
    }
};

// ─── Helpers ────────────────────────────────────────────────────────────────
sf::Vector2i cellAt(int mx, int my) {
    int c = (mx - BOARD_X) / CELL_SIZE;
    int r = (my - BOARD_Y) / CELL_SIZE;
    if (r < 0 || r > 8 || c < 0 || c > 8) return { -1, -1 };
    return { r, c };
}

void drawRoundedRect(sf::RenderWindow& win, float x, float y, float w, float h,
    float radius, sf::Color fill, sf::Color outline = sf::Color::Transparent,
    float outThick = 0.f) {
    sf::RectangleShape rect;
    rect.setPosition(x + radius, y);
    rect.setSize({ w - 2 * radius, h });
    rect.setFillColor(fill);
    win.draw(rect);
    rect.setPosition(x, y + radius);
    rect.setSize({ w, h - 2 * radius });
    win.draw(rect);
    // corners
    sf::CircleShape corner(radius);
    corner.setFillColor(fill);
    float positions[4][2] = { {x,y},{x + w - 2 * radius,y},{x,y + h - 2 * radius},{x + w - 2 * radius,y + h - 2 * radius} };
    for (auto& p : positions) { corner.setPosition(p[0], p[1]); win.draw(corner); }
    if (outThick > 0.f) {
        sf::RectangleShape border({ w, h });
        border.setPosition(x, y);
        border.setFillColor(sf::Color::Transparent);
        border.setOutlineColor(outline);
        border.setOutlineThickness(outThick);
        win.draw(border);
    }
}

// ─── Main ───────────────────────────────────────────────────────────────────
int main() {
    srand((unsigned)time(nullptr));

    sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "Sudoku",
        sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    // Load font — try system paths; fallback to default if unavailable
    sf::Font font;
    bool fontLoaded = false;
    const char* fontPaths[] = {
        "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf",
        "/usr/share/fonts/truetype/freefont/FreeSansBold.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "/System/Library/Fonts/Helvetica.ttc",
        nullptr
    };
    for (int i = 0; fontPaths[i]; i++) {
        if (font.loadFromFile(fontPaths[i])) { fontLoaded = true; break; }
    }
    // If no font found, SFML will use the fallback bitmap font — still functional.

    Game game;

    // ── Button helpers ──────────────────────────────────────────────────────
    auto makeText = [&](const std::string& s, unsigned size, sf::Color col,
        float x, float y) -> sf::Text {
            sf::Text t;
            t.setFont(font);
            t.setString(s);
            t.setCharacterSize(size);
            t.setFillColor(col);
            t.setPosition(x, y);
            return t;
        };
    auto centeredText = [&](const std::string& s, unsigned size, sf::Color col,
        float cx, float cy) -> sf::Text {
            sf::Text t = makeText(s, size, col, 0, 0);
            sf::FloatRect b = t.getLocalBounds();
            t.setOrigin(b.width / 2.f, b.height / 2.f);
            t.setPosition(cx, cy);
            return t;
        };

    struct Button {
        sf::FloatRect rect;
        std::string   label;
        bool hovered = false;
    };

    // ── Main loop ───────────────────────────────────────────────────────────
    sf::Clock clock;
    float winFlash = 0.f;

    while (window.isOpen()) {
        sf::Event ev;
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        float dt = clock.restart().asSeconds();
        if (winFlash > 0.f) winFlash -= dt;

        // ── MENU ────────────────────────────────────────────────────────────
        if (game.state == GameState::MENU) {
            while (window.pollEvent(ev)) {
                if (ev.type == sf::Event::Closed) window.close();
                if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape)
                    window.close();
                if (ev.type == sf::Event::MouseButtonPressed) {
                    int mx = ev.mouseButton.x, my = ev.mouseButton.y;
                    // Easy / Medium / Hard buttons at y=370, 440, 510
                    if (mx > 200 && mx < 500) {
                        if (my > 360 && my < 420) { game.difficulty = 0; game.newGame(); }
                        if (my > 430 && my < 490) { game.difficulty = 1; game.newGame(); }
                        if (my > 500 && my < 560) { game.difficulty = 2; game.newGame(); }
                    }
                }
            }
            window.clear(BG);
            // Title
            sf::Text title = centeredText("SUDOKU", 72, ACCENT, WINDOW_W / 2.f, 140.f);
            window.draw(title);
            sf::Text sub = centeredText("Select difficulty to begin", 20, { 120,130,170,255 }, WINDOW_W / 2.f, 220.f);
            window.draw(sub);

            const char* labels[] = { "EASY","MEDIUM","HARD" };
            sf::Color   cols[] = { {60,200,100,255},{90,130,255,255},{220,80,80,255} };
            for (int i = 0; i < 3; i++) {
                float by = 360.f + i * 70.f;
                bool hov = (mouse.x > 200 && mouse.x < 500 && mouse.y > by && mouse.y < by + 60);
                drawRoundedRect(window, 200, by, 300, 60, 10, hov ? BTN_HOV : BTN_BG, cols[i], 2.f);
                sf::Text bt = centeredText(labels[i], 26, cols[i], WINDOW_W / 2.f, by + 30.f);
                window.draw(bt);
            }
            window.display();
            continue;
        }

        // ── WON ─────────────────────────────────────────────────────────────
        if (game.state == GameState::WON) {
            while (window.pollEvent(ev)) {
                if (ev.type == sf::Event::Closed) window.close();
                if (ev.type == sf::Event::KeyPressed) {
                    if (ev.key.code == sf::Keyboard::Escape) game.state = GameState::MENU;
                    if (ev.key.code == sf::Keyboard::N) game.newGame();
                }
                if (ev.type == sf::Event::MouseButtonPressed) {
                    int mx = ev.mouseButton.x, my = ev.mouseButton.y;
                    if (mx > 150 && mx < 350 && my > 500 && my < 560) game.newGame();
                    if (mx > 370 && mx < 550 && my > 500 && my < 560) game.state = GameState::MENU;
                }
            }
            // Draw completed board faintly
            window.clear(BG);
            // Board
            for (int r = 0; r < 9; r++) {
                for (int c = 0; c < 9; c++) {
                    float cx = (float)BOARD_X + c * (float)CELL_SIZE;
                    float cy = (float)BOARD_Y + r * (float)CELL_SIZE;
                    sf::RectangleShape cell({ (float)CELL_SIZE - 1,(float)CELL_SIZE - 1 });
                    cell.setPosition(cx + 1, cy + 1);
                    cell.setFillColor({ 20,60,40,180 });
                    window.draw(cell);
                    int val = game.puzzle[r][c];
                    if (val != 0) {
                        sf::Text t = centeredText(std::to_string(val), 28, TXT_HINT,
                            cx + CELL_SIZE / 2.f, cy + CELL_SIZE / 2.f - 4.f);
                        window.draw(t);
                    }
                }
            }
            sf::Text win1 = centeredText("YOU WIN!", 64, ACCENT, WINDOW_W / 2.f, 180.f);
            window.draw(win1);
            std::string ms = "Mistakes: " + std::to_string(game.mistakes);
            sf::Text win2 = centeredText(ms, 28, { 180,190,220,255 }, WINDOW_W / 2.f, 270.f);
            window.draw(win2);
            // Buttons
            bool h1 = (mouse.x > 150 && mouse.x < 350 && mouse.y>500 && mouse.y < 560);
            bool h2 = (mouse.x > 370 && mouse.x < 550 && mouse.y>500 && mouse.y < 560);
            drawRoundedRect(window, 150.f, 500.f, 200.f, 60.f, 10.f, h1 ? BTN_HOV : BTN_BG, ACCENT, 2.f);
            drawRoundedRect(window, 370.f, 500.f, 180.f, 60.f, 10.f, h2 ? BTN_HOV : BTN_BG, ACCENT, 2.f);
            sf::Text b1 = centeredText("NEW GAME", 22, ACCENT, 250.f, 530.f);
            sf::Text b2 = centeredText("MENU", 22, ACCENT, 460.f, 530.f);
            window.draw(b1); window.draw(b2);
            window.display();
            continue;
        }

        // ── PLAYING ─────────────────────────────────────────────────────────
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) window.close();

            if (ev.type == sf::Event::KeyPressed) {
                if (ev.key.code == sf::Keyboard::Escape) {
                    game.state = GameState::MENU; continue;
                }
                // Arrow keys
                if (ev.key.code == sf::Keyboard::Up && game.selR > 0) game.selR--;
                if (ev.key.code == sf::Keyboard::Down && game.selR < 8) game.selR++;
                if (ev.key.code == sf::Keyboard::Left && game.selC > 0) game.selC--;
                if (ev.key.code == sf::Keyboard::Right && game.selC < 8) game.selC++;
                // Delete / Backspace
                if ((ev.key.code == sf::Keyboard::Delete || ev.key.code == sf::Keyboard::BackSpace)
                    && game.selR >= 0 && !game.fixed[game.selR][game.selC]) {
                    game.puzzle[game.selR][game.selC] = 0;
                    game.wrong[game.selR][game.selC] = false;
                }
                // Digit keys (both main and numpad)
                int digit = -1;
                if (ev.key.code >= sf::Keyboard::Num1 && ev.key.code <= sf::Keyboard::Num9)
                    digit = ev.key.code - sf::Keyboard::Num1 + 1;
                if (ev.key.code >= sf::Keyboard::Numpad1 && ev.key.code <= sf::Keyboard::Numpad9)
                    digit = ev.key.code - sf::Keyboard::Numpad1 + 1;
                if (digit >= 1 && digit <= 9) {
                    game.enterDigit(digit);
                    if (game.isSolved()) { game.state = GameState::WON; winFlash = 1.f; }
                }
                // H = hint
                if (ev.key.code == sf::Keyboard::H) {
                    game.useHint();
                    if (game.isSolved()) game.state = GameState::WON;
                }
                // N = new game
                if (ev.key.code == sf::Keyboard::N) game.newGame();
            }

            if (ev.type == sf::Event::MouseButtonPressed) {
                int mx = ev.mouseButton.x, my = ev.mouseButton.y;
                sf::Vector2i cell = cellAt(mx, my);
                if (cell.x >= 0) { game.selR = cell.x; game.selC = cell.y; }

                // Bottom buttons — New Game (left), Hint (right)
                if (my > 670 && my < 720) {
                    if (mx > 40 && mx < 200) game.newGame();
                    if (mx > 220 && mx < 380) { game.useHint(); if (game.isSolved()) game.state = GameState::WON; }
                    if (mx > 400 && mx < 560) game.state = GameState::MENU;
                }
            }
        }

        // ── DRAW ─────────────────────────────────────────────────────────────
        window.clear(BG);

        // Title bar
        {
            sf::Text title = makeText("SUDOKU", 32, ACCENT, 40.f, 20.f);
            window.draw(title);

            // Mistakes / Hints info on right
            std::string info = "Mistakes: " + std::to_string(game.mistakes)
                + "   Hints: " + std::to_string(game.hints);
            sf::Text inf = makeText(info, 18, { 150,160,200,255 }, 300.f, 28.f);
            window.draw(inf);
        }

        // Digit input row (1-9 clickable at bottom)
        // ── Number pad row ───────────────────────────────────────────────────
        {
            float py = 630.f;
            for (int d = 1; d <= 9; d++) {
                float px = 40.f + (d - 1) * 58.f + 10.f;
                bool hov = (mouse.x > px && mouse.x < px + 48 && mouse.y > py && mouse.y < py + 48);
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && hov) {
                    game.enterDigit(d);
                    if (game.isSolved()) game.state = GameState::WON;
                }
                sf::RectangleShape btn({ 48.f, 48.f });
                btn.setPosition(px, py);
                btn.setFillColor(hov ? BTN_HOV : BTN_BG);
                btn.setOutlineColor(ACCENT);
                btn.setOutlineThickness(1.5f);
                window.draw(btn);
                sf::Text dt = centeredText(std::to_string(d), 26, ACCENT, px + 24, py + 24);
                window.draw(dt);
            }
        }

        // ── Bottom action buttons ────────────────────────────────────────────
        {
            float by = 700.f;
            std::string hintLbl = "HINT (" + std::to_string(game.hints) + ")";

            auto drawBtn = [&](float x, float y, float w, float h,
                const std::string& lbl, sf::Color accentCol) {
                    bool hov = (mouse.x > x && mouse.x < x + w && mouse.y > y && mouse.y < y + h);
                    drawRoundedRect(window, x, y, w, h, 8, hov ? BTN_HOV : BTN_BG, accentCol, 2.f);
                    sf::Text t = centeredText(lbl, 18, accentCol, x + w / 2.f, y + h / 2.f);
                    window.draw(t);
                };
            drawBtn(40.f, by, 160.f, 45.f, "NEW GAME", { 60,200,100,255 });
            drawBtn(215.f, by, 160.f, 45.f, hintLbl, { 90,130,255,255 });
            drawBtn(390.f, by, 160.f, 45.f, "MENU", { 220,120,80,255 });
        }

        
        int selVal = (game.selR >= 0 && game.selC >= 0) ? game.puzzle[game.selR][game.selC] : -1;

        for (int r = 0; r < 9; r++) {
            for (int c = 0; c < 9; c++) {
                float cx = (float)BOARD_X + c * (float)CELL_SIZE;
                float cy = (float)BOARD_Y + r * (float)CELL_SIZE;
                int val = game.puzzle[r][c];

                // Cell background color
                sf::Color bg = game.fixed[r][c] ? CELL_FIXED : CELL_BG;
                if (game.wrong[r][c])           bg = CELL_WRONG;
                else if (r == game.selR && c == game.selC) bg = CELL_SEL;
                else if (r == game.selR || c == game.selC) bg = { 28,35,60,255 };
                else if (selVal > 0 && val == selVal)       bg = CELL_SAME;

                sf::RectangleShape cell({ (float)CELL_SIZE - 1, (float)CELL_SIZE - 1 });
                cell.setPosition(cx + 0.5f, cy + 0.5f);
                cell.setFillColor(bg);
                window.draw(cell);

                if (val != 0)
                {
                    sf::Color tc = game.wrong[r][c] ? TXT_WRONG
                        : game.fixed[r][c] ? TXT_FIXED
                        : TXT_USER;
                    sf::Text t = centeredText(std::to_string(val), 28, tc,
                        cx + CELL_SIZE / 2.f, cy + CELL_SIZE / 2.f - 4.f);
                    window.draw(t);
                }
            }
        }

        for (int i = 0; i <= 9; i++)
        {
            bool thick = (i % 3 == 0);
            sf::Color lc = thick ? BOX_LINE : GRID_LINE;
            float     lw = thick ? 2.5f : 1.f;

           
            sf::RectangleShape hl({ (float)BOARD_SIZE, lw });
            hl.setPosition(BOARD_X, BOARD_Y + i * CELL_SIZE - lw / 2.f);
            hl.setFillColor(lc);
            window.draw(hl);

            
            sf::RectangleShape vl({ lw, (float)BOARD_SIZE });
            vl.setPosition(BOARD_X + i * CELL_SIZE - lw / 2.f, BOARD_Y);
            vl.setFillColor(lc);
            window.draw(vl);
        }

       
        {
            sf::Text keys = makeText(
                "[1-9] Enter  [Del] Clear  [H] Hint  [Arrows] Move  [N] New  [Esc] Menu",
                13, { 70,80,110,255 }, 40.f, 758.f);
            window.draw(keys);
        }

        window.display();
    }

    return 0;
}