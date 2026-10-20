#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <unordered_set>
#include <algorithm>
using namespace std;

int main() {
    int n, rooms, conflicts;

    cout << "Enter number of courses: ";
    cin >> n;
    cout << "Enter number of rooms: ";
    cin >> rooms;
    cout << "Enter number of conflicts: ";
    cin >> conflicts;

    vector<int> roomCapacity(rooms);
    cout << "Enter room capacities:\n";
    for (int i = 0; i < rooms; i++)
        cin >> roomCapacity[i];

    vector<string> course(n);
    vector<int> students(n);

    cout << "Enter each course name and student count:\n";
    for (int i = 0; i < n; i++)
        cin >> course[i] >> students[i];

    vector<vector<int>> graph(n);

    cout << "Enter conflicting course ID pairs (1 to " << n << "):\n";
    for (int i = 0; i < conflicts; i++) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;

        if (a < 0 || a >= n || b < 0 || b >= n || a == b) {
            cout << "Invalid conflict pair\n";
            return 1;
        }

        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    // Remove duplicate conflict edges
    for (auto& neighbors : graph) {
        sort(neighbors.begin(), neighbors.end());
        neighbors.erase(
            unique(neighbors.begin(), neighbors.end()),
            neighbors.end()
        );
    }

    int largestRoom = 0;
    for (int capacity : roomCapacity)
        largestRoom = max(largestRoom, capacity);

    for (int i = 0; i < n; i++) {
        if (students[i] > largestRoom) {
            cout << "Cannot schedule " << course[i]
                 << ": no room has enough seats.\n";
            return 1;
        }
    }

    vector<int> slot(n, -1);
    vector<int> assignedRoom(n, -1);
    vector<unordered_set<int>> neighborSlots(n);

    // Available rooms in each slot: {capacity, room ID}
    vector<set<pair<int, int>>> freeRooms;

    for (int scheduled = 0; scheduled < n; scheduled++) {
        int chosen = -1;

        // DSATUR: choose the course whose neighbors use
        // the most different slots.
        for (int i = 0; i < n; i++) {
            if (slot[i] != -1)
                continue;

            if (chosen == -1 ||
                neighborSlots[i].size() > neighborSlots[chosen].size() ||
                (neighborSlots[i].size() == neighborSlots[chosen].size() &&
                 graph[i].size() > graph[chosen].size())) {
                chosen = i;
            }
        }

        int selectedSlot = -1;
        int selectedRoom = -1;

        // Try an existing slot with no conflict and a suitable room.
        for (int s = 0; s < (int)freeRooms.size(); s++) {
            if (neighborSlots[chosen].count(s))
                continue;

            auto it = freeRooms[s].lower_bound({students[chosen], -1});

            if (it != freeRooms[s].end()) {
                selectedSlot = s;
                selectedRoom = it->second;
                freeRooms[s].erase(it);
                break;
            }
        }

        // If necessary, create a new slot.
        if (selectedSlot == -1) {
            selectedSlot = (int)freeRooms.size();
            freeRooms.emplace_back();

            for (int r = 0; r < rooms; r++)
                freeRooms.back().insert({roomCapacity[r], r});

            auto it = freeRooms.back().lower_bound({students[chosen], -1});
            selectedRoom = it->second;
            freeRooms.back().erase(it);
        }

        slot[chosen] = selectedSlot;
        assignedRoom[chosen] = selectedRoom;

        for (int neighbor : graph[chosen]) {
            if (slot[neighbor] == -1)
                neighborSlots[neighbor].insert(selectedSlot);
        }
    }

    cout << "\nExam slots used: " << freeRooms.size() << "\n";

    for (int s = 0; s < (int)freeRooms.size(); s++) {
        cout << "Slot " << s + 1 << ":\n";

        for (int i = 0; i < n; i++) {
            if (slot[i] == s) {
                cout << "  " << course[i]
                     << " - Room " << assignedRoom[i] + 1
                     << " (" << students[i] << "/"
                     << roomCapacity[assignedRoom[i]]
                     << " seats)\n";
            }
        }
    }

    return 0;
}