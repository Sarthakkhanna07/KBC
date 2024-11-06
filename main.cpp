#include "kbc.h"

int main() {
    KBC game;
    string username;

    while (true) {
        game.clrscr();
        game.kbcheader();
        cout << "1. Signup\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Choose an option: ";
        int option;
        cin >> option;

        if (option == 1) {
            game.signup();
        } else if (option == 2) {
            if (game.login(username)) {
                game.playGame(username);
            }
        } else if (option == 3) {
            break;
        } else {
            cout << "Invalid option. Please try again.\n";
        }

        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }

    return 0;
}
