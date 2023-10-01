#include <iostream>
#include <vector>
using namespace std;


long long gcd(long long a, long long b)
{
    int tmp;
    while (b > 0)
    {
        tmp = a;
        a = b;
        b = tmp % b;
    }
    return a;
}


vector <long long> gcd_ext(long long a, long long b)
{
    long long d, x, y, tmp;
    vector <long long> res;
    if (b == 0)
    {
        res = { a, 1, 0 };
        return res;
    }
    res = gcd_ext(b, a % b);
    d = res[0];
    x = res[1];
    y = res[2];
    tmp = x;
    x = y;
    y = tmp - (a / b) * y;
    res = { d, x, y };
    return res;
}


int main()
{
    setlocale(LC_ALL, "RU");

    long long a, b, c, d, x0, y0;
    cin >> a >> b >> c;
    if (c % gcd(a, b) != 0)
    {
        cout << "Решений нет." << endl;
    }
    else
    {
        vector <long long> res = gcd_ext(a, b);
        d = res[0];
        x0 = res[1];
        y0 = res[2];
        cout << "x = " << (c * x0) / d << " + (" << b / d << " * t)" << endl;
        cout << "y = " << (c * y0) / d << " - (" << a / d << " * t)" << endl;
    }
}