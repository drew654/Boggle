#include "boggle.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>

using std::cout, std::endl;
using std::string, std::vector, std::pair;
using std::ifstream;

boggle::boggle() {
    vector<vector<char>> vec(5, vector<char>(5));
    board = vec;
    this->shuffle();
}

void boggle::print_board() {
    cout << "┌───────────┐" << endl;
    for (unsigned int row = 0; row < board.size(); ++row) {
        cout << "│ ";
        for (unsigned int col = 0; col < board.at(row).size(); ++col) {
            cout << board.at(row).at(col) << (col < board.at(row).size()-1 ? " " : "");
        }
        cout << " │" << endl;
    }
    cout << "└───────────┘" << endl;
    cout << endl;
}

// TODO: Fix so that dice are in random spot on the board
void boggle::shuffle() {
    vector<vector<char>> dice {
        {'Q', 'B', 'Z', 'J', 'X', 'K'},
        {'T', 'O', 'U', 'O', 'T', 'O'},
        {'O', 'V', 'W', 'R', 'G', 'R'},
        {'A', 'A', 'A', 'F', 'S', 'R'},
        {'A', 'U', 'M', 'E', 'E', 'G'},
        {'H', 'H', 'L', 'R', 'D', 'O'},
        {'N', 'H', 'D', 'T', 'H', 'O'},
        {'L', 'H', 'N', 'R', 'O', 'D'},
        {'A', 'F', 'A', 'I', 'S', 'R'},
        {'Y', 'I', 'F', 'A', 'S', 'R'},
        {'T', 'E', 'L', 'P', 'C', 'I'},
        {'S', 'S', 'N', 'S', 'E', 'U'},
        {'R', 'I', 'Y', 'P', 'R', 'H'},
        {'D', 'O', 'R', 'D', 'L', 'N'},
        {'C', 'C', 'W', 'N', 'S', 'T'},
        {'T', 'T', 'O', 'T', 'E', 'M'},
        {'S', 'C', 'T', 'I', 'E', 'P'},
        {'E', 'A', 'N', 'D', 'N', 'N'},
        {'M', 'N', 'N', 'E', 'A', 'G'},
        {'U', 'O', 'T', 'O', 'W', 'N'},
        {'A', 'E', 'A', 'E', 'E', 'E'},
        {'Y', 'I', 'F', 'P', 'S', 'R'},
        {'E', 'E', 'E', 'E', 'M', 'A'},
        {'I', 'T', 'I', 'T', 'I', 'E'},
        {'E', 'T', 'I', 'L', 'I', 'C'}
    };

    srand(time(0));
    int k = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            board.at(i).at(j) = dice.at(k).at(rand() % 5);
            ++k;
        }
    }

    board = {
        {'B', 'O', 'O', 'F', 'E'},
        {'L', 'D', 'N', 'F', 'I'},
        {'C', 'S', 'P', 'D', 'W'},
        {'T', 'C', 'N', 'E', 'W'},
        {'A', 'Y', 'E', 'I', 'L'}};
}

bool boggle::is_word(string input) {
    ifstream inFS;
    inFS.open("word-list.txt");
        
    string index;
    while (!inFS.eof()) {
        getline(inFS, index);
        if (input == index) {
            return true;
        }
    }

    inFS.close();
    return false;
}

bool boggle::is_partial_word(string input) {
    ifstream inFS;
    inFS.open("word-list.txt");

    string index;
    while (!inFS.eof()) {
        getline(inFS, index);
        if (input == index.substr(0, input.size())) {
            return true;
        }
    }

    inFS.close();
    return false;
}

void boggle::find_words_at(unsigned int row, unsigned int col, vector<string> &words, string cur) {
    if (row >= board.size() || col >= board.at(0).size() || (!is_partial_word(cur) && cur != "")) {\
        return;
    }
    else {
        cur += tolower(board.at(row).at(col));
    }
    if (is_word(cur)) {
        words.push_back(cur);
    }

    cout << "cur: |" << cur << "|" << endl;

    // Sets - so that the current letter isn't used again
    char t = board.at(row).at(col);
    board.at(row).at(col) = '-';

    find_words_at(row - 1, col - 1, words, cur);
    find_words_at(row - 1, col, words, cur);
    find_words_at(row - 1, col + 1, words, cur);
    find_words_at(row, col + 1, words, cur);
    find_words_at(row + 1, col + 1, words, cur);
    find_words_at(row + 1, col, words, cur);
    find_words_at(row + 1, col - 1, words, cur);
    find_words_at(row, col - 1, words, cur);

    board.at(row).at(col) = t;

    return;
}

// TODO: Make it so that there are no duplicate words in the end
vector<string> boggle::find_all_words() {
    vector<string> words;
    for (unsigned int row = 0; row < board.size(); ++row) {
        for (unsigned int col = 0; col < board.at(0).size(); ++col) {
            cout << "starting at: " << board.at(row).at(col) << endl;
            find_words_at(row, col, words, "");
        }
    }
    return words;
}

void boggle::find_words_at2(unsigned int row, unsigned int col, vector<mapped_word> &words, mapped_word cur) {
    if (row >= board.size() || col >= board.at(0).size() || (!is_partial_word(cur.word) && cur.word != "")) {
        return;
    }
    else {
        cur.word += tolower(board.at(row).at(col));
        cur.letters.push_back({row, col});
    }
    if (is_word(cur.word)) {
        cout << "word: " << cur.word << endl;
        words.push_back(cur);
    }

    // Sets - so that the current letter isn't used again
    char t = board.at(row).at(col);
    board.at(row).at(col) = '-';

    find_words_at2(row - 1, col - 1, words, cur);
    find_words_at2(row - 1, col, words, cur);
    find_words_at2(row - 1, col + 1, words, cur);
    find_words_at2(row, col + 1, words, cur);
    find_words_at2(row + 1, col + 1, words, cur);
    find_words_at2(row + 1, col, words, cur);
    find_words_at2(row + 1, col - 1, words, cur);
    find_words_at2(row, col - 1, words, cur);

    board.at(row).at(col) = t;

    return;
}

void boggle::find_all_words2() {
    vector<mapped_word> words;
    mapped_word cur;
    for (unsigned int row = 0; row < board.size(); ++row) {
        for (unsigned int col = 0; col < board.at(row).size(); ++col) {
            cout << "starting at: " << board.at(row).at(col) << endl;
            find_words_at2(row, col, words, cur);
        }
    }
    cout << endl;

    for (auto word : words) {
        cout << word.word << endl;
        word.print_word_chart();
    }
}

void boggle::solve() {
    vector<string> s = find_all_words();
    cout << endl;
    cout << "result: " << endl;
    for (auto i : s) {
        cout << i << endl;
    }
}

void boggle::solve2() {
    find_all_words2();
}

void boggle::mapped_word::print_board(vector<vector<char>> b) {
    cout << "┌───────────┐" << endl;
        for (unsigned int row = 0; row < b.size(); ++row) {
            cout << "│ ";
            for (unsigned int col = 0; col < b.at(row).size(); ++col) {
                cout << b.at(row).at(col) << (col < b.at(row).size()-1 ? " " : "");
            }
            cout << " │" << endl;
        }
    cout << "└───────────┘" << endl;
}

vector<vector<char>> boggle::mapped_word::word_chart() {
    vector<vector<char>> b(5, vector<char>(5, ' '));
    for (unsigned int windex = 0; windex < word.size(); ++windex) {
        b.at(letters.at(windex).first).at(letters.at(windex).second) = toupper(word.at(windex));
    }
    return b;
}

void boggle::mapped_word::print_word_chart() {
    print_board(word_chart());
}

bool boggle::word_in_board(string input) {
    vector<mapped_word> words;
    mapped_word cur;
    vector<pair<int, int>> start_points;
    for (unsigned int row = 0; row < board.size(); ++row) {
        for (unsigned int col = 0; col < board.at(row).size(); ++col) {
            if (board.at(row).at(col) == toupper(input.at(0))) {
                start_points.push_back({row, col});
            }
        }
    }

    for (auto i : start_points) {
        search_for_word_at(i.first, i.second, words, cur, input);
    }

    for (auto i : words) {
        if (i.word == input) {
            return true;
        }
    }
    return false;
}

void boggle::search_for_word_at(unsigned int row, unsigned int col, vector<mapped_word> &words, mapped_word cur, string word) {
    if (row >= board.size() || col >= board.at(row).size() || (!is_partial_word(cur.word) && cur.word != "")) {
        return;
    }
    else if (!partial_word_of(cur.word, word)) {
        return;
    }
    else {
        cur.word += tolower(board.at(row).at(col));
        cur.letters.push_back({row, col});
    }
    if (is_word(cur.word)) {
        words.push_back(cur);
    }

    // Sets - so that the current letter isn't used again
    char t = board.at(row).at(col);
    board.at(row).at(col) = '-';

    search_for_word_at(row - 1, col - 1, words, cur, word);
    search_for_word_at(row - 1, col, words, cur, word);
    search_for_word_at(row - 1, col + 1, words, cur, word);
    search_for_word_at(row, col + 1, words, cur, word);
    search_for_word_at(row + 1, col + 1, words, cur, word);
    search_for_word_at(row + 1, col, words, cur, word);
    search_for_word_at(row + 1, col - 1, words, cur, word);
    search_for_word_at(row, col - 1, words, cur, word);

    board.at(row).at(col) = t;

    return;
}

bool boggle::partial_word_of(string piece, string whole) {
    if (piece == whole.substr(0, piece.size())) {
        return true;
    }
    else {
        return false;
    }
}