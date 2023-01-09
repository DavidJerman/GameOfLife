//
// Created by pinkynyte on 1/4/23.
//

#ifndef GAMEOFLIFE_GAME_H
#define GAMEOFLIFE_GAME_H

#include <random>
#include <vector>
#include "constants.h"
#include "olcPixelGameEngine.h"

class game : public olc::PixelGameEngine {
public:
    game();

private:
    // Inherited via PixelGameEngine
    bool OnConsoleCommand(const std::string &command) override;

    bool OnUserCreate() override;

    bool OnUserUpdate(float fElapsedTime) override;

    // Game logic
    bool addCells();

    bool calculateNewState();

    void decreaseGameSpeed();

    void increaseGameSpeed();

    void newState();

    void pauseSimulation(bool mute);

    bool removeCells();

    // Utility
    void copyTemp();

    float getRandomizationChance() const;

    bool parseCommand(const std::string &command);

    void saveOldState();

    void setBorders(bool _borders);

    void setClassicMode(bool _classicMode);

    void setRandomizationChance(float chance);

    bool setGameSpeed(unsigned int speed);

    // I/O
    void loadConfig(const std::string &configPath);

    void saveState(const std::string &path);

    void loadState(const std::string &path);

    // Drawing
    void clearState();

    void fullUpdateBoard();

    void updateBoard();

    // I/O
    bool ioWorking = false;

    // Game state
    bool grid[ROWS][COLS]{};
    bool temp[ROWS][COLS]{};
    bool prev[ROWS][COLS]{};
    bool paused = false;
    float randomizeChance = 0.5f;
    bool classicMode = true;
    bool border = true;
    std::vector<unsigned int> gameSpeedsMs = {1000, 500, 250, 100, 50, 25, 10, 5, 1, 0};
    int gameSpeed = 0;

    // Rand
    std::random_device rd;
    std::mt19937 rng;
    std::shared_ptr<std::uniform_int_distribution<std::mt19937::result_type>> dist;

    // Colors
    olc::Pixel deadCellColor = olc::BLACK;
    olc::Pixel aliveCellColor = olc::WHITE;

    // Algorithm
    bool parseAlgo(const std::string &algorithm);

    bool birth[9]{};
    bool survival[9]{};
};


#endif //GAMEOFLIFE_GAME_H
