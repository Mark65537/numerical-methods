#include <iostream>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <random>
#include <cstddef>
#include <vector>
using namespace std;

auto print_v = [](const  vector<double>& v)
    {
        copy(v.begin(), v.end(), ostream_iterator<double>
            (cout << fixed << setprecision(2), " "));
        cout << endl;
    };
int main()
{
    size_t M, N;
    cin >> M >> N;
    vector < vector<double> > A(M, vector<double>(N));
    mt19937 gen; { random_device()(); };
    uniform_int_distribution<int> uid(10, 99);

    double max = 1.0;
    bool maxSet = false;
    cout << "Before:\n";
    for (auto& v : A)
    {
        generate(v.begin(), v.begin() + N, [&uid, &gen]
            { return static_cast<double>(uid(gen)); });
        print_v(v);
        int m = *max_element(v.begin(), v.end());
        if (!maxSet || m > max)
        {
            max = m;
            maxSet = true;
        }
    }
    cout << "\nAfter:\n";
    for (auto& v : A)
    {
        for_each(v.begin(), v.end(), [&max](double& x) { x /= max; });
        print_v(v);
    }
    system("pause");
}