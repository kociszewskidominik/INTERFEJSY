#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string loadText(const string& path) {
    ifstream file(path);
    if (!file.is_open()) return "Nie mozna otworzyc pliku.";
    return string((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));
}

string selfPrint() {
    ifstream file(__FILE__);
    if (!file.is_open()) return "Nie mozna odczytac zrodla.";
    return string((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));
}

bool configureCOM(const char* portName, string& out) {
    DCB dcb = { 0 };
    HANDLE hComm = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hComm == INVALID_HANDLE_VALUE) return false;

    dcb.DCBlength = sizeof(DCB);
    if (!GetCommState(hComm, &dcb)) { CloseHandle(hComm); return false; }

    dcb.BaudRate = CBR_9600;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity = NOPARITY;

    if (!SetCommState(hComm, &dcb)) { CloseHandle(hComm); return false; }
    GetCommState(hComm, &dcb);

    out = "BaudRate: " + to_string(dcb.BaudRate) +
          "\nByteSize: " + to_string(dcb.ByteSize) +
          "\nStopBits: " + to_string(dcb.StopBits) +
          "\nParity: " + to_string(dcb.Parity);

    CloseHandle(hComm);
    return true;
}

void clear() {
    system("cls");
}

void pauseScreen() {
    cout << "\n\n>:";
    cin.get();
}

int main() {
    while (true) {
        clear();
        cout << "> 1. Konfiguruj COM3\n";
        cout << "> 2. Self-print\n";
        cout << "> 3. Wyswietl plik tekstowy\n";
        cout << "> 4. Wyjscie\n\n";
        cout << ">: ";

        int opcja;
        cin >> opcja;
        cin.ignore();

        clear();

        if (opcja == 1) {
            string result;
            if (configureCOM("COM3", result)) cout << result;
            else cout << "Blad konfiguracji";
            pauseScreen();
        }

        else if (opcja == 2) {
            cout << selfPrint();
            pauseScreen();
        }

        else if (opcja == 3) {
            cout << loadText("C:\\Users\\kocis\\grzegorz.txt");
            pauseScreen();
        }

        else if (opcja == 4) {
            return 0;
        }
    }
}
