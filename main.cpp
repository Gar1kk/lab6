////Ветка feature-logic-georgiy: логика перемещения плиток (Путин Георгий)

#include <iostream>
#include <iomanip>  // для красивого вывода
#include <cstdlib>  // rand, srand
#include <ctime>    // time

using namespace std;

const int SIZE = 4; // размер поля 4x4

void initBoard(int board[SIZE][SIZE]) {
    int value = 1;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (i == SIZE - 1 && j == SIZE - 1) {
                board[i][j] = 0; // пустая клетка
            } else {
                board[i][j] = value;
                value++;
            }
        }
    }
}

void printBoard(int board[SIZE][SIZE]) {
    cout << "\nТекущее состояние поля:\n";
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                cout << setw(3) << "."; // пустая клетка
            } else {
                cout << setw(3) << board[i][j];
            }
        }
        cout << "\n";
    }
}

// находим позицию пустой клетки (0)
void findEmpty(int board[SIZE][SIZE], int &emptyRow, int &emptyCol) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                emptyRow = i;
                emptyCol = j;
                return;
            }
        }
    }
}

// перемешивание: делаем много случайных допустимых ходов из решённого состояния
void shuffleBoard(int board[SIZE][SIZE]) {
    int emptyRow, emptyCol;
    findEmpty(board, emptyRow, emptyCol);

    int movesCount = 1000; // количество случайных ходов

    int dr[4] = {-1, 1, 0, 0}; // смещения по строкам
    int dc[4] = {0, 0, -1, 1}; // смещения по столбцам

    for (int k = 0; k < movesCount; k++) {
        int direction = rand() % 4; // случайное направление
        int newRow = emptyRow + dr[direction];
        int newCol = emptyCol + dc[direction];

        if (newRow >= 0 && newRow < SIZE && newCol >= 0 && newCol < SIZE) {
            // меняем местами пустую клетку и соседнюю
            int temp = board[emptyRow][emptyCol];
            board[emptyRow][emptyCol] = board[newRow][newCol];
            board[newRow][newCol] = temp;

            emptyRow = newRow;
            emptyCol = newCol;
        }
    }
}

// проверка, собрана ли головоломка
bool isSolved(int board[SIZE][SIZE]) {
    int value = 1;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (i == SIZE - 1 && j == SIZE - 1) {
                if (board[i][j] != 0) return false;
            } else {
                if (board[i][j] != value) return false;
                value++;
            }
        }
    }
    return true;
}

// попытка сдвинуть выбранную плитку (tile) в сторону пустой клетки
bool moveTile(int board[SIZE][SIZE], int tile) {
    int tileRow = -1, tileCol = -1;
    int emptyRow = -1, emptyCol = -1;

    // ищем плитку и пустую клетку
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == tile) {
                tileRow = i;
                tileCol = j;
            }
            if (board[i][j] == 0) {
                emptyRow = i;
                emptyCol = j;
            }
        }
    }

    if (tileRow == -1) return false;

    int dr = tileRow - emptyRow;
    int dc = tileCol - emptyCol;

    // плитка должна быть соседней по вертикали или горизонтали
    if ((dr == 1 && dc == 0) || (dr == -1 && dc == 0) ||
        (dr == 0 && dc == 1) || (dr == 0 && dc == -1)) {

        int temp = board[emptyRow][emptyCol];
        board[emptyRow][emptyCol] = board[tileRow][tileCol];
        board[tileRow][tileCol] = temp;
        return true;
    }

    return false;
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned int>(time(0)));

    int board[SIZE][SIZE];
    initBoard(board);
    shuffleBoard(board);

    int moves = 0;

    cout << "Числовая головоломка '15'\n";
    cout << "Соберите числа от 1 до 15, пустая клетка обозначена точкой.\n";
    cout << "Введите число плитки, которую хотите сдвинуть (0 — выход).\n";

    while (true) {
        printBoard(board);

        if (isSolved(board)) {
            cout << "\nПоздравляем! Вы решили головоломку за " << moves << " ход(ов).\n";
            break;
        }

        cout << "\nВаш ход (номер плитки или 0 для выхода): ";
        int tile;
        cin >> tile;

        if (!cin) {
            cout << "Ошибка ввода. Завершение игры.\n";
            break;
        }

        if (tile == 0) {
            cout << "Вы вышли из игры.\n";
            break;
        }

        if (tile < 1 || tile > 15) {
            cout << "Введите число от 1 до 15.\n";
            continue;
        }

        bool ok = moveTile(board, tile);
        if (!ok) {
            cout << "Эту плитку нельзя сдвинуть, она не рядом с пустой клеткой.\n";
        } else {
            moves++;
        }
    }

    return 0;
}
