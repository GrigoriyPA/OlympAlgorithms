#include <iostream>
#include <vector>
using namespace std;


void selection_sort(vector<int>& a)
{
    for (int i = 0; i < (int)a.size() - 1; ++i) 
    {
        int imin = i;
        for (int j = i + 1; j < a.size(); ++j) 
        {
            if (a[imin] > a[j])
            {
                imin = j;
            }
        }
        swap(a[i], a[imin]);
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

    selection_sort(a);

    for (int i = 0; i < size; i++)
    {
        cout << a[i] << " ";
    }
    cout << endl;

    return 0;
}