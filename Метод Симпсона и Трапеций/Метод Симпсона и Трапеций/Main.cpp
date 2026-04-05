#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

// Задаём отрезок по оси X: считаем «площадь под графиком» f(x) от xLeft до xRight
// (в математике это называется определённым интегралом — здесь достаточно думать о площади).
const double xLeft = -1, xRight = M_PI;

// Останавливаемся, когда два подряд приближения отличаются меньше чем на kEpsilon
// (идея как в численных методах: «сошлось» — дальше смысла мало).
constexpr double kEpsilon = 1e-5;

// Замени тело f(x), если нужна другая кривая; остальной код трогать не обязательно.
/**
 * Подынтегральная функция f(x) = x * e^(-x).
 * \param x аргумент
 * \return значение f(x)
 */
double f(double x) {
	return x * exp(-x);
}

/**
 * Составное правило Симпсона; на каждом шаге число отрезков растёт как n := n^level (как в исходной программе).
 * Итерации до достижения точности kEpsilon.
 * \param xLeft левый конец отрезка интегрирования
 * \param xRight правый конец отрезка интегрирования
 * \return приближённое значение интеграла
 */
double integrateSimpson(double xLeft, double xRight) {
	double nIntervals = 2;
	int level = 1;
	double integralPrevious = 0;
	double integralCurrent = 0;

	do {
		integralPrevious = integralCurrent;
		integralCurrent = 0;
		// Увеличиваем число мелких отрезков на оси X (больше точек — обычно точнее оценка).
		// Формула n := n^level — особенность этой учебной версии, не «стандартное» удвоение сетки.
		nIntervals = pow(nIntervals, level);
		++level;

		const double stepSize = (xRight - xLeft) / nIntervals; // шаг между соседними x
		const int numSegments = static_cast<int>(nIntervals + 0.5); // целое число отрезков (округление от double)

		// Обходим равномерную сетку точек от левого до правого конца включительно.
		for (int i = 0; i <= numSegments; ++i) {
			const double xNode = xLeft + i * stepSize;
			// Взвешенная сумма: крайние точки с весом 1, через одну — 4, остальные чётные — 2 (правило Симпсона).
			// фиксированный шаблон коэффициентов 1,4,2,4,...,2,4,1 даёт оценку «площади».
			const int simpsonWeight = (i == 0 || i == numSegments) ? 1 : (i % 2 == 0 ? 2 : 4);
			integralCurrent += simpsonWeight * f(xNode);
		}
		integralCurrent *= stepSize / 3.0; // общий множитель из формулы Симпсона
	} while (abs(integralCurrent - integralPrevious) > kEpsilon);

	return integralCurrent;
}

/**
 * Метод трапеций с удвоением числа отрезков до сходимости по критерию kEpsilon.
 * \param xLeft левый конец отрезка интегрирования
 * \param xRight правый конец отрезка интегрирования
 * \return приближённое значение интеграла
 */
double integrateTrapezoidal(double xLeft, double xRight) {
	double nSeg = 2;
	double integralPrevious = 0;
	double integralCurrent = 0;

	do {
		integralPrevious = integralCurrent;
		const int numSegments = static_cast<int>(nSeg);
		const double stepSize = (xRight - xLeft) / numSegments;

		// Идея: разбить [xLeft, xRight] на numSegments отрезков одинаковой длины;
		// на каждом отрезке график заменяют прямой (трапеция), площади складывают.
		// Эквивалентная запись через узлы: половины значений на концах + все внутренние целиком.
		double trapezoidNodeSum = 0.5 * (f(xLeft) + f(xRight));
		for (int i = 1; i < numSegments; ++i)
			trapezoidNodeSum += f(xLeft + i * stepSize);
		integralCurrent = trapezoidNodeSum * stepSize;

		nSeg *= 2; // следующая итерация — в два раза больше отрезков (мельче сетка)
	} while (abs(integralCurrent - integralPrevious) > kEpsilon);

	return integralCurrent;
}

int main() {
	setlocale(0, "");

	cout << "Интегрируется функция: f(x) = x * e^(-x)." << endl << endl;
	cout << "Интеграл от " << xLeft << " до " << xRight << endl << endl;

	cout << "Метод Симпсона: " << endl;
	cout << "Ответ: " << integrateSimpson(xLeft, xRight) << endl << endl;

	cout << "Метод Трапеций: " << endl;
	cout << "Ответ: " << integrateTrapezoidal(xLeft, xRight) << endl << endl;

	return 0;
}
