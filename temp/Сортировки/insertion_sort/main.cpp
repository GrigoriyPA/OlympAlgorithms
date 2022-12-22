#include <iostream>
#include <vector>
using namespace std;


void insertion_sort(vector<int>& a)
{
    for (int i = 1; i < a.size(); ++i)
    {
        int tmp = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > tmp)
        {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = tmp;
    }
}


int main()
{
    int size;

    cin >> size;

    vector <int> a(size);
    for (int i = 0; i < size; i++)
    {
        cin >> a[i];
    }

    insertion_sort(a);

    for (int i = 0; i < size; i++)
    {
        cout << a[i] << " ";
    }
    cout << endl;

    return 0;
}