#include <iostream>
#include <vector>
using namespace std;


void bubble_sort(vector<int>& a)
{
    int n = a.size();
    bool unordered = true;
    while (unordered) 
    {
        unordered = false;
        for (int i = 0; i < n - 1; ++i) 
        {
            if (a[i] > a[i + 1]) 
            {
                swap(a[i], a[i + 1]);
                unordered = true;
            }
        }
        --n;
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

    bubble_sort(a);

    for (int i = 0; i < size; i++)
    {
        cout << a[i] << " ";
    }
    cout << endl;

    return 0;
}