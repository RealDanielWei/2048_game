#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <stack>
#include <conio.h>

#define KEY_UP 'w'
#define KEY_DOWN 's'
#define KEY_LEFT 'a'
#define KEY_RIGHT 'd'
#define TARGET 2048
using namespace std;

class Board {
private:
	int row_num, col_num;
	int **matrix;
	default_random_engine engine;

	vector<pair<int, int>> get_empty_slot() {
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

	int compress(pair<int, int> base_point, pair<int, int> direction) {
		vector<int> vec;
		auto point = base_point;
		while (point.first >= 0 && point.first < this->row_num && point.second >= 0 and point.second < this->col_num) {
			if (this->matrix[point.first][point.second] != 0) {
				int value = this->matrix[point.first][point.second];
				if (vec.size()!=0 && vec[vec.size()-1]==value) {
					vec[vec.size() - 1] += value;
				}
				else {
					vec.push_back(value);
				}
			}
			point.first += direction.first;
			point.second += direction.second;
		}
		//rewrite this row/column
		point = base_point;
		int index = 0;
		int max_value = 0;
		while (point.first >= 0 && point.first < this->row_num && point.second >= 0 and point.second < this->col_num) {
			if (index < vec.size()) {
				this->matrix[point.first][point.second] = vec[index];
				if (vec[index] > max_value) {
					max_value = vec[index];
				}
				index++;
			}
			else {
				this->matrix[point.first][point.second] = 0;
			}
			point.first += direction.first;
			point.second += direction.second;
		}
		return max_value;
	}

public:
	Board(int m, int n) {
		this->row_num = m;
		this->col_num = n;
		this->matrix = new int*[this->row_num];
		for (int i = 0; i < this->row_num; i++) {
			this->matrix[i] = new int[this->col_num]();
		}
		this->engine = default_random_engine();
	}

	~Board() {
		for (int i = 0; i < this->row_num; i++) {
			delete[] this->matrix[i];
		}
		delete[] this->matrix;
	}

	void show() {
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
	}

	void replot() {
		system("cls");
		this->show();
	}

	int assign_one_slot() {
		auto vec = this->get_empty_slot();
		uniform_int_distribution<int> u(0, vec.size() - 1);
		int id = u(this->engine);
		this->matrix[vec[id].first][vec[id].second] = 2;
		return vec.size() - 1;
	}

	int one_press(char ch) {
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

	void start() {
		this->assign_one_slot();
		int n = this->assign_one_slot();
		this->replot();
		int max = 0;
		while (n > 0) {
			char ch = _getch();
			if (ch != KEY_UP && ch != KEY_DOWN && ch != KEY_LEFT && ch != KEY_RIGHT) {
				continue;
			}
			max = this->one_press(ch);
			this->replot();
			if (max == TARGET) {
				cout << "You win!" << endl;
				break;
			}
			else {
				n = this->assign_one_slot();
				this->replot();
			}
		}
		if (n <= 0) {
			cout << "You lose!" << endl;
		}
	}
};