#include <iostream>
#include <Math.h>
#include <vector>
#include <cstdint>
#include <queue>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <set>
#include <unordered_map>

enum Ways
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

struct Coordinats
{
	uint8_t x;
	uint8_t y;

	Coordinats(uint8_t i, uint8_t j) : x(i), y(j) {}
	Coordinats() {}
};

int final_empty_place;
const int MAP_TABLE_SIZE = 128;
const int WAYS_VECTOR_RESERVE = 40;
const int THRESHOLD = 5;
Coordinats maping_table[MAP_TABLE_SIZE];
const bool USE_CLOCK = true;
int side;

struct BoardState
{
	std::vector<Ways> moves;
	std::vector<uint16_t> board;
	Coordinats board_empty_place = Coordinats(0, 0);
	uint16_t manhat_score = 0;

	BoardState deep_copy() const
	{
		return *this;
	}

	int get_manhatan_score() const
	{
		int sum = 0;
		bool jump_one = false;
		int size = side * side;
		for (int i = 0; i < size; i++)
		{
			if (board[i] == 0)
				continue;
			int current_y = (i >= side) ? i % side : i;
			sum += abs(i / side - maping_table[board[i]].x) + abs(current_y - maping_table[board[i]].y);
		}
		return sum + moves.size();
	}

	std::vector<BoardState *> produce_new_states()
	{
		std::vector<BoardState *> next_generation;

		// UP
		if (this->board_empty_place.x < side - 1 && (this->moves.size() == 0 || (this->moves.size() > 0 && this->moves[this->moves.size() - 1] != Ways::DOWN)))
		{
			BoardState *up = new BoardState();
			*up = deep_copy();
			up->board[board_empty_place.x * side + board_empty_place.y] = board[(board_empty_place.x + 1) * side + board_empty_place.y];
			up->board[(board_empty_place.x + 1) * side + board_empty_place.y] = 0;
			up->board_empty_place.x = board_empty_place.x + 1;
			up->moves.push_back(Ways::UP);
			up->manhat_score = up->get_manhatan_score();
			next_generation.push_back(up);
		}

		// DOWN
		if (this->board_empty_place.x > 0 && (this->moves.size() == 0 || (this->moves.size() > 0 && this->moves[this->moves.size() - 1] != Ways::UP)))
		{
			BoardState *down = new BoardState();
			*down = deep_copy();
			down->board[board_empty_place.x * side + board_empty_place.y] = board[(board_empty_place.x - 1) * side + board_empty_place.y];
			down->board[(board_empty_place.x - 1) * side + board_empty_place.y] = 0;
			down->board_empty_place.x = board_empty_place.x - 1;
			down->moves.push_back(Ways::DOWN);
			down->manhat_score = down->get_manhatan_score();
			next_generation.push_back(down);
		}

		// LEFT
		if (this->board_empty_place.y < side - 1 && (this->moves.size() == 0 || (this->moves.size() > 0 && this->moves[this->moves.size() - 1] != Ways::RIGHT)))
		{
			BoardState *left = new BoardState();
			*left = deep_copy();
			left->board[board_empty_place.x * side + board_empty_place.y] = board[board_empty_place.x * side + board_empty_place.y + 1];
			left->board[board_empty_place.x * side + board_empty_place.y + 1] = 0;
			left->board_empty_place.y = board_empty_place.y + 1;
			left->moves.push_back(Ways::LEFT);
			left->manhat_score = left->get_manhatan_score();
			next_generation.push_back(left);
		}

		// RIGHT
		if (this->board_empty_place.y > 0 && (this->moves.size() == 0 || (this->moves.size() > 0 && this->moves[this->moves.size() - 1] != Ways::LEFT)))
		{
			BoardState *right = new BoardState();
			*right = deep_copy();
			right->board[board_empty_place.x * side + board_empty_place.y] = board[board_empty_place.x * side + board_empty_place.y - 1];
			right->board[board_empty_place.x * side + board_empty_place.y - 1] = 0;
			right->board_empty_place.y = board_empty_place.y - 1;
			right->moves.push_back(Ways::RIGHT);
			right->manhat_score = right->get_manhatan_score();
			next_generation.push_back(right);
		}

		return next_generation;
	}
};

class Compare
{
public:
	bool operator()(const BoardState &first, const BoardState &second)
	{
		return first.manhat_score > second.manhat_score;
	}
};

bool check_if_possible(const BoardState &starting_state)
{
	int inversion = 0;

	for (int i = 0; i < side * side - 1; i++)
	{
		for (int j = i + 1; j < side * side; j++)
		{
			if (starting_state.board[i] > starting_state.board[j])
			{
				if (starting_state.board[i] == 0 || starting_state.board[j] == 0)
				{
					continue;
				}
				if (starting_state.board[i] > starting_state.board[j])
				{
					inversion++;
				}
			}
		}
	}

	if (side % 2 == 0)
	{ // Even side
		if ((inversion + starting_state.board_empty_place.x) % 2 == 0)
		{
			return false;
		}
	}
	else
	{ // Odd side
		if (inversion % 2 != 0)
		{
			return false;
		}
	}
	return true;
}

void map_table()
{
	int counter = 1;
	bool seen = false;
	for (uint8_t i = 0; i < side; i++)
	{
		for (uint8_t j = 0; j < side; j++)
		{
			if (!seen && counter == final_empty_place + 1)
			{
				maping_table[0] = Coordinats(i, j);
				seen = true;
				counter--;
			}
			else
			{
				maping_table[counter] = Coordinats(i, j);
			}
			counter++;
		}
	}
}

void print_winner(const BoardState &state, const std::chrono::high_resolution_clock::time_point& start)
{
	if (USE_CLOCK)
	{
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = end - start;
		std::cout << std::fixed << std::setprecision(2);
		std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
	}
	std::cout << state.moves.size() << "\n";
	for (int j = 0; j < state.moves.size(); j++)
	{
		switch (state.moves[j])
		{
		case Ways::UP:
			std::cout << "up\n";
			break;
		case Ways::DOWN:
			std::cout << "down\n";
			break;
		case Ways::LEFT:
			std::cout << "left\n";
			break;
		case Ways::RIGHT:
			std::cout << "right\n";
			break;
		}
	}
}

int main()
{
	int piece_count = 0;
	std::cin >> piece_count;
	std::cin >> final_empty_place;

	side = sqrt(piece_count + 1);

	BoardState starting_state = BoardState();
	starting_state.moves.reserve(WAYS_VECTOR_RESERVE);
	starting_state.board.reserve(piece_count + 1);

	for (uint8_t i = 0; i < side; i++)
	{
		for (uint8_t j = 0; j < side; j++)
		{
			int number = -1;
			std::cin >> number;
			if (number == 0)
			{
				starting_state.board_empty_place = Coordinats(i, j);
			}
			starting_state.board.push_back(number);
		}
	}
	auto start = std::chrono::high_resolution_clock::now();
	map_table();

	if (!check_if_possible(starting_state))
	{
		std::cout << "-1\n";
		if (USE_CLOCK)
		{
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed = end - start;
			std::cout << std::fixed << std::setprecision(2);
			std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
		}
		return 0;
	}

	starting_state.manhat_score = starting_state.get_manhatan_score();
	if (starting_state.manhat_score == 0)
	{
		std::cout << "0\n";
		if (USE_CLOCK)
		{
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed = end - start;
			std::cout << std::fixed << std::setprecision(4);
			std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
		}
		return 0;
	}

	std::priority_queue<BoardState, std::vector<BoardState>, Compare> star_queue;
	uint16_t threshold = THRESHOLD;
	uint16_t next_threshold = THRESHOLD + 1;

	while (true)
	{
		star_queue.push(starting_state);

		while (!star_queue.empty())
		{
			BoardState current_state = star_queue.top();
			std::vector<BoardState*> result = current_state.produce_new_states();

			for (int i = 0; i < result.size(); i++)
			{
				uint16_t manhat_score = result[i]->manhat_score;
				if (manhat_score - result[i]->moves.size() == 0)
				{
					print_winner(*result[i], start);
					for (BoardState* state : result) {
						delete state;
					}
					return 0;
				}
				else if (manhat_score <= threshold)
				{
					star_queue.push(*result[i]);
				}
				else
				{
					next_threshold = std::max(next_threshold, manhat_score);
				}
			}
			for (BoardState* state : result) {
				delete state;
			}
			star_queue.pop();
		}
		threshold = next_threshold;
	}

	std::cout << "How are you here???\n";
	return 0;
}
