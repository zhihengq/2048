#include <ncurses.h>
#include <cstdint>
#include <thread>
#include <chrono>

#include "game.h"
#include "ui/ncurses_controller.h"
#include "ai/random_generator.h"

namespace {

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

}  // namespace

int main() {
    using _2048::Game;
    using _2048::ai::RandomGenerator;
    using _2048::ui::NcursesController;

    RandomGenerator generator;
    NcursesController &control = NcursesController::instance();
    Game game(&control, &generator, &control);

    init(game);

    // take turns
    bool player_turn = true;
    while (true) {
        if (player_turn) {
            if (game.Play())
                player_turn = false;
            else if (ProcessMenu(control.GetLastKey(), game))
                return 0;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            flushinp();     // throw away unread keys during wait
            game.Generate();
            player_turn = true;
        }
    }

    return 0;
}
