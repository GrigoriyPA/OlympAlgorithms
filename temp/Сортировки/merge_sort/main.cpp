#include <iostream>
#include <vector>
using namespace std;


vector <int> merge(vector <int> a, vector <int> b)
{
    vector <int> res;
    int i = 0;
    int j = 0;
    while (i < a.size() && j < b.size())
    {
        if (a[i] <= b[j])
        {
            res.push_back(a[i++]);
        }
        else
        {
            res.push_back(b[j++]);
        }
    }
    while (i < a.size())
    {
        res.push_back(a[i++]);
    }
    while (j < b.size())
    {
        res.push_back(b[j++]);
    }
    return res;
}


vector <int> merge_sort(vector <int>& a, int l, int r)
{
    if (l == r)
    {
        vector <int> res(1, a[l]);
        return res;
    }
    return merge(merge_sort(a, l, (l + r) / 2), merge_sort(a, (l + r) / 2 + 1, r));
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

    a = merge_sort(a, 0, size - 1);

    for (int i = 0; i < size; i++)
    {
        cout << a[i] << " ";
    }
    cout << endl;

    return 0;
}