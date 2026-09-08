#include <iostream>
#include <algorithm>
#include <iomanip>
using namespace std;
struct Item
{
    int id;
    double weight;
    double value;
    int type;
    double ratio;
};
bool compare(Item a, Item b)
{
    return a.ratio > b.ratio;
}
int main()
{
    int n;
    double W;
    cout << "Enter number of items: ";
    cin >> n;
    Item items[1000];
    cout << "Enter capacity W: ";
    cin >> W;
    for (int i = 0; i < n; i++)
    {
        items[i].id = i + 1;
        cout << "\nItem " << i + 1 << endl;
        cout << "Enter weight/resource (wi): ";
        cin >> items[i].weight;
        cout << "Enter value/profit (vi): ";
        cin >> items[i].value;
        cout << "Enter type (1 = Divisible, 0 = Indivisible): ";
        cin >> items[i].type;
        items[i].ratio = items[i].value / items[i].weight;
    }
    sort(items, items + n, compare);
    double remaining = W;
    double used = 0;
    double totalValue = 0;
    cout << fixed << setprecision(2);
    cout << "\nSelected Items\n";
    for (int i = 0; i < n && remaining > 0; i++)
    {
        if (items[i].weight <= remaining)
        {
            remaining -= items[i].weight;
            used += items[i].weight;
            totalValue += items[i].value;
            cout << items[i].id << "\t"
                 << items[i].weight << "\t"
                 << items[i].value << endl;
        }
        else
        {
            if (items[i].type == 1)
            {
                double fraction = remaining / items[i].weight;
                double partialValue = items[i].value * fraction;
                used += remaining;
                totalValue += partialValue;
                cout << items[i].id << "\t"
                     << remaining << "\t"
                     << partialValue
                     << " (Partial)" << endl;
                remaining = 0;
            }
            else
            {
                continue;
            }
        }
    }
    cout << "\nTotal Used: " << used << endl;
    cout << "Remaining Capacity: " << remaining << endl;
    cout << "Maximum Total Value: " << totalValue << endl;
    return 0;
}