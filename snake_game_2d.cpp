#include <iostream>
#include <vector>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <unistd.h> // For usleep() (POSIX)

using namespace std;

// Define the board dimensions
const int WIDTH = 20;
const int HEIGHT = 10;

// Define the directions
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

// Snake segment
struct SnakeSegment {
    int x, y;
};

// Function prototypes
void Setup();
void Draw();
void Input();
void Logic();
bool CheckCollision();
bool CheckFoodCollision();
void GenerateFood();
void Grow();
void SleepFor(unsigned int milliseconds);

// Global variables
bool gameOver;
Direction dir;
vector<SnakeSegment> snake;
SnakeSegment food;
int score;

void Setup() {
    gameOver = false;
    dir = STOP;
    SnakeSegment initialSegment = { WIDTH / 2, HEIGHT / 2 };
    snake.push_back(initialSegment);
    score = 0;
    GenerateFood();
}

void Draw() {
    system("clear"); // Clear the console (POSIX, use "cls" on Windows)

    // Draw the top border
    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << endl;

    // Draw the game board
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0)
                cout << "#";
            bool printed = false;

            // Draw the snake
            for (const auto& segment : snake) {
                if (segment.x == x && segment.y == y) {
                    cout << "*";
                    printed = true;
                    break;
                }
            }

            // Draw the food
            if (food.x == x && food.y == y) {
                cout << "@";
                printed = true;
            }

            // Draw empty space
            if (!printed)
                cout << " ";
        }
        cout << "#" << endl;
    }

    // Draw the bottom border
    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << endl;

    // Display score
    cout << "Score: " << score << endl;
}

void Input() {
    char c;
    if (cin >> c) {
        switch (c) {
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
            case 'q':
                gameOver = true;
                break;
        }
    }
}

void Logic() {
    SnakeSegment newHead = snake.front();

    // Move the snake
    switch (dir) {
        case LEFT:
            newHead.x--;
            break;
        case RIGHT:
            newHead.x++;
            break;
        case UP:
            newHead.y--;
            break;
        case DOWN:
            newHead.y++;
            break;
    }

    // Check for collisions
    if (CheckCollision()) {
        gameOver = true;
        return;
    }

    // Check if the snake has eaten the food
    if (CheckFoodCollision()) {
        score += 10;
        GenerateFood();
        Grow();
    } else {
        snake.pop_back(); // Remove the tail segment
    }

    // Add the new head segment
    snake.insert(snake.begin(), newHead);
}

bool CheckCollision() {
    const auto& head = snake.front();

    // Check wall collision
    if (head.x < 0 || head.x >= WIDTH || head.y < 0 || head.y >= HEIGHT)
        return true;

    // Check self collision
    for (size_t i = 1; i < snake.size(); ++i) {
        if (head.x == snake[i].x && head.y == snake[i].y)
            return true;
    }

    return false;
}

bool CheckFoodCollision() {
    return snake.front().x == food.x && snake.front().y == food.y;
}

void GenerateFood() {
    food.x = rand() % WIDTH;
    food.y = rand() % HEIGHT;
}

void Grow() {
    snake.push_back(snake.back()); // Add a new segment to the end of the snake
}

void SleepFor(unsigned int milliseconds) {
    usleep(milliseconds * 1000); // Sleep for a given number of milliseconds (POSIX)
}

int main() {
    srand(static_cast<unsigned>(time(0))); // Seed the random number generator
    Setup();

    while (!gameOver) {
        Draw();
        Input();
        Logic();
        SleepFor(200); // Sleep for a short period to control game speed
    }

    cout << "Game Over! Your score was " << score << endl;
    return 0;
}

