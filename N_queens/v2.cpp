#include <iostream>
#include <cstdint>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>

const int RESTART = 10;

std::vector<int> nQueens;
std::vector<int> queens_per_row;
std::vector<int> queens_per_left_diagonal;
std::vector<int> queens_per_right_diagonal;
std::vector<int> current_max_conflict_cols;
std::vector<int> current_min_conflict_rows;

int queens_count;

int get_max_conflict_col() {

    int max_conf = 0;

    current_max_conflict_cols.clear();

    for (int i = 0; i < queens_count; i++) {
        int row = nQueens[i];
        int col = i;
        int curr_conf = queens_per_row[row];
        curr_conf += queens_per_left_diagonal[col + row];
        curr_conf += queens_per_right_diagonal[col - row + queens_count - 1];

        if (curr_conf > max_conf) {
            max_conf = curr_conf;
            current_max_conflict_cols.clear();
            current_max_conflict_cols.push_back(i);
        }
        else if (curr_conf == max_conf) {
            current_max_conflict_cols.push_back(i);
        }
    }

    if (max_conf == 3) {
        return -1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, current_max_conflict_cols.size() - 1);
    int random_index = dis(gen);

    return current_max_conflict_cols[random_index];
}

int get_min_conflict_by_row(int col, int prev_row) {

    int min_conf = INT32_MAX;

    current_min_conflict_rows.clear();

    for (int row = 0; row < queens_count; row++) {

        int curr_row_conf = queens_per_row[row];
        curr_row_conf += queens_per_left_diagonal[col + row];
        curr_row_conf += queens_per_right_diagonal[col - row + queens_count - 1];
        
        
        if (row == prev_row) {
            curr_row_conf -= 3;
        }

        if (curr_row_conf < min_conf) {
            min_conf = curr_row_conf;
            current_min_conflict_rows.clear();
            current_min_conflict_rows.push_back(row);
        }
        else if (curr_row_conf == min_conf) {
            current_min_conflict_rows.push_back(row);
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, current_min_conflict_rows.size() - 1);

    return current_min_conflict_rows[dis(gen)];
}

bool is_solved() {

    for (int i = 0; i < queens_count; i++) {
        if (queens_per_row[i] != 1) {
            return false;
        }
    }
    for (int i = 0; i < queens_count * 2; i++) {
        if (queens_per_left_diagonal[i] > 1 || queens_per_right_diagonal[i] > 1) {
            return false;
        }
    }

    return true;
}

void init() {
    nQueens.clear();
    queens_per_left_diagonal.clear();
    queens_per_right_diagonal.clear();
    queens_per_row.clear();

    nQueens.resize(queens_count);
    queens_per_left_diagonal.resize(2*queens_count);
    queens_per_right_diagonal.resize(2*queens_count);
    queens_per_row.resize(queens_count);

    for (int i = 0; i < queens_count; i++) {
        nQueens[i] = i;
        queens_per_row[i] = 1;
    }

    for (int i = 0; i < queens_count * 2; i += 2) {
        queens_per_left_diagonal[i] = 1;
    }
    queens_per_right_diagonal[queens_count - 1] = queens_count;

}

void solve() {

    init();
    for (int i = 0; i < queens_count * RESTART; i++) {

        int col = get_max_conflict_col();

        if (col == -1) {
            return;
        }

        int prev_row = nQueens[col];
        int row = get_min_conflict_by_row(col, prev_row);

        nQueens[col] = row;

        queens_per_row[prev_row]--;
        queens_per_left_diagonal[col + prev_row]--;
        queens_per_right_diagonal[col - prev_row + queens_count - 1]--;

        queens_per_row[row]++;
        queens_per_left_diagonal[col + row]++;
        queens_per_right_diagonal[col - row + queens_count - 1]++;
        
        /*
        for (int i = 0; i < queens_count; i++) {
            if (nQueens[i] < 10) std::cout << " ";
            std::cout << nQueens[i] << " ";
        }
        std::cout << "\n";
        */
    }

    solve();
}


int main() {

    std::cin >> queens_count;

    if (queens_count == 1) {
        std::cout << "[0]\n";
        return 0;
    }

    if (queens_count == 2 || queens_count == 3) {
        std::cout << "-1\n";
        return 0;
    }

    nQueens.resize(queens_count);
    queens_per_left_diagonal.resize(queens_count * 2 + 1);
    queens_per_right_diagonal.resize(queens_count * 2 + 1);
    queens_per_row.resize(queens_count);

    auto start = std::chrono::high_resolution_clock::now();
    solve();

    if (queens_count <= 100) {
        std::cout << "[";
        for (int i = 0; i < queens_count - 1; i++) {
            std::cout << nQueens[i] << ", ";
        }
        std::cout << nQueens[queens_count - 1];
        std::cout << "]";
    }
    else {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << elapsed.count() << "\n";
    }

    return 0;
}