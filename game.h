//
// Created by pinkynyte on 1/4/23.
//

#ifndef GAMEOFLIFE_GAME_H
#define GAMEOFLIFE_GAME_H

#include "constants.h"
#include "olcPixelGameEngine.h"

class game : public olc::PixelGameEngine {
public:
    game();

public:
    bool OnUserCreate() override;

    bool OnUserUpdate(float fElapsedTime) override;

    bool calculateNewState();

private:
    bool grid[ROWS][COLS]{};
    bool next[ROWS][COLS]{};
    bool paused = false;

    void newState();

    void clearState();

    void pauseSimulation();

    void updateBoard();

    void overwriteGrid();
};


#endif //GAMEOFLIFE_GAME_H
