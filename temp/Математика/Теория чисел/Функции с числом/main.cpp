#include <iostream>
#include <vector>
using namespace std;


long long be(long long base, long long degree)
{
    long long tmp;
    if (degree == 0)
    {
        return 1;
    }
    if (degree % 2 == 0)
    {
        tmp = be(base, degree / 2);
        return tmp * tmp;
    }
    else
    {
        return be(base, degree - 1) * base;
    }
}


bool check_prime(long long a)
{
    long long d = 2;
    while (d * d <= a)
    {
        if (a % d == 0)
        {
            return false;
        }
        d++;
    }
    return true;
}


vector < pair <long long, long long> > factorization(long long a)
{
    vector < pair <long long, long long> > res;
    long long d = 2;
    while (d * d <= a)
    {
        if (a % d == 0)
        {
            res.push_back(make_pair(d, 0));
            while (a % d == 0)
            {
                res[res.size() - 1].second++;
                a /= d;
            }
        }
        d++;
    }
    if (a > 1)
    {
        res.push_back(make_pair(a, 1));
    }
    return res;
}


long long sgm0(long long a)
{
    vector < pair <long long, long long> > dividers = factorization(a);
    long long res = 1;
    for (int i = 0; i < dividers.size(); i++)
    {
        res *= dividers[i].second + 1;
    }
    return res;
}


long long sgm1(long long a)
{
    vector < pair <long long, long long> > dividers = factorization(a);
    long long res = 1, sum;
    for (long long i = 0; i < dividers.size(); i++)
    {
        sum = 0;
        for (long long j = 0; j <= dividers[i].second; j++)
        {
            sum += be(dividers[i].first, j);
        }
        res *= sum;
    }
    return res;
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


int main()
{
    setlocale(LC_ALL, "RU");

    long long a;
    bool prime;
    cin >> a;

    prime = check_prime(a);
    if (prime)
    {
        cout << "Число " << a << " простое." << endl;
    }
    else
    {
        cout << "Число " << a << " составное." << endl;
    }

    cout << "Разложение числа на простые множители:" << endl << a << " = ";
    vector < pair <long long, long long> > dividers = factorization(a);
    for (int i = 0; i < dividers.size(); i++)
    {
        cout << dividers[i].first;
        if (dividers[i].second > 1)
        {
            cout << "^" << dividers[i].second;
        }
        if (i < dividers.size() - 1)
        {
            cout << " * ";
        }
    }
    cout << endl;

    cout << "Количество делителей у числа " << a << ": " << sgm0(a) << endl;

    cout << "Сумма делителей у числа " << a << ": " << sgm1(a) << endl;

    cout << "Функция Эйлера от числа " << a << ": " << phi(a) << endl;
}