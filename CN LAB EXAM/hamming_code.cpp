#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;
bool isPowerOfTwo(int x) {
    return (x > 0) && ((x & (x - 1)) == 0);
}

int calculateSyndrome(const vector<int>& code, int r, int n) {
    int syndrome = 0;
    for (int i = 0; i < r; i++) {
        int parityPos = 1 << i; // 1, 2, 4, 8, ...
        int parity = 0;
        for (int j = 1; j <= n; j++) {
            if (j & parityPos) {
                parity ^= code[j];
            }
        }
        if (parity != 0) {
            syndrome += parityPos;
        }
    }
    return syndrome;
}

void printCodeword(const vector<int>& code, int n) {
    for (int i = 1; i <= n; i++) {
        cout << code[i];
    }
    cout << endl;
}

int main() {
    string data;
    cout << "IMPLEMENTATION OF HAMMING CODE (Error Detection + Correction)\n\n";
    cout << "Enter data bits: ";
    cin >> data;

    int m = data.length();
    int r = 0;

    while ((1 << r) < (m + r + 1)) {
        r++;
    }

    int n = m + r;
    vector<int> codeword(n + 1, 0); 

    int dataIdx = 0;
    for (int i = 1; i <= n; i++) {
        if (!isPowerOfTwo(i)) {
            codeword[i] = data[dataIdx++] - '0';
        }
    }

    for (int i = 0; i < r; i++) {
        int parityPos = 1 << i;
        int parity = 0;
        for (int j = 1; j <= n; j++) {
            if (j & parityPos) {
                parity ^= codeword[j];
            }
        }
        codeword[parityPos] = parity;
    }

    cout << "\nSENDER SIDE\n";
    cout << "Data bits (m = " << m << ")              : " << data << "\n";
    cout << "Redundant/parity bits required : r = " << r 
         << " (since 2^" << r << " >= " << m << " + " << r << " + 1)\n";
    cout << "Total codeword length          : n = m + r = " << n << "\n";
    cout << "Parity bit positions           : ";
    for (int i = 0; i < r; i++) {
        cout << (1 << i) << " ";
    }
    cout << "\nEncoded Hamming Codeword       : ";
    printCodeword(codeword, n);

    cout << "\nRECEIVER SIDE (Error-free channel)\n";
    cout << "Received Codeword              : ";
    printCodeword(codeword, n);
    int errorPos = calculateSyndrome(codeword, r, n);
    if (errorPos == 0) {
        cout << "Result                         : NO ERROR\n";
    } else {
        cout << "Result                         : ERROR DETECTED at position " << errorPos << "\n";
    }

    cout << "\nRECEIVER SIDE (Single-bit error)\n";
    int flipPos = 5; 
    if (flipPos > n) flipPos = 1; 

    vector<int> corrupted = codeword;
    corrupted[flipPos] ^= 1;

    cout << "Bit position flipped           : " << flipPos << "\n";
    cout << "Received Codeword (corrupted)  : ";
    printCodeword(corrupted, n);

    errorPos = calculateSyndrome(corrupted, r, n);
    if (errorPos == 0) {
        cout << "Result                         : NO ERROR\n";
    } else {
        cout << "Result                         : ERROR DETECTED\n";
        corrupted[errorPos] ^= 1; 
        cout << "Corrected Codeword             : ";
        printCodeword(corrupted, n);
    }

    string recoveredData = "";
    for (int i = 1; i <= n; i++) {
        if (!isPowerOfTwo(i)) {
            recoveredData += to_string(corrupted[i]);
        }
    }
    cout << "Recovered Data Bits            : " << recoveredData << "\n";

    return 0;
}