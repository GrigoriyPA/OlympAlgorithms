#include <iostream>
#include <vector>
using namespace std;


vector <long long> sieve(long long a, long long b)
{
    vector <bool> prime(b + 1, true);
    prime[0] = false;
    prime[1] = false;
    for (long long i = 2; i <= b; i++)
    {
        if (prime[i])
        {
            for (long long j = i * i; j <= b; j += i)
            {
                prime[j] = false;
            }
        }
    }
    vector <long long> res;
    for (long long i = a; i <= b; i++)
    {
        if (prime[i])
        {
            res.push_back(i);
        }
    }
    return res;
}


int main()
{
    setlocale(LC_ALL, "RU");

    long long a, b;
    cin >> a >> b;

    cout << "Простые числа в интервале [" << a << "; " << b << "]:" << endl;
    vector <long long> prime_numbers = sieve(a, b);
    for (int i = 0; i < prime_numbers.size(); i++)
    {
        cout << prime_numbers[i] << " ";
    }
    cout << endl;
}