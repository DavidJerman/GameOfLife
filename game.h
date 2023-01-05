//
// Created by pinkynyte on 1/4/23.
//

#ifndef GAMEOFLIFE_GAME_H
#define GAMEOFLIFE_GAME_H

#include <random>
#include "constants.h"
#include "olcPixelGameEngine.h"

class game : public olc::PixelGameEngine {
public:
    game();

    void setBorders(bool _borders);

    void setClassicMode(bool _classicMode);

private:
    bool OnUserCreate() override;

    bool OnUserUpdate(float fElapsedTime) override;

    bool OnConsoleCommand(const std::string &command) override;

    bool calculateNewState();

    void newState();

    void clearState();

    void pauseSimulation();

    void updateBoard();

    void overwriteGrid();

    void setRandomizationChance(float chance);

    float getRandomizationChance() const;

    bool addCells();

    bool removeCells();

    void loadConfig(const std::string& configPath);

    bool parseCommand(const std::string &command);

    bool grid[ROWS][COLS]{};
    bool next[ROWS][COLS]{};
    bool paused = false;
    float randomizeChance = 0.5f;
    bool classicMode = true;
    bool border = true;

    // Rand
    std::random_device rd;
    std::mt19937 rng;
    std::shared_ptr<std::uniform_int_distribution<std::mt19937::result_type>> dist;
};


#endif //GAMEOFLIFE_GAME_H
