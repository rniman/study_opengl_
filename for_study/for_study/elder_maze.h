#pragma once
#include "make_Shader.h"
#include <unordered_map>


//class maze
//{
//private:
//
//
//public:
//	static GLuint set_num;
//
//};
//
//GLuint maze::set_num = 0;

struct Cell
{
	//������ �࿭�ε���
	int col, row;
	//�׷�
	int group;
	//���Ἲ?
	bool up, down, left, right;
};

struct Group
{
	//���
	std::vector<Cell*> member;
	//������ ��
	std::vector<Cell*> last_rows;
};

//�Է�
int input;

int group_count = 1;
//���� ��
int cur_col = 0;
int draw_array[200][200];
Cell cells[90][90];
std::unordered_map<int, Group> groups;


void set_cell_group(int group);

void init()
{
	for (int i = 0; i < input; i++) 
	{
		for (int j = 0; j < input; j++) 
		{
			cells[i][j].row = j;
			cells[i][j].col = i;
		}
	}
}

void set_row()
{
	for (int i = 0; i < input; i++)
	{
		if (cells[cur_col][i].group == 0) // �׷��� ���� �������� �ʾҴٸ� �� �׷� ����
		{
			cells[cur_col][i].col = cur_col;
			cells[cur_col][i].row = i;
			cells[cur_col][i].group = group_count;

			//�׷쿡 ����
			groups.insert(std::make_pair(group_count, Group()));
			groups[group_count].member.push_back(&cells[cur_col][i]);
			groups[group_count].last_rows.push_back(&cells[cur_col][i]);

			//�׷� ī��Ʈ +1
			group_count++;
		}
	}
}

void rnd_union()
{
	int cur_group = -1;
	int next_group = -1;
	int dir;
	bool flag;
	std::uniform_int_distribution<int> dis(0, 2);

	for (int i = 0; i < input; i++)
	{
		cur_group = cells[cur_col][i].group;
		flag = false;

		switch (dis(gen))
		{
		case 0: // ������ ��ġ��
			//���Ұ� ���� ���� �ƴϰ� ���� �׷��� �ƴ϶�� �������ش�
			next_group = cells[cur_col][i + 1].group;

			if (i + 1 < input && cur_group != next_group) {
				cells[cur_col][i + 1].left = true;
				cells[cur_col][i].right = true;

				groups[cur_group].last_rows.push_back(&cells[cur_col][i + 1]);

				flag = true;
			}
			break;
		case 1: // ���� ��ġ��
			next_group = cells[cur_col][i - 1].group;

			if (i - 1 > -1 && cur_group != next_group) {
				cells[cur_col][i - 1].right = true;
				cells[cur_col][i].left = true;

				groups[cur_group].last_rows.push_back(&cells[cur_col][i - 1]);

				flag = true;
			}
			break;
		case 2: // ��ġ�� �ʱ�
			break;
		}
	
		if (flag)
		{
			groups[cur_group].member.insert(groups[cur_group].member.end(), groups[next_group].member.begin(), groups[next_group].member.end());
			groups.erase(next_group);
			set_cell_group(cur_group);
		}
	}

}

void set_cell_group(int group)
{
	for (int i = 0; i < groups[group].member.size(); i++)
	{
		groups[group].member[i]->group = group;
	}
}

//�Ʒ������� ������
void group_down()
{
	std::unordered_map<int, Group>::iterator iter;
	std::vector<Cell*> dest;
	int down_count = 0;
	int cur_row;

	for (iter = groups.begin(); iter != groups.end(); iter++)
	{
		down_count = 0;
		
		dest.clear();
		dest.assign(iter->second.last_rows.begin(), iter->second.last_rows.end());

		std::uniform_int_distribution<int> dis(0, 1);
		std::uniform_int_distribution<int> dis2(0, dest.size() - 1);


		groups[iter->first].last_rows.clear();

		for (int i = 0; i < dest.size(); i++)
		{
			//�����ϰų� ���ų� ����

			if (dis(gen))
			{
				cur_row = dest[i]->row;

				groups[iter->first].member.push_back(&cells[cur_col + 1][cur_row]);
				groups[iter->first].last_rows.push_back(&cells[cur_col + 1][cur_row]);

				cells[cur_col + 1][cur_row].up = true;
				cells[cur_col][cur_row].down = true;

				set_cell_group(iter->first);
				down_count++;
			}
		}

		//�ƹ��͵� �Ʒ��� �ȳ������� �ϳ��� ��� �ؾ���
		if (down_count == 0 && dest.size() != 0)
		{
			cur_row = iter->second.member[dis2(gen)]->row;

			groups[iter->first].member.push_back(&cells[cur_col + 1][cur_row]);
			groups[iter->first].last_rows.push_back(&cells[cur_col + 1][cur_row]);

			cells[cur_col + 1][cur_row].up = true;
			cells[cur_col][cur_row].down = true;

			set_cell_group(iter->first);
		}

	}

}

void last_line()
{
	for (int i = 0; i < input - 1; i++)
	{
		//������ ���� �ΰ��� ���Ұ� ���� �׷��� �ƴ϶�� ����
		if (cells[cur_col][i + 1].group != cells[cur_col][i].group)
		{
			cells[cur_col][i].right = true;
			cells[cur_col][i + 1].left = true;
		}
	}
}

void init_array()
{
	for (int i = 0; i < input * 2 + 1; i++)
	{
		for (int j = 0; j < input * 2 + 1; j++)
		{
			draw_array[i][j] = 1;
		}
	}
}

void set_array_block()
{
	for (int i = 0; i < input; i++)
	{
		for (int j = 0; j < input; j++)
		{
			draw_array[i * 2 + 1][j * 2 + 1] = 0;

			if (cells[i][j].up)
				draw_array[i * 2][j * 2 + 1] = 0;

			if (cells[i][j].down)
				draw_array[i * 2 + 2][j * 2 + 1] = 0;

			if (cells[i][j].left)
				draw_array[i * 2 + 1][j * 2] = 0;

			if (cells[i][j].right)
				draw_array[i * 2 + 1][j * 2 + 2] = 0;

		}
	}
}


void draw_maze()
{
	for (int i = 0; i < input * 2 + 1; i++)
	{
		for (int j = 0; j < input * 2 + 1; j++)
		{
			if (draw_array[i][j] > 0)
			{
				std::cout << "��";
			}
			else
				std::cout << "  ";
		}
		std::cout << std::endl;
	}
}


void print_group()
{
	for (int i = 0; i < input; i++)
	{
		for (int j = 0; j < input; j++)
		{
			{
				printf("%4d", cells[i][j].group);
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void print_maze() {
	init_array();
	set_array_block();
	draw_maze();
	//print_group();
}