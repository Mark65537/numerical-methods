#include <iostream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <vector>

using namespace std;

// Печать всей матрицы
void printMatrix(const vector<vector<double>>& matrix)
{
    for (const auto& row : matrix)
    {
        for (double x : row)
            cout << fixed << setprecision(2) << x << " ";

        cout << '\n';
    }
}

// Генерация матрицы случайными числами
vector<vector<double>> generateMatrix(size_t M, size_t N)
{
    // Инициализация генератора случайных чисел
    mt19937 gen(random_device{}());

    vector<vector<double>> matrix(M, vector<double>(N));

    uniform_int_distribution<int> dist(10, 99);

    for (auto& row : matrix)
    {
        // Заполняем строку случайными числами
        generate(row.begin(), row.end(), [&]() {
            return static_cast<double>(dist(gen));
            });
    }

    return matrix;
}

// Поиск максимального элемента во всей матрице
double findMax(const vector<vector<double>>& matrix)
{
    double maxVal = 0.0;

    for (const auto& row : matrix)
    {
        // Ищем максимум в строке и сравниваем с глобальным
        maxVal = max(maxVal, *max_element(row.begin(), row.end()));
    }

    return maxVal;
}

// Нормализация матрицы (деление на максимум)
void normalizeMatrix(vector<vector<double>>& matrix, double maxVal)
{
    for (auto& row : matrix)
    {
        for (double& x : row)
        {
            x /= maxVal;
        }
    }
}

int main()
{
    size_t M, N;

    // Подсказка пользователю
    cout << "Enter number of rows (M) and columns (N): ";
    cin >> M >> N;

    // Генерация матрицы
    auto matrix = generateMatrix(M, N);

    cout << "\nBefore:\n";
    printMatrix(matrix);

    // Поиск максимума
    double maxVal = findMax(matrix);

    // Нормализация
    normalizeMatrix(matrix, maxVal);

    cout << "\nAfter (normalized):\n";
    printMatrix(matrix);

    return 0;
}