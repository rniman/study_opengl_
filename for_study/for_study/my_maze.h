#pragma once
#include <stack>
#include <vector>
#include <random>

enum DIRECTION
{
	TOP = 0,
	BOTTOM,
	LEFT,
	RIGHT,
};

class tile
{
public:
	int mX, mY;
	//true->�����ִ� false->�տ��ִ�
	bool wallOpen[4] = { true, true, true, true };
	bool visited = false;

	tile(int i_x, int i_y)
	{
		mX = i_x;
		mY = i_y;
	}

	tile() 
	{
		mX = 5;
		mY = 5;
	};

};

class maze
{
private:
	int mMazeWidth;
	int mMazeLength;

public:
	std::vector<std::vector<tile>> Maze;
	tile* curTile;
	bool reset;

	int visitedCellCount;

	std::stack<tile*> tileStack;
	static bool completeGenerate;

	void initialize(const int& w, const int& l);
	void generator();

	//�ֺ� Ÿ�� �˻�
	tile* checkNeighborTiles(const tile& curTile)
	{
		//���� Ÿ�� ��ġ
		int x = curTile.mX;
		int y = curTile.mY;

		bool top = true;
		bool right = true;
		bool bottom = true;
		bool left = true;

		if (y == 0) top = false;
		if (x == mMazeWidth - 1)right = false;
		if (y == mMazeLength - 1)bottom = false;
		if (x == 0) left = false;

		//�湮 ���� ���� �̿� Ÿ�� Ž��

		//�̿� Ÿ�� ����� ����
		std::vector<tile*> nbTiles;

		 
		if (top) //ž���ְ� �ְ� �湮���� ���� ���̶�� �̿�Ÿ�� ���Ϳ� �ִ´�
			if (!Maze[y - 1][x].visited)
				nbTiles.push_back(&Maze[y - 1][x]);

		if (right)
			if (!Maze[y][x + 1].visited)
				nbTiles.push_back(&Maze[y][x + 1]);

		if (bottom)
			if (!Maze[y + 1][x].visited)
				nbTiles.push_back(&Maze[y + 1][x]);

		if (left)
			if (!Maze[y][x - 1].visited)
				nbTiles.push_back(&Maze[y][x - 1]);

		//�̿��� Ÿ���� �����ְ� �湮���� �������� �ִٸ�
		if (!nbTiles.empty())
		{
			//�̿� ���Ϳ��� �ƹ��ų� �ϳ��� �����Ѵ�
			int r = getRandom(0, nbTiles.size() - 1);
			return nbTiles[r];
		}
		else //���ٸ� -> �������� ����
			return nullptr;
	}

	int getRandom(int min, int max) {
		std::random_device ranDevice;
		std::mt19937 gen(ranDevice());
		std::uniform_int_distribution<int> dis(min, max);

		return (int)dis(gen);
	}

	void openWall(const int dir, int x, int y)
	{
		//���⿡ ����
		switch (dir)
		{
		case TOP://top�̶�� 
			Maze[y][x].wallOpen[0] = false;
			Maze[y - 1][x].wallOpen[2] = false;
			break;
		case BOTTOM:
			Maze[y][x].wallOpen[2] = false;
			Maze[y + 1][x].wallOpen[0] = false;
			break;
		case RIGHT:
			Maze[y][x].wallOpen[1] = false;
			Maze[y][x + 1].wallOpen[3] = false;
			break;
		case LEFT:
			Maze[y][x].wallOpen[3] = false;
			Maze[y][x - 1].wallOpen[1] = false;
			break;
		}
	}

	void openWall(const tile& firstTile, const tile& secondTile)
	{
		//���� üũ
		int checkDirection;

		if (firstTile.mX == secondTile.mX) { //��Ÿ���� x���� ������ ���Ʒ� ��
			checkDirection = firstTile.mY - secondTile.mY;
			if (checkDirection < 0) 
			{	//0���� �۴ٴ� �ι�° Ÿ���� �Ʒ��� �ִٴ� �� -> ������ �վ��ش�
				openWall(BOTTOM, firstTile.mX, firstTile.mY);
			}
			else 
			{	//ž�� �մ´�
				openWall(TOP, firstTile.mX, firstTile.mY);
			}
		}

		else { //y���� ������ 
			checkDirection = firstTile.mX - secondTile.mX;
			if (checkDirection < 0)
			{
				openWall(RIGHT, firstTile.mX, firstTile.mY); 
			}
			else 
			{
				openWall(LEFT, firstTile.mX, firstTile.mY); 
			}
		}
	}

	void ResetMaze() {
		if (reset) return;
		while (!tileStack.empty()) {
			tileStack.pop();
		}
		Maze.clear();
		visitedCellCount = 1;
		maze::completeGenerate = false;
		reset = true;
	}
};

void maze::initialize(const int& w, const int& l)
{
	mMazeWidth = w;
	mMazeLength = l;
	visitedCellCount = 1;

	//�ʱ�ȭ
	Maze = std::vector<std::vector<tile>>(mMazeLength);
	for (int i = 0; i < mMazeLength; ++i)
	{
		Maze[i] = std::vector<tile>(mMazeWidth);
	}

	for (int y = 0; y < mMazeLength; y++)
	{
		for (int x = 0; x < mMazeWidth; x++)
		{
			Maze[y][x].mX = x;
			Maze[y][x].mY = y;
			for (int i = 0; i < 4; ++i)
				Maze[y][x].wallOpen[i] = true;

			Maze[y][x].visited = false;
		}
	}
	completeGenerate = false;
	
	//�ϴ� ����
	curTile = &Maze[0][0];

	reset = false;
}

void maze::generator()
{
	curTile->visited = true;
	tile* nextTile = this->checkNeighborTiles(*curTile);
	
	if (visitedCellCount < (mMazeLength * mMazeWidth))
	{
		if (nextTile != nullptr)
		{
			nextTile->visited = true;

			//stack push
			tileStack.push(curTile);

			openWall(*curTile, *nextTile);

			//����
			curTile = nextTile;

			visitedCellCount++;
		}
		else if (!tileStack.empty()) {
			curTile = tileStack.top();
			tileStack.pop();
		}
	}
	else
	{
		//std::cout << visitedCellCount<< " �̷� �ϼ�" << std::endl;
		maze::completeGenerate = true;
	}

}

bool maze::completeGenerate = false;