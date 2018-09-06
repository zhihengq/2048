#include <cstdint>
#include <thread>
#include <chrono>
#include <ncurses.h>
#include "game.h"
#include "ncurses_controller.h"
#include "random_generator.h"

inline void init(_2048::Game &game) {
    game.Reset(_2048::GameState(4, 4));
    for (uint32_t i = 0; i < 2; i++)
        game.Generate();
}

// return true if the user choose to quit
// return false otherwise
bool ProcessMenu(int key, _2048::Game &game) {
    switch (key) {
        case 'q':
            return true;
        case 'r':
            init(game);
            return false;
        default:
            return false;
    }
}

int main() {
    _2048::RandomGenerator generator;
    _2048::NcursesController &control = _2048::NcursesController::instance();
    _2048::Game game(&control, &generator, &control);

    init(game);

    // take turns
    bool player_turn = true;
    while (true) {
        if (player_turn) {
            if (game.Play())
                player_turn = false;
            else if (ProcessMenu(control.GetLastKey(), game))
                return 0;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        } else {
            game.Generate();
            player_turn = true;
        }
    }

    return 0;
}
