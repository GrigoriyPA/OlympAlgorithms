#include <iostream>
using namespace std;


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


long long get_opposite(long long a, long long p)
{
    a %= p;
    return bem(a, p - 2, p);
}

               
int main()
{
    long long a, p;
    cin >> a >> p;
    cout << get_opposite(a, p) << endl;
}