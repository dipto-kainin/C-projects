#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>

#define WIDTH 40
#define HEIGHT 20
#define INITIAL_SPEED 100
#define SPEED_INCREMENT 10

typedef struct {
    int x, y;
} Position;

typedef struct {
    Position head;
    Position body[100];
    int length;
    int speed;
    int score;
    int lives;
} Snake;

Position food;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum Direction dir;

void welcomeArt(void);
void gameMenu(Snake *snake);
void playGame(Snake *snake);
void Setup(Snake *snake);
void Draw(Snake *snake);
void Input();
void Logic(Snake *snake);
void increaseSpeed(Snake *snake);
void placeFood(Snake *snake);
void gameOver(Snake *snake);
void displayScore(Snake *snake);

int main() {
    welcomeArt();
    Snake snake;
    gameMenu(&snake);
    return 0;
}

void welcomeArt(void) {
    system("cls");
    printf("\n");
    // Set text color to green
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
    printf("\t\t    _________         _________            \n");
    printf("\t\t   /         \\       /         \\           \n");
    printf("\t\t  /  /~~~~~\\  \\     /  /~~~~~\\  \\          \n");
    printf("\t\t  |  |     |  |     |  |     |  |          \n");
    printf("\t\t  |  |     |  |     |  |     |  |          \n");
    printf("\t\t  |  |     |  |     |  |     |  |         / \n");
    printf("\t\t  |  |     |  |     |  |     |  |       // \n");
    printf("\t\t (o  o)    \\  \\_____/  /     \\  \\_____/ / \n");
    printf("\t\t  \\__/      \\         /       \\        /  \n");
    printf("\t\t    |        ~~~~~~~~~         ~~~~~~~~     \n");
    // Reset text color
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("\t\t    ^                                        \n");
    printf("\t    Welcome To The Snake Game!            \n");
    printf("\t    Press Any Key To Start...    \n");
    printf("\n");

    while (!_kbhit());
    _getch();
}

void gameMenu(Snake *snake) {
    system("cls");
    printf("\n");
    // Set text color to cyan for the menu
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN);
    printf("\t\t\t\t\t\t   SNAKE GAME\n\n");
    printf("\t\t\t\t   1. Start Game\n");
    printf("\t\t\t\t   2. Instructions\n");
    printf("\t\t\t\t   3. Exit\n");
    // Reset text color
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("\n");

    char choice;
    choice = _getch();
    switch(choice) {
        case '1':
            Setup(snake);
            playGame(snake);
            break;
        case '2':
            // Display instructions
            break;
        case '3':
            exit(0);
            break;
        default:
            gameMenu(snake);
            break;
    }
}

void playGame(Snake *snake) {
    while (1) {
        Draw(snake);
        Input();
        Logic(snake);
        Sleep(snake->speed);
    }
}

void Setup(Snake *snake) {
    srand(time(NULL));
    snake->head.x = WIDTH / 2;
    snake->head.y = HEIGHT / 2;
    snake->length = 1;
    snake->speed = INITIAL_SPEED;
    snake->score = 0;
    dir = STOP;
    placeFood(snake);
}

void Draw(Snake *snake) {
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screenInfo;
    GetConsoleScreenBufferInfo(output, &screenInfo);

    COORD pos;

    // Calculate the position to center align the game board
    int centerX = (screenInfo.dwSize.X - WIDTH) / 2;
    int centerY = (screenInfo.dwSize.Y - HEIGHT) / 2;
    pos.X = centerX;
    pos.Y = centerY;
    SetConsoleCursorPosition(output, pos);

    // Set border color
    SetConsoleTextAttribute(output, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0 || j == WIDTH - 1 || i == 0 || i == HEIGHT - 1)
                printf("#");

            else if (i == food.y && j == food.x) {
                // Set food color
                SetConsoleTextAttribute(output, FOREGROUND_RED);
                printf("F");
            }
            else if (i == snake->head.y && j == snake->head.x) {
                // Set snake head color
                SetConsoleTextAttribute(output, FOREGROUND_GREEN);
                printf("O");
            }
            else {
                int printTail = 0;
                for (int k = 0; k < snake->length; k++) {
                    if (snake->body[k].x == j && snake->body[k].y == i) {
                        if (k % 2 == 0)
                            // Set snake body color
                            SetConsoleTextAttribute(output, FOREGROUND_GREEN);
                        else
                            // Set alternate color for snake body segment
                            SetConsoleTextAttribute(output, FOREGROUND_GREEN | FOREGROUND_BLUE);
                        printf("o");
                        printTail = 1;
                    }
                }
                if (!printTail)
                    printf(" ");
            }
            // Reset text color
            SetConsoleTextAttribute(output, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
        printf("\n");
        pos.Y++;
        SetConsoleCursorPosition(output, pos);
    }

    // Display score and lives
    pos.X = centerX;
    pos.Y = centerY - 2; // Adjust position above the game board
    SetConsoleCursorPosition(output, pos);
    // Set text color for score and lives
    SetConsoleTextAttribute(output, FOREGROUND_BLUE | FOREGROUND_GREEN);
    printf("SCORE: %d\tLIVES: %d", snake->score, snake->lives);
    // Reset text color
    SetConsoleTextAttribute(output, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}


void Input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a':
                if (dir != RIGHT)
                    dir = LEFT;
                break;
            case 'd':
                if (dir != LEFT)
                    dir = RIGHT;
                break;
            case 'w':
                if (dir != DOWN)
                    dir = UP;
                break;
            case 's':
                if (dir != UP)
                    dir = DOWN;
                break;
            case 'x':
                gameOver(NULL);
                break;
            case 72: // Up arrow key
                if (dir != DOWN)
                    dir = UP;
                break;
            case 80: // Down arrow key
                if (dir != UP)
                    dir = DOWN;
                break;
            case 75: // Left arrow key
                if (dir != RIGHT)
                    dir = LEFT;
                break;
            case 77: // Right arrow key
                if (dir != LEFT)
                    dir = RIGHT;
                break;
        }
    }
}

void Logic(Snake *snake) {
    Position prev = snake->head;
    Position prev2;
    snake->body[0] = snake->head;
    for (int i = 1; i < snake->length; i++) {
        prev2 = snake->body[i];
        snake->body[i] = prev;
        prev = prev2;
    }
    switch (dir) {
        case LEFT:
            snake->head.x--;
            break;
        case RIGHT:
            snake->head.x++;
            break;
        case UP:
            snake->head.y--;
            break;
        case DOWN:
            snake->head.y++;
            break;
        default:
            break;
    }

    if (snake->head.x >= WIDTH)
        snake->head.x = 0;
    else if (snake->head.x < 0)
        snake->head.x = WIDTH - 1;
    if (snake->head.y >= HEIGHT)
        snake->head.y = 0;
    else if (snake->head.y < 0)
        snake->head.y = HEIGHT - 1;

    for (int i = 1; i < snake->length; i++) {
        if (snake->body[i].x == snake->head.x && snake->body[i].y == snake->head.y)
            gameOver(snake);
    }

    if (snake->head.x == food.x && snake->head.y == food.y) {
        snake->length++;
        snake->score++;
        placeFood(snake);
        increaseSpeed(snake);
    }
}

void increaseSpeed(Snake *snake) {
    if (snake->speed > 20)
        snake->speed -= SPEED_INCREMENT;
}

void placeFood(Snake *snake) {
    do {
        food.x = rand() % (WIDTH - 2) + 1;
        food.y = rand() % (HEIGHT - 2) + 1;
    } while (food.x == snake->head.x && food.y == snake->head.y);
}

void gameOver(Snake *snake) {
    system("cls");
    printf("Game Over! Your score: %d\n", snake->score);
    printf("Press 'R' to restart or 'X' to exit...\n");
    while (1) {
        if (_kbhit()) {
            char key = _getch();
            if (key == 'r' || key == 'R') {
                Setup(snake);
                playGame(snake);
                break;
            }
            else if (key == 'x' || key == 'X') {
                exit(0);
            }
        }
    }
}

void displayScore(Snake *snake) {
    COORD pos = {0, 0};
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
    printf("SCORE: %d\n", snake->score);
}
