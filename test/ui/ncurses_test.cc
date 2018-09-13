#include "ui/ncurses_viewer.h"
#include "ui/ncurses_controller.h"

#include <iostream>
#include <string>

#include "game_state.h"
#include "game.h"
#include "viewer.h"
#include "player.h"

namespace {

using _2048::GameState;
using _2048::Game;
using _2048::Viewer;
using _2048::Player;
using _2048::ui::NcursesViewer;
using _2048::ui::NcursesController;

static void Usage(char *name) {
    std::cerr << "Usage: " << name
            << " (NcursesViewer|NcursesController)" << std::endl;
}

enum class ViewerType {
    kNcursesViewer, kNcursesController
};

static void User(Game &game, const Viewer &view, ViewerType type) {
    switch (type) {
        case ViewerType::kNcursesViewer:
            while (getch() != 'n') { }
            return;
        case ViewerType::kNcursesController:
            {
                auto &control = static_cast<const NcursesController &>(view);
                do {
                    game.Play();
                } while (control.GetLastKey() != 'n');
                return;
            }
    }
}

}  // namespace

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Not enough arguments" << std::endl;
        Usage(argv[0]);
        return 1;
    }

    Viewer *view;
    Player *player;
    ViewerType type;
    if (std::string("NcursesViewer") == argv[1]) {
        view = &NcursesViewer::instance();
        player = nullptr;
        type = ViewerType::kNcursesViewer;
    } else if (std::string("NcursesController") == argv[1]) {
        view = &NcursesController::instance();
        player = &NcursesController::instance();
        type = ViewerType::kNcursesController;
    } else {
        std::cerr << "Unrecognized Viewer: " << argv[1] << std::endl;
        Usage(argv[0]);
        return 1;
    }

    Game game(view, nullptr, player);

    // 2x2
    // [[2, 4 ],
    //  [8, 16]]
    GameState s1(2, 2);
    s1.GenerateTile(GameState::Position(0, 0), 1);
    s1.GenerateTile(GameState::Position(0, 1), 2);
    s1.GenerateTile(GameState::Position(1, 0), 3);
    s1.GenerateTile(GameState::Position(1, 1), 4);
    game.Reset(std::move(s1));
    User(game, *view, type);

    // 4x4
    // [[_,    2,  _,  4],
    //  [_,    2,  _,  _],
    //  [8,    4,  4,  _],
    //  [2048, 64, 32, _]]
    GameState s2(4, 4);
    s2.GenerateTile(GameState::Position(0, 1), 1);
    s2.GenerateTile(GameState::Position(0, 3), 2);
    s2.GenerateTile(GameState::Position(1, 1), 1);
    s2.GenerateTile(GameState::Position(2, 0), 3);
    s2.GenerateTile(GameState::Position(2, 1), 2);
    s2.GenerateTile(GameState::Position(2, 2), 2);
    s2.GenerateTile(GameState::Position(3, 0), 11);
    s2.GenerateTile(GameState::Position(3, 1), 6);
    s2.GenerateTile(GameState::Position(3, 2), 5);
    game.Reset(std::move(s2));
    User(game, *view, type);

    return 0;
}
