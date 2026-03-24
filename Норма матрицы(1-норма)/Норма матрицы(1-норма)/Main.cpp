#include <iostream>
#include <windows.h>
#include <math.h>
using namespace std;
int main()
{
	int s, max, i, j, N, M, a[20][20];
	s = 0;
	max = 0;

	cout << "N=";
	cin >> N;
	cout << "M=";
	cin >> M;

	cout << "Input matrix A \n";
	for (i = 0; i < N; i++)
		for (j = 0; j < M; j++)
			cin >> a[i][j];

	for (j = 0; j < M; j++)
	{
		for (i = 0; i < N; i++)
			s += abs(a[i][j]);
		if (max < s)
			max = s;
		s = 0;
	}

	cout << "narma matrix A \n" << max << endl;
	system("pause");
	return 0;
}