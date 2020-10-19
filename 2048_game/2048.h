#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <stack>
#include <time.h>
#include <conio.h>

#define KEY_UP 'w'
#define KEY_DOWN 's'
#define KEY_LEFT 'a'
#define KEY_RIGHT 'd'
#define TARGET 2048
using namespace std;

class Board {		//Game board class
private:
	int row_num, col_num;					// size of game board
	int **matrix;							// 2-D array as game board		
	long score;								// player's score

	vector<pair<int, int>> get_empty_slot() {			// get the coordinates of all the empty slots
		vector<pair<int, int>> vec;
		for (int i = 0; i < this->row_num; i++) {
			for (int j = 0; j < this->col_num; j++) {
				if (this->matrix[i][j] == 0) {
					vec.push_back(make_pair(i, j));
				}
			}
		}
		return vec;
	}

	int compress(pair<int, int> base_point, pair<int, int> direction) {				// one "compress" operation along one row or column and return the maximum value ever encountered
		vector<int> vec;															//compress operation is along the reverse direction of input direction towards input base point
		auto point = base_point;							//start from the base point and go against input direction
		while (point.first >= 0 && point.first < this->row_num && point.second >= 0 and point.second < this->col_num) {		//while the coordinate is valid
			if (this->matrix[point.first][point.second] != 0) {						//skip empty slots
				int value = this->matrix[point.first][point.second];
				if (vec.size()!=0 && vec[vec.size()-1]==value) {			// if two numbers are the same then merge them
					vec[vec.size() - 1] += value;
					this->score += 2 * value;
				}
				else {
					vec.push_back(value);									//if they are different then keep it
				}
			}
			point.first += direction.first;									//move to the next coordinate
			point.second += direction.second;
		}
		//rewrite this row/column in the same way
		point = base_point;
		int index = 0;
		int max_value = 0;
		while (point.first >= 0 && point.first < this->row_num && point.second >= 0 and point.second < this->col_num) {
			if (index < vec.size()) {										//write down compressed row/column 
				this->matrix[point.first][point.second] = vec[index];
				if (vec[index] > max_value) {
					max_value = vec[index];									//keep record of the maximum value ever generated
				}
				index++;
			}
			else {
				this->matrix[point.first][point.second] = 0;				//leave other slots empty
			}
			point.first += direction.first;									//move to the next coordinate
			point.second += direction.second;
		}
		return max_value;
	}

	void show() {								//print out the board and score
		for (int i = 0; i < this->row_num; i++) {
			cout << "|\t";
			for (int j = 0; j < this->col_num; j++) {
				if (this->matrix[this->row_num - 1 - i][j] == 0) {
					cout << "-";
				}
				else {
					cout << this->matrix[this->row_num - 1 - i][j];
				}
				cout << "\t|\t";
			}
			cout << endl;
		}
		cout << "Your Score:" << this->score << endl;
	}

	void replot() {							// replot the whole window and board
		system("cls");
		cout << "Welcome to the 2048 game made by Daniel Wei!" << endl;
		cout << "Press W, A, S or D to move and Q to quit." << endl;
		cout << "You need to produce a number of 2048 to win! Good luck!" << endl;
		this->show();
	}

	int assign_one_slot() {							// fill new number to a random empty slot
		auto vec = this->get_empty_slot();
		srand(time(NULL));							//generate random number between [0,vec.size() - 1]
		int id = rand() % vec.size();
		this->matrix[vec[id].first][vec[id].second] = 2;
		return vec.size() - 1;						// return number of empty slots for termination check
	}

	int one_press(char ch) {					// handles what to do when user presses a key
		int max_value = 0;
		switch (ch)
		{
		case KEY_UP:
			for (int i = 0; i < this->col_num; i++) {
				max_value = max(this->compress(make_pair(this->row_num - 1, i), make_pair(-1, 0)), max_value);
			}
			break;
		case KEY_DOWN:
			for (int i = 0; i < this->col_num; i++) {
				max_value = max(this->compress(make_pair(0, i), make_pair(1, 0)), max_value);
			}
			break;
		case KEY_LEFT:
			for (int i = 0; i < this->row_num; i++) {
				max_value = max(this->compress(make_pair(i, 0), make_pair(0, 1)), max_value);
			}
			break;
		case KEY_RIGHT:
			for (int i = 0; i < this->row_num; i++) {
				max_value = max(this->compress(make_pair(i, this->col_num - 1), make_pair(0, -1)), max_value);
			}
			break;
		default:
			break;
		}
		return max_value;
	}

public:
	Board(int m, int n) {							//constructor: allocate memory and initialize data members
		this->row_num = m;
		this->col_num = n;
		this->matrix = new int*[this->row_num];
		for (int i = 0; i < this->row_num; i++) {
			this->matrix[i] = new int[this->col_num]();
		}
		this->score = 0;
	}

	~Board() {										//destructor: properly release memory allocated
		for (int i = 0; i < this->row_num; i++) {
			delete[] this->matrix[i];
		}
		delete[] this->matrix;
	}

	void start() {
		this->assign_one_slot();					// fill two slots with numbers at the beginning
		int n = this->assign_one_slot();
		this->replot();								//refresh the window
		int max = 0;								
		while (n > 0) {
			char ch = _getch();						//wait for the user to press a key
			ch = tolower(ch);						//incase capslock is pressed
			if (ch == 'q') {						//check if the user wants to quit
				return;
			}
			if (ch != KEY_UP && ch != KEY_DOWN && ch != KEY_LEFT && ch != KEY_RIGHT) {		// other keys won't affect the game
				continue;
			}
			max = this->one_press(ch);				//respond to the key and get resulted max value for termination check 
			this->replot();							//refresh the window
			if (max == TARGET) {					//user wins if reached the target number
				cout << "You win!" << endl;
				break;
			}
			else {
				n = this->assign_one_slot();		//randomly add another number to start the next round
				this->replot();						//refresh the window
			}
		}
		if (n <= 0) {
			cout << "You lose!" << endl;			//user loses if no more empty slots left
		}
	}
};