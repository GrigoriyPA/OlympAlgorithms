#include <iostream>
#include <vector>
using namespace std;


void counting_sort(vector<int>& a)
{
    int max_num = -1;
    for (auto el: a)
    {
        if (max_num < el)
        {
            max_num = el;
        }
    }
    vector <int> cnt(max_num + 1, 0);
    for (auto el: a)
    {
        cnt[el]++;
    }
    a.resize(0);
    for (int d = 0; d < cnt.size(); d++)
    {
        a.insert(a.end(), cnt[d], d);
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

    counting_sort(a);

    for (int i = 0; i < size; i++)
    {
        cout << a[i] << " ";
    }
    cout << endl;

    return 0;
}