#include <iostream>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Supply
{
    int id;
    double weight;
    double utility;
    int type;
    double ratio;
};

bool compare(Supply a, Supply b)
{
    return a.ratio > b.ratio;
}

int main()
{
    int n;
    double W;

    cout << "Emergency Relief Supply Distribution\n";
    cout << "Enter number of relief supplies: ";
    cin >> n;

    Supply supplies[1000];

    cout << "Enter boat capacity W (kg): ";
    cin >> W;

    for (int i = 0; i < n; i++)
    {
        supplies[i].id = i + 1;

        cout << "\nSupply " << i + 1 << endl;
        cout << "Enter weight (kg): ";
        cin >> supplies[i].weight;

        cout << "Enter utility value: ";
        cin >> supplies[i].utility;

        cout << "Enter type (1 = Divisible, 0 = Indivisible): ";
        cin >> supplies[i].type;

        supplies[i].ratio = supplies[i].utility / supplies[i].weight;
    }

    sort(supplies, supplies + n, compare);

    double remaining = W;
    double used = 0;
    double totalUtility = 0;

    cout << fixed << setprecision(2);

    cout << "\nSelected Relief Supplies\n";
    cout << "ID\tWeight\tUtility\n";

    for (int i = 0; i < n && remaining > 0; i++)
    {
        if (supplies[i].weight <= remaining)
        {
            remaining -= supplies[i].weight;
            used += supplies[i].weight;
            totalUtility += supplies[i].utility;

            cout << supplies[i].id << "\t"
                 << supplies[i].weight << "\t"
                 << supplies[i].utility << endl;
        }
        else
        {
            if (supplies[i].type == 1)
            {
                double fraction = remaining / supplies[i].weight;
                double partialUtility = supplies[i].utility * fraction;

                used += remaining;
                totalUtility += partialUtility;

                cout << supplies[i].id << "\t"
                     << remaining << "\t"
                     << partialUtility
                     << " (Partial)" << endl;

                remaining = 0;
            }
            else
            {
                continue;
            }
        }
    }

    cout << "\nTotal Weight Used: " << used << " kg" << endl;
    cout << "Remaining Boat Capacity: " << remaining << " kg" << endl;
    cout << "Maximum Total Utility: " << totalUtility << endl;

    return 0;
}