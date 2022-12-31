#include <iostream>
#include <string>
#include <ctime>
#include <conio.h>
#include <Windows.h>
#include <vector>

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD coordScreen = { 0, 0 };
/* here"s where we"ll home thecursor */
DWORD cCharsWritten;
CONSOLE_SCREEN_BUFFER_INFO csbi;
/* to get buffer info */
DWORD dwConSize = 0;

bool gameOver = false;
const int width = 40;
const int height = 20;
int x = 0, y = 0, fruitX = 0, fruitY = 0, score = 0;
eDirection* dir = nullptr;
std::vector<int>* tailX = nullptr;
std::vector<int>* tailY = nullptr;
std::string* top_bot = nullptr;
std::string* mid = nullptr;


void cls();

void SetUp();

void Draw();

void Input();

void Logic();

void endGame();

int main() {
	SetUp();
	while (!gameOver) {
		Draw();
		Input();
		Logic();
	}
	endGame();
	return 0;
}

void cls() {
	
	/* number of character cells in the current buffer */
	/* get the number of character cells in the current buffer */
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	/* fill the entire screen with blanks */
	FillConsoleOutputCharacter(hConsole, (TCHAR)" ", dwConSize, coordScreen, &cCharsWritten);
	/* get the current text attribute */
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	/* now set the buffer"s attributes accordingly */
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	/* put the cursor at (0, 0) */
	SetConsoleCursorPosition(hConsole, coordScreen);
	return;
}

void SetUp() {
	top_bot = new std::string;
	mid = new std::string;
	dir = new eDirection;
	*dir = STOP;
	tailX = new std::vector<int>;
	tailY = new std::vector<int>;
	(*top_bot).push_back('#');
	(*mid).push_back('#');
	for (int i = 1; i < width; ++i) {
		(*top_bot).push_back('#');
		(*mid).push_back(' ');
	}
	*top_bot += "#\n";
	*mid += "#\n";
	srand(std::time(NULL));
	gameOver = false;
	x = width / 2;
	y = height / 2;
	fruitX = rand() % (width - 2) + 1;
	fruitY = rand() % (height - 2) + 1;
}

void Draw() {
	cls();
	std::string* map = new std::string;
	int i = 0, j = 0;
	std::cout << "Score: " << score << std::endl;
	if (y == 0) {
		(*top_bot)[x] = 'O';
		*map += *top_bot;
		(*top_bot)[x] = '#';
	}
	else *map += *top_bot;
	for (i = 1; i < height - 1; ++i) {
		for (j = 0; j < (*tailX).size(); ++j) {
			if ((*tailY)[j] == i) {
				(*mid)[(*tailX)[j]] = 'o';
			}
		}
		if (i == y) {
			(*mid)[x] = 'O';
		}
		if (i == fruitY) {
			(*mid)[fruitX] = 'F';
		}
		*map += *mid;
		if (x != 0) (*mid)[x] = ' ';
		else (*mid)[x] = '#';
		(*mid)[fruitX] = ' ';
		for (j = 0; j < (*tailX).size(); ++j) {
			if ((*tailY)[j] == i) {
				(*mid)[(*tailX)[j]] = ' ';
			}
		}
	}
	if (y == height - 1) {
		(*top_bot)[x] = 'O';
		*map += *top_bot;
		(*top_bot)[x] = '#';
	}
	else *map += *top_bot;
	std::cout << *map;
	delete map;
}

void Input() {
	if (_kbhit()) {
		switch (_getch())
		{
		case 'a':
			*dir = LEFT;
			break;
		case 'd':
			*dir = RIGHT;
			break;
		case 'w':
			*dir = UP;
			break;
		case 's':
			*dir = DOWN;
			break;
		case 'x':
			*dir = STOP;
			gameOver = true;
			break;
		default:
			break;
		}
	}
}

void Logic() {
	for (int i = (*tailX).size() - 1; i > 0; --i) {
		(*tailX)[i] = (*tailX)[static_cast<std::vector<int, std::allocator<int>>::size_type>(i) - 1];
		(*tailY)[i] = (*tailY)[static_cast<std::vector<int, std::allocator<int>>::size_type>(i) - 1];
	}
	if ((*tailX).size() > 0) {
		(*tailX)[0] = x;
		(*tailY)[0] = y;
	}
	Sleep(300);
	switch (*dir)
	{
	case LEFT:
		--x;
		break;
	case RIGHT:
		++x;
		break;
	case UP:
		--y;
		break;
	case DOWN:
		++y;
		break;
	default:
		break;
	}
	if (x < 1 || x > width - 2 || y < 1 || y > height - 2) {
		gameOver = true;
	}

	for (int i = 0; i < (*tailX).size(); ++i) {
		if ((*tailX)[i] == x && (*tailY)[i] == y) {
			gameOver = true;
		}
	}

	if (x == fruitX && y == fruitY) {
		score += 10;
		(*tailX).push_back(x);
		(*tailY).push_back(y);
		fruitX = rand() % (width - 2) + 1;
		fruitY = rand() % (height - 2) + 1;
	}
}

void endGame() {
	Draw();
	if (top_bot != nullptr) delete top_bot;
	if (mid != nullptr) delete mid;
	if (tailX != nullptr) delete tailX;
	if (tailY != nullptr) delete tailY;
	if (dir != nullptr) delete dir;
}