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


long long sum(long long a, long long b, long long p)
{
    a %= p;
    b %= p;
    return (a + b) % p;
}


long long sub(long long a, long long b, long long p)
{
    a %= p;
    b %= p;
    return (a - b) % p;
}


long long mul(long long a, long long b, long long p)
{
    a %= p;
    b %= p;
    return (a * b) % p;
}


long long division(long long a, long long b, long long p)
{
    b = get_opposite(b, p);
    if (b == -1)
    {
        return -1;
    }
    else
    {
        return mul(a, b, p);
    }
}


int main()
{
    long long a, b, p;
    cin >> a >> b >> p;
    cout << "(a + b) % p: " << sum(a, b, p) << endl;
    cout << "(a - b) % p: " << sub(a, b, p) << endl;
    cout << "(a * b) % p: " << mul(a, b, p) << endl;
    cout << "(a / b) % p: " << division(a, b, p) << endl;
    cout << "(a ^ b) % p: " << bem(a, b, p) << endl;
}