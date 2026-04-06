#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

// Считает количество строк в файле `a1.txt` (использовалось для определения размерности).
// Возвращает: число считанных строк (k).
int scan1()
{
	int k = 0;
	string buf;
	ifstream f;
	f.open("a1.txt");
	if (f)
	{
		while (!f.eof())
		{
			getline(f, buf);
			k++;
		}
		f.seekg(0, ios::beg);
		f.close();
	}
	return k;
}

// Считывает матрицу k×k из файла `a1.txt` в slay.
// slay: выделенная матрица k×k, заполняется на месте.
// k: размер матрицы.
void scan2(double** slay, int k)
{
	string buf;
	ifstream f;
	f.open("a1.txt");
	while (!f.eof())
	{

		for (int i = 0; i < k; i++)
			for (int j = 0; j < k; j++)
				f >> slay[i][j];
	}

	f.close();
}

// Нормирует строку расширенной матрицы (делит на ведущий элемент).
// slay: строка длины k+1 (k коэффициентов + правая часть).
// k: размер системы (без правой части).
// buf1: ведущий элемент, на который выполняется деление.
void alg_1(double* slay, int k, double buf1)
{
	for (int j = 0; j < k + 1; j++)
	{
		slay[j] /= buf1;
	}
}

// Выполняет обратный ход после приведения к верхнетреугольному виду:
// нормирует диагональные элементы и зануляет элементы над диагональю.
// slay: расширенная матрица k×(k+1), модифицируется на месте.
// k: размер системы.
void alg_2(double** slay, int k)
{
	double* buf = new double[k + 1], buf1, * buf2 = new double[k + 1];
	int u;
	for (int i = k - 1; i >= 0; i--)
	{
		buf1 = slay[i][i];
		alg_1(slay[i], k, buf1);
		u = 0;
		for (int m = i; m > 0; m--)
		{
			u++;
			for (int j = k + 1; j > 0; j--)
			{
				buf[j] = slay[i][j];
				buf[j] *= slay[i - u][i];
				slay[i - u][j] -= buf[j];
			}
			//print(slay,k);
		}
	}
}

// Решает СЛАУ методом вращений Гивенса для расширенной матрицы k×(k+1).
// На выходе правая часть (последний столбец) содержит решение после обратного хода (`alg_2`).
// slay: расширенная матрица [A|b], модифицируется на месте.
// k: размер системы.
void alg(double** slay, int k)
{
	double* buf = new double[k + 1], buf1, * buf2 = new double[k + 1], c, s;
	int u;
	for (int i = 0; i < k - 1; i++)
	{
		buf1 = slay[i][i];
		u = 0;
		for (int m = i; m < k - 1; m++)
		{
			u++;
			c = slay[i][i] / (sqrt(slay[i][i] * slay[i][i] + slay[i + u][i] * slay[i + u][i]));
			s = slay[i + u][i] / (sqrt(slay[i][i] * slay[i][i] + slay[i + u][i] * slay[i + u][i]));
			//cout << c << " " << s << endl;
			for (int j = 0; j < k + 1; j++)
			{
				buf[j] = slay[i][j] * c + slay[i + u][j] * s;
				buf2[j] = slay[i + u][j] * c - slay[i][j] * s;
			}
			for (int j = 0; j < k + 1; j++)
			{
				if (j == i) slay[i + u][i] = 0;
				slay[i][j] = buf[j];
				slay[i + u][j] = buf2[j];

			}
			//print(slay,k);
		}
	}
	alg_2(slay, k);
}