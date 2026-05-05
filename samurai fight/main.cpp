#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <vector>
#include <map>
#include <windows.h>

using namespace std;

// Названия предметов с цветами
string get_choice_name(int c) {
    if (c == 1) return "\033[1;31mSword\033[0m"; // Красный
    if (c == 2) return "\033[1;34mShield\033[0m"; // Синий
    return "\033[1;32mMoney\033[0m"; // Зеленый
}

// Отображение статуса
void print_status(const string& p_name, int p_wins, int ai_wins) {
    cout << "\n\033[1;35m--- Status ---\033[0m" << endl;
    cout << p_name << " ";
    for (int i = 0; i < 3; ++i) {
        if (i < p_wins) cout << "\033[1;32m* \033[0m"; else cout << "* ";
    }
    cout << "| AI ";
    for (int i = 0; i < 3; ++i) {
        if (i < ai_wins) cout << "\033[1;31m* \033[0m"; else cout << "* ";
    }
    cout << "\n----------------" << endl;
}

// Радужный текст
void print_rainbow(string text) {
    string colors[] = { "\033[1;31m", "\033[1;33m", "\033[1;32m", "\033[1;36m", "\033[1;34m", "\033[1;35m" };
    for (int i = 0; i < text.length(); ++i) cout << colors[i % 6] << text[i];
    cout << "\033[0m" << endl;
}

// Логика ИИ
int get_ai_choice(const vector<int>& history, int difficulty) {
    int p_last = history.back();
    int r = rand() % 100;
    int t_random = (difficulty == 1) ? 50 : (difficulty == 2 ? 50 : 20);
    int t_counter = (difficulty == 1) ? 40 : (difficulty == 2 ? 25 : 40);

    if (r < t_random) return rand() % 3 + 1;
    if (r < t_random + t_counter) {
        if (p_last == 1) return 2; if (p_last == 2) return 3; return 1;
    }
    if (p_last == 1) return 3; if (p_last == 2) return 1; return 2;
}

// Вывод статистики и достижений
void show_stats(const string& winner, const vector<int>& p_hist, const vector<int>& a_hist, int total_rounds, const string& p_name) {
    cout << "\n--- Stats ---" << endl;
    cout << "Total rounds: \033[1;35m" << total_rounds << "\033[0m" << endl;
    cout << "Winner: \033[1;93m" << winner << "\033[0m" << endl;

    map<int, int> counts;
    const vector<int>& hist = (winner == "AI") ? a_hist : p_hist;
    int fav = 0;
    if (!hist.empty()) {
        for (int m : hist) counts[m]++;
        fav = 1;
        for (auto const& [key, val] : counts) if (val > counts[fav]) fav = key;
        cout << "Favorite item: " << get_choice_name(fav) << endl;
    }
    else {
        cout << "Favorite item: None" << endl;
    }

    cout << "\n\033[1;38;2;255;165;0mAchievements:\033[0m" << endl;
    bool win = (winner == p_name);
    string achs[] = { " * Sword-master", " * Defender-master", " * Quick-thief" };
    string desc[] = { "(win with your favourite item - Sword)", "(win with your favourite item - Shield)", "(win with your favourite item - Money)" };

    for (int i = 0; i < 3; ++i) {
        bool earned = (win && fav == (i + 1));
        if (earned) cout << "\033[1;32m" << achs[i] << " " << desc[i] << "\033[0m" << endl;
        else cout << "\033[1;31m" << achs[i] << " (???)\033[0m" << endl;
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    srand(time(0));
    int p_wins = 0, ai_wins = 0, difficulty, total_rounds = 0;
    string name, input;
    vector<int> p_hist, a_hist;

    cout << "Enter name: "; cin >> name;
    while (true) {
        cout << "Choose difficulty (1-\033[1;32mEasy\033[0m, 2-\033[1;33mMedium\033[0m, 3-\033[1;31mHard\033[0m): ";
        cin >> input;
        try { difficulty = stoi(input); if (difficulty >= 1 && difficulty <= 3) break; }
        catch (...) {}
    }

    while (p_wins < 3 && ai_wins < 3) {
        print_status(name, p_wins, ai_wins);
        int p_choice = 0;
        while (true) {
            cout << "Choose: 1.Sword, 2.Shield, 3.Money (or 'srndr'): ";
            cin >> input;
            if (input == "srndr") { ai_wins = 3; goto end_game; }
            try { p_choice = stoi(input); if (p_choice >= 1 && p_choice <= 3) break; }
            catch (...) {}
            cout << "Invalid input!" << endl;
        }
        int ai_choice = (p_hist.empty()) ? (rand() % 3 + 1) : get_ai_choice(p_hist, difficulty);

        cout << "You chose " << get_choice_name(p_choice) << ", AI chose " << get_choice_name(ai_choice) << "." << endl;
        total_rounds++;

        if (p_choice == ai_choice) cout << "Tie! Repeat." << endl;
        else if ((p_choice == 1 && ai_choice == 3) || (p_choice == 2 && ai_choice == 1) || (p_choice == 3 && ai_choice == 2)) {
            cout << "You win round!" << endl; p_wins++;
        }
        else {
            cout << "AI wins round!" << endl; ai_wins++;
        }
        p_hist.push_back(p_choice); a_hist.push_back(ai_choice);
    }
end_game:
    print_rainbow(p_wins == 3 ? "\nChampion!" : "\nAI Champion!");
    show_stats((p_wins == 3 ? name : "AI"), p_hist, a_hist, total_rounds, name);
    return 0;
}