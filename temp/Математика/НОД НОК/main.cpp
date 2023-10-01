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


long long lcm(long long a, long long b)
{
    int gcd_a_b = gcd(a, b);
    return (a * b) / gcd_a_b;
}


int main()
{
    setlocale(LC_ALL, "RU");

    long long a, b;
    cin >> a >> b;
    cout << "мнд (a, b): " << gcd(a, b) << endl;
    cout << "мнй (a, b): " << lcm(a, b) << endl;
}