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
            cell = rand() % 2;
    return true;
}


bool game::OnUserUpdate(float fElapsedTime) {
    // If n is pressed, randomize the grid
    if (GetKey(olc::Key::N).bPressed)
        newState();

    // If c is pressed, clear the grid
    if (GetKey(olc::Key::C).bPressed)
        clearState();

    // If space is pressed, pause the simulation
    if (GetKey(olc::Key::SPACE).bPressed)
        pauseSimulation();

    // If paused, continue to the next frame
    if (paused)
        return true;

    // Tick
    auto tickRes = calculateNewState();

    // Update the board
    updateBoard();

    // If calculateNewState, assign next to grid
    if (tickRes)
        overwriteGrid();

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
    if (paused)
        DrawString(0, 0, "Paused", olc::BLUE, 1);
    else {
        Clear(olc::BLACK);
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
    for (auto &row: grid)
        for (auto &cell: row)
            cell = rand() % 2;
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
