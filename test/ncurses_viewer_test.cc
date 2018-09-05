#include <cstdint>
#include <exception>
#include <ncurses.h>
#include "game_state.h"
#include "game.h"
#include "ncurses_viewer.h"

using _2048::GameState;

int main() {
    _2048::Game game(&_2048::NcursesViewer::instance());

    // 2x2
    // [[2, 4 ],
    //  [8, 16]]
    GameState s1(2, 2);
    s1.GenerateTile(GameState::Position(0, 0), 1);
    s1.GenerateTile(GameState::Position(0, 1), 2);
    s1.GenerateTile(GameState::Position(1, 0), 3);
    s1.GenerateTile(GameState::Position(1, 1), 4);
    game.Reset(std::move(s1));
    while (KEY_RIGHT != getch()) { }

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
    while (KEY_RIGHT != getch()) { }

    return 0;
}
