#ifndef KBC_H
#define KBC_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

class KBC {
private:
    vector<pair<string, int>> rankings;  // Pair of username and prize money
    map<string, string> users;           // Username and password map
    set<int> usedLifelines;              // Track used lifelines

    const int LIFELINE_5050 = 1;
    const int LIFELINE_EXPERT = 2;

public:
    KBC();
    void clrscr();
    void kbcheader();
    void showRules();
    void printQuestion(int questionNumber, bool applyLifeline = false);
    bool checkAnswer(int questionNumber, const string& answer);
    int getQuestionCount();
    int getPrizeMoney(int questionNumber);
    void signup();
    bool login(string& username);
    void loadUsers();
    void saveUsers();
    void loadRankings();
    void saveRankings();
    void displayRanking();
    void playGame(const string& username);
    void useLifeline(int questionNumber);
    void apply5050(int questionNumber);
    void askExpert(int questionNumber);
};

#endif
