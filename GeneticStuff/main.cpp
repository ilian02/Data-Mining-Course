#include <iostream>
#include <vector>
#include <queue>
#include <stdlib.h>
#include <time.h>

int max_kilos = 0;
int item_count = 0;
const int TOTAL_GENERATIONS = 100;
const int TOTAL_GENERATION_ITEMS = 30;

int items_weights[10000];
int items_prices[10000];

/*
struct Item {
    int weight;
    int price;
};
*/

struct State {
    std::vector<bool> items;
    int current_weight;
    int current_price;

    State(int size) {
        items.resize(size);
    }

    State() {}
};


std::vector<State> current_states(TOTAL_GENERATION_ITEMS);

void init_states() {
    srand (time(NULL));
    for (int i = 0; i < TOTAL_GENERATION_ITEMS; i++) {
        std::cout << "Filling " << i << "\n";
        State state = State(item_count);
        for (int j = 0; j < item_count; j++) {
            int rand_index = rand() % item_count;
            if (state.current_weight + items_weights[rand_index] <= max_kilos) {
                state.items[rand_index] = true;
                state.current_weight += items_weights[rand_index];
                state.current_price += items_prices[rand_index];
            }
        }
        current_states[i] = state;
    }
}

void simulate() {

}

int main() {

    std::cin >> max_kilos >> item_count;

    for(int i = 0; i < item_count; i++) {
        std::cin >> items_weights[i] >> items_prices[i];
    }
    
    init_states();

    
    for (int i = 0; i < TOTAL_GENERATION_ITEMS; i++) {
        for (int j = 0; j < item_count; j++) {
            std::cout << current_states[i].items[j] << " ";
        }
        std::cout << '\n';
    }
    

    return 0;
}
