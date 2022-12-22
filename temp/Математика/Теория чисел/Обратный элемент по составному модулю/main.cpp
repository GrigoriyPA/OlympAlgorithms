#include <iostream>
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


long long bem(long long base, long long degree, long long p)
{
    long long tmp;
    if (degree == 0)
    {
        return 1;
    }
    if (degree % 2 == 0)
    {
        tmp = bem(base % p, degree / 2, p);
        return (tmp * tmp) % p;
    }
    else
    {
        return (bem(base % p, degree - 1, p) * (base % p)) % p;
    }
}


long long phi(long long a)
{
    long long res = a, d = 2;
    while (d * d <= a)
    {
        if (a % d == 0)
        {
            res -= res / d;
        }
        while (a % d == 0)
        {
            a /= d;
        }
        d++;
    }
    if (a > 1)
    {
        res -= res / a;
    }
    return res;
}


long long get_opposite(long long a, long long p)
{
    a %= p;
    if (gcd(a, p) != 1)
    {
        return -1;
    }
    else
    {
        return bem(a, phi(p) - 1, p);
    }
}


int main()
{
    setlocale(LC_ALL, "RU");

    long long a, p;
    cin >> a >> p;
    long long opposite = get_opposite(a, p);
    if (opposite == -1)
    {
        cout << "ƒл€ числа " << a << " не существует обратного числа по модулю " << p << "." << endl;
    }
    else
    {
        cout << opposite << endl;
    }
}