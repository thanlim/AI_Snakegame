all: snake_qlearning_test snake_astar_test snake_astar snake_qlearning
snake_qlearning: snake_qlearning.o
	g++ snake_qlearning.o -g -std=c++11 -o snake_qlearning
    
snake_qlearning.o: snake_qlearning.cpp
	g++ -std=c++11 -g -c snake_qlearning.cpp
    
snake_astar: snake_astar.o
	g++ snake_astar.o -g -std=c++11 -o snake_astar
snake_astar.o : snake_astar.cpp
	g++ -std=c++11 -g -c snake_astar.cpp

snake_astar_test: snake_astar_test.o
	g++ snake_astar_test.o -g -std=c++11 -o snake_astar_test

snake_astar_test.o: snake_astar_test.cpp
	g++ -std=c++11 -g -c snake_astar_test.cpp

snake_qlearning_test: snake_qlearning_test.o
	g++ snake_qlearning_test.o -g -std=c++11 -o snake_qlearning_test

snake_qlearning_test.o: snake_qlearning_test.cpp
	g++ -std=c++11 -g -c snake_qlearning_test.cpp

clean:
	rm *.o snake_qlearning snake_astar snake_qlearning_test snake_astar_test 