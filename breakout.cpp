#include <iostream>
#include <iomanip>
#include <conio.h>
#include <vector>
#include "breakout.h"
#include "console.h"

namespace Breakout
{

struct Block
{
    int posX = 0;
    int posY = 0;
    int size = 1;
};

enum class Direction
{
    Stop = 0,
    UpLeft,
    UpRight,
    DownLeft,
    DownRight
};

struct Ball
{
    Direction direction = Direction::Stop;
    int posX = 0;
    int posY = -1;
};

bool isRunning = true;
char key = 0;
Block player;
int gameLeftLimit = 0;
int gameRightLimit = 0;
int gameUpLimit = 0;
int gameDownLimit = 0;
std::vector<Block> blocks;
Ball ball;
int score = 0;
bool ballmove = false;

void Grow()
{
    int oldPlayerSize = player.size;
    ++player.size;
    if (player.posX - player.size > gameLeftLimit + 1 && player.posX + player.size < gameRightLimit - 2)
        return;
    else if (player.posX - player.size > gameLeftLimit + 1)
        player.posX -= player.size - oldPlayerSize;
    else if (player.posX + player.size < gameRightLimit - 2)
        player.posX += player.size - oldPlayerSize;
}

void Start()
{
    ball.direction = Direction::UpRight;
}

void Input()
{
    if (_kbhit() == 0)
        return;
    else
        key = (char)_getch();

    if (key == 27) // Esc.
        isRunning = false;
    else if (key == 'a' && player.posX - player.size > gameLeftLimit + 1)
    {
        --player.posX;
        if (ball.direction == Direction::Stop)
        {
            ToXY(ball.posX, ball.posY);
            std::cout << " ";
            --ball.posX;
        }
    }
    else if (key == 'd' && player.posX + player.size < gameRightLimit - 2)
    {
        ++player.posX;
        if (ball.direction == Direction::Stop)
        {
            ToXY(ball.posX, ball.posY);
            std::cout << " ";
            ++ball.posX;
        }
    }
    else if (key == '+')
        Grow();
    else if (key == ' ')
        Start();
}

void PrintBoard()
{
    for (auto i = gameUpLimit; i < gameDownLimit; ++i)
    {
        // Creates top most and bottom most borders.
        if (i == gameUpLimit || i == gameDownLimit - 1)
        {
            for (auto j = gameLeftLimit; j < gameRightLimit; ++j)
                std::cout << "*";
        }
        // Creates the borders on the left and right.
        else
        {
            ToXY(0, i);
            std::cout << "*";
            ToXY(gameRightLimit - 1, i);
            std::cout << "*";
        }
        std::cout << "\n";
    }

    ToXY(0, gameDownLimit + 1);
    std::cout << "Space to start. A to move left, D to move right.\nEsc to close game.\n";
}

void Load()
{
    gameRightLimit = Console::width - 1;
    gameDownLimit = Console::height - 5;
    PrintBoard();

    for (int i = -3; i < 3; ++i)
    {
        for (int j = -3; j < 3; ++j)
        {
            blocks.push_back({ i * 3, j, 1 });
        }
    }

    for (int i = 0; i < (int)blocks.size(); ++i)
    {
        int blockX = (gameRightLimit >> 1) + blocks[i].posX;
        int blockY = (gameDownLimit >> 1) - (gameDownLimit >> 2) - blocks[i].posY;
        blocks[i].posX = blockX;
        blocks[i].posY = blockY;
    }

    player.posX = (gameRightLimit >> 1) + player.posX;
    player.posY = gameDownLimit - 4;
    ball.posX = (gameRightLimit >> 1) + ball.posX;
    ball.posY = gameDownLimit - 4 + ball.posY;
}

bool Collide(int posX, int posY, Direction& ballDirection)
{
    // Walls check.
    switch (ballDirection)
    {
    case Breakout::Direction::Stop:
        break;
    case Breakout::Direction::UpLeft:
        if (posX - 1 == gameLeftLimit && posY - 1 == gameUpLimit)
        {
            ballDirection = Direction::DownRight;
            return true;
        }
        if (posX - 1 == gameLeftLimit)
        {
            ballDirection = Direction::UpRight;
            return true;
        }
        if (posY - 1 == gameUpLimit)
        {
            ballDirection = Direction::DownLeft;
            return true;
        }
        break;
    case Breakout::Direction::UpRight:
        if (posX + 2 == gameRightLimit && posY - 1 == gameUpLimit)
        {
            ballDirection = Direction::DownLeft;
            return true;
        }
        if (posX + 2 == gameRightLimit)
        {
            ballDirection = Direction::UpLeft;
            return true;
        }
        if (posY - 1 == gameUpLimit)
        {
            ballDirection = Direction::DownRight;
            return true;
        }
        break;
    case Breakout::Direction::DownLeft:
        if (posX - 1 == gameLeftLimit)
        {
            ballDirection = Direction::DownRight;
            return true;
        }
        if (posY + 2 == gameDownLimit)
        {
            ToXY(0, gameDownLimit + 2);
            std::cout << "Game over\n";
            isRunning = false;
            return true;
        }
        break;
    case Breakout::Direction::DownRight:
        if (posX + 2 == gameRightLimit)
        {
            ballDirection = Direction::DownLeft;
            return true;
        }
        if (posY + 2 == gameDownLimit)
        {
            ToXY(0, gameDownLimit + 2);
            std::cout << "Game over\n";
            isRunning = false;
            return true;
        }
        break;
    default:
        break;
    }

    // Blocks check.
    for (int i = 0; i < blocks.size(); ++i)
    {
        if (posY - 1 == blocks[i].posY)
        {
            if (posX == blocks[i].posX)
            {
                ToXY(blocks[i].posX - blocks[i].size, blocks[i].posY);
                std::cout << " ";
                ToXY(blocks[i].posX + blocks[i].size, blocks[i].posY);
                std::cout << " ";
                blocks.erase(blocks.begin() + i);
                if (ballDirection == Direction::UpLeft) ballDirection = Direction::DownLeft;
                else if (ballDirection == Direction::UpRight) ballDirection = Direction::DownRight;
                else if (ballDirection == Direction::DownLeft) ballDirection = Direction::UpLeft;
                else if (ballDirection == Direction::DownRight) ballDirection = Direction::UpRight;
                ++score;
                return true;
            }
            if (posX == blocks[i].posX - blocks[i].size)
            {
                ToXY(blocks[i].posX - blocks[i].size, blocks[i].posY);
                std::cout << " ";
                ToXY(blocks[i].posX + blocks[i].size, blocks[i].posY);
                std::cout << " ";
                blocks.erase(blocks.begin() + i);
                if (ballDirection == Direction::UpLeft) ballDirection = Direction::DownLeft;
                else if (ballDirection == Direction::UpRight) ballDirection = Direction::UpLeft;
                else if (ballDirection == Direction::DownLeft) ballDirection = Direction::UpLeft;
                else if (ballDirection == Direction::DownRight) ballDirection = Direction::DownLeft;
                ++score;
                return true;
            }
            if (posX == blocks[i].posX + blocks[i].size)
            {
                ToXY(blocks[i].posX - blocks[i].size, blocks[i].posY);
                std::cout << " ";
                ToXY(blocks[i].posX + blocks[i].size, blocks[i].posY);
                std::cout << " ";
                blocks.erase(blocks.begin() + i);
                if (ballDirection == Direction::UpLeft) ballDirection = Direction::UpRight;
                else if (ballDirection == Direction::UpRight) ballDirection = Direction::DownRight;
                else if (ballDirection == Direction::DownLeft) ballDirection = Direction::DownRight;
                else if (ballDirection == Direction::DownRight) ballDirection = Direction::UpRight;
                ++score;
                return true;
            }
        }
    }

    // Player check.
    if (posX >= player.posX - player.size && posX < player.posX + player.size && posY + 1 == player.posY)
    {
        if (ballDirection == Direction::DownLeft)
        {
            ballDirection = Direction::UpLeft;
            return true;
        }
        else if (ballDirection == Direction::DownRight)
        {
            ballDirection = Direction::UpRight;
            return true;
        }
        
    }

    return false;
}

void Update()
{
    ToXY(0, 0);

    if (blocks.size() == 0)
    {
        ToXY(0, gameDownLimit + 2);
        std::cout << "You win\n";
        isRunning = false;
        return;
    }

    if (score % 10 == 0)
    {
        Grow();
        ++score;
    }

    if (ballmove)
    {
        bool collided = Collide(ball.posX, ball.posY, ball.direction);

        if (collided == false)
        {
            ToXY(ball.posX, ball.posY);
            std::cout << " ";
            switch (ball.direction)
            {
            case Direction::DownLeft:
                --ball.posX;
                ++ball.posY;
                break;
            case Direction::DownRight:
                ++ball.posX;
                ++ball.posY;
                break;
            case Direction::UpLeft:
                --ball.posX;
                --ball.posY;
                break;
            case Direction::UpRight:
                ++ball.posX;
                --ball.posY;
                break;
            default:
                break;
            }
        }
    }
    ballmove = !ballmove;

    if (player.posX - player.size - 1 > 0)
    {
        ToXY(player.posX - player.size - 1, player.posY);
        std::cout << " ";
    }
    for (int i = -player.size; i <= player.size; ++i)
    {
        ToXY(player.posX + i, player.posY);
        std::cout << "=";
    }
    if (player.posX + player.size + 1 < gameRightLimit - 1)
    {
        ToXY(player.posX + player.size + 1, player.posY);
        std::cout << " ";
    }

    for (int i = 0; i < (int)blocks.size(); ++i)
    {
        ToXY(blocks[i].posX - blocks[i].size, blocks[i].posY);
        std::cout << "[";
        ToXY(blocks[i].posX + blocks[i].size, blocks[i].posY);
        std::cout << "]";
    }

    ToXY(ball.posX, ball.posY);
    std::cout << "O";

    ToXY(0, Console::height - 1);
}

}
