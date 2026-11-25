#include <iostream>
#include <string>
using namespace std;

// Perform modulo-2 division and return the remainder
string mod2div(string dividend, const string &divisor)
{
    int n = dividend.length();
    int m = divisor.length();

    for (int i = 0; i <= n - m; i++)
    {
        if (dividend[i] == '1')
        {
            for (int j = 0; j < m; j++)
            {
                dividend[i + j] = (dividend[i + j] == divisor[j]) ? '0' : '1';
            }
        }
    }

    // Return the last (m - 1) bits as the remainder
    return dividend.substr(n - m + 1);
}

// Check if the remainder contains all zeros
bool isZero(const string &rem)
{
    return rem.find('1') == string::npos;
}

int main()
{
    string data, gen;

    cout << "IMPLEMENTATION OF CRC (CYCLIC REDUNDANCY CHECK)\n";
    cout << "Enter data bits           : ";
    cin >> data;
    cout << "Enter generator polynomial: ";
    cin >> gen;

    int r = gen.length() - 1;

    // Append r zeros to the original data
    string appended = data + string(r, '0');

    // Compute CRC remainder
    string crc = mod2div(appended, gen);

    // Form transmitted codeword (Data + CRC)
    string codeword = data + crc;

    cout << "\nSENDER SIDE\n";
    cout << "Original Data                   : " << data << "\n";
    cout << "Generator Polynomial            : " << gen << " (degree " << r << ")\n";
    cout << "Data after appending " << r << " zeros    : " << appended << "\n";
    cout << "CRC (Remainder)                 : " << crc << "\n";
    cout << "Transmitted Codeword (Data+CRC) : " << codeword << "\n";

    // Receiver Side: Error-free channel
    string rem = mod2div(codeword, gen);
    cout << "\nRECEIVER SIDE (Error-free channel)\n";
    cout << "Received Codeword               : " << codeword << "\n";
    cout << "Remainder after division        : " << rem << "\n";
    if (isZero(rem))
        cout << "Result                          : NO ERROR (data accepted)\n";
    else
        cout << "Result                          : ERROR DETECTED\n";

    // Receiver Side: Noisy channel (flipping bit at index 2)
    string corrupted = codeword;
    int flipPos = 2;
    corrupted[flipPos] = (corrupted[flipPos] == '0') ? '1' : '0';

    rem = mod2div(corrupted, gen);
    cout << "\nRECEIVER SIDE (Noisy channel)\n";
    cout << "Bit position flipped            : " << flipPos << "\n";
    cout << "Received Codeword (corrupted)   : " << corrupted << "\n";
    cout << "Remainder after division        : " << rem << "\n";
    if (isZero(rem))
        cout << "Result                          : NO ERROR (data accepted)\n";
    else
        cout << "Result                          : ERROR DETECTED\n";

    return 0;
}