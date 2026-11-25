#include <iostream>
using namespace std;

int main() {
    int scenario[4] = {0, 1, 2, 0};
    int seq = 0, expected = 0, tx = 0, timeouts = 0, retrans = 0;

    for (int i = 0; i < 4; i++) {
        int attempt = 0;
        cout << "\nFRAME " << i << " (seq=" << seq << ")\n";

        while (true) {
            attempt++;
            tx++;
            if (attempt > 1) retrans++;

            cout << "[Sender] Sending DATA(seq=" << seq << "), attempt #" << attempt << "\n";

            if (scenario[i] == 1 && attempt == 1) {
                cout << "[Network] DATA lost -> TIMEOUT! Retransmitting...\n";
                timeouts++;
                continue;
            }

            if (seq == expected) {
                cout << "[Receiver] DATA received. Sending ACK.\n";
                expected = 1 - expected;
            } else {
                cout << "[Receiver] Duplicate DATA discarded. Resending ACK.\n";
            }

            if (scenario[i] == 2 && attempt == 1) {
                cout << "[Network] ACK lost -> TIMEOUT! Retransmitting...\n";
                timeouts++;
                continue;
            }

            cout << "[Sender] ACK received. Moving to next frame.\n";
            seq = 1 - seq;
            break;
        }
    }

    cout << "\nSummary: Frames=4, Transmissions=" << tx
         << ", Timeouts=" << timeouts << ", Retransmissions=" << retrans << "\n";
    return 0;
}