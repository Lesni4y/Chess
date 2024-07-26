#include <iostream>
#include <stdlib.h>
using namespace std;

int figure = 0, previousY, enemy = 0;
char previousX;
const int rows = 10, columns = 10;
char board[rows][columns];
void intro(); // ������� ������ �������� ��������� � ���������� ��� ������������
void startPosition(); // �������, � ������� �������� ��������� �������� �����
void initializeBoard(); // ������� ������������� �����
void figurePositioning(); // ������� ����������� ����� �� �����
void printBoard(); // ������� ������ �������� ��������� �����
int cellOccupied(); // ������� ��������, ��� �� ������� ��������� ������ �� ������� ����
void numberInput(); // ������� ����� ������ ������ ��� ����
int figureExistence(); // ������� �������� ����������� ������ ������
int turnCheck(); // ������� �������� ����������� ����
void inputCoordinates(); // ������� ����� ��������� ������
int xCoordinate(char); // �������, ������������� ����� �������������� ���������� � ����� �������������� �������, ������������ �����
int yCoordinate(int); // �������, ������������� ����� ������������ ���������� � ����� ������������� �������, ������������ �����
int figureNameCheck(char); // ������� �������� ������������ ����� �������� ������
int figureStepCheck(char); // ������� �������� ������������ ���� ��� ������� ������
void captureFact(); // ������� �������� ����� ������ ������� ������� - ������ ����������
int figureJumpCheck(int, int, int, int, int, int); // ������� �������� ���� ����� ������
void step(); // ������� ����������� ������ �� ����
int outOfBoard(int, int); // ������� �������� ������������ ���������

struct Figure // ���������������� ��������� ������ - ������, ���������� ��������� ����������:
{
	char name; // �������� ������, �������� �������������
	char xLetter; // ��������� ���������� �, �������� �������������
	int yInverted; // ���������� y �� ��������������, �������� �������������
	int x, y;  // ���������� ������, ��������������� ��������� xCoordinate � yCoordinate ��� ����������� ����������� ����� ��� ������ �����
};
Figure figures[3]; // �������� ��� ���������������� ���������� - ��� ��� �������� �����, ������ ��� �������� �������� ������, ������� ����������� ���, 
// ����� ��������� ������������ ����� ���������

int main()
{
	intro(); // ������� ������ �������� ��������� � ���������� ��� ������������
	startPosition(); // �������, � ������� �������� ��������� �������� �����
	do { // ����, ����������� ���� ����� ��������� ����� ��������	
		numberInput(); // ������� ����� ������ ������ ��� ����:
		bool flag = false;
		do {
			inputCoordinates(); // ������� ����� ��������� ������
			if (outOfBoard(figures[figure].x, figures[figure].y) == -7) continue; // ������� �������� ������������ ���������
			if (figureStepCheck(figures[figure].name) == -5) continue; // ������� �������� ������������ ���� ��� ������� ������
			// printf("%d - %d - %d - %d - %d - %d\n", figures[2].x, figures[2].y, figures[figure].x, figures[figure].y, figures[enemy].x, figures[enemy].y); // ����������
			if (figureJumpCheck(figures[2].x, figures[2].y, figures[figure].x, figures[figure].y, figures[enemy].x, figures[enemy].y) == -3) continue; // ������� �������� ���� ����� ������
			// � ������ ���������� ������ - ��������� � ����� ������� ������� ������:
			printf("\nFigure %d (%c) changes position from %c%d to %c%d.\n", figure + 1, figures[figure].name, previousX, previousY, figures[figure].xLetter, figures[figure].yInverted);
			captureFact(); // ������� �������� ����� ������ ������� ������� - ������ ����������
			step(); // ������� ����������� ������ �� ����
			printBoard(); // ������� ����������� ������� �����
			flag = true;
		} while (flag == false);
	} while ((-1 < figure) && (figure < 3)); // ���� (����) ������������ �� ��� ���, ���� ������������ �� ����� "q" �� ����� ������ ������ ��� ����
	cout << "Game over! See you later." << endl << endl;
	system("pause");
}

void intro() // ������� ������ �������� ��������� � ���������� ��� ������������
{
	initializeBoard();  // ������� ������������� �����
	cout << "\tChess.\n\nFigure names: K (King), Q (Queen), R (Rook), B (Bishop), k (knight).\nSide: \"w\" - white, \"b\" - black.\nFirst of all you need to input figure's name. Then you need to input it's coordinates - capital letter of horizontal line then press Enter and digit of vertical line then press Enter.\nAfter entering figures and their start position you will enter a number of figure to make a step (1 or 2) and coordinates of each in same way.\nTo quit a game type \"exit\".\n\nAn example of start figures's position:\nFigure 1: R, w. Figure 2: B, b.\n\nAn example of board:\n\n||||||||||||||||||||\n";
	board[3][4] = 'R', board[4][2] = 'B';
	printBoard(); // ������� ������ �������� ��������� �����
	cout << "||||||||||||||||||||\n\n\nYour game is starting.\n\n";
	board[3][4] = '.', board[4][2] = '.';
}
void startPosition() // �������, � ������� �������� ��������� �������� �����
{
	for (figure = 0; figure < 2; figure++) // ������ ��������� - ����, � ������� ������� �������� ���� �������� �����
	{
		cout << "Inpute figure's name:\n";
		cin >> figures[figure].name; // �������� ��� ������: k (����), K (������), Q (�����), R (�����) ��� B (����)
		if (figureNameCheck(figures[figure].name) == -4) { cout << "Wrong figure name\n" << endl; continue; } // ������� �������� ������������ ����� �������� ������.
	// � ������ ������, ��������� ��������������� ���������, � ���� ���������� �������, ��������� ����� ������ �������� ������
		inputCoordinates(); // ������� ����� ��������� ������
		if (cellOccupied() == -6) continue; // ������� ��������, ��� �� ������� ��������� ������ �� ������� ����
		figurePositioning(); // ������� ����������� ����� �� �����
		printBoard(); // ������� ����������� ������� �����
		cout << endl;
	}
}

void numberInput() // ������� ����� ������ ������ ��� ����:
{
	bool flag = false;
	do
	{
		cout << "Input figure's number to make a step: \n";
		cin >> figure; // ���������� figure ������� �������� 1 ��� 2
		if (figureExistence() == -1) continue; // ������� �������� ����������� ������ ������
		if (turnCheck() == -2) continue; // ������� �������� ����������� ����
		figure--; // ���������� figure ����������� �� �������, ����� � � ������� ����� ���� ���������� � ��������������� �������� � ������� (������ 1 ����������� �� 0,
		// ������ 2 - �� 1
		enemy = (figure == 0) ? 1 : 0; // ���������� enemy ������� ��������������� �� ������� ������ ��������
		previousX = figures[figure].xLetter; // � ���������� ���������� previousX ������������ ������� �������� �������� ����� �������������� ����������
		previousY = figures[figure].yInverted; // � ������������� ���������� previousY ������������ ������� �������� �������� ����� ������������ ����������
		// ����� � ��������� ���������� ���������������� ��������� � 3-�� �������� ������� ������������ ������� �������� ������ � � ����������:
		figures[2].name = figures[figure].name, figures[2].x = figures[figure].x, figures[2].y = figures[figure].y;
		flag = true;
	} while (flag == false);
}

int figureExistence() // ������� �������� ����������� ������ ������
{
	if (figure < 1 || figure > 2) { cout << "Wrong figure number. \n" << endl; return -1; } // ���� �������� ���������� �� 1 ��� 2, ��������� ������, ���� �����������
}

int turnCheck() // ������� �������� ����������� ����
{
	if (figure == enemy || figure == (enemy + 2)) 
	{ 
		cout << "It's not your turn. \n" << endl; 
		return -2;
	}
}

void inputCoordinates() // ������� ����� ��������� ������
{
		cout << "Input figure's step: \n";
		cin >> figures[figure].xLetter >> figures[figure].yInverted; // �������� ��������� �������������� � �������� ������������ ����������
		figures[figure].x = xCoordinate(figures[figure].xLetter); // ������� xCoordinate ����������� ����� � ����� �������������� �������, ������������ �����
		figures[figure].y = yCoordinate(figures[figure].yInverted); // ������� yCoordinate ����������� ����� � ����� ������������� �������, ������������ �����
}

int xCoordinate(char xLetter) // �������, ������������� ����� �������������� ���������� � ����� �������������� �������, ������������ �����
{
	return xLetter - 'A' + 1;
}

int yCoordinate(int y) // �������, ������������� ����� ������������ ���������� � ����� ������������� �������, ������������ �����
{
	return 9 - y;
}

int figureStepCheck(char name) // ������� �������� ������������ ���� ��� ������� ������
{
	switch (name) {
	case 'k':
		if (abs(figures[figure].x - figures[2].x) == 2 && abs(figures[figure].y - figures[2].y) == 1 ||
			abs(figures[figure].x - figures[2].x) == 1 && abs(figures[figure].y - figures[2].y) == 2)
		{
			// cout << "CASE KNIGHT OK" << endl; // ����������
			break;
		}
		
	case 'Q':
		if (abs(figures[figure].x - figures[2].x) == abs(figures[figure].y - figures[2].y) ||
			abs(figures[figure].x - figures[2].x) == 0 || abs(figures[figure].y - figures[2].y) == 0)
		{
			// cout << "CASE QUEEN OK" << endl; // ����������
			break;
		}
		
	case 'R':
		if (abs(figures[figure].x - figures[2].x) == 0 || abs(figures[figure].y - figures[2].y) == 0)
		{
			// cout << "CASE ROOK OK" << endl; // ����������
			break;
		}
		
	case 'B':
		if (abs(figures[figure].x - figures[2].x) == abs(figures[figure].y - figures[2].y))
		{
			// cout << "CASE BISHOP OK" << endl; // ����������
			break;
		}
		
	case 'K':
		if (abs(figures[figure].x - figures[2].x) <= 1 && abs(figures[figure].y - figures[2].y) <= 1)
		{
			// cout << "CASE KING OK" << endl; // ����������
			break;
		} 
		
	//case 'P': // ���� ��� ����� - � ����������
	//	if (abs(figures[figure].x - figures[2].x) == 0 && (figures[figure].y - figures[2].y) == 1)
	//	{
	//		cout << "CASE PAWN STEP OK" << endl; // ����������
	//		break;
	//	}
	//	else if (figures[figure].y == 2 && abs(figures[figure].x - figures[2].x) == 0 && (figures[figure].y - figures[2].y) == 2)
	//	{
	//		cout << "CASE PAWN long STEP OK" << endl; // ����������
	//		break;
	//	}
	//	else if (abs(figures[figure].x - figures[2].x) == 1 && (figures[figure].y - figures[2].y) == 1 && board[figures[2].y][figures[2].x] != ".")
	//	{
	//		cout << "CASE PAWN ATTACK OK" << endl; // ����������
	//		break;
	//	}
	//	else if (abs(figures[figure].x - figures[2].x) == 1 && (figures[figure].y - figures[2].y) == 1 && board[figures[2].y][figures[2].x] != ".")
	//	{
	//		cout << "CASE PAWN en passant ATTACK OK" << endl; // ����������
	//		break;
	//
	default:
		cout << "Wrong step for this figure!\n" << endl; return -5;
	}
}

int cellOccupied() // ������� ��������, ��� �� ������� ��������� ������ �� ������� ����
{
	if (board[figures[0].y][figures[0].x] == board[figures[1].y][figures[1].x])
	{
		cout << "This cell is occupied by another figure!\n" << endl; return -6;
	}
}

int figureJumpCheck(int previousX, int previousY, int figuresX, int figuresY, int enemyX, int enemyY) // ������� �������� ���� ����� ������
{
	if (
		figures[figure].name != 'k' &&
		((previousY == enemyY && previousY == figuresY) && (abs(previousX - figuresX) > abs(previousX - enemyX)) && (previousX - figuresX) * (previousX - enemyX) > 0) || // �������� ��������� �� ���������
		((previousX == enemyX && previousX == figuresX) && (abs(previousY - figuresY) > abs(previousY - enemyY)) && (previousY - figuresY) * (previousY - enemyY) > 0) || // �������� ��������� �� �����������
		((abs(previousX - figuresX) == abs(previousY - figuresY) && (abs(previousX - figuresX) > abs(previousX - enemyX)) && (abs(previousX - enemyX) == abs(previousY - enemyY)) &&
			((previousX - figuresX) * (previousX - enemyX) > 0) && ((previousY - figuresY) * (previousY - enemyY) > 0))) // �������� ��������� �� ���������
		)
	{
		cout << "Figure can't jump over other one!\n" << endl;
		return -3;
	}
}

int outOfBoard(int x, int y) // ������� �������� ������������ ���������
{
	if (x < 1 || x > 8 || y < 1 || y > 8) 
	{
		cout << "Step is out of board!\n" << endl; return -7; 
	}
}

int figureNameCheck(char name) // ������� �������� ������������ ����� �������� ������
{
	if (name != 'k' && name != 'K' && name != 'B' && name != 'Q' && name != 'R') return -4;
}

void step() // ������� ����������� ������ �� ����
{
	board[figures[2].y][figures[2].x] = '.'; // �� ��� �����, ��� ���� ������, �������� �����
	board[figures[figure].y][figures[figure].x] = figures[figure].name; // ����� ���������� ������ ������������ �� �����
}

void captureFact()  // ������� �������� ����� ������ ������� ������� - ������ ����������
{
	if (board[figures[figure].y][figures[figure].x] == figures[enemy].name) // �������� ����� ������ ������� ������� - ������ ����������
	{ // ��� ���������� � ��� ������, ���� �� �������� ����������� ������� ������, ���������� ���-��, �������� �� ����� (������ ������)
		// ��������� ��������� � ������ ��������� ������, ���������� enemy ������������� �� �������:
		printf("\nFigure %d (%c) captures figure %d (%c).\n", figure + 1, figures[figure].name, enemy + 1, figures[enemy].name);
		figure = 3;
	}
}

void initializeBoard() // ������� ������������� �����
{
	for (int i = rows - 2; i >= 1; i--) {
		for (int j = 0; j < columns; j++) {
			if (j == 0 || j == columns - 1) {
				board[i][j] = '9' - i;
			}
			else {
				board[i][j] = '.';
			}
		}
	}

	// ��������� ������ � �������: {" ","A","B","C","D","E","F","G","H"," "}
	for (int j = 0; j < columns; j++) {
		if (j == 0 || j == columns - 1) {
			board[0][j] = board[columns - 1][j] = ' ';
		}
		else {
			board[0][j] = board[columns - 1][j] = 'A' + (j - 1);
		}
	}
}

void figurePositioning() // ������� ����������� ����� �� �����
{
	board[figures[0].y][figures[0].x] = figures[0].name;
	board[figures[1].y][figures[1].x] = figures[1].name;
}

void printBoard() // ������� ������ �������� ��������� �����
{
	cout << "\nYour chessboard: " << endl;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++)
		{
			// if ((board[i][j] == figures[1].name) && (board[i][j] != '.')) { printf("\n\x1B[00;32m%c \e[0m", board[i][j]); printf("\n"); continue; }
			cout << board[i][j] << ' ';
		}
		cout << endl;
	} cout << endl;
}