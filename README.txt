instruction: 

After you download the zip file: 

thanlim@Thans-MBP AI_Project_Final % make

g++ -std=c++11 -g -c snake_qlearning_test.cpp
g++ snake_qlearning_test.o -g -std=c++11 -o snake_qlearning_test
g++ -std=c++11 -g -c snake_astar_test.cpp
g++ snake_astar_test.o -g -std=c++11 -o snake_astar_test
g++ -std=c++11 -g -c snake_astar.cpp
g++ snake_astar.o -g -std=c++11 -o snake_astar
g++ -std=c++11 -g -c snake_qlearning.cpp
g++ snake_qlearning.o -g -std=c++11 -o snake_qlearning

thanlim@Thans-MBP AI_Project_Final % ls 

AI_FINAL_SNAKE_AStar		snake_astar_test.cpp
AI_FINAL_SNAKE_QLearning	snake_astar_test.o
Makefile			snake_qlearning
README.txt			snake_qlearning.cpp
snake_astar			snake_qlearning.o
snake_astar.cpp			snake_qlearning_test
snake_astar.o			snake_qlearning_test.cpp
snake_astar_test		snake_qlearning_test.o
thanlim@Thans-MBP AI_Project_Final % 

You can pipe the results to .txt files so that you can compare the results of the two algorithms. 

thanlim@Thans-MBP AI_Project_Final % ./snake_astar_test >> AStar_Result.txt
thanlim@Thans-MBP AI_Project_Final % ./snake_qlearning_test >> Qlearning_Result.txt
Note: snake_qlearning_test takes a little longer time to finsih running because the episode was set to 100000. 

If you want to see how the snake plays the game, please run these executable files. For A* algorithm: 
thanlim@Thans-MBP AI_Project_Final % ./snake_astar

For Q learning algorithm: 
thanlim@Thans-MBP AI_Project_Final % ./snake_qlearning


