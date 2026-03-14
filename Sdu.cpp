#include<iostream>
#include<ctime>
#include <cstdlib>
using namespace std;
void alot(int**& a)
{
    a = new int* [9];
    for (int i = 0; i < 9; i++)
        a[i] = new int[9];
}
void del(int**& a)
{
    for (int i = 0; i < 9; i++)
        delete[]a[i];
    delete[]a;
    a = nullptr;
}
bool valid(int** p,int r,int c, int x)
{
	for (int i = 0; i < 9; i++)
	{
		if (x == p[r][i]) return false;
		if (x == p[i][c])   return false;
	}
	int row = r / 3;
	int col = c / 3;
	for (int k = 0; k < 3; k++)
	{
		for (int m = 0; m < 3; m++)
		{
			if (p[row*3 + k][col*3 + m] == x)
				return false;
		}
	}
	return true;
}
void shuffle(int *d) {
	for (int i = 8; i > 0; i--) {
		int j = rand() % (i + 1);
		swap(d[i], d[j]);
	}
}
bool solve(int **p)
{
    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            if (p[r][c] == 0)
            {
                int digits[9] = { 1,2,3,4,5,6,7,8,9 };
                shuffle(digits);

                for (int i = 0; i < 9; i++)
                {
                    int val = digits[i];
                    if (valid(p, r, c, val))
                    {
                        p[r][c] = val;
                        if (solve(p)) return true;
                        p[r][c] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}
void print(int** m)
{
    cout << "  1 2 3  4 5 6  7 8 9\n";
    for (int i = 0; i < 9; i++)
    {
        if (i % 3 == 0)
            cout << " +------+------+------+\n"; 

        cout << i + 1 << "|"; 
        for (int j = 0; j < 9; j++)
        {
            if (m[i][j] == 0)
                cout << ".";
            else
                cout << m[i][j];

            cout << " ";

            if ((j + 1) % 3 == 0)
                cout << "|"; 
        }
        cout << endl;
    }
    cout << " +------+------+------+\n";
}
void generate(int** solution, int** puzzle, int**& empty)
{
    alot(empty);

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            empty[i][j] = 0;

    solve(empty);  

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++) {
            solution[i][j] = empty[i][j];
            puzzle[i][j] = empty[i][j];
        }

    int removals = 45;
    while (removals--)
    {
        int r = rand() % 9;
        int c = rand() % 9;
        if (puzzle[r][c] != 0)
            puzzle[r][c] = 0;
        else
            removals++;
    }
}
int main() {
    srand(time(0));  

    int** solution = nullptr, ** puzzle = nullptr, ** empty = nullptr;

    alot(solution);
    alot(puzzle);


    generate(solution, puzzle, empty);


    bool fixed[9][9];
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            fixed[i][j] = (puzzle[i][j] != 0);

    while (true) {
        print(puzzle);
        cout << "Enter row(1-9) col(1-9) value(1-9) or 0 to exit: ";
        int r, c, val;
        cin >> r;
        if (r == 0) break;
        cin >> c >> val;

        r--; c--; 

        if (r < 0 || r>8 || c < 0 || c>8 || val < 1 || val>9) {
            cout << "Invalid input. Try again.\n";
            continue;
        }

        if (fixed[r][c]) {
            cout << "Cannot change a fixed cell!\n";
            continue;
        }

        if (solution[r][c] == val) {
            puzzle[r][c] = val;
            cout << "Correct!\n";
        }
        else {
            cout << "Wrong move.\n";
        }

  
        bool solved = true;
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                if (puzzle[i][j] == 0) solved = false;
        if (solved) {
            print(puzzle);
            cout << "Congratulations! You solved the Sudoku!\n";
            break;
        }
    }


    del(solution);
    del(puzzle);
    del(empty);

    return 0;
}
