#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <vector>
#include <map>

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

// Вывод финального экрана
void show_final_screen(const string& winner, int p_wins, int ai_wins, int total_rounds, const vector<int>& p_hist, const string& p_name) {
    print_rainbow(p_wins == 3 ? "\nChampion!" : "\nAI Champion!");

    // Статистика бирюзовым цветом
    cout << "\n\033[1;36m--- Stats ---\033[0m" << endl;
    cout << "Total rounds: \033[1;35m" << total_rounds << "\033[0m" << endl;
    cout << "Winner: \033[1;93m" << winner << "\033[0m" << endl;

    map<int, int> counts;
    int fav = 0;
    if (!p_hist.empty()) {
        for (int m : p_hist) counts[m]++;
        fav = 1;
        for (auto const& [key, val] : counts) if (val > counts[fav]) fav = key;
        cout << "Favorite item: " << get_choice_name(fav) << endl;
    }

    // Достижения
    cout << "\n\033[1;38;2;255;165;0mAchievements:\033[0m" << endl;
    bool win = (p_wins == 3);

    struct Ach { string name; string cond; bool earned; };
    vector<Ach> achs = {
        {"Ace", "(win without losses)", win && ai_wins == 0},
        {"Triple Strike", "(win in 3 rounds)", win && total_rounds == 3},
        {"Quick Fight", "(win in 4-5 rounds)", win && total_rounds >= 4 && total_rounds <= 5},
        {"Long Battle", "(win in 12+ rounds)", win && total_rounds >= 12},
        {"Clutch", "(win from 0:2 score)", win && p_wins == 0 && ai_wins == 2},
        {"Death", "(lost game)", !win},
        {"Fast Death", "(lost in 5 or less rounds)", !win && total_rounds <= 5},
        {"Sword Master", "(fav: Sword, win)", win && fav == 1},
        {"Shield Master", "(fav: Shield, win)", win && fav == 2},
        {"Rich Thief", "(fav: Money, win)", win && fav == 3}
    };


    for (auto const& a : achs) {
        if (a.earned) cout << "\033[1;32m* " << a.name << " " << a.cond << "\033[0m" << endl;
        else cout << "\033[1;31m* " << a.name << " (???)\033[0m" << endl;
    }
}

int main() {
    srand(time(0));
    int p_wins = 0, ai_wins = 0, difficulty, total_rounds = 0;
    string name, input;
    vector<int> p_hist;

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
            // Цвета в строке выбора
            cout << "Choose: 1.\033[1;31mSword\033[0m, 2.\033[1;34mShield\033[0m, 3.\033[1;32mMoney\033[0m (or 'srndr'): ";
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
        p_hist.push_back(p_choice);
    }
end_game:
    show_final_screen((p_wins == 3 ? name : "AI"), p_wins, ai_wins, total_rounds, p_hist, name);
    return 0;
}