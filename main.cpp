#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

using namespace std;

// Game variables
bool gameOver;
bool isPaused;
const int width = 20;
const int height = 17;
int x, y, foodX, foodY, score, level, speed;
int tailX[100], tailY[100];
int nTail;
char foodLetter;
string targetWord;
int letterIndex;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

string GenerateRandomWord() {
    string words[] = { "SNAKE", "APPLE", "HELLO", "WORLD", "GAMES" };
    int randomIndex = rand() % (sizeof(words) / sizeof(words[0]));
    return words[randomIndex];
}

void Setup() {
    gameOver = false;
    isPaused = false;
    dir = STOP; // Ensures snake stays still at the start
    x = width / 2;
    y = height / 2;
    foodX = rand() % width;
    foodY = rand() % height;
    score = 0;
    level = 1;
    speed = 100;
    nTail = 0;
    targetWord = GenerateRandomWord();
    letterIndex = 0;
    foodLetter = targetWord[letterIndex];
}

void MainMenu() {
    system("cls");
    cout << "=================" << endl;
    cout << "  SNAKE WORLD" << endl;
    cout << "=================" << endl;
    cout << "1. Start Game\n";
    cout << "2. Quit\n";
    cout << "Enter your choice: ";
    char choice;
    while (true) {
        choice = _getch();
        if (choice == '1') {
            Setup();
            return;
        }
        else if (choice == '2') {
            exit(0);
        }
    }
}

void PauseMenu() {
    system("cls");
    cout << "================" << endl;
    cout << "  GAME PAUSED" << endl;
    cout << "================" << endl;
    cout << "1. RESUME" << endl;
    cout << "2. QUIT" << endl;
    char choice;
    while (true) {
        choice = _getch();
        if (choice == '1') {
            return;
        }
        else if (choice == '2') {
            exit(0);
        }
    }
}

void Retry() {
    system("cls");
    cout << "================" << endl;
    cout << "   GAME OVER" << endl;
    cout << "================" << endl;
    cout << "" << endl;
    cout << "Final Score : " << score << endl;
    cout << "Reached Level : " << level << endl;
    cout << "" << endl;
    cout << "1. RETRY" << endl;
    cout << "2. QUIT" << endl;
    while (true) {
        char choice = _getch();
        if (choice == '1') {
            Setup();
            return;
        }
        else if (choice == '2') {
            exit(0);
        }
    }
}

void Draw() {
    COORD cursorPosition = { 0, 0 };
    SetConsoleCursorPosition(console, cursorPosition);
    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << endl;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) cout << "#";
            if (i == y && j == x) cout << "O";
            else if (i == foodY && j == foodX) cout << foodLetter;
            else {
                bool printTail = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o";
                        printTail = true;
                    }
                }
                if (!printTail) cout << " ";
            }
            if (j == width - 1) cout << "#";
        }
        cout << endl;
    }
    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << "\nScore: " << score << "  Level: " << level << "  Next Letter: " << foodLetter << endl;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a': if (dir != RIGHT) dir = LEFT; break;
        case 'd': if (dir != LEFT) dir = RIGHT; break;
        case 'w': if (dir != DOWN) dir = UP; break;
        case 's': if (dir != UP) dir = DOWN; break;
        case 'p': PauseMenu(); break;
        case 'x': gameOver = true; break;
        }
    }
}

void Logic() {
    if (dir == STOP) return; // Prevent movement if no key is pressed

    int prevX = tailX[0], prevY = tailY[0], prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
    case LEFT: x--; break;
    case RIGHT: x++; break;
    case UP: y--; break;
    case DOWN: y++; break;
    }

    if (x >= width || x < 0 || y >= height || y < 0) gameOver = true;
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y) gameOver = true;
    }

    if (x == foodX && y == foodY) {
        score += 10;
        nTail++;
        letterIndex++;
        if (letterIndex >= targetWord.length()) {
            system("cls");
            cout << "====================" << endl;
            cout << "  CONGRATULATIONS!" << endl;
            cout << "====================" << endl;
            cout << "" << endl;
            cout << "You completed the word" << endl;
            cout << "   " << targetWord << "!\n";
            cout << "" << endl;
            cout << "1. NEXT LEVEL" << endl;
            cout << "2. QUIT" << endl;
            while (true) {
                char choice = _getch();
                if (choice == '1') {
                    level++;
                    speed -= 5;
                    if (speed < 40) speed = 40;
                    targetWord = GenerateRandomWord();
                    letterIndex = 0;
                    break;
                }
                else if (choice == '2') exit(0);
            }
        }
        foodLetter = targetWord[letterIndex];
        foodX = rand() % width;
        foodY = rand() % height;
    }
}

int main() {
    srand(time(0));
    MainMenu();
    while (true) {
        while (!gameOver) {
            Draw();
            Input();
            Logic();
            Sleep(speed);
        }
        Retry();
    }
    return 0;
}