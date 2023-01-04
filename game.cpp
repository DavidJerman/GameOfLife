//
// Created by pinkynyte on 1/4/23.
//

#include "game.h"

game::game() {
    sAppName = "Game Of Life";
    for (auto &row: grid)
        for (auto &cell: row)
            cell = false;
    for (auto &row: next)
        for (auto &cell: row)
            cell = false;
}


bool game::OnUserCreate() {
    // Called once at the start, so create things here
    // Seed the grid with random values
    for (auto &row: grid)
        for (auto &cell: row)
            if (rand() % 100 < randomizeChance * 100)
                cell = true;
    ConsoleCaptureStdOut(true);
    return true;
}


bool game::OnUserUpdate(float fElapsedTime) {
    // called once per frame
    if (!paused && !IsConsoleShowing()) {
        // If R is pressed, randomize the grid
        if (GetKey(olc::Key::R).bPressed)
            newState();

        // If C is pressed, clear the grid
        if (GetKey(olc::Key::C).bPressed)
            clearState();

        // If mouse clicked
        if (GetMouse(0).bHeld) {
            return addCells();
        }

        if (GetKey(olc::Key::N).bPressed && !classicMode) {
            calculateNewState();
            updateBoard();
            overwriteGrid();
        }
    }

    // If space is pressed, pause the simulation
    if (!IsConsoleShowing())
        if (GetKey(olc::Key::SPACE).bPressed)
            pauseSimulation();

    // Show console
    if (GetKey(olc::Key::TAB).bPressed)
        ConsoleShow(olc::Key::TAB, false);

    // If paused, continue to the next frame
    if (paused)
        return true;

    // Tick
    if (classicMode)
        calculateNewState();

    // Update the board
    updateBoard();

    // If calculateNewState, assign next to grid
    if (classicMode)
        overwriteGrid();

    return true;
}

bool game::addCells() {// Get the mouse position
    auto mousePos = GetMousePos();
    // Get the cell position
    next[mousePos.x][mousePos.y] = true;
    updateBoard();
    overwriteGrid();
    // Skip the rest of the loop
    return true;
}


void game::overwriteGrid() {
    std::memcpy(grid, next, sizeof(grid));
}


void game::updateBoard() {
    for (int x = 0; x < ScreenWidth(); x++)
        for (int y = 0; y < ScreenHeight(); y++)
            if (next[x][y] != grid[x][y])
                Draw(x, y, next[x][y] ? olc::WHITE : olc::BLACK);
}


void game::pauseSimulation() {
    paused = !paused;
    if (paused) {
        DrawString(0, 0, "Paused", olc::BLUE, 1);
        std::cout << "Paused the simulation" << std::endl;
    } else {
        Clear(olc::BLACK);
        std::cout << "Resumed the simulation" << std::endl;
        // Draw the grid
        for (int x = 0; x < ROWS; x++)
            for (int y = 0; y < COLS; y++)
                if (next[x][y])
                    Draw(x, y, next[x][y] ? olc::WHITE : olc::BLACK);
    }
}


void game::clearState() {
    for (auto &row: grid)
        for (auto &cell: row)
            cell = false;
    for (auto &row: next)
        for (auto &cell: row)
            cell = false;
    // Clear the screen
    Clear(olc::BLACK);
}


void game::newState() {
    clearState();
    for (auto &row: grid)
        for (auto &cell: row)
            if (rand() % 100 < randomizeChance * 100)
                cell = true;
}


bool game::calculateNewState() {
    // Calculate the next board - V1
    for (int x = 0; x < ScreenWidth(); x++) {
        for (int y = 0; y < ScreenHeight(); y++) {
            int neighbors = 0;
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    if (i == 0 && j == 0)
                        continue;
                    if (x + i < 0 || x + i >= ScreenWidth() || y + j < 0 || y + j >= ScreenHeight())
                        continue;
                    if (grid[x + i][y + j])
                        neighbors++;
                }
            }
            if (grid[x][y]) {
                if (neighbors == 2 || neighbors == 3)
                    next[x][y] = true;
                else
                    next[x][y] = false;
            } else {
                if (neighbors == 3)
                    next[x][y] = true;
                else
                    next[x][y] = false;
            }
        }
    }
    return true;
}


void game::setRandomizationChance(float chance) {
    if (chance < 0 || chance > 100)
        return;
    randomizeChance = chance / 100;
}

bool game::OnConsoleCommand(const std::string &command) {
    std::stringstream ss(command);
    std::string cmd;
    ss >> cmd;
    if (cmd == "set") {
        std::string var;
        ss >> var;
        if (var == "rand") {
            float chance;
            ss >> chance;
            setRandomizationChance(chance);
            std::cout << "Randomization chance set to " << getRandomizationChance() << std::endl;
            return true;
        }
        if (var == "mode") {
            std::string mode;
            ss >> mode;
            if (mode == "classic") {
                setClassicMode(true);
                std::cout << "Mode set to classic" << std::endl;
                return true;
            }
            if (mode == "stepped") {
                setClassicMode(false);
                std::cout << "Mode set to to step by step" << std::endl;
                return true;
            }
        }
    }
    if (cmd == "randomize" || cmd == "rand" || cmd == "r") {
        newState();
        std::cout << "Randomized the grid" << std::endl;
        return true;
    }
    if (cmd == "clear" || cmd == "c") {
        clearState();
        std::cout << "Cleared the grid" << std::endl;
        return true;
    }
    if (cmd == "pause" || cmd == "p") {
        pauseSimulation();
        return true;
    }
    if (cmd == "next" || cmd == "n") {
        calculateNewState();
        updateBoard();
        overwriteGrid();
        std::cout << "Calculated the next state" << std::endl;
        return true;
    }
    if (cmd == "help" || cmd == "h") {
        std::cout << "Available commands:" << std::endl;
        std::cout << "set rand <chance>              > Sets the randomization chance (0-100)" << std::endl;
        std::cout << "randomize | rand | r           > Randomizes the grid" << std::endl;
        std::cout << "clear | c                      > Clears the grid" << std::endl;
        std::cout << "pause | p                      > Pauses/resumes the simulation" << std::endl;
        std::cout << "next | n                       > Next step/generation" << std::endl;
        std::cout << "help | h                       > Shows this help message" << std::endl;
        return true;
    }
    return false;
}

float game::getRandomizationChance() const {
    return randomizeChance;
}

void game::setClassicMode(bool _classicMode) {
    game::classicMode = _classicMode;
}
