// This is a template helping you to do program assignment2.
// We implemented the file I/O part.
// You may focus on the method, _getNextMove(),
// which is a method to decide where to place your stone based on the current
// game state, including (1) valid position, (2) your position, (3) your
// opponent position, (4) board and (5) the winner of first game.
// !! Remember to change the team number in main() !!

//#include "stdafx.h"
#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <time.h> 

using namespace std;

double Max_rec(struct state *, double , double );
double Min_rec(struct state *, double , double );
void print_board (vector<int >, int );
double calculate_evaluation (vector<int >, string);

class Agent {
	/**
	* Game agent.
	* The procedure:
	* 1. process_state_info: check game state
	* 2. is_my_turn: if it is my turn, decide a position to put stone
	* 3. step: call get_next_move and write the result to move file
	*/

public:
	Agent(int team_number)
		: team_number(team_number),
		cur_move(-1),
		next_first_move(-1),
		first_winner(-1),
		game_stop(false)
	{
		this->state_file = "state_" + to_string(team_number) + ".txt";
		this->move_file = "move_" + to_string(team_number) + ".txt";
	}

	/**
	* Read state file and get valid position.
	* If not my turn to make move, return an empty list.
	*/
	void processStateInfo();

	/**
	*  Get the next move and write it into move file.
	*/
	void step();

	/**
	* If the valid position is not empty, it is my turn.
	*/
	bool isMyTurn();

	bool isGameStop();

	int isFirstWinner();

private:
	/**
	* Get a position from valid_pos randomly.
	* You should implement your algorithm here.
	*
	* These utilities may be helpful:
	* getValidPos()
	* getMyPos()
	* getOpponentPos()
	* getBoard()
	*
	* Check them below for more detail
	*/
	int _getNextMove();

	/**
	* Write my move into move file.
	*/
	void _writeMove(int pos);

	/**
	* Get the valid position where you can put your stone.
	* A list of int. e.g. [0, 1, 3, 5, 6, 9, 12,...]
	*/
	vector<int> getValidPos();

	/**
	* Get the position where you have put.
	* A list of int. e.g. [2, 4, 7, 8, ...]
	*/
	vector<int> getMyPos();

	/**
	* Get the position where your opponent have put.
	* A list of int. e.g. [10, 11, 13, ...]
	*/
	vector<int> getOpponentPos();

	/**
	* Get current board. 0: valid pos, 1: your pos, 2: opponent pos
	* A list of int. e.g. [0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 2, 2, 0, 2...]
	*/
	vector<int> getBoard();

	/**
	* Are you the winner of first game?
	* Return -1: the first game is still continuing.
	*         1: you are the winner of first game.
	*         0: you are not.
	*/

	int team_number;
	string state_file, move_file;
	int cur_move, next_first_move;
	int first_winner;

	bool game_stop;

	vector<int> valid_pos, my_pos, opponent_pos, board;
};

int main() {
	//srand(time(NULL));

	// change the number to your team number.
	Agent* agent = new Agent(4);

	while (true) {
		agent->processStateInfo();
		if (agent->isGameStop()) break;
		if (agent->isMyTurn()) agent->step();
	}

	free(agent);
	return 0;
}

bool isEmpty(std::ifstream& pFile) {
	return pFile.peek() == std::ifstream::traits_type::eof();
}

void Agent::processStateInfo() {
	ifstream infile(state_file);
	if (!infile.is_open()) return;
	if (isEmpty(infile)) return;

	int move;
	infile >> move;

	board.clear();
	valid_pos.clear();
	my_pos.clear();
	opponent_pos.clear();

	int x;
	for (int i = 0; i < 217; ++i) {
		infile >> x;
		board.push_back(x);
	}

	infile >> first_winner;
	infile.close();

	if (move == -100) {
		game_stop = true;
		return;
	}

	// The only condition of move read from state file being less than our
	// record move (cur_move) is that the second game starts.
	// So, if move is less than cur_move and is not next_first_move,
	// just skip it
	if (move > cur_move ||
		(move == next_first_move && cur_move != next_first_move)) {
		// If we are making the first move of first game,
		// record the first move of the second game
		if (cur_move == -1) next_first_move = move == 1 ? 2 : 1;

		// Record current move
		cur_move = move;
		for (int i = 0; i < 217; ++i) {
			if (board[i] == 0)
				valid_pos.push_back(i);
			else if (board[i] == 1)
				my_pos.push_back(i);
			else if (board[i] == 2)
				opponent_pos.push_back(i);
		}
	}
}

void Agent::step() {
	int pos = _getNextMove();
	print_board(board, pos);
	cout<<"Value max "<< calculate_evaluation(board, "max")<<endl;
	cout<<"Value min "<< calculate_evaluation(board, "min")<<endl;
	_writeMove(pos);
	cout<<"Step:  "<<cur_move<<endl;
}

int fit_progression (int start, int progression_base, int times, int limit=8, int mode=1 ){

	int index;

	if(progression_base>0)index = progression_base-1;
	else index = progression_base+1;

	int sum = start;
	for(int i=0; i<times; ++i){
		if(i<limit){
			if(index>0)index++;
			else index--;
		}

		else if(i>limit){
			if(index>0)index--;
			else index++;
		}

		else if(mode==2){
			if(index>0)index--;
			else index++;
		}

		sum += index;
	}
	return sum;
}

int check_condition(vector<int > line, int condition[], int size){
	int condition_number = 0;
	
	for(int i=0; i<line.size(); ++i){
		if(i+size<line.size()){
			
			int flag = 1;
			for(int j=0; j<size; ++j){
				if(condition[j]  !=  line[i+j])flag=0;
			}
			if(flag)condition_number++;
			
			
		}
	}
	return condition_number;
}
int* set_array(int arr[], int a=-1, int b=-1, int c=-1, int d=-1 , int e=-1, int f=-1 )
{
	arr[0] = a;
	arr[1] = b;
	arr[2] = c;
	arr[3] = d;
	arr[4] = e;
	arr[5] = f;
	
	return arr;
}

inline double calculate_value(vector<int > XD,string node_type){
	double sum=0;
	// calculate evaluation for a vector is just a line
	// cout<<node_type;
	int arr[6];
	// cout<<node_type;
	if(node_type == "min"){
		int size=5;
		/*against jump 4*/
		sum += check_condition(XD, set_array(arr,2, 2, 2, 1, 2), size) * 3000;
		sum += check_condition(XD, set_array(arr,2, 2, 1, 2, 2), size) * 3000;
		sum += check_condition(XD, set_array(arr,2, 1, 2, 2, 2), size) * 3000;
		
		sum -= check_condition(XD, set_array(arr,1, 1, 1, 2, 1), size) * 4000;
		sum -= check_condition(XD, set_array(arr,1, 1, 2, 1, 1), size) * 4000;
		sum -= check_condition(XD, set_array(arr,1, 2, 1, 1, 1), size) * 4000;
		
		/*活三*/
		sum += check_condition(XD, set_array(arr,0, 1, 1, 1, 0), size) * 800;
		sum -= check_condition(XD, set_array(arr,0, 2, 2, 2, 0), size) * 1800;
		
		/*against free 3*/
		sum += check_condition(XD, set_array(arr,0, 2, 2, 2, 1), size) * 30;
		sum += check_condition(XD, set_array(arr,1, 2, 2, 2, 0), size) * 30;
		
		sum -= check_condition(XD, set_array(arr,2, 1, 1, 1, 0), size) * 50;
		sum -= check_condition(XD, set_array(arr,0, 1, 1, 1, 2), size) * 50;
		
		/*jump 三*/
		size = 6;
		sum += check_condition(XD, set_array(arr,0, 1, 1, 0, 1, 0), size) * 800;
		sum += check_condition(XD, set_array(arr,0, 1, 0, 1, 1, 0), size) * 800;
		sum -= check_condition(XD, set_array(arr,0, 2, 2, 0, 2, 0), size) * 1200;
		sum -= check_condition(XD, set_array(arr,0, 2, 0, 2, 2, 0), size) * 1200;

		/*五*/
		size = 5;
		sum += check_condition(XD, set_array(arr,1, 1, 1, 1, 1), size) * 9000;
		sum -= check_condition(XD, set_array(arr,2, 2, 2, 2, 2), size) * 10000;
		
		/*活4*/
		size = 6;
		sum += check_condition(XD, set_array(arr,0, 1, 1, 1, 1, 0), size) * 3000;
		sum -= check_condition(XD, set_array(arr,0, 2, 2, 2, 2, 0), size) * 5000;
		
		/*直四*/
		sum += check_condition(XD, set_array(arr,0, 1, 1, 1, 1, 2), size) * 600;
		sum += check_condition(XD, set_array(arr,2, 1, 1, 1, 1, 0 ), size) * 600;
		
		sum -= check_condition(XD, set_array(arr,0, 2, 2, 2, 2, 1), size) * 850;
		sum -= check_condition(XD, set_array(arr,1, 2, 2, 2, 2, 0), size) * 850;
		
		
		size = 5;
		sum += check_condition(XD, set_array(arr,0, 1, 1, 1, 1), size) * 600;
		sum += check_condition(XD, set_array(arr,1, 1, 1, 1, 0 ), size) * 600;
		
		sum -= check_condition(XD, set_array(arr,0, 2, 2, 2, 2), size) * 850;
		sum -= check_condition(XD, set_array(arr,2, 2, 2, 2, 0), size) * 850;
		
		/*against 4*/
		
		size = 6;
		sum += check_condition(XD, set_array(arr,1, 2, 2, 2, 2, 1), size) * 5000;
		
		sum -= check_condition(XD, set_array(arr,2, 1, 1, 1, 1, 2), size) * 7000;
		
		/*跳四*/
		size = 5;
		sum += check_condition(XD, set_array(arr,1, 1, 1, 0, 1), size) * 900;
		sum += check_condition(XD, set_array(arr,1, 1, 0, 1, 1), size) * 900;
		sum += check_condition(XD, set_array(arr,1, 0, 1, 1, 1), size) * 900;
		sum -= check_condition(XD, set_array(arr,2, 2, 2, 0, 2), size) * 2000;
		sum -= check_condition(XD, set_array(arr,2, 2, 0, 2, 2), size) * 2000;
		sum -= check_condition(XD, set_array(arr,2, 0, 2, 2, 2), size) * 2000;
	
		/*against jump 3*/
		size = 6;
		sum += check_condition(XD, set_array(arr,0, 2, 2, 1, 2, 0), size) * 30;
		sum += check_condition(XD, set_array(arr,0, 2, 1, 2, 2, 0), size) * 30;
		
		sum -= check_condition(XD, set_array(arr,0, 1, 1, 2, 1, 0), size) * 50;
		sum -= check_condition(XD, set_array(arr,0, 1, 2, 1, 1, 0), size) * 50;
		
		/*死三*/
		size = 5;
		sum += check_condition(XD, set_array(arr,1, 0, 1, 0, 1), size) * 30;
		sum += check_condition(XD, set_array(arr,1, 1, 0, 0, 1), size) * 30;
		sum += check_condition(XD, set_array(arr,1, 0, 0, 1, 1), size) * 30;
		sum += check_condition(XD, set_array(arr,2, 1, 1, 1, 0), size) * 50;
		sum += check_condition(XD, set_array(arr,0, 1, 1, 1, 2), size) * 50;
		
		sum += check_condition(XD, set_array(arr,2, 1, 1, 0, 1, 0), 6) * 40;
		sum += check_condition(XD, set_array(arr,2, 1, 0, 1, 1, 0), 6) * 40;
		sum += check_condition(XD, set_array(arr,0, 1, 1, 0, 1, 2), 6) * 40;
		sum += check_condition(XD, set_array(arr,0, 1, 0, 1, 1, 2), 6) * 40;
		
		sum -= check_condition(XD, set_array(arr,2, 0, 2, 0, 2), size) * 20;
		sum -= check_condition(XD, set_array(arr,2, 2, 0, 0, 2), size) * 20;
		sum -= check_condition(XD, set_array(arr,2, 0, 0, 2, 2), size) * 20;
		sum -= check_condition(XD, set_array(arr,1, 2, 2, 2, 0), size) * 30;
		sum -= check_condition(XD, set_array(arr,0, 2, 2, 2, 1), size) * 30;
		
		sum -= check_condition(XD, set_array(arr,1, 2, 2, 0, 2, 0), 6) * 25;
		sum -= check_condition(XD, set_array(arr,1, 2, 0, 2, 2, 0), 6) * 25;
		sum -= check_condition(XD, set_array(arr,0, 2, 0, 2, 2, 1), 6) * 25;
		sum -= check_condition(XD, set_array(arr,0, 2, 2, 0, 2, 1), 6) * 25;

		
		/*活2*/
		sum += check_condition(XD, set_array(arr,1, 1, 0, 0, 0), size) * 10;
		sum += check_condition(XD, set_array(arr,0, 1, 1, 0, 0), size) * 15;
		sum += check_condition(XD, set_array(arr,0, 0, 1, 1, 0), size) * 15;
		sum += check_condition(XD, set_array(arr,0, 0, 0, 1, 1), size) * 10;
		sum += check_condition(XD, set_array(arr,1, 0, 1, 0, 0), size) * 10;
		sum += check_condition(XD, set_array(arr,0, 1, 0, 1, 0), size) * 10;
		sum += check_condition(XD, set_array(arr,0, 0, 1, 0, 1), size) * 10;
		sum += check_condition(XD, set_array(arr,1, 0, 0, 1, 0), size) * 5;
		sum += check_condition(XD, set_array(arr,0, 1, 0, 0, 1), size) * 5;
		
		sum -= check_condition(XD, set_array(arr,2, 2, 0, 0, 0), size) * 10;
		sum -= check_condition(XD, set_array(arr,0, 2, 2, 0, 0), size) * 15;
		sum -= check_condition(XD, set_array(arr,0, 0, 2, 2, 0), size) * 15;
		sum -= check_condition(XD, set_array(arr,0, 0, 0, 2, 2), size) * 10;
		sum -= check_condition(XD, set_array(arr,2, 0, 2, 0, 0), size) * 10;
		sum -= check_condition(XD, set_array(arr,0, 2, 0, 2, 0), size) * 10;
		sum -= check_condition(XD, set_array(arr,0, 0, 2, 0, 2), size) * 10;
		sum -= check_condition(XD, set_array(arr,2, 0, 0, 2, 0), size) * 5;
		sum -= check_condition(XD, set_array(arr,0, 2, 0, 0, 2), size) * 5;
		
		}

	else {
		int size=5;
		/*against jump 4*/
		
		sum -= check_condition(XD, set_array(arr,2, 2, 2, 1, 2), size) * 3000;
		sum -= check_condition(XD, set_array(arr,2, 2, 1, 2, 2), size) * 3000;
		sum -= check_condition(XD, set_array(arr,2, 1, 2, 2, 2), size) * 3000;
		
		sum += check_condition(XD, set_array(arr,1, 1, 1, 2, 1), size) * 4000;
		sum += check_condition(XD, set_array(arr,1, 1, 2, 1, 1), size) * 4000;
		sum += check_condition(XD, set_array(arr,1, 2, 1, 1, 1), size) * 4000;
		
		/*活三*/
		sum += check_condition(XD, set_array(arr,0, 2, 2, 2, 0), size) * 800;
		sum -= check_condition(XD, set_array(arr,0, 1, 1, 1, 0), size) * 1800;
		
		/*against free 3*/
		sum += check_condition(XD, set_array(arr,2, 1, 1, 1, 0), size) * 30;
		sum += check_condition(XD, set_array(arr,0, 1, 1, 1, 2), size) * 30;
		
		sum -= check_condition(XD, set_array(arr,0, 2, 2, 2, 1), size) * 50;
		sum -= check_condition(XD, set_array(arr,1, 2, 2, 2, 0), size) * 50;
		
		/*jump 三*/
		size = 6;
		sum += check_condition(XD, set_array(arr,0, 2, 2, 0, 2, 0), size) * 800;
		sum += check_condition(XD, set_array(arr,0, 2, 0, 2, 2, 0), size) * 800;
		
		sum -= check_condition(XD, set_array(arr,0, 1, 1, 0, 1, 0), size) * 1200;
		sum -= check_condition(XD, set_array(arr,0, 1, 0, 1, 1, 0), size) * 1200;
		

		/*五*/
		size = 5;
		sum -= check_condition(XD, set_array(arr,1, 1, 1, 1, 1), size) * 10000;
		sum += check_condition(XD, set_array(arr,2, 2, 2, 2, 2), size) * 9000;
		
		/*活4*/
		size = 6;
		sum += check_condition(XD, set_array(arr,0, 2, 2, 2, 2, 0), size) * 3000;
		sum -= check_condition(XD, set_array(arr,0, 1, 1, 1, 1, 0), size) * 5000;
		
		/*直四*/		
		sum += check_condition(XD, set_array(arr,0, 2, 2, 2, 2, 1), size) * 600;
		sum += check_condition(XD, set_array(arr,1, 2, 2, 2, 2, 0), size) * 600;
		
		sum -= check_condition(XD, set_array(arr,0, 1, 1, 1, 1, 2), size) * 850;
		sum -= check_condition(XD, set_array(arr,2, 1, 1, 1, 1, 0 ), size) * 850;
		
		
		size = 5;
		sum += check_condition(XD, set_array(arr,0, 2, 2, 2, 2), size) * 600;
		sum += check_condition(XD, set_array(arr,2, 2, 2, 2, 0), size) * 600;
		
		sum -= check_condition(XD, set_array(arr,0, 1, 1, 1, 1), size) * 850;
		sum -= check_condition(XD, set_array(arr,1, 1, 1, 1, 0 ), size) * 850;
		
		/*against 4*/
		
		size = 6;
		sum += check_condition(XD, set_array(arr,2, 1, 1, 1, 1, 2), size) * 5000;
		sum -= check_condition(XD, set_array(arr,1, 2, 2, 2, 2, 1), size) * 7000;
		
		/*跳四*/
		size = 5;
		sum += check_condition(XD, set_array(arr,2, 2, 2, 0, 2), size) * 900;
		sum += check_condition(XD, set_array(arr,2, 2, 0, 2, 2), size) * 900;
		sum += check_condition(XD, set_array(arr,2, 0, 2, 2, 2), size) * 900;
		
		sum -= check_condition(XD, set_array(arr,1, 1, 1, 0, 1), size) * 2000;
		sum -= check_condition(XD, set_array(arr,1, 1, 0, 1, 1), size) * 2000;
		sum -= check_condition(XD, set_array(arr,1, 0, 1, 1, 1), size) * 2000;
	
		/*against jump 3*/
		size = 6;
		sum += check_condition(XD, set_array(arr,0, 1, 1, 2, 1, 0), size) * 30;
		sum += check_condition(XD, set_array(arr,0, 1, 2, 1, 1, 0), size) * 30;
		sum -= check_condition(XD, set_array(arr,0, 2, 2, 1, 2, 0), size) * 50;
		sum -= check_condition(XD, set_array(arr,0, 2, 1, 2, 2, 0), size) * 50;
		
		/*死三*/
		size = 5;
		sum -= check_condition(XD, set_array(arr,1, 0, 1, 0, 1), size) * 20;
		sum -= check_condition(XD, set_array(arr,1, 1, 0, 0, 1), size) * 20;
		sum -= check_condition(XD, set_array(arr,1, 0, 0, 1, 1), size) * 20;
		
		sum -= check_condition(XD, set_array(arr,2, 1, 1, 1, 0), size) * 30;
		sum -= check_condition(XD, set_array(arr,0, 1, 1, 1, 2), size) * 30;
		    
		sum -= check_condition(XD, set_array(arr,2, 1, 1, 0, 1, 0), 6) * 25;
		sum -= check_condition(XD, set_array(arr,2, 1, 0, 1, 1, 0), 6) * 25;
		sum -= check_condition(XD, set_array(arr,0, 1, 1, 0, 1, 2), 6) * 25;
		sum -= check_condition(XD, set_array(arr,0, 1, 0, 1, 1, 2), 6) * 25;
		
		sum += check_condition(XD, set_array(arr,2, 0, 2, 0, 2), size) * 30;
		sum += check_condition(XD, set_array(arr,2, 2, 0, 0, 2), size) * 30;
		sum += check_condition(XD, set_array(arr,2, 0, 0, 2, 2), size) * 30;
		
		sum += check_condition(XD, set_array(arr,1, 2, 2, 2, 0), size) * 50;
		sum += check_condition(XD, set_array(arr,0, 2, 2, 2, 1), size) * 50;
		    
		sum += check_condition(XD, set_array(arr,1, 2, 2, 0, 2, 0), 6) * 40;
		sum += check_condition(XD, set_array(arr,1, 2, 0, 2, 2, 0), 6) * 40;
		sum += check_condition(XD, set_array(arr,0, 2, 0, 2, 2, 1), 6) * 40;
		sum += check_condition(XD, set_array(arr,0, 2, 2, 0, 2, 1), 6) * 40;

		
		/*活2*/
		sum += check_condition(XD, set_array(arr,2, 2, 0, 0, 0), size) * 10;
		sum += check_condition(XD, set_array(arr,0, 2, 2, 0, 0), size) * 15;
		sum += check_condition(XD, set_array(arr,0, 0, 2, 2, 0), size) * 15;
		sum += check_condition(XD, set_array(arr,0, 0, 0, 2, 2), size) * 10;
		sum += check_condition(XD, set_array(arr,2, 0, 2, 0, 0), size) * 10;
		sum += check_condition(XD, set_array(arr,0, 2, 0, 2, 0), size) * 10;
		sum += check_condition(XD, set_array(arr,0, 0, 2, 0, 2), size) * 10;
		sum += check_condition(XD, set_array(arr,2, 0, 0, 2, 0), size) * 5;
		sum += check_condition(XD, set_array(arr,0, 2, 0, 0, 2), size) * 5;
		
		sum -= check_condition(XD, set_array(arr,1, 1, 0, 0, 0), size) * 10;
		sum -= check_condition(XD, set_array(arr,0, 1, 1, 0, 0), size) * 15;
		sum -= check_condition(XD, set_array(arr,0, 0, 1, 1, 0), size) * 15;
		sum -= check_condition(XD, set_array(arr,0, 0, 0, 1, 1), size) * 10;
		sum -= check_condition(XD, set_array(arr,1, 0, 1, 0, 0), size) * 10;
		sum -= check_condition(XD, set_array(arr,0, 1, 0, 1, 0), size) * 10;
		sum -= check_condition(XD, set_array(arr,0, 0, 1, 0, 1), size) * 10;
		sum -= check_condition(XD, set_array(arr,1, 0, 0, 1, 0), size) * 5;
		sum -= check_condition(XD, set_array(arr,0, 1, 0, 0, 1), size) * 5;
		
		
	}
	return (node_type == "max")?-sum:sum;

}

inline double calculate_evaluation (vector<int > XD, string node_type){

	double evaluation_value = 0;

	int line_count = 17;
	int progression_base = 10;
	vector<int > tmp;

	//For row line evaluation

	int row_end = 8;
	int row_start = 0;


	for(int i=0; i<line_count; ++i){
		for(int j=row_start; j<=row_end; ++j){
			//do calculate row evaluation value; j is the index
			tmp.push_back(XD[j]);
		}
		evaluation_value += calculate_value(tmp,node_type);
		row_start = row_end+1;
		row_end = fit_progression(8, progression_base, i+1, 8, 2);
		tmp.clear();
	}

	//For left to right line evaluation

	for(int i=0; i<line_count; ++i){
		int lefttoright_start = 8;

		if(i<=8){
			int times = lefttoright_start+i;
			int start = lefttoright_start-i;

			for(int j=0; j<=times; ++j){
				int value_index = fit_progression(start, progression_base, j);
				//do calculate lefttoright evaluation value; value_index is the index
				tmp.push_back(XD[value_index]);
			}
			evaluation_value += calculate_value(tmp,node_type);
		}

		else{
			int times = lefttoright_start+i-9;
			int start = 199+i;

			for(int j=0; j<=times; ++j){
				int value_index = fit_progression(start, -progression_base, j);
				//do calculate lefttoright evaluation value; value_index is the index
				tmp.push_back(XD[value_index]);
			}
			evaluation_value += calculate_value(tmp,node_type);
		}
		tmp.clear();

	}
	//For right to left line evaluation

	int righttoleft_start = 8;
	progression_base = 9;

	for(int i=0; i<line_count; ++i){

		if(i<=8){
			int times = righttoleft_start+i;
			int start = i;

			for(int j=0; j<=times; ++j){
				int value_index = fit_progression(start, progression_base, j);
				//do calculate righttoleft evaluation value; value_index is the index
				tmp.push_back(XD[value_index]);
			}
			evaluation_value += calculate_value(tmp,node_type);
		}

		else{
			int times = righttoleft_start+i-9;
			int start = 225-i;

			for(int j=0; j<=times; ++j){
				int value_index = fit_progression(start, -progression_base, j);
				//do calculate righttoleft evaluation value; value_index is the index
				tmp.push_back(XD[value_index]);
			}
			evaluation_value += calculate_value(tmp,node_type);
		}
		tmp.clear();

	}

	return evaluation_value;
}

inline bool check_aline(int near_position, int checkpoint, vector<pair<int, int> > XD){
	for(int i=0;i<XD.size();++i){
		if(XD[i].first == checkpoint){
			for(int j=-near_position;j<=near_position;++j){
				if(j==0)continue;
				int index = i+j;
				if(index>=0 && index<XD.size()){
					if(XD[index].second != 0)return true;
				}
			}
		}
	}
	return false;
}

inline bool is_near (int checkpoint, vector<int > XD, int near_position = 2){
	int line_count = 17;
	int progression_base = 10;
	vector<pair<int, int> > tmp;

	//For row line evaluation

	int row_end = 8;
	int row_start = 0;

	for(int i=0; i<line_count; ++i){
		for(int j=row_start; j<=row_end; ++j){
			//do calculate row evaluation value; j is the index
			tmp.push_back(make_pair(j,XD[j]));
		}
		if(check_aline(near_position, checkpoint, tmp))return true;
		row_start = row_end+1;
		row_end = fit_progression(8, progression_base, i+1, 8, 2);
		tmp.clear();
	}

	//For left to right line evaluation

	for(int i=0; i<line_count; ++i){
		int lefttoright_start = 8;

		if(i<=8){
			int times = lefttoright_start+i;
			int start = lefttoright_start-i;

			for(int j=0; j<=times; ++j){
				int value_index = fit_progression(start, progression_base, j);
				//do calculate lefttoright evaluation value; value_index is the index
				tmp.push_back(make_pair(value_index,XD[value_index]));
			}
		}

		else{
			int times = lefttoright_start+i-9;
			int start = 199+i;

			for(int j=0; j<=times; ++j){
				int value_index = fit_progression(start, -progression_base, j);
				//do calculate lefttoright evaluation value; value_index is the index
				tmp.push_back(make_pair(value_index,XD[value_index]));
			}
		}
		if(check_aline(near_position, checkpoint, tmp))return true;
		tmp.clear();

	}
	//For right to left line evaluation

	int righttoleft_start = 8;
	progression_base = 9;

	for(int i=0; i<line_count; ++i){

		if(i<=8){
			int times = righttoleft_start+i;
			int start = i;

			for(int j=0; j<=times; ++j){
				int value_index = fit_progression(start, progression_base, j);
				//do calculate righttoleft evaluation value; value_index is the index
				tmp.push_back(make_pair(value_index,XD[value_index]));
			}
		}

		else{
			int times = righttoleft_start+i-9;
			int start = 225-i;

			for(int j=0; j<=times; ++j){
				int value_index = fit_progression(start, -progression_base, j);
				//do calculate righttoleft evaluation value; value_index is the index
				tmp.push_back(make_pair(value_index,XD[value_index]));
			}
		}
		if(check_aline(near_position, checkpoint, tmp))return true;
		tmp.clear();

	}

	return false;
}

struct state {
	int pos;
	double evaluation_value;
	double value;
	vector<int> board;
	int level;
	string type;
	vector<struct state*> child;
};

int limit_level = 3;

double Max_rec(struct state *node, double A, double B){
	vector<int> valid_pos;
	int level = node->level + 1;
	double a = A;
	double b = B;
	double v = -9999999;

	if(level == limit_level){
		v = calculate_evaluation(node->board, "max");
		node->value = v;
		return v;
	}

	for(int i=0;i<217;++i){
		if(node->board[i] == 0)valid_pos.push_back(i);
	}

	for(int i=0; i<valid_pos.size(); ++i){
		if(is_near(valid_pos[i], node->board)){

			struct state *child = new struct state;
			for(int j=0;j<217;++j){
				if(valid_pos[i] == j){
					child->board.push_back(1);
				}
				else
					child->board.push_back(node->board[j]);
			}
			child->type = "min";
			child->level = level;
			child->pos = valid_pos[i];

			node->child.push_back(child);

			v = max(v, Min_rec(child, a, b));

			if(v >= b){
				node->value = v;
				return v;
			}
			a = max(a, v);
		}
	}
	node->value = v;
	return v;
	valid_pos.clear();
}

double Min_rec(struct state *node, double A, double B){
	vector<int> valid_pos;
	int level = node->level + 1;
	double a = A;
	double b = B;
	double v = 9999999;

	if(level == limit_level){
		v = calculate_evaluation(node->board, "min");
		node->value = v;
		return v;
	}

	for(int i=0;i<217;++i){
		if(node->board[i] == 0)valid_pos.push_back(i);
	}

	for(int i=0; i<valid_pos.size(); ++i){
		if(is_near(valid_pos[i], node->board)){

			struct state *child = new struct state;
			for(int j=0;j<217;++j){
				if(valid_pos[i] == j){
					child->board.push_back(2);
				}
				else
					child->board.push_back(node->board[j]);
			}
			child->type = "max";
			child->level = level;
			child->pos = valid_pos[i];

			node->child.push_back(child);

			v = min(v, Max_rec(child, a, b));
			if(v <= a){
				node->value = v;
				return v;
			}
			b = min(b, v);
		}
	}
	node->value = v;
	return v;
	valid_pos.clear();
}

void print_line(vector<int > XD, int number=0){
	int space = 17 - XD.size();
	for(int i=0; i<space; ++i){
		cout<<setw(2)<<"";
	}
	if(number){
		for(int i=0; i<XD.size(); ++i){
			if(XD[i] == -1)
				cout<<setw(3)<<"A"<<" ";
			else if(XD[i] == 1)
				cout<<setw(3)<<"O"<<" ";
			else if(XD[i] == 2)
				cout<<setw(3)<<"X"<<" ";
			else
				cout<<setw(3)<<XD[i]<<" ";

		}
	}
	else{
		for(int i=0; i<XD.size(); ++i){
			if(XD[i] == -1)
				cout<<setw(3)<<"A"<<" ";
			else if(XD[i] == 0)
				cout<<setw(3)<<"_"<<" ";
			else if(XD[i] == 1)
				cout<<setw(3)<<"O"<<" ";
			else if(XD[i] == 2)
				cout<<setw(3)<<"X"<<" ";
			else
				cout<<setw(3)<<XD[i]<<" ";

		}
	}
	cout<<endl;
}

void print_board (vector<int > XD, int pos){

	int line_count = 17;
	int progression_base = 10;
	vector<int > tmp;

	//For row line evaluation

	int row_end = 8;
	int row_start = 0;


	cout<<"board number"<<endl;
	for(int i=0; i<line_count; ++i){
		for(int j=row_start; j<=row_end; ++j){
			//do calculate row evaluation value; j is the index
			//cout<<j<<" ";
			if(XD[j]!=0)
				tmp.push_back(XD[j]);
			else if(j==pos)
				tmp.push_back(-1);
			else
				tmp.push_back(j);
		}
		print_line(tmp, 1);
		//cout<<endl;
		row_start = row_end+1;
		row_end = fit_progression(8, progression_base, i+1, 8, 2);
		tmp.clear();
	}
	row_end = 8;
	row_start = 0;

	cout<<"board spot"<<endl;
	for(int i=0; i<line_count; ++i){
		for(int j=row_start; j<=row_end; ++j){
			//do calculate row evaluation value; j is the index
			//cout<<j<<" ";
			if(j == pos)
				tmp.push_back(-1);
			else 
				tmp.push_back(XD[j]);
		}
		print_line(tmp);
		//cout<<endl;
		row_start = row_end+1;
		row_end = fit_progression(8, progression_base, i+1, 8, 2);
		tmp.clear();
	}

	return;
}

void free_root(struct state *node){
	for(int i=0; i<node->child.size();++i){
		free_root(node->child[i]);
	}
	free(node);
	return;
}

int Agent::_getNextMove() {

	bool first_move_flag = true;

	string type = "max";
	struct state *root = new struct state ;
	
	for(int i=0;i<board.size();++i){
		root->board.push_back( board[i]);
		if( board[i] != 0)first_move_flag = false;
	}

	if(first_move_flag){
		free(root);
		return 108;
	}

	root->type = type;
	root->level = 1;

	
	//for time
	double START,END;
	START = clock();
	
	
	Max_rec(root, -9999999, 9999999);
	double max_value = -9999999;
	int next_pos;
	for(int i=0; i<root->child.size();++i){
		if(max_value < (root->child[i]->value)){
			max_value = root->child[i]->value;
			next_pos = root->child[i]->pos;
		}
	}

	free_root(root);
	
	END = clock();
	double exc_time =  (END - START) / CLOCKS_PER_SEC;
	/*
	if(exc_time < 0.5) limit_level += 1;
	else if(exc_time > 4)limit_level-=1;
	*/
	cout<<endl<<"Time: "<<exc_time<<endl;
	
	return next_pos;
	
}

void Agent::_writeMove(int pos) {
	ofstream outfile(move_file);
	outfile << cur_move << " " << pos;
	outfile.close();
}

bool Agent::isMyTurn() { return valid_pos.size() != 0; }

bool Agent::isGameStop() { return game_stop; }

vector<int> Agent::getValidPos() { return valid_pos; }

vector<int> Agent::getMyPos() { return my_pos; }

vector<int> Agent::getOpponentPos() { return opponent_pos; }

vector<int> Agent::getBoard() { return board; }

int Agent::isFirstWinner() { return first_winner; }
