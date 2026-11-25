#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

int main() {
    int a, b, c, d;
    char dot;
    cout << left << setw(18) << "IP Address" << setw(12) << "Class" << "Default Mask\n";
    cout << string(45, '-') << "\n";

    while (cin >> a >> dot >> b >> dot >> c >> dot >> d) {
        string cls, mask;
        if (a >= 1 && a <= 126)        { cls = "Class A"; mask = "255.0.0.0"; }
        else if (a >= 128 && a <= 191) { cls = "Class B"; mask = "255.255.0.0"; }
        else if (a >= 192 && a <= 223) { cls = "Class C"; mask = "255.255.255.0"; }
        else if (a >= 224 && a <= 239) { cls = "Class D"; mask = "N/A (multicast)"; }
        else if (a >= 240 && a <= 255) { cls = "Class E"; mask = "N/A (experimental)"; }
        else                           { cls = "Special"; mask = "N/A"; }

        string ip = to_string(a) + "." + to_string(b) + "." + to_string(c) + "." + to_string(d);
        cout << left << setw(18) << ip << setw(12) << cls << mask << "\n";
    }
    return 0;
}