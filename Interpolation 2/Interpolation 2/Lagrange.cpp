#include "Functions.h"

//рассчёт полинома лагранжа
double polinom_lagrang(vector <long double>& X, vector<long double>& Y, double t) 
{
	long double result = 0.0;
	int N = X.size();
	for (short i = 0; i < N; i++)
	{
		long double P = 1.0;

		for (short j = 0; j < N; j++)
			if (j != i)
				P *= (t - X[j]) / (X[i] - X[j]);

		result += P * Y[i];
	}

	return result;
}

void Lagrange(int q)
{
	//все действия аналогичны описанным в сплайнах
	if (q == 1)
	{
		
		cout << "\nИнтерполяция с помощью полинома Лагранжа(задание 1)\n";

		ofstream L_5("Lagrang_5.txt");
		ofstream L_20("Lagrang_20.txt");
		int N1 = 5;
		int N2 = 20;
		int a = -1;
		int b = 1;
		int length = b - a;
		vector<long double> y;
		vector<long double> x;
		y.resize(N1);
		x.resize(N1);
		int U = 1000;
		for (int i = 0; i < N1; i++)
		{
			x[i] = a + (long double)i*length / (N1 - 1.);
			y[i] = First_Funct(x[i]);

		}

		long double x_1;
		long double y_1;
		long double y_2;

		for (int i = 0; i <= U; i++)
		{

			x_1 = (a + (long double)i*length / U);
			y_1 = First_Funct(x_1);
			y_2 = polinom_lagrang(x, y, x_1);
			L_5 << scientific << x_1 << '\t' << y_1 << '\t' << y_2 << '\n';
		}
		L_5.close();

		x_1 = a + (long double)length / (2.* (N1 - 1.));
		y_1 = First_Funct(x_1);
		y_2 = polinom_lagrang(x, y, x_1);
		cout << scientific << "\n\nЛевая средняя точка для N= " << N1 << ": " << x_1 << "\nЗначение функции: " << y_1 << "\nЗначение полинома: " << y_2 << endl;

		x_1 = b - length / (2.* (N1 - 1.));
		y_1 = First_Funct(x_1);
		y_2 = polinom_lagrang(x, y, x_1);
		cout << scientific << "\n\nПравая средняя точка для N= " << N1 << ": " << x_1 << "\nЗначение функции: " << y_1 << "\nЗначение полинома: " << y_2 << endl;

		x.clear();
		y.clear();
		y.resize(N2);
		x.resize(N2);
		for (int i = 0; i < N2; i++)
		{

			x[i] = a + (long double)i*length / (N2 - 1.);
			y[i] = First_Funct(x[i]);

		}

		for (int i = 0; i <= U; i++)
		{

			x_1 = (a + (long double)i*length / U);
			y_1 = First_Funct(x_1);
			y_2 = polinom_lagrang(x, y, x_1);
			L_20 << scientific << x_1 << '\t' << y_1 << '\t' << y_2 << '\n';
		}
		L_20.close();

		x_1 = a + length / (2.* (N2 - 1.));
		y_1 = First_Funct(x_1);
		y_2 = polinom_lagrang(x, y, x_1);
		cout << "\n\nЛевая средняя точка для N= " << N2 << ": " << x_1 << "\nЗначение функции: " << y_1 << "\nЗначение полинома: " << y_2 << endl;

		x_1 = b - length / (2.* (N2 - 1.));
		y_1 = First_Funct(x_1);
		y_2 = polinom_lagrang(x, y, x_1);
		cout << "\n\nПравая средняя точка для N= " << N2 << ": " << x_1 << "\nЗначение функции: " << y_1 << "\nЗначение полинома: " << y_2 << endl;
	}

	else
	{
		
		cout << "\nИнтерполяция с помощью полинома Лагранжа(задание 2)\n";
	
		int N = 20;
		ofstream L_20("Lagrang2_20.txt");

		int a = 0;
		int b = PI;
		int length = b - a;
		vector<long double> y;
		vector<long double> x;
		y.resize(N);
		x.resize(N);
		int U = 1000;
		for (int i = 0; i < N; i++)
		{
			x[i] = a + (long double)i*length / (N - 2);
			y[i] = Second_Funct(x[i]);

		}

		long double x_1;
		long double y_1;
		long double y_2;

		for (int i = 0; i <= U; i++)
		{

			x_1 = (a + (long double)i*length / U);
			y_1 = Second_Funct(x_1);
			y_2 = polinom_lagrang(x, y, x_1);
			L_20 << scientific << x_1 << '\t' << y_1 << '\t' << y_2 << '\n';
		}
		L_20.close();

		for (int i = 0; i < N; i++)
		{
			x_1 = a + (long double)i*length / (N - 2) + 0.5*(long double)length / (N - 2);
			y_1 = Second_Funct(x[i]);
			y_2 = polinom_lagrang(x, y, x[i]);
			cout << "\nCредняя точка: " << x_1 << "\nЗначение функции: " << y_1 << "\nЗначение полинома: " << y_2 << endl;
		}

	}
}