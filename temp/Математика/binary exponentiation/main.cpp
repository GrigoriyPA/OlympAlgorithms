#include <iostream>
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


int main()
{
    long long base, degree;

    cin >> base >> degree;

    cout << be(base, degree) << endl;

    return 0;
}