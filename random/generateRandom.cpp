#include <iostream>

using namespace std;

int main() {
    int len = 999;
    cout << len << endl;
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            int r = rand();
            char temp = 0;
            if (r % 2 == 0) {
                temp = 'c';
            }
            else {
                temp = 'w';
            }
            if (j == (len - 1)) {
                cout << temp;
            } else {
                cout << temp << " ";
            }
        }
        cout << endl;
    }

    cout << 500 << " " << 500 << endl;
}