#include "Functions.h"
#include "C_Spline.h"

//выполнение задания для сплайнов

void Main_Spline(int q)//q - это первое задание или второе
{
	if (q == 1)
	{
		cout << "\nИнтерполяция с помощью кубического сплайна(задание 1)\n";
		ofstream S_5("Spline_5.txt");//файл, в котором будет значения интерполяции, с сеткой в 1000 точек и 5 узлов интерполяции, и самой функции
		ofstream S_20("Spline_20.txt");//файл, в котором будет значения интерполяции, с сеткой в 1000 точек и 20 узлов интерполяции и самой функции
		int N1 = 5;//кол-во узлов
		int N2 = 20;//кол-во узлов
		int a = -1;//граница отрезка рассчёта интерполяции
		int b = 1;//граница отрезка рассчёта интерполяции
		int length = b - a;
		vector<long double> y;
		vector<long double> x;
		y.resize(N1);
		x.resize(N1);
		int U = 1000;//размерность сетки пространства, где рассчитываются значения
		for (int i = 0; i < N1; i++)
		{
			x[i] = a + (long double)i*length / (N1 - 1.);//разбиваем отрезок на N1 равных частей
			y[i] = First_Funct(x[i]);//узнаём значение функции в узле

		}

		cubic_spline Sp;//создали объект класса, который будет описывать наш сплайн
		Sp.build_spline(x, y, N1);//строим сплайн, вся реализация находится в классе
		long double x_1;
		long double y_1;
		long double y_2;

		//поиск координат, значения функции и сплайна в сетке пространства
		for (int i = 0; i <= U; i++)
		{

			x_1 = (a + (long double)i*length / U);
			y_1 = First_Funct(x_1);
			y_2 = Sp.f(x_1);
			S_5 << scientific << x_1 << '\t' << y_1 << '\t' << y_2 << '\n';//вывод полученных результатов в файл
		}
		S_5.close();

		x_1 = a + (long double)length / (2.* (N1 - 2));
		y_1 = First_Funct(x_1);
		y_2 = Sp.f(x_1);
		cout << scientific << "\n\nЛевая средняя точка для N= " << N1 << ": " << x_1 << "\nЗначение функции: " << y_1 << "\nЗначение сплайна: " << y_2;

		x_1 = b - length / (2.* (N1 - 2));
		y_1 = First_Funct(x_1);
		y_2 = Sp.f(x_1);
		cout << scientific << "\n\nПравая средняя точка для N= " << N1 << ": " << x_1 << "\nЗначение функции: " << y_1 << "\nЗначение сплайна: " << y_2 << endl;

		x.clear();
		y.clear();
		y.resize(N2);
		x.resize(N2);
		//абсолютно все теже действия делаем, только для кол-ва узлов 20
		//используем всё тот же объект Sp класса, только переопределяем его
		for (int i = 0; i < N2; i++)
		{

			x[i] = a + (long double)i*length / (N2 - 1.);
			y[i] = First_Funct(x[i]);

		}

		Sp.build_spline(x, y, N2);

		for (int i = 0; i <= U; i++)
		{

			x_1 = (a + (long double)i*length / U);
			y_1 = First_Funct(x_1);
			y_2 = Sp.f(x_1);
			S_20 << scientific << x_1 << '\t' << y_1 << '\t' << y_2 << '\n';
		}
		S_20.close();

		x_1 = a + length / (2.* (N2 - 1.));
		y_1 = First_Funct(x_1);
		y_2 = Sp.f(x_1);
		cout << scientific << "\n\nЛевая средняя точка для N= " << N2 << ": " << x_1 << "\nЗначение функции: " << y_1 << "\nЗначение сплайна: " << y_2 << endl;

		x_1 = b - length / (2.* (N2 - 1.));
		y_1 = First_Funct(x_1);
		y_2 = Sp.f(x_1);
		cout << scientific << "\n\nПравая средняя точка для N= " << N2 << ": " << x_1 << "\nЗначение функции: " << y_1 << "\nЗначение сплайна: " << y_2 << endl;
	}

	else
	{
		
		cout << "\nИнтерполяция с помощью кубического сплайна(задание 2)\n";

		//всё аналогично предыдущему

		int N = 20;
		ofstream S_20("Spline2_20.txt");

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
			x[i] = a + (long double)i*length / (N - 1.);
			y[i] = Second_Funct(x[i]);

		}

		cubic_spline Sp;
		Sp.build_spline(x, y, N);
		long double x_1;
		long double y_1;
		long double y_2;

		for (int i = 0; i <= U; i++)
		{

			x_1 = (a + (long double)i*length / U);
			y_1 = Second_Funct(x_1);
			y_2 = Sp.f(x_1);
			S_20 << scientific << x_1 << '\t' << y_1 << '\t' << y_2 << '\n';
		}
		S_20.close();


		for (int i = 0; i < N; i++)
		{
			x_1 = a + 0.5*(long double)i*length / (N - 1.) + 0.5*(long double)length / (N - 1.);;
			y_1 = Second_Funct(x[i]);
			y_2 = Sp.f(x[i]);
			cout << scientific << "\nCредняя точка: " << x_1 << "\nЗначение функции: " << y_1 << "\nЗначение сплайна: " << y_2 << endl;
		}

	}
}

//В ИТОГЕ МЫ ИМЕЕМ М ТРЁХ ФАЙЛАХ ДАННЫЕ ДЛЯ ПОСТРОЕНИЯ ГРАФИКОВ, НАПРИМЕР В Excel