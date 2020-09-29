#include <iostream>
#include<Windows.h>
#include<conio.h>
#include<cassert>
#include<string>
#include<Windows.h>
#include<thread>
#include<fstream>
#include<sstream>
using namespace std;
//globals

struct coOrdinate
{
    int x;
    int y;
}snakePosition[100];

bool gameOver;
const int frame_width = 40;
const int frame_height = 30;
enum direction { LEFT = 0, UP = 1, RIGHT = 2, DOWN = 3 };
int xPos= frame_width / 2;
int yPos= frame_height / 2;
int xfood;
int yfood;
direction dir;
int score;
int snakeSize;
//faster than system("cls")
// hence avoid flikering

int getPreviousHighScore()
{
    string line;
    ifstream fin;
    fin.open("highscore.txt");
    while (fin)
        getline(fin, line);
    fin.close();
    /*stringstream sstr(line);
    int score;
    sstr >> score;
    return score;*/
    int Prevscore = stoi(line);
    return Prevscore;
}
void updateHighScore()
{
    string line;
    ofstream fout;
    fout.open("highscore.txt");
    fout << score;
    cout << "score = " << score;
    return;
}
void clearScr()
{
    HANDLE hOut;
    COORD Position;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

int randomizeX()
{
    int temp= (rand() % (frame_width - 1)) + 1;
    if (temp == frame_width - 1)
         temp -= 1;
    return temp;
}
int randomizeY()
{
    int temp= (rand() % (frame_height - 1)) + 1;
    if (temp == frame_height - 1)
        temp -= 1;
    return temp;
}

void soundEatFood()
{
    PlaySound(L"Sounds\\food.wav",NULL,SND_ASYNC| SND_NOSTOP);
    return;
}
void gameOverFunction()
{
    system("cls");
    for (int i = 0; i < 20; i++)
    {
        cout << "# ";
    }
    cout << "\n\n\n\n\tGAME OVER!!!" << endl;
    cout << "\n\n\n\n\n\tYOUR SCORE: " << score << "\n\n\n";
    for (int i = 0; i < 20; i++)
    {
        cout << "# ";
    }
    cout << "\n";
    for (int i = 0; i < snakeSize; i++)
    {
        cout << snakePosition[i].x << ", " << snakePosition[i].y << endl;
    }
    
}
bool askPlayAgain()
{
    cout << "\n\nPress <space> to play again\n\n";
    char ch = _getch();
    cout << "\n\nch= " << ch << endl;
    //_getch();
    switch(ch)
    {
    case ' ' : return true;
    default: return false;
    }
}

bool hitWall()
{
    //between walls
    if (xPos > 0 && xPos < frame_width-1 && yPos>0 && yPos < frame_height-1)
        return false;
    return true;

}
bool selfEat()
{
    for (int i = 1; i < snakeSize; i++)
    {
        if (snakePosition[i].x == xPos && snakePosition[i].y == yPos)
            return true;
    }
    return false;
}
void Setup()
{
    xPos = frame_width / 2;
    yPos = frame_height / 2;
    snakeSize = 0;
    score = 0;
    gameOver = false;
    xfood = randomizeX();
    yfood = randomizeY();
    dir = RIGHT;
    int x_init_snake_pos = frame_width / 2;
    int y_init_snake_pos = frame_height / 2;
    snakePosition[0].x = xPos;
    snakePosition[0].y = yPos;
    clearScr();
    
}
void Update()
{
    //reconstruct
    //system("cls");
    clearScr();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
    for (int i = 0; i < frame_height; i++)
    {
        for (int j = 0; j < frame_width; j++)
        {
            if (i == 0 || i == frame_height - 1)
            {
                if (j <= frame_width / 2)
                    cout << "X ";
            }
            else if (i == yfood && j == xfood)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
                cout << "0";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
            }
            else if (i == yPos && j == xPos)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                cout << "O";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
            }

            else if (j == 0 || j == frame_width - 1)
            {
                cout << "X ";
            }
            else
            {
                bool print = false;
                for (int k = 1; k < snakeSize; k++)
                {
                    if (snakePosition[k].x == j && snakePosition[k].y == i)
                    {
                        cout << "o";
                        print = true;
                    }
                }
                if(!print)
                    cout << " ";
            }
        }
        cout << endl;
    }
    
    /*cout << "\n\nSCORE: " << score << endl;;
    cout << "\n\nsnakesize= " << snakeSize << endl;
    cout << "\nsnake pos: " << endl;
    for (int i = 0; i < snakeSize; i++)
    {
        cout << snakePosition[i].x << ", " << snakePosition[i].y << endl;
    }*/

    if (_kbhit())
    {
        char ch = _getch();

        switch (ch)
        {
        case 'a':
            if(dir!=RIGHT)
                dir = LEFT;
            break;
        case 'd'://xPos++;
            if (dir != LEFT)
                dir = RIGHT;
            break;
        case 'w'://yPos--;
            if (dir != DOWN)
                dir = UP;
            break;
        case 's'://yPos++;
            if (dir != UP)
                dir = DOWN;
            break;
        }
    }
}

void logic()
{
    switch (dir)
    {
    case UP:yPos--;
        break;
    case DOWN:yPos++;
        break;
    case LEFT:xPos--;
        break;
    case RIGHT:xPos++;
        break;
    }

    for (int i = snakeSize - 1; i > 0; i--)
        snakePosition[i] = snakePosition[i - 1];
    snakePosition[0].x = xPos;
    snakePosition[0].y = yPos;
    
    //eat food
    if (xPos == xfood && yPos == yfood)
    {
        soundEatFood();
        score += 10;
        xfood = randomizeX();
        yfood = randomizeY();
        snakeSize++;
    }
    //gameover
    if (hitWall() || selfEat())
    {
        gameOver = true;
    }

    
}
int main()
{
    cout<<endl<<PlaySound(L"Sounds//sample.wav",NULL, SND_LOOP| SND_ASYNC)<<endl;
    bool cont = true;
    _getch();
    
    while (cont)    
    {
        system("CLS");
        
        Setup();

        Sleep(10);
        gameOver = false;
        while (!gameOver)
        {
            Update();
            logic();
            Sleep(5);
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        gameOverFunction();
        int PrevhighScore = getPreviousHighScore();
        if (score > PrevhighScore)
        {
            updateHighScore();
            cout << "Congratulations!!! new high score" << endl;
        }
        cont=askPlayAgain();
    }
}

