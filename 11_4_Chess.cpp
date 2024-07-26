#include <iostream>
#include <stdlib.h>
using namespace std;

int figure = 0, previousY, enemy = 0;
char previousX;
const int rows = 10, columns = 10;
char board[rows][columns];
void intro(); // функция печати названия программы и инструкций для пользователя
void startPosition(); // функция, в которой задаются стартовые значения фигур
void initializeBoard(); // функция инициализации доски
void figurePositioning(); // функция выставления фигур на доску
void printBoard(); // функция печати текущего состояния доски
int cellOccupied(); // функция проверки, нет ли попытки поставить фигуру на занятое поле
void numberInput(); // функция ввода номера фигуры для хода
int figureExistence(); // функция проверки корректного номера фигуры
int turnCheck(); // функция проверки очередности хода
void inputCoordinates(); // функция ввода координат фигуры
int xCoordinate(char); // функция, преобразующая буквы горизонтальной координаты в линию горизонтальную массива, отображающую доску
int yCoordinate(int); // функция, преобразующая цифру вертикальной координаты в линию вертикального массива, отображающую доску
int figureNameCheck(char); // функция проверки корректности буквы введённой фигуры
int figureStepCheck(char); // функция проверки корректности хода для текущей фигуры
void captureFact(); // функция проверки факта взятия текущей фигурой - фигуры противника
int figureJumpCheck(int, int, int, int, int, int); // функция проверки хода через фигуру
void step(); // функция перемещения фигуры на поле
int outOfBoard(int, int); // функция проверки корректности координат

struct Figure // пользовательская структура данных - фигура, включающая следующие переменные:
{
	char name; // название фигуры, вводимое пользователем
	char xLetter; // буквенная координата х, вводимая пользователем
	int yInverted; // координата y до инвертирования, вводимая пользователем
	int x, y;  // координаты фигуры, преобразованные функциями xCoordinate и yCoordinate для корректного отображения фигур при печати доски
};
Figure figures[3]; // создание трёх пользовательских переменных - две для играющих фигур, третья для хранения значения фигуры, которой совершается ход, 
// перед проверкой корректности новых координат

int main()
{
	intro(); // функция печати названия программы и инструкций для пользователя
	startPosition(); // функция, в которой задаются стартовые значения фигур
	do { // цикл, имитирующий игру двумя заданными ранее фигурами	
		numberInput(); // функция ввода номера фигуры для хода:
		bool flag = false;
		do {
			inputCoordinates(); // функция ввода координат фигуры
			if (outOfBoard(figures[figure].x, figures[figure].y) == -7) continue; // функция проверки корректности координат
			if (figureStepCheck(figures[figure].name) == -5) continue; // функция проверки корректности хода для текущей фигуры
			// printf("%d - %d - %d - %d - %d - %d\n", figures[2].x, figures[2].y, figures[figure].x, figures[figure].y, figures[enemy].x, figures[enemy].y); // отладочное
			if (figureJumpCheck(figures[2].x, figures[2].y, figures[figure].x, figures[figure].y, figures[enemy].x, figures[enemy].y) == -3) continue; // функция проверки хода через фигуру
			// в случае отсутствия ошибок - сообщение о смене позиции текущей фигуры:
			printf("\nFigure %d (%c) changes position from %c%d to %c%d.\n", figure + 1, figures[figure].name, previousX, previousY, figures[figure].xLetter, figures[figure].yInverted);
			captureFact(); // функция проверки факта взятия текущей фигурой - фигуры противника
			step(); // функция перемещения фигуры на поле
			printBoard(); // функция отображения текущей доски
			flag = true;
		} while (flag == false);
	} while ((-1 < figure) && (figure < 3)); // цикл (игра) продолжается до тех пор, пока пользователь не введёт "q" во время выбора фигуры для хода
	cout << "Game over! See you later." << endl << endl;
	system("pause");
}

void intro() // функция печати названия программы и инструкций для пользователя
{
	initializeBoard();  // функция инициализации доски
	cout << "\tChess.\n\nFigure names: K (King), Q (Queen), R (Rook), B (Bishop), k (knight).\nSide: \"w\" - white, \"b\" - black.\nFirst of all you need to input figure's name. Then you need to input it's coordinates - capital letter of horizontal line then press Enter and digit of vertical line then press Enter.\nAfter entering figures and their start position you will enter a number of figure to make a step (1 or 2) and coordinates of each in same way.\nTo quit a game type \"exit\".\n\nAn example of start figures's position:\nFigure 1: R, w. Figure 2: B, b.\n\nAn example of board:\n\n||||||||||||||||||||\n";
	board[3][4] = 'R', board[4][2] = 'B';
	printBoard(); // Функция печати текущего состояния доски
	cout << "||||||||||||||||||||\n\n\nYour game is starting.\n\n";
	board[3][4] = '.', board[4][2] = '.';
}
void startPosition() // функция, в которой задаются стартовые значения фигур
{
	for (figure = 0; figure < 2; figure++) // начало программы - цикл, в котором задаётся значение двух играющих фигур
	{
		cout << "Inpute figure's name:\n";
		cin >> figures[figure].name; // вводится имя фигуры: k (конь), K (Король), Q (Ферзь), R (Ладья) или B (Слон)
		if (figureNameCheck(figures[figure].name) == -4) { cout << "Wrong figure name\n" << endl; continue; } // функция проверки корректности буквы введённой фигуры.
	// В случае ошибки, выводится соответствующее сообщение, и цикл начинается сначала, предлагая снова ввести название фигуры
		inputCoordinates(); // функция ввода координат фигуры
		if (cellOccupied() == -6) continue; // функция проверки, нет ли попытки поставить фигуру на занятое поле
		figurePositioning(); // функция выставления фигур на доску
		printBoard(); // функция отображения текущей доски
		cout << endl;
	}
}

void numberInput() // функция ввода номера фигуры для хода:
{
	bool flag = false;
	do
	{
		cout << "Input figure's number to make a step: \n";
		cin >> figure; // переменной figure задаётся значение 1 или 2
		if (figureExistence() == -1) continue; // функция проверки корректного номера фигуры
		if (turnCheck() == -2) continue; // функция проверки очередности хода
		figure--; // переменная figure уменьшается на единицу, чтобы с её помощью можно было обращаться к соответствющему элементу в массиве (фигура 1 расположена на 0,
		// фигура 2 - на 1
		enemy = (figure == 0) ? 1 : 0; // переменной enemy задаётся противоположное от текущей фигуры значение
		previousX = figures[figure].xLetter; // в символьную переменную previousX записывается текущее значение значение буквы горизонтальной координаты
		previousY = figures[figure].yInverted; // в целочисленную переменную previousY записывается текущее значение значение цифры вертикальной координаты
		// далее в служебные переменные пользовательской структуры в 3-ие значения массива записываются текущее название фигуры и её координаты:
		figures[2].name = figures[figure].name, figures[2].x = figures[figure].x, figures[2].y = figures[figure].y;
		flag = true;
	} while (flag == false);
}

int figureExistence() // функция проверки корректного номера фигуры
{
	if (figure < 1 || figure > 2) { cout << "Wrong figure number. \n" << endl; return -1; } // если значение отличается от 1 или 2, выводится ошибка, цикл повторяется
}

int turnCheck() // функция проверки очередности хода
{
	if (figure == enemy || figure == (enemy + 2)) 
	{ 
		cout << "It's not your turn. \n" << endl; 
		return -2;
	}
}

void inputCoordinates() // функция ввода координат фигуры
{
		cout << "Input figure's step: \n";
		cin >> figures[figure].xLetter >> figures[figure].yInverted; // вводится буквенная горизонтальная и цифровая вертикальная координаты
		figures[figure].x = xCoordinate(figures[figure].xLetter); // функция xCoordinate преобразует букву в линию горизонтальную массива, отображающую доску
		figures[figure].y = yCoordinate(figures[figure].yInverted); // функция yCoordinate преобразует цифру в линию вертикального массива, отображающую доску
}

int xCoordinate(char xLetter) // функция, преобразующая буквы горизонтальной координаты в линию горизонтальную массива, отображающую доску
{
	return xLetter - 'A' + 1;
}

int yCoordinate(int y) // функция, преобразующая цифру вертикальной координаты в линию вертикального массива, отображающую доску
{
	return 9 - y;
}

int figureStepCheck(char name) // функция проверки корректности хода для текущей фигуры
{
	switch (name) {
	case 'k':
		if (abs(figures[figure].x - figures[2].x) == 2 && abs(figures[figure].y - figures[2].y) == 1 ||
			abs(figures[figure].x - figures[2].x) == 1 && abs(figures[figure].y - figures[2].y) == 2)
		{
			// cout << "CASE KNIGHT OK" << endl; // Отладочное
			break;
		}
		
	case 'Q':
		if (abs(figures[figure].x - figures[2].x) == abs(figures[figure].y - figures[2].y) ||
			abs(figures[figure].x - figures[2].x) == 0 || abs(figures[figure].y - figures[2].y) == 0)
		{
			// cout << "CASE QUEEN OK" << endl; // Отладочное
			break;
		}
		
	case 'R':
		if (abs(figures[figure].x - figures[2].x) == 0 || abs(figures[figure].y - figures[2].y) == 0)
		{
			// cout << "CASE ROOK OK" << endl; // Отладочное
			break;
		}
		
	case 'B':
		if (abs(figures[figure].x - figures[2].x) == abs(figures[figure].y - figures[2].y))
		{
			// cout << "CASE BISHOP OK" << endl; // Отладочное
			break;
		}
		
	case 'K':
		if (abs(figures[figure].x - figures[2].x) <= 1 && abs(figures[figure].y - figures[2].y) <= 1)
		{
			// cout << "CASE KING OK" << endl; // Отладочное
			break;
		} 
		
	//case 'P': // кейс для пешки - в разработке
	//	if (abs(figures[figure].x - figures[2].x) == 0 && (figures[figure].y - figures[2].y) == 1)
	//	{
	//		cout << "CASE PAWN STEP OK" << endl; // Отладочное
	//		break;
	//	}
	//	else if (figures[figure].y == 2 && abs(figures[figure].x - figures[2].x) == 0 && (figures[figure].y - figures[2].y) == 2)
	//	{
	//		cout << "CASE PAWN long STEP OK" << endl; // Отладочное
	//		break;
	//	}
	//	else if (abs(figures[figure].x - figures[2].x) == 1 && (figures[figure].y - figures[2].y) == 1 && board[figures[2].y][figures[2].x] != ".")
	//	{
	//		cout << "CASE PAWN ATTACK OK" << endl; // Отладочное
	//		break;
	//	}
	//	else if (abs(figures[figure].x - figures[2].x) == 1 && (figures[figure].y - figures[2].y) == 1 && board[figures[2].y][figures[2].x] != ".")
	//	{
	//		cout << "CASE PAWN en passant ATTACK OK" << endl; // Отладочное
	//		break;
	//
	default:
		cout << "Wrong step for this figure!\n" << endl; return -5;
	}
}

int cellOccupied() // функция проверки, нет ли попытки поставить фигуру на занятое поле
{
	if (board[figures[0].y][figures[0].x] == board[figures[1].y][figures[1].x])
	{
		cout << "This cell is occupied by another figure!\n" << endl; return -6;
	}
}

int figureJumpCheck(int previousX, int previousY, int figuresX, int figuresY, int enemyX, int enemyY) // функция проверки хода через фигуру
{
	if (
		figures[figure].name != 'k' &&
		((previousY == enemyY && previousY == figuresY) && (abs(previousX - figuresX) > abs(previousX - enemyX)) && (previousX - figuresX) * (previousX - enemyX) > 0) || // проверка перескока по вертикали
		((previousX == enemyX && previousX == figuresX) && (abs(previousY - figuresY) > abs(previousY - enemyY)) && (previousY - figuresY) * (previousY - enemyY) > 0) || // проверка перескока по горизонтали
		((abs(previousX - figuresX) == abs(previousY - figuresY) && (abs(previousX - figuresX) > abs(previousX - enemyX)) && (abs(previousX - enemyX) == abs(previousY - enemyY)) &&
			((previousX - figuresX) * (previousX - enemyX) > 0) && ((previousY - figuresY) * (previousY - enemyY) > 0))) // проверка перескока по диагонали
		)
	{
		cout << "Figure can't jump over other one!\n" << endl;
		return -3;
	}
}

int outOfBoard(int x, int y) // функция проверки корректности координат
{
	if (x < 1 || x > 8 || y < 1 || y > 8) 
	{
		cout << "Step is out of board!\n" << endl; return -7; 
	}
}

int figureNameCheck(char name) // функция проверки корректности буквы введённой фигуры
{
	if (name != 'k' && name != 'K' && name != 'B' && name != 'Q' && name != 'R') return -4;
}

void step() // функция перемещения фигуры на поле
{
	board[figures[2].y][figures[2].x] = '.'; // на том месте, где была фигура, рисуется точка
	board[figures[figure].y][figures[figure].x] = figures[figure].name; // новые координаты фигуры отображаются на доске
}

void captureFact()  // функция проверки факта взятия текущей фигурой - фигуры противника
{
	if (board[figures[figure].y][figures[figure].x] == figures[enemy].name) // проверка факта взятия текущей фигурой - фигуры противника
	{ // это происходит в том случае, если по введённым координатам текущей фигуры, напечатано что-то, отличное от точки (другая фигура)
		// выводится сообщение о взятии вражеской фигуры, переменная enemy увеличивается на единицу:
		printf("\nFigure %d (%c) captures figure %d (%c).\n", figure + 1, figures[figure].name, enemy + 1, figures[enemy].name);
		figure = 3;
	}
}

void initializeBoard() // функция инициализации доски
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

	// заполняем строки с буквами: {" ","A","B","C","D","E","F","G","H"," "}
	for (int j = 0; j < columns; j++) {
		if (j == 0 || j == columns - 1) {
			board[0][j] = board[columns - 1][j] = ' ';
		}
		else {
			board[0][j] = board[columns - 1][j] = 'A' + (j - 1);
		}
	}
}

void figurePositioning() // функция выставления фигур на доску
{
	board[figures[0].y][figures[0].x] = figures[0].name;
	board[figures[1].y][figures[1].x] = figures[1].name;
}

void printBoard() // функция печати текущего состояния доски
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