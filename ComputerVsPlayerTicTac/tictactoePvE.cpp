#include <iostream>
#include <vector>


enum Token {
    X,
    Y,
    None
};

struct Coords {
    int x;
    int y;
    Coords(int x, int y) { this->x = x; this->y = y; };
};

void print_state(const std::vector<std::vector<Token>>& board) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (j != 0)
                std::cout << "|";
            switch (board[i][j])
            {
            case Token::X: std::cout << " X "; break;
            case Token::Y: std::cout << " Y "; break;
            case Token::None: std::cout << "   "; break;
            }
        }
        std::cout << "\n";
    }
}

// Row is between 0, 1 or 2
bool check_row(const std::vector<std::vector<Token>>& board, int row) {
    return  (board[row][0] != Token::None && board[row][0] == board[row][1] && board[row][0] == board[row][2]);
}

// Col is between 0, 1 or 2
bool check_col(const std::vector<std::vector<Token>>& board, int col) {
    return  (board[0][col] != Token::None && board[0][col] == board[1][col] && board[0][col] == board[2][col]);
}

bool check_diagonals(const std::vector<std::vector<Token>>& board) {
    return (board[1][1] != Token::None &&
        ((board[0][0] == board[1][1] && board[0][0] == board[2][2])
            ||
            (board[0][2] == board[1][1] && board[0][2] == board[2][0])));
}

bool check_if_won(const std::vector<std::vector<Token>>& board) {
    for (int i = 0; i < 3; i++) {
        if (check_row(board, i) || check_col(board, i)) {
            return true;
        }
    }
    return check_diagonals(board);
}

int bestX, bestY;

Coords check_for_mate_in_one(std::vector<std::vector<Token>> board, Token turn, bool first_move) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == Token::None) {
                board[i][j] = turn;
                if (check_if_won(board)) {
                    return Coords(i, j);
                }
                board[i][j] = Token::None;
            }
        }
    }
    return Coords(-1, -1);
}

int compute_best_plan(std::vector<std::vector<Token>> board, Token turn, int moves_made, int alpha, int beta) {
    int best_x, best_y, best_outcome = 0;
    if (turn == Token::Y) {
        best_outcome = INT_MIN;
    }
    else {
        best_outcome = INT_MAX;
    }

    Coords mate_in_one = check_for_mate_in_one(board, turn, moves_made == 0);
    if (mate_in_one.x != -1) {
        best_x = mate_in_one.x;
        best_y = mate_in_one.y;
        if (moves_made == 0) {
            bestX = best_x;
            bestY = best_y;
        }
        if (turn == Token::Y) { 
            return 10 - moves_made;
        }
        else {
            return -1;
        }
    } 

    bool new_move = false;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == Token::None) {
                board[i][j] = turn;
                new_move = true;
                int outcome;

                // If alpha >= beta -> prune

                if (turn == Token::X) {
                    outcome = compute_best_plan(board, Token::Y, moves_made + 1, alpha, beta);
                    if (outcome < best_outcome) {
                        best_x = i;
                        best_y = j;
                        best_outcome = outcome;
                        if (best_outcome < beta)
                            beta = best_outcome;
                        if (alpha >= beta) {
                            return beta;
                        }
                    }
                }
                else if (turn == Token::Y) {
                    outcome = compute_best_plan(board, Token::X, moves_made + 1, alpha, beta);
                    if (outcome > best_outcome) {  
                        best_x = i;
                        best_y = j;
                        best_outcome = outcome;
                        if (best_outcome > alpha)
                            alpha = outcome;
                        if (alpha >= beta) {
                            return alpha;
                        }
                    }
                }
                board[i][j] = Token::None;
            }
        }
    }

    if (moves_made == 0 && new_move) {
        bestX = best_x;
        bestY = best_y;
    }

    if (new_move)
        return best_outcome;
    else
        return 0;
}

int main()
{
    std::vector<std::vector<Token>> board(3, std::vector<Token>(3, Token::None));
    std::cout << "Who starts?\n1) Player\n2) Computer\n";
    int turn;
    std::cin >> turn;

    bool gameover = false;
    int moves = 0;
    while (!gameover && moves < 9) {
        if (turn == 1) {
            int x, y;
            std::cout << "\nEnter new coords\n> ";
            std::cin >> x >> y;
            if (0 < x && 4 > x && 0 < y && 4 > y && board[x - 1][y - 1] == Token::None) {
                board[x - 1][y - 1] = Token::X;
                turn++;
                moves++;
                print_state(board);
            }
            else {
                std::cout << "Something is wrong with the given input, try again!\n";
                print_state(board);
            }
        }
        else {
            // std::cout << "Implement computer here\n";
            int alpha = INT_MIN, beta = INT_MAX;
            compute_best_plan(board, Token::Y, 0, alpha, beta);
            std::cout << "\nRobot plays " << bestX + 1 << " " << bestY + 1 << "\n";
            board[bestX][bestY] = Token::Y;
            print_state(board);
            moves++;
            turn--;
        }
        if (check_if_won(board)) {
            if (turn == 1) {
                std::cout << "\nComputer won!\n";
            }
            else {
                std::cout << "\nYou won!\n";
            }
            gameover = true;
        }
    }

    if (gameover == false) {
        std::cout << "\nStalemate!\n";
    }
    print_state(board);
}