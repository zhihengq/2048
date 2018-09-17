#include <ncurses.h>
#include <cstdint>
#include <iostream>
#include <string>
#include <stdexcept>
#include <thread>
#include <chrono>

#include "game.h"
#include "ui/ncurses_viewer.h"
#include "ai/random_generator.h"
#include "ai/random_player.h"

namespace {

inline void init(_2048::Game &game) {
    game.Reset(_2048::GameState(10, 10));
    for (uint32_t i = 0; i < 2; i++)
        game.Generate();
}

// return true if the user choose to quit
// return false otherwise
bool ProcessMenu(int key, _2048::Game &game, bool &pause) {
    using _2048::ui::NcursesViewer;
    switch (key) {
        case 'q':
            return true;
        case 'r':
            init(game);
            return false;
        case 'p':
            pause = !pause;
            NcursesViewer::instance().SetStatusLine(pause ? U"Pause" : U"");
            nodelay(stdscr, pause ? FALSE : TRUE);
            return false;
        default:
            return false;
    }
}

}  // namespace

int main(int argc, char **argv) {
    // parse cycle
    uint32_t cycle = 1000;
    if (argc > 1) {
        try {
            cycle = std::stoul(argv[1]);
        } catch (const std::logic_error &e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }
    }

    using _2048::Game;
    using _2048::ai::RandomGenerator;
    using _2048::ai::RandomPlayer;
    using _2048::ui::NcursesViewer;

    RandomGenerator generator;
    RandomPlayer player;
    NcursesViewer &view = NcursesViewer::instance();
    Game game(&view, &generator, &player);

    init(game);

    // take turns
    bool player_turn = true;
    bool pause = true;
    view.SetStatusLine(U"Pause");
    while (true) {
        int key = getch();
        if (ProcessMenu(key, game, pause))
            return 0;
        if (!pause) {
            if (player_turn) {
                std::this_thread::sleep_for(
                        std::chrono::milliseconds(cycle * 4 / 5));
                if (game.Play())
                    player_turn = false;
            } else {
                std::this_thread::sleep_for(
                        std::chrono::milliseconds(cycle * 1 / 5));
                game.Generate();
                player_turn = true;
            }
        }
    }

    return 0;
}
