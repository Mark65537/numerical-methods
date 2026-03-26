#pragma once

#include "Functions.h"

//Описываем наш класс, организующий сплайн
class cubic_spline
{
private:
	// Структура, описывающая сплайн на каждом сегменте сетки
	struct spline_tuple
	{
		double a, b, c, d, x;
	};

	spline_tuple *splines; // Сплайн
	size_t n; // Количество узлов сетки

	void free_mem(); // Освобождение памяти

public:
	cubic_spline(); //конструктор
	~cubic_spline(); //деструктор

					 // Построение сплайна
					 // x - узлы сетки
					 // y - значения функции в узлах сетки
					 // n - количество узлов сетки
	void build_spline(vector<long double>& x, vector<long double>& y, size_t n);

	// Вычисление значения интерполированной функции в произвольной точке
	double f(double x) const;
};