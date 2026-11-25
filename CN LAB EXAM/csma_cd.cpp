#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>

using namespace std;

struct Station {
    char name;
    int readySlot;
    int attempts = 0;
    bool done = false;
};

int main() {
    srand(7);
    const int MAX_ATTEMPTS = 16;

    vector<Station> stations = {{'A', 0}, {'B', 0}, {'C', 2}};
    int slot = 0, completed = 0;

    while (completed < stations.size()) {
        vector<int> contending;
        for (int i = 0; i < stations.size(); i++) {
            if (!stations[i].done && stations[i].readySlot == slot)
                contending.push_back(i);
        }

        if (contending.size() == 1) {
            int idx = contending[0];
            cout << "Slot " << slot << ": " << stations[idx].name
                 << " transmitted SUCCESSFULLY.\n";
            stations[idx].done = true;
            completed++;
        } else if (contending.size() > 1) {
            cout << "Slot " << slot << ": COLLISION between ";
            for (int idx : contending) cout << stations[idx].name << " ";
            cout << "\n";

            for (int idx : contending) {
                stations[idx].attempts++;
                if (stations[idx].attempts >= MAX_ATTEMPTS) {
                    cout << "  " << stations[idx].name << ": Max attempts reached. Abandoned.\n";
                    stations[idx].done = true;
                    completed++;
                } else {
                    int k = min(stations[idx].attempts, 10);
                    int range = 1 << k; 
                    int backoff = rand() % range;
                    stations[idx].readySlot = slot + 1 + backoff;
                    cout << "  " << stations[idx].name << ": attempt #" << stations[idx].attempts
                         << ", backoff=" << backoff << " -> retry at slot "
                         << stations[idx].readySlot << "\n";
                }
            }
        }
        slot++;
    }

    cout << "\nSUMMARY:\n";
    for (auto &s : stations) {
        cout << "Station " << s.name << ": "
             << (s.attempts >= MAX_ATTEMPTS ? "Abandoned" : "Success")
             << " (collisions = " << s.attempts << ")\n";
    }
    return 0;
}