#include "kbc.h"
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

KBC::KBC() {
    loadUsers();
    loadRankings();
    srand(time(0));  // Seed for random lifeline generation
}

void KBC::clrscr() {
    std::system("clear");  // For Unix/Linux, replace with "cls" for Windows
}

void KBC::kbcheader() {
    cout << "------------------------------\n";
    cout << "    Kaun Banega Crorepati\n";
    cout << "------------------------------\n";
}

void KBC::showRules() {
    clrscr();
    kbcheader();
    cout << "Rules:\n";
    cout << "1. Answer each question by typing A, B, C, or D.\n";
    cout << "2. Use 'L' during a question to access a lifeline.\n";
    cout << "3. Lifelines:\n";
    cout << "   - 50-50: Removes two incorrect options.\n";
    cout << "   - Ask the Expert: Expert suggests the correct answer.\n";
    cout << "4. Win prize money as you answer correctly!\n";
    cout << "-----------------------------------\n";
    cout << "Press Enter to start the game...";
    cin.ignore();
    cin.get();
}

void KBC::printQuestion(int questionNumber, bool applyLifeline) {
    if (questionNumber == 1) {
        cout << "Question 1: What is the capital of France?\n";
        cout << "A) Paris\nB) Berlin\nC) Madrid\nD) Rome\n";
        if (applyLifeline) {
            apply5050(1);  // Only for demo purposes
        }
    } else if (questionNumber == 2) {
        cout << "Question 2: What is 2 + 2?\n";
        cout << "A) 3\nB) 4\nC) 5\nD) 6\n";
        if (applyLifeline) {
            apply5050(2);  // Only for demo purposes
        }
    }
}

bool KBC::checkAnswer(int questionNumber, const string& answer) {
    if (questionNumber == 1 && answer == "A") return true;
    if (questionNumber == 2 && answer == "B") return true;
    return false;
}

int KBC::getQuestionCount() {
    return 2;
}

int KBC::getPrizeMoney(int questionNumber) {
    if (questionNumber == 1) return 1000;
    if (questionNumber == 2) return 2000;
    return 0;
}

void KBC::signup() {
    clrscr();
    kbcheader();
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (users.find(username) != users.end()) {
        cout << "Username already exists. Try a different one.\n";
        return;
    }

    users[username] = password;
    saveUsers();
    cout << "Signup successful! You can now log in.\n";
}

bool KBC::login(string& username) {
    clrscr();
    kbcheader();
    string password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (users.find(username) != users.end() && users[username] == password) {
        cout << "Login successful!\n";
        return true;
    } else {
        cout << "Invalid credentials. Please try again.\n";
        return false;
    }
}

void KBC::loadUsers() {
    ifstream infile("users.txt");
    if (!infile) return;
    string username, password;
    while (infile >> username >> password) {
        users[username] = password;
    }
    infile.close();
}

void KBC::saveUsers() {
    ofstream outfile("users.txt");
    for (const auto& user : users) {
        outfile << user.first << " " << user.second << endl;
    }
    outfile.close();
}

void KBC::loadRankings() {
    ifstream infile("ranking.txt");
    if (!infile) return;
    string username;
    int prize;
    while (infile >> username >> prize) {
        rankings.push_back({username, prize});
    }
    infile.close();
}

void KBC::saveRankings() {
    ofstream outfile("ranking.txt");
    for (const auto& entry : rankings) {
        outfile << entry.first << " " << entry.second << endl;
    }
    outfile.close();
}

void KBC::displayRanking() {
    clrscr();
    kbcheader();
    cout << "\nRankings:\n";
    cout << "\n";
    cout << "Username            Total Prize\n";
    cout << "-----------------------------------\n";

    sort(rankings.begin(), rankings.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return b.second < a.second;  // Sort by prize money in descending order
    });

    for (const auto& entry : rankings) {
        cout << entry.first;
        for(int i = entry.first.size(); i<=22; i++){
            cout << " ";
        }
        cout << "$" << entry.second << endl;
    }
}

void KBC::playGame(const string& username) {
    int currentQuestion = 1;
    int totalPrizeMoney = 0;
    string answer;

    showRules();
    usedLifelines.clear();  // Reset lifelines for each new game

    while (currentQuestion <= getQuestionCount()) {
        clrscr();
        kbcheader();
        printQuestion(currentQuestion);
        cout << "\nYour answer (or press L for Lifeline): ";
        cin >> answer;

        if (answer == "L") {
            useLifeline(currentQuestion);
            continue;
        }

        if (checkAnswer(currentQuestion, answer)) {
            int prizeMoney = getPrizeMoney(currentQuestion);
            totalPrizeMoney += prizeMoney;
            cout << "Correct! You have won $" << prizeMoney << ".\n";
            cout << "Total Prize Money: $" << totalPrizeMoney << "\n";
            currentQuestion++;
        } else {
            cout << "Incorrect. Game Over.\n";
            break;
        }

        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }

    if (currentQuestion > getQuestionCount()) {
        cout << "Congratulations! You've completed the game.\n";
        cout << "Total Prize Money: $" << totalPrizeMoney << "\n";
    }

    rankings.push_back({username, totalPrizeMoney});
    saveRankings();
    displayRanking();
}

void KBC::useLifeline(int questionNumber) {
    if (usedLifelines.size() >= 2) {
        cout << "All lifelines used!\n";
        return;
    }

    cout << "\nAvailable Lifelines:\n";
    if (usedLifelines.find(LIFELINE_5050) == usedLifelines.end()) {
        cout << "1. 50-50\n";
    }
    if (usedLifelines.find(LIFELINE_EXPERT) == usedLifelines.end()) {
        cout << "2. Ask the Expert\n";
    }
    cout << "Choose a lifeline: ";
    int choice;
    cin >> choice;

    if (choice == 1 && usedLifelines.find(LIFELINE_5050) == usedLifelines.end()) {
        apply5050(questionNumber);
        usedLifelines.insert(LIFELINE_5050);
    } else if (choice == 2 && usedLifelines.find(LIFELINE_EXPERT) == usedLifelines.end()) {
        askExpert(questionNumber);
        usedLifelines.insert(LIFELINE_EXPERT);
    } else {
        cout << "Invalid choice or lifeline already used!\n";
    }
}

void KBC::apply5050(int questionNumber) {
    cout << "50-50 Lifeline Used!\n";
    // Mock: Removing two random wrong answers. For real implementation, you would update question data.
    if (questionNumber == 1) {
        cout << "Remaining Options: A) Paris  C) Madrid\n";
    } else if (questionNumber == 2) {
        cout << "Remaining Options: B) 4  D) 6\n";
    }
}

void KBC::askExpert(int questionNumber) {
    cout << "Ask the Expert Lifeline Used!\n";
    if (questionNumber == 1) {
        cout << "Expert: I think option A (Paris) is correct.\n";
    } else if (questionNumber == 2) {
        cout << "Expert: I think option B (4) is correct.\n";
    }
}
