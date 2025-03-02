#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

// Game variables
bool gameOver;
bool isPaused;
const int width = 20;
const int height = 17;
int x, y, foodX, foodY, score, level, speed;
int tailX[100], tailY[100]; // Vectors used to store snake tail positions
int nTail;
char foodLetter;
string targetWord = "LINEAR_ALGEBRA";
int letterIndex = 0;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

// Function to initialize the game
void Setup()
{
    gameOver = false;
    isPaused = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    foodX = rand() % width;
    foodY = rand() % height;
    score = 0;
    level = 1;
    speed = 100;
    foodLetter = targetWord[letterIndex];
    nTail = 0;
}

// Function to move cursor position
void gotoxy(int x, int y)
{
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

// Function to draw the game board
void Draw()
{
    gotoxy(0, 0);
    cout << "Score: " << score << "  Level: " << level << "  Target: " << targetWord << "\n";

    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j == 0)
                cout << "#";
            if (i == y && j == x)
                cout << "O"; // Snake head
            else if (i == foodY && j == foodX)
                cout << foodLetter; // Food as next letter
            else
            {
                bool print = false;
                for (int k = 0; k < nTail; k++) // Iterating through tail positions (vectors)
                {
                    if (tailX[k] == j && tailY[k] == i)
                    {
                        cout << "o"; // Snake tail
                        print = true;
                    }
                }
                if (!print)
                    cout << " ";
            }
            if (j == width - 1)
                cout << "#";
        }
        cout << endl;
    }
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;
}

// Function to handle user input
void Input()
{
    if (_kbhit())
    {
        char currentKey = _getch();
        if (currentKey == 'p')  // If the user presses 'p', pause the game
        {
            isPaused = !isPaused;
        }
        else if (!isPaused) // Allow movement only if the game is not paused
        {
            switch (currentKey)
            {
            case 'a':
                if (dir != RIGHT) dir = LEFT;
                break;
            case 'd':
                if (dir != LEFT) dir = RIGHT;
                break;
            case 'w':
                if (dir != DOWN) dir = UP;
                break;
            case 's':
                if (dir != UP) dir = DOWN;
                break;
            case 'x':
                gameOver = true;
                break;
            }
        }
    }
}

// Function to update game logic
void Logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    // Game Over if the snake hits the wall
    if (x >= width || x < 0 || y >= height || y < 0)
    {
        gameOver = true;
        return;
    }

    // Game Over if the snake hits itself
    for (int i = 0; i < nTail; i++) // Checking collision with snake tail (vectors)
    {
        if (tailX[i] == x && tailY[i] == y)
        {
            gameOver = true;
            return;
        }
    }

    // Eating food logic
    if (x == foodX && y == foodY)
    {
        score += 10;
        letterIndex++;
        if (letterIndex < targetWord.length())
        {
            foodLetter = targetWord[letterIndex];
            foodX = rand() % width;
            foodY = rand() % height;
            nTail++;
        }
        else
        {
            level++;
            if (level > 5)
            {
                level = 1;
                speed = 100;
            }
            letterIndex = 0;
            foodLetter = targetWord[letterIndex];
            foodX = rand() % width;
            foodY = rand() % height;
            nTail = 0;
            speed -= 20;
        }
    }
}

// Function to prompt retry or exit
void Retry()
{
    char choice;
    system("cls");  // Clear the screen before retrying
    cout << "Game Over! Final Score: " << score << "  Reached Level: " << level << endl;
    cout << "Press 'r' to retry or 'q' to quit: ";
    cin >> choice;
    if (choice == 'r')
    {
        Setup();  // Reset the game state and start over
        gameOver = false; // Set gameOver to false to continue playing
    }
    else
    {
        exit(0);  // Quit the game
    }
}

// Function to show the main menu
void MainMenu()
{
    char choice;
    while (true)
    {
        system("cls"); // Clear the screen
        cout << "==================" << endl;
        cout << "    SNAKE GAME   " << endl;
        cout << "==================" << endl;
        cout << "1. Start Game" << endl;
        cout << "2. Quit" << endl;
        cout << "Choose an option: ";
        cin >> choice;
        if (choice == '1')
        {
            break;  // Start the game
        }
        else if (choice == '2')
        {
            exit(0);  // Quit the game
        }
    }
}

// Function to show the pause menu
void PauseMenu()
{
    char choice;
    while (isPaused)
    {
        system("cls");
        cout << "=============" << endl;
        cout << "   PAUSED   " << endl;
        cout << "=============" << endl;
        cout << "1. Resume" << endl;
        cout << "2. Quit" << endl;
        cout << "Choose an option: ";
        cin >> choice;
        if (choice == '1')
        {
            isPaused = false;
        }
        else if (choice == '2')
        {
            exit(0);
        }
    }
}

// Main function
int main()
{
    MainMenu(); // Show main menu before starting the game

    while (true)
    {
        Setup();  // Set up the game state
        while (!gameOver)
        {
            Draw();
            Input();
            Logic();
            Sleep(speed);

            if (isPaused)
            {
                PauseMenu(); // Show the pause menu when the game is paused
            }
        }
        // Once the game is over, call retry function
        Retry();
    }
    return 0;
}
