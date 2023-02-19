#pragma once
#include "make_Shader.h"
#include <iostream>
#include <queue>
#include <list>

class path
{
private:
	bool wall; //1�̸� �̷η� �����ִ�. 0�̸� ���̴�
	bool visit = false;
	int col;
	int row;
	path* prev = nullptr;

public:
	int dist = 0;

	bool get_wall()const { return wall; }
	bool get_visit()const { return visit; }
	int get_col()const { return col; }
	int get_row()const { return row; }
	path* get_prev()const { return prev; }
	

	GLvoid set_wall(const bool& i_bool) { wall = i_bool; }
	GLvoid set_visit() { visit = true; }
	GLvoid set_col(const int& i_col) { col = i_col; }
	GLvoid set_row(const int& i_row) { row = i_row; }
	GLvoid set_prev(path* i_prev_path) { prev = i_prev_path; }
};

bool is_path(const path path_matrix[][25], int dist_col, int dist_row);

std::vector<int> find_path(const std::vector<std::vector<wall>>& w_list, const int& start_col, const int& start_row, const int& target_col, const int& target_row)
{
	path path_matrix[25][25];

	for (int i = 0; i < 25; ++i)
	{
		for (int j = 0; j < 25; ++j)
		{
			path_matrix[i][j].set_wall(w_list[i][j].maze_state);
			path_matrix[i][j].set_col(i);
			path_matrix[i][j].set_row(j);
		}
	}

	int dist_col[] = { -1, 0, 1, 0 };
	int dist_row[] = { 0, 1, 0, -1 };

	std::vector<path> final;
	std::queue<path> adjacent_queue;

	adjacent_queue.push(path_matrix[start_col][start_row]);

	path_matrix[start_col][start_row].set_visit();
	path_matrix[start_col][start_row].dist = 1;

	//ť�� �������� �ݺ��Ѵ�.
	while (!adjacent_queue.empty())
	{
		path now_path = adjacent_queue.front();
		adjacent_queue.pop();

		if (now_path.get_col() == target_col && now_path.get_row() == target_row)
		{
			path now = path_matrix[target_col][target_row];

			while (now.get_prev() != nullptr)
			{
				final.push_back(now);
				now = *now.get_prev();
			}
			break;
		}

		for (int i = 0; i < 4; ++i)
		{
			int search_col = now_path.get_col() + dist_col[i];
			int search_row = now_path.get_row() + dist_row[i];
			if (is_path(path_matrix, search_col, search_row))
			{

				path_matrix[search_col][search_row].set_visit();
				path_matrix[search_col][search_row].set_prev(&path_matrix[now_path.get_col()][now_path.get_row()]);
				path_matrix[search_col][search_row].dist = now_path.dist + 1;
				adjacent_queue.push(path_matrix[search_col][search_row]);

			}
		}
	}

	if (final.empty())
	{
		std::vector<int> last = { start_col, start_row };
		return last;
	}
	else
	{
		std::vector<int> last = { final[final.size() - 1].get_col(),final[final.size() - 1].get_row() };
		return last;
	}
	
}

bool is_path(const path path_matrix[][25], int dist_col, int dist_row)
{
	// maze ������ ����� �ȵ�.
	if (dist_row < 0 || dist_col < 0 || dist_row > 24 || dist_col > 24) {
		return false;
	}

	// �̹� �湮�� ��嵵 �ȵ�. wall �� ���� ����.
	if (path_matrix[dist_col][dist_row].get_visit() || !path_matrix[dist_col][dist_row].get_wall()) {
		return false;
	}
	return true;
}