#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int width = 40;
const int height = 20;

int ballX, ballY;
int ballDirX = -1, ballDirY = -1;
int paddleX;
int paddleWidth = 7;
int score = 0;
bool gameOver = false;

int ballSpeed = 1;
int ballUpdateCounter = 0;

const int brickRows = 5;
const int brickCols = 10;
vector<pair<int, int>> bricks;

const int frameRate = 12.5;

void SetCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void HideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = false;
    cursorInfo.dwSize = 100;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void Setup() {
    ballX = width / 2;
    ballY = height / 2;

    paddleX = width / 2 - paddleWidth / 2;

    bricks.clear();
    for (int y = 2; y < 2 + brickRows; y++) {
        for (int x = 1; x < width - 2; x += (width - 2) / brickCols) {
            bricks.push_back({ x, y });
        }
    }

    HideCursor();
}

void Draw() {
    SetCursorPosition(0, 0);

    for (int i = 0; i < width; i++)
        cout << "#";
    cout << endl;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x == 0 || x == width - 1)
                cout << "#";
            else if (x == ballX && y == ballY)
                cout << "O";
            else if (y == height - 1 && x >= paddleX && x < paddleX + paddleWidth)
                cout << "=";
            else {
                bool isBrick = false;
                for (auto& brick : bricks) {
                    if (x == brick.first && y == brick.second) {
                        cout << "*";
                        isBrick = true;
                        break;
                    }
                }
                if (!isBrick)
                    cout << " ";
            }
        }
        cout << endl;
    }

    for (int i = 0; i < width; i++)
        cout << "#";
    cout << endl;

    cout << "Score: " << score << endl;
}

void Input() {
    if (_kbhit()) {
        char ch = _getch();
        if (ch == 'a' || ch == 'A')
            paddleX = max(0, paddleX - 2);
        if (ch == 'd' || ch == 'D')
            paddleX = min(width - paddleWidth - 1, paddleX + 2);
    }
}

void BallLogic() {
    if (ballUpdateCounter % ballSpeed == 0) {
        ballX += ballDirX;
        ballY += ballDirY;

        if (ballX <= 0 || ballX >= width - 1)
            ballDirX = -ballDirX;
        if (ballY <= 0)
            ballDirY = -ballDirY;

        if (ballY == height - 1 && ballX >= paddleX && ballX < paddleX + paddleWidth)
            ballDirY = -ballDirY;

        for (int i = 0; i < bricks.size(); i++) {
            if (ballX == bricks[i].first && ballY == bricks[i].second) {
                bricks.erase(bricks.begin() + i);
                ballDirY = -ballDirY;
                score += 10;
                break;
            }
        }

        if (ballY > height) {
            gameOver = true;
        }

        if (bricks.empty()) {
            gameOver = true;
        }
    }

    ballUpdateCounter++;
}

void PlayGame() {
    Setup();
    while (!gameOver) {
        auto start = high_resolution_clock::now();

        Draw();
        Input();
        BallLogic();

        auto end = high_resolution_clock::now();
        duration<double, milli> frameTime = end - start;
        double frameDuration = 1000.0 / frameRate;

        if (frameTime.count() < frameDuration) {
            Sleep(static_cast<DWORD>(frameDuration - frameTime.count()));
        }
    }

    system("cls");
    if (bricks.empty()) {
        cout << "You Win. Your Score - " << score << endl;
    }
    else {
        cout << "Game Over. Your Score - " << score << endl;
    }

    Sleep(3000);
}

int main() {
    PlayGame();
    return 0;
}
