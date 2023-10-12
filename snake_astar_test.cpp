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

using namespace std;

class Snake
{
private:
    int snake_x, snake_y, snake_length, prevInput, board_length, board_height;
    int apple_x, apple_y;
    int points;
    int board[12][12];
    int h_board[12][12];
    char path_board[12][12];
    bool up, left, down, right, exit, eaten;
    vector<char> path;


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

        path = {};
        resetBoard();
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

        for (int i = 0; i < board_height; i++)
        {
            for (int j = 0; j < board_length; j++)
            {
                path_board[i][j] = 'x';
            }
        }

        board[snake_x][snake_y] = snake_length;
        board[apple_x][apple_y] = -10;
    }
    ~Snake()
    {

    }

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

void Snake::play()
{
    //while(true)
    //{
    for (size_t start_x = 1; start_x < 11; ++start_x)
    {
        for (size_t start_y = 1; start_y <11; ++start_y)
        {
            snake_x = start_x;
            snake_y = start_y; 
            resetBoard();
            exit = false;

            resetPath();
            resetPathBoard();
            calc_h_board();
            // displayHBoard();
            calcAStar();
            // displayPathBoard();
            getPath();

            // for(int i = 0; i < path.size(); i++){
            // 	cout << path[i];
            // } 
            // cout << endl;
            points = 0; 
            int step = 0;
            displayBoard();
            while(exit != true)
            {
                getInput();

                updatePlayer();
                updateBoard();
                //cout << "Points scored: " << points << endl;
                //displayBoard();

                // sleep and clear the input after each display
                // avoids multiple moves in one turn
                //std::this_thread::sleep_for(std::chrono::milliseconds(250));
                fseek(stdin,0,SEEK_END);
                step++;
                //exit = true;
            }
            cout << "These are steps: " <<step <<endl;
            cout << "Points scored: " << points << endl;
        }
    }
    //}
}

void Snake::getInput()
{
    up = false;
    left = false;
    down = false;
    right = false;
    exit = false;

    
    if(path.size() == 0)
    {
        exit = true;
        return;
    }
    
    char input = path[0];

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
    
    path.erase(path.begin());
}

void Snake::updatePlayer()
{
    if (up == true)
    {
        // move the snake
        snake_x -= 1;
        // make sure we're not going out of bounds
        if(snake_x == 0)
            snake_x += 1;
    }
    else if (left == true)
    {
        snake_y -= 1;
        if(snake_y == 0)
            snake_y += 1;
    }
    else if (down == true)
    {
        snake_x += 1;
        if(snake_x == 11)
            snake_x -= 1;
    }
    else if (right == true)
    {
        snake_y += 1;
        if(snake_y == 11)
            snake_y -= 1;

    }

    // record if we are eating an apple
    if(snake_x == apple_x && snake_y == apple_y){
        eaten = true;
        points++;
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
                //apple_x = randX;
                //apple_y = randY;
                board[apple_x][apple_y] = -10;
                collision = false;
            }
        }while(collision);
    }
    board[snake_x][snake_y] = snake_length;
}

void Snake::calc_h_board()
{
    for (int i = 0; i < board_height; i++)
    {
        for (int j = 0; j < board_length; j++)
        {
            if(i == 0 || i == 11 || j == 0 || j == 11)
            {
                h_board[i][j] = 999;
            }
            else
            {
                int x_dif = 0;
                int y_dif = 0;

                int man_dist = 0;

                x_dif = apple_x - i;
				y_dif = apple_y - j;

				if (x_dif < 0)
					x_dif = -x_dif;

				if (y_dif < 0)
					y_dif = -y_dif;

				h_board[i][j] = x_dif + y_dif;
            }
        }
    }
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
void Snake::displayHBoard()
{
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            cout << h_board[i][j];
            cout << "\t";
        }
        cout << endl;
    }
}

void Snake::calcAStar()
{
    list<tuple<int, int> > open_list = {};
    list<tuple<int, int> > close_list = {};

    tuple<int, int> temp = make_tuple (snake_x, snake_y);

    open_list.push_back(temp);

    while (open_list.size() != 0)
    {
		int lowest_val = h_board[get<0>(getTuple(open_list, 0))][get<1>(getTuple(open_list, 0))];
    	tuple<int, int> lowest_tup = getTuple(open_list, 0);

		// get the lowest value state on the list
        for (int i = 0; i < open_list.size(); i++)
        {
            if(lowest_val >= h_board[get<0>(getTuple(open_list, i))][get<1>(getTuple(open_list, i))])
            {
                lowest_val = h_board[get<0>(getTuple(open_list, i))][get<1>(getTuple(open_list, i))];
                lowest_tup = getTuple(open_list, i);
            }
        }
		
		open_list.remove_if([&lowest_tup](tuple<int, int> elem){ 
			return get<0>(elem) == get<0>(lowest_tup) && get<1>(elem) == get<1>(lowest_tup); });

		
		tuple<int, int> down_child = make_tuple (get<0>(lowest_tup)+1, get<1>(lowest_tup));
        checkChild(down_child, lowest_tup, open_list, close_list, 's');
        tuple<int, int> up_child = make_tuple (get<0>(lowest_tup)-1, get<1>(lowest_tup));
        checkChild(up_child, lowest_tup, open_list, close_list, 'w');
        tuple<int, int> right_child = make_tuple (get<0>(lowest_tup), get<1>(lowest_tup)+1);
        checkChild(right_child, lowest_tup, open_list, close_list, 'd');
        tuple<int, int> left_child = make_tuple (get<0>(lowest_tup), get<1>(lowest_tup)-1);
        checkChild(left_child, lowest_tup, open_list, close_list, 'a');
		// add current state to closed list
        close_list.push_back(lowest_tup);
		if(board[get<0>(lowest_tup)][get<1>(lowest_tup)] == -10){
			break;
		}
			
    }

}

bool Snake::checkChild(tuple<int, int> child, tuple<int, int> lowest_tup, list<tuple<int, int> > &open_list, list<tuple<int, int> > &close_list, char dir)
{
    
	bool found_closed = existsInList(child, close_list);
	bool found_open = existsInList(child, open_list);

    int test_child = board[get<0>(child)][get<1>(child)];

    if(test_child != 0 && test_child != -10)
    {
        return false;
    }
    
    if(!found_open && !found_closed && (test_child == 0 || test_child == -10))
    {
        open_list.push_back(child);
        path_board[get<0>(child)][get<1>(child)] = dir;
        
		if(test_child == -10)
		{
			return true;
		}
        return false;
    }
    
    return false;

}

bool Snake::existsInList(tuple<int, int> x, list<tuple<int, int> > myList){

	for(int i = 0; i < myList.size(); i++){
		if(get<0>(x) == get<0>(getTuple(myList,i)) && get<1>(x) == get<1>(getTuple(myList, i))){
			return true;
		}
	} return false;
}


void Snake::getPath()
{
    tuple<int, int> goal = make_tuple (apple_x, apple_y);
    tuple<int, int> start = make_tuple (snake_x, snake_y);

    while(true)
    {
        char path_value = path_board[get<0>(goal)][get<1>(goal)];
        
        if(path_value == 'x')
        {
            reverse(path.begin(), path.end());
            return;
        }

        path.push_back(path_value);

        if(path_value == 'w')
        {
            goal = make_tuple (get<0>(goal)+1, get<1>(goal));
        }
        else if(path_value == 's')
        {
            goal = make_tuple (get<0>(goal)-1, get<1>(goal));
        }
        else if(path_value == 'a')
        {
            goal = make_tuple (get<0>(goal), get<1>(goal)+1);
        }
        else if(path_value == 'd')
        {
            goal = make_tuple (get<0>(goal), get<1>(goal)-1);
        }
    }
}

void Snake::displayPath()
{
    for(int i = 0; i < path.size(); i++)
    {
        cout << path[i];
    }
}

void Snake::resetPath()
{
    path.clear();
}

void Snake::resetPathBoard()
{
    for(int i = 0; i < 12; i++)
    {
        for(int j = 0; j < 12; j++)
        {
            path_board[i][j] = 'x';
        }
    }
}

tuple<int, int> getTuple(list<tuple<int, int> > _list, int _i)
{
    list<tuple<int, int> >::iterator it = _list.begin();
    for(int i=0; i<_i; i++){
        ++it;
    }
    return *it;
}

