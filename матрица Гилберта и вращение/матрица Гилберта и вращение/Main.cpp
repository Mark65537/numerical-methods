#include <iostream>//a[i,j]=1/(i+j-1)
#include "Вращение.h"
using namespace std;

// Возвращает оценку нормы матрицы по строкам: max_i |sum_j A[i][j]|.
// slay: квадратная матрица k?k.
// k: размер матрицы.
double norma(double** slay, int k)
{
	double* buf = new double[k], buf1 = 0;;
	for (int i = 0; i < k; i++)
		buf[i] = 0;
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < k; j++)
			buf[i] += slay[i][j];
	}
	for (int i = 0; i < k; i++)
	{

		if (abs(buf[i]) >= buf1)
			buf1 = abs(buf[i]);
	}
	return buf1;
}



// Заполняет матрицу Гилберта H размера k?k: H[i][j] = 1 / (i + j + 1).
// slay: выделенная матрица k?k, заполняется на месте.
// k: размер матрицы.
void scan_1(double** slay, int k)
{
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < k; j++)
		{
			double z = (i + j + 1);
			slay[i][j] = 1 / z;
			//cout << z << ' ';
		}
		cout << endl;
	}
}

// Печатает матрицу k?k в консоль с фиксированной точностью.
// slay: матрица k?k.
// k: размер матрицы.
void print(double** slay, int k)
{
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < k; j++)
			cout << fixed << setprecision(4) << slay[i][j] << ' ';
		cout << endl;
	}
	cout << endl;
}

// Строит обратную матрицу A^{-1} для slay (A) через решение k СЛАУ вида A x = e_u.
// slay: исходная матрица A (k?k), используется как коэффициенты.
// k: размер матрицы.
// slay1: выходная матрица A^{-1} (k?k), заполняется по столбцам.
void obrmatr(double** slay, int k, double** slay1)
{

	double** E = new double* [k];
	for (int i = 0; i < k; i++)
	{
		E[i] = new double[k];
		for (int j = 0; j < k; j++)
		{
			if (i != j)
				E[i][j] = 0;
			else
				E[i][i] = 1;
		}
	}
	cout << "";
	double** buf = new double* [k];
	for (int i = 0; i < k; i++)
		buf[i] = new double[k + 1];
	int u = 0;
	for (int i = 0; i < k; i++)
	{
		cout << endl;
		for (int j = 0; j < k; j++)
		{
			for (int m = 0; m < k; m++)
				buf[j][m] = slay[j][m];
			buf[j][k] = E[u][j];
		}
		alg(buf, k);
		//print(buf, k);
		for (int j = 0; j < k; j++)
			slay1[j][u] = buf[j][k];

		u++;
	}
	print(slay1, k);
	double** buf1 = new double* [k];
	for (int i = 0; i < k; i++)
	{
		buf1[i] = new double[k];
		buf[i] = 0;
	}
	for (int i = 0; i < k; i++)
		for (int j = 0; j < k; j++) {
			buf1[i][j] = 0;
			for (int l = 0; l < k; l++)
				buf1[i][j] += slay[i][l] * slay1[l][j];
		}print(buf1, k);
}

// Точка входа: формирует матрицу Гилберта, находит её обратную, оценивает нормы и число обусловленности.
int main()
{
	setlocale(0, "");
	int k = 10;
	//матрица Гилберта
	double** slay = new double* [k];
	for (int i = 0; i < k; i++)
		slay[i] = new double[k];

	//обратная матрица Гилберта
	double** slay1 = new double* [k];
	for (int i = 0; i < k; i++)
		slay1[i] = new double[k];
	scan_1(slay, k);
	print(slay, k);
	double n = norma(slay, k);
	cout << "норма матрицы Гилберта " << n << endl;
	obrmatr(slay, k, slay1);
	double n1 = norma(slay1, k);

	//число обусловленности q
	double q = n1 * n;
	cout << "число обусловленности матрицы Гилберта " << q << endl;
	system("pause");
	return 0;
}