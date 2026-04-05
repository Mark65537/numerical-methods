#include <iostream>
#include <fstream>
#include <math.h>

#define M_PI 3.14159265358979323846

using namespace std;
double h = 0, E = 0.00001, m = 2, mm = 2, a = -1, b = M_PI, sum = 0, x = 0, aa = 0, aaa = 0, sum_old = 0,
rez, rez1, summ, sumn, summ_old;
int l = 1;

double f(double x)
{
	return (x * exp(-x));
}

void Simps() {

	aa = a;
	do {
		sum_old = sum;
		sum = 0;
		m = pow(m, l);
		h = (b - a) / m;
		//cout<<"Шаг="<<h<<endl;

		for (int i = 0; i <= m; i++) {

			if (i == 0) {
				x = a;
			}
			else {
				x = a + h;
			}a = x;

			sum += f(x) * ((i == 0 || i == m) ? 1 : ((i & 1) == 0) ? 2 : 4);
		}

		sum *= h / 3;
		//cout<<"Cимпсон="<<sum<<endl;

		l++;
		a = aa;

		rez = fabs(sum - sum_old);
		//cout<<"rez="<<rez<<endl;

	} while (rez > E);

	cout << "Ответ: " << sum << endl;
	cout << endl;
	cout << endl;
}



void Trap() {
	aa = a;

	do {

		summ_old = summ;
		summ = 0;
		sumn = 0;

		h = (b - a) / mm;

		//cout<<"Шаг="<<h<<endl;

		for (int i = 0; i <= mm; i++) {
			if (i == 0) {
				x = a;
			}
			else {
				x = a + h;
			}a = x;
			if (i != 0 && i != mm) {
				summ += f(x);
			}
			else {
				sumn += f(x);
			}
		}
		summ += (sumn / 2);
		summ *= h;

		//cout<<"Трапец="<<summ<<endl;

		mm = mm * 2;
		a = aa;

		rez1 = fabs(summ - summ_old);
		//cout<<"rez="<<rez1<<endl;

	} while (rez1 > E);

	cout << "Ответ: " << summ << endl;
	cout << endl;
	cout << endl;
}

void main()
{
	setlocale(LC_ALL, "Russian");

	cout << "Интегрируется функция: f(x) = x * e^(-x)." << endl << endl;

	cout << "Метод Симпсона: " << endl;
	Simps();

	cout << "Метод Трапеций: " << endl;
	Trap();
}