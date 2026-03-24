#include <cstdlib>
#include <iostream>
#include "fstream"
#include "windows.h"
#include <math.h>
#include <iomanip>

using namespace std;

float c, s;
const int n = 5;
const int m = n + 1;
int stroka_1 = 0,
stroka_A = 1,
endd = n,
k;
float x[m],
r[m],
mtrx[n][m],
mtrx2[n][m];

// Считывает расширенную матрицу из файла и печатает её на экран.
void zapis()
{
	ifstream fin("mtx.txt");
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			fin >> mtrx[i][j];

	for (int i = 0; i < n; i++)
	{
		cout << endl;
		for (int j = 0; j < m; j++)
			cout << " " << mtrx[i][j];
	}
	cout << endl;
	fin.close();
}

// Вычисляет коэффициент поворота (cos/sin) для метода вращений.
float cs(float m1, float m2, float m3)
{
	return (m1 / sqrt(pow(m2, 2) + pow(m3, 2)));
}


//void vivod(int n,int m,float mtrx2[10][10])
//{
//for (int i = 1; i <n; i++)
//	{
//		cout << endl;
//		for (int j = 1; j <m; j++)
//			cout << " " << mtrx2[i][j];
//	}
//}

// Выполняет одно вращение двух строк и обновляет рабочую матрицу.
void reshenie(int stroka_1, int stroka_A)
{
	c = cs(mtrx2[stroka_1][stroka_1], mtrx2[stroka_1][stroka_1], mtrx2[stroka_A][stroka_1]);
	s = cs(mtrx2[stroka_A][stroka_1], mtrx2[stroka_1][stroka_1], mtrx2[stroka_A][stroka_1]);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (i == stroka_1)
			{
				mtrx2[stroka_1][j] = c * mtrx[stroka_1][j] + s * mtrx[stroka_A][j];
			}
			else if (i == stroka_A)
			{
				mtrx2[stroka_A][j] = c * mtrx[stroka_A][j] - s * mtrx[stroka_1][j];
			}
			else
				break;
		}
	}
	std::memcpy(mtrx, mtrx2, sizeof(float) * (n * m));
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	for (int i = 0; i < n; i++)
	{
		x[i] = 0;
	}
	x[n] = 1;
	cout << "до:\n";
	zapis();
	std::memcpy(mtrx2, mtrx, sizeof(float) * (n * m));

	// ПРЕОБРАЗОВАНИЕ МАТРИЦЫ
	for (int i = 0; i < n; i++)
	{
		for (int j = i; j <= n; j++)
			reshenie(i, j + 1);
	}

	//ВЫВОД МАТРИЦЫ
	cout << "последнии с и s:\n";
	cout << c << " " << s << endl;
	cout << "\nпосле:\n";
	for (int i = 0; i < n; i++)
	{
		cout << endl;
		for (int j = 0; j < m; j++)
			cout << " " << setiosflags(ios::fixed) << setprecision(4) << mtrx2[i][j];
	}
	//ВЫЧИСЛЕНИЯ ОТВЕТА
	cout << "\nroots:\n";
	for (int i = n - 1; i >= 0; i--)
	{
		for (int j = 0; j < m; j++)
		{
			if (mtrx2[i][j] == mtrx2[i][i])
				continue;
			if (j == (m - 1)) k = -1;
			else  k = 1;
			x[i] += ((mtrx2[i][j] * x[j]) * k) / (-mtrx2[i][i]);

		}
		cout << setiosflags(ios::fixed) << setprecision(4) << x[i] << endl;
	}
	//ВЫЧИСЛЕНИЯ вектора невязки
	cout << "\nвектора невязки:\n";
	for (int i = n - 1; i >= 0; i--)
	{
		for (int j = 0; j < m; j++)
		{
			if (j == (m - 1)) k = -1;
			else  k = 1;
			r[i] += ((mtrx2[i][j] * x[j]) * k);

		}
		cout << setiosflags(ios::fixed) << setprecision(12) << r[i] << endl;
	}
	//x[2]=mtrx2[2][3]/mtrx2[2][2];
	//x[1]=(mtrx2[1][3]-mtrx2[1][2]*x[2])/mtrx2[1][1];
 //   x[0]=(mtrx2[0][1]*x[1]+mtrx2[0][2]*x[2]-mtrx2[0][3])/(-mtrx2[0][0]);
	cout << endl;
	system("pause");
	return 0;
}
