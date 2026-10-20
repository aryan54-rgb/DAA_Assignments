#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Item {
    int weight;
    int value;
    int priority;
};

int main() {
    int n, W, bonus;

    cout << "Enter number of items: ";
    cin >> n;
    cout << "Enter truck capacity: ";
    cin >> W;
    cout << "Enter bonus for priority items: ";
    cin >> bonus;

    vector<Item> items(n + 1);

    for (int i = 1; i <= n; i++) {
        cout << "\nItem " << i << "\n";
        cout << "Enter weight: ";
        cin >> items[i].weight;
        cout << "Enter utility: ";
        cin >> items[i].value;
        cout << "Priority item? (1 = Yes, 0 = No): ";
        cin >> items[i].priority;
    }

    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

    for (int i = 1; i <= n; i++) {
        int score = items[i].value + bonus * items[i].priority;

        for (int w = 0; w <= W; w++) {
            dp[i][w] = dp[i - 1][w];

            if (items[i].weight <= w) {
                dp[i][w] = max(
                    dp[i][w],
                    score + dp[i - 1][w - items[i].weight]
                );
            }
        }
    }

    vector<int> selected;
    int remaining = W;

    for (int i = n; i >= 1; i--) {
        if (dp[i][remaining] != dp[i - 1][remaining]) {
            selected.push_back(i);
            remaining -= items[i].weight;
        }
    }

    reverse(selected.begin(), selected.end());

    int totalWeight = 0;
    int totalUtility = 0;

    cout << "\nSelected Items\n";
    cout << "ID\tWeight\tUtility\tPriority\n";

    for (int id : selected) {
        cout << id << "\t"
             << items[id].weight << "\t"
             << items[id].value << "\t"
             << items[id].priority << "\n";

        totalWeight += items[id].weight;
        totalUtility += items[id].value;
    }

    cout << "\nTotal Weight: " << totalWeight << "\n";
    cout << "Remaining Capacity: " << W - totalWeight << "\n";
    cout << "Total Utility: " << totalUtility << "\n";
    cout << "Priority-Adjusted Utility: " << dp[n][W] << "\n";

    return 0;
}