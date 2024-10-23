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
const int THRESHOLD = 3;
Coordinats maping_table[MAP_TABLE_SIZE];
const bool USE_CLOCK = true;
int side;
int curr_threshold = 0;
int next_threshold = 999;

struct BoardState
{
	std::vector<Ways> moves;
	std::vector<uint16_t> board;
	Coordinats board_empty_place = Coordinats(0, 0);
	uint16_t manhat_score = 0;

	void print_winner(std::chrono::high_resolution_clock::time_point& start) {
		if (USE_CLOCK)
		{
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed = end - start;
			std::cout << std::fixed << std::setprecision(2);
			std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
		}
		std::cout << moves.size() << "\n";
		for (int j = 0; j < moves.size(); j++)
		{
			switch (moves[j])
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

	bool produce_new_states(std::chrono::high_resolution_clock::time_point& start)
	{
		// UP
		if (this->board_empty_place.x < side - 1 && (this->moves.size() == 0 || (this->moves.size() > 0 && this->moves[this->moves.size() - 1] != Ways::DOWN)))
		{
			BoardState up = BoardState();
			up = deep_copy();
			up.board[board_empty_place.x * side + board_empty_place.y] = board[(board_empty_place.x + 1) * side + board_empty_place.y];
			up.board[(board_empty_place.x + 1) * side + board_empty_place.y] = 0;
			up.board_empty_place.x = board_empty_place.x + 1;
			up.moves.push_back(Ways::UP);
			up.manhat_score = up.get_manhatan_score();
			if (up.manhat_score - up.moves.size() == 0) {
				up.print_winner(start);
				return true;
			}
			if (up.manhat_score <= curr_threshold) {
				if (up.produce_new_states(start)) {
					return true;
				}
			}
			else {
				next_threshold = next_threshold < up.manhat_score ? next_threshold : up.manhat_score;
			}
		}

		// DOWN
		if (this->board_empty_place.x > 0 && (this->moves.size() == 0 || (this->moves.size() > 0 && this->moves[this->moves.size() - 1] != Ways::UP)))
		{
			BoardState down = BoardState();
			down = deep_copy();
			down.board[board_empty_place.x * side + board_empty_place.y] = board[(board_empty_place.x - 1) * side + board_empty_place.y];
			down.board[(board_empty_place.x - 1) * side + board_empty_place.y] = 0;
			down.board_empty_place.x = board_empty_place.x - 1;
			down.moves.push_back(Ways::DOWN);
			down.manhat_score = down.get_manhatan_score();
			if (down.manhat_score - down.moves.size() == 0) {
				down.print_winner(start);
				return true;
			}
			if (down.manhat_score <= curr_threshold) {
				if (down.produce_new_states(start)) {
					return true;
				}
			}
			else {
				next_threshold = next_threshold < down.manhat_score ? next_threshold : down.manhat_score;
			}

		}

		// LEFT
		if (this->board_empty_place.y < side - 1 && (this->moves.size() == 0 || (this->moves.size() > 0 && this->moves[this->moves.size() - 1] != Ways::RIGHT)))
		{
			BoardState left = BoardState();
			left = deep_copy();
			left.board[board_empty_place.x * side + board_empty_place.y] = board[board_empty_place.x * side + board_empty_place.y + 1];
			left.board[board_empty_place.x * side + board_empty_place.y + 1] = 0;
			left.board_empty_place.y = board_empty_place.y + 1;
			left.moves.push_back(Ways::LEFT);
			left.manhat_score = left.get_manhatan_score();
			if (left.manhat_score - left.moves.size() == 0) {
				left.print_winner(start);
				return true;
			}
			if (left.manhat_score <= curr_threshold) {
				if (left.produce_new_states(start)) {
					return true;
				}
			}
			else {
				next_threshold = next_threshold < left.manhat_score ? next_threshold : left.manhat_score;
			}
		}

		// RIGHT
		if (this->board_empty_place.y > 0 && (this->moves.size() == 0 || (this->moves.size() > 0 && this->moves[this->moves.size() - 1] != Ways::LEFT)))
		{
			BoardState right = BoardState();
			right = deep_copy();
			right.board[board_empty_place.x * side + board_empty_place.y] = board[board_empty_place.x * side + board_empty_place.y - 1];
			right.board[board_empty_place.x * side + board_empty_place.y - 1] = 0;
			right.board_empty_place.y = board_empty_place.y - 1;
			right.moves.push_back(Ways::RIGHT);
			right.manhat_score = right.get_manhatan_score();
			if (right.manhat_score - right.moves.size() == 0) {
				right.print_winner(start);
				return true;
			}
			if (right.manhat_score <= curr_threshold) {
				if (right.produce_new_states(start)) {
					return true;
				}
			}
			else {
				next_threshold = next_threshold < right.manhat_score ? next_threshold : right.manhat_score;
			}
		}
		return false;
	}
};

bool check_if_possible(const BoardState& starting_state)
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

	while (true)
	{
		if (starting_state.produce_new_states(start)) {
			return 0;
		}
		else {
			curr_threshold = next_threshold;
			next_threshold = 999;
		}

	}
	std::cout << "How are you here???\n";
	return 0;
}
