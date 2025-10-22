#include <iostream>
#include <string>
#import <cctype>
using namespace std;

#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>
    int getkey() {return _getch();}
#else
    #include <termios.h>
    #include <unistd.h>
    #include <errno.h>

    static void enableRawMode(termios &orig) {
        termios raw;
        tcgetattr(STDIN_FILENO, & orig);
        raw = orig;
        raw.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STD_FILENO, TCSANOW, &raw);
    }
    static void disableRawMode(const termios &orig) {
        tcsetattr(STD_FILENO, TCSANOW, &orig);
    }

    int getkey() {
        termios orig;
        enableRawMode(orig);
        int c = getchar();
        disableRawMode(orig);
        if (c == EOF) return -1;
        return c;
    }
#endif

int main() {
        cout << "press keys ('`' to quit). Keystrokes are immediately shown: \n";
        while (true) {
            int k = getkey();
            if (k == -1) break;
            if (k == 0 || k == 0xE0) {
                #if defined(_WIN32) || defined(_WIN_64)
                    int k2 = _getch();
                    cout << "[special: " << k2 << " ]\n";
                #else
                    cout << "[prefix: " << k << " ]\n";
                #endif
                continue;
            }
            if (k == '\r') k = '\n';
            cout << "Code: " << k << "  Char: ";
            if (isprint(k)) cout << static_cast<char>(k) << '\n';
            else cout << "## NON-PRINTABLE ##\n";
            if (k == '`') break;
        }
        cout << "Exiting...\n";
        return 0;
    }