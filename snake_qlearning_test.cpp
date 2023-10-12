#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <list>
#include <tuple>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>

using namespace std;

class Snake
{
private:
    int snake_x, snake_y, snake_length, prevInput, board_length, board_height, action, psnake_x, psnake_y;
    int apple_x, apple_y;
    int points;
    int board[12][12];
    //int h_board[12][12];
    float q_table[12][12][4], mx, epsilon, discount_factor;
    char path_board[12][12];
    bool up, left, down, right, exit, eaten;
    vector<char> path;
    ofstream myfile;


public:
    Snake()
    {
        up = false;
        left = false;
        down = false;
        right = false;
        exit = false;
        eaten = false;
        prevInput = 2;

        board_height = 12;
        board_length = 12;

        snake_x = 1;
        snake_y = 1;

        apple_x = 5;
        apple_y = 5;

        points = 0;

        snake_length = 3;
        epsilon = 0.9;
        discount_factor = 1.0;

        path = {};
        resetBoard();
        for (int i = 0; i < board_height; i++)
        {
            for (int j = 0; j < board_length; j++)
            {
                path_board[i][j] = 'x';
            }
        }
        for (int i = 0; i < board_height; i++)
        {
            for (int j = 0; j < board_length; j++)
            {
                for (int k = 0; k < 4; k++)
                {
                    q_table[i][j][k] = 0;
                }
            }
        }
        
    }
    ~Snake()
    {

    }
    char getActionForIndex(size_t index);
    void updateQTable();
    void displayBoard();
    void play();
    void getInput();
    void updatePlayer();
    void updateBoard();
    void calc_h_board();
    void displayHBoard();
    void displayPathBoard();
    void calcAStar();
    bool checkChild(tuple<int, int> child, tuple<int, int> lowest_tup, list<tuple<int, int> > &open_list, list<tuple<int, int> > &close_list, char dir);
    void getPath();
    void resetPath();
    void displayPath();
    void resetPathBoard();
	bool existsInList(tuple<int, int> x, list<tuple<int, int> > myList);
    void resetBoard();
    void reset();

};

tuple<int, int> getTuple(list<tuple<int, int> >, int);

int main(){

    // instructions
  
    cout << "To play use 'wasd' to move." << endl;
    cout << "To exit use 'x'." << endl;
    cout << "Press enter to start." << endl;
    
    Snake my_snake;
    my_snake.play();
    
    return 0;
}
void Snake::resetBoard(){
    for (int i = 0; i < board_height; i++)
    {
        for (int j = 0; j < board_length; j++)
        {
            if(i == 0 || i == 11)
            {
                board[i][j] = -1;
            }
            else if(j == 0 || j == 11)
            {
                board[i][j] = -1;
            }
            else
            {
                board[i][j] = 0;
            }
        }
    }
    board[snake_x][snake_y] = snake_length;
    board[apple_x][apple_y] = -10;
}
void Snake::displayBoard()
{
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            if (board[i][j] == -1){
                cout << 'X';
            } else if(board[i][j] == snake_length){
                cout << 'e';
            } else if(board[i][j] > 0){
                cout << 'o';
            } else if (board[i][j] == 0){
                cout << '_';
            } else if(board[i][j] == -10){
                cout << 'a';
            } else cout << '_';
        }
        cout << endl;
    }
}

void Snake::displayPathBoard()
{
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            cout << path_board[i][j];
            cout << "\t";
        }
        cout << endl;
    }
}
void Snake::updateQTable(void){
    int reward = 0;
    if(eaten) reward = 100;
    if(exit) reward = -500;
    if(board[snake_x][snake_y] > 0) reward = -300; //We ate ourselves

    //Add q equation
    q_table[psnake_x][psnake_y][action] += epsilon * ((reward + (discount_factor * mx)) - q_table[psnake_x][psnake_y][action]);//Need to add Q equation with episolon and reward values

}

void Snake::play()
{
    size_t max_episodes = 100000;
    for(size_t i = 0 ; i < max_episodes; ++i)
    {
        for (size_t start_x = 1; start_x < 11; ++start_x)
        {
            for (size_t start_y = 1; start_y <11; ++start_y)
            {
                if( i >= max_episodes -1) cout <<"Episode "<< i <<endl;
                exit = false;
               //this is where we will reset snake position
                //reset();
                snake_x = start_x;
                snake_y = start_y; 
                resetBoard();
                int step = 0;
                points = 0;
                if( i >= max_episodes -1) displayBoard();
                
                while(exit != true)
                {
                   
                    getInput();
                    updatePlayer();
                    updateQTable();
                    if (exit) break;
                    updateBoard();
                    //cout << "Points scored: " << points << endl;
                    //displayBoard();

                    // sleep and clear the input after each display
                    // avoids multiple moves in one turn
                    //std::this_thread::sleep_for(std::chrono::milliseconds(250));
                    fseek(stdin,0,SEEK_END);
                    step++;
                    //exit = true;
                    if (step > 100) break;
                }
                if( i >= max_episodes -1){
                    cout << "These are steps: " <<step <<endl;
                    cout << "Points scored: " << points << endl;    
                    
                }
                
            }
        }
    }

}

char Snake::getActionForIndex(size_t index){
    char res = 'x';
    switch (index)
    {
    case 0:
        res = 'w';
        break;
    case 1:
        res = 'a';
        break;
    case 2:
        res = 's';
        break;
    case 3:
        res = 'd';
        break;
    default:
        break;
    }
    return res;
}
void Snake::getInput()
{
    up = false;
    left = false;
    down = false;
    right = false;
    exit = false;

//    // Set terminal to raw mode
//    system("stty raw");
//    // Wait for single character
//    input = getchar();
//    // Reset terminal to normal "cooked" mode
//    system("stty cooked");
    
    mx = 0;
    size_t mxi = 0;
    for(size_t i =0; i < 4; ++i){
        if(q_table[snake_x][snake_y][i] > mx){
            mx = q_table[snake_x][snake_y][i];
            mxi = i;
        }
    }
    action = mxi;
    char input = getActionForIndex(mxi);

    if (input == 'w')
    {
        up = true;
    }
    else if (input == 'a')
    {
        left = true;
    }
    else if (input == 's')
    {
        down = true;
    }
    else if (input == 'd')
    {
        right = true;
    }
    else if (input == 'x')
    {
        exit = true;
    }
    else {
        switch(prevInput){
            case 1:
                up = true;
                break;
            case 2:
                right = true;
                break;
            case 3:
                down = true;
                break;
            case 4:
                left = true;
                break;
        }
    }
}

void Snake::updatePlayer()
{   
    psnake_x = snake_x;
    psnake_y = snake_y;
    if (up == true)
    {
        // move the snake
        snake_x -= 1;
    }
    else if (left == true)
    {
        snake_y -= 1;
    }
    else if (down == true)
    {
        snake_x += 1;
    }
    else if (right == true)
    {
        snake_y += 1;

    }

    // record if we are eating an apple
    if(snake_x == apple_x && snake_y == apple_y){
        eaten = true;
        points++;
    }
    if(snake_x > board_length || snake_x < 0 || snake_y > board_height || snake_y < 0){
        exit = true;
        //cout << snake_x <<endl;
        //cout << snake_y <<endl;
    }
}

void Snake::updateBoard()
{
    for (int i = 1; i < board_height-1; i++)
    {
        for (int j = 1; j < board_length-1; j++)
        {
            if(board[i][j] > 0)
            {
                board[i][j]--;
            }
        }
    }
    if(up){
        prevInput = 1;
    } else if(right){
        prevInput = 2;
    } else if(down){
        prevInput = 3;
    } else if(left){
        prevInput = 4;
    }

    if(eaten)
    {
        snake_length++;
        eaten = false;
		exit = true;
        // increment all snake body
        for (int i = 1; i < board_height-1; i++)
        {
            for (int j = 1; j < board_length-1; j++)
            {
                if(board[i][j] > 0)
                {
                    board[i][j]++;
                }
            }
        }

        //generate a new apple
        int randX;
        int randY;
        bool collision = true;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(1, 10);

        // make sure the apple appears on an empty spot
        do{
            randX = distr(gen);
            randY = distr(gen);
            if(board[randX][randY] == 0){
//                apple_x = randX;
//                apple_y = randY;
                board[apple_x][apple_y] = -10;
                collision = false;
            }
        }while(collision);
    }
    board[snake_x][snake_y] = snake_length;
}

bool Snake::existsInList(tuple<int, int> x, list<tuple<int, int> > myList){

	for(int i = 0; i < myList.size(); i++){
		if(get<0>(x) == get<0>(getTuple(myList,i)) && get<1>(x) == get<1>(getTuple(myList, i))){
			return true;
		}
	} return false;
}

tuple<int, int> getTuple(list<tuple<int, int> > _list, int _i)
{
    list<tuple<int, int> >::iterator it = _list.begin();
    for(int i=0; i<_i; i++){
        ++it;
    }
    return *it;
}

