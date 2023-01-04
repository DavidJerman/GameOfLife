//
// Created by pinkynyte on 1/4/23.
//

#ifndef GAMEOFLIFE_GAME_H
#define GAMEOFLIFE_GAME_H

#include "constants.h"
#include "olcPixelGameEngine.h"

class game : public olc::PixelGameEngine {
public:
    game()
    {
        sAppName = "Game Of Life";
        for (auto &row: grid)
            for (auto &cell: row)
                cell = false;
        for (auto &row: next)
            for (auto &cell: row)
                cell = false;
    }

public:
    bool OnUserCreate() override
    {
        // Called once at the start, so create things here
        // Seed the grid with random values
        for (auto &row: grid)
            for (auto &cell: row)
                cell = rand() % 2;
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // If n is pressed, randomize the grid
        if (GetKey(olc::Key::N).bPressed)
            for (auto &row: grid)
                for (auto &cell: row)
                    cell = rand() % 2;

        // If c is pressed, clear the grid
        if (GetKey(olc::Key::C).bPressed) {
            for (auto &row: grid)
                for (auto &cell: row)
                    cell = false;
            for (auto &row: next)
                for (auto &cell: row)
                    cell = false;
            // Clear the screen
            Clear(olc::BLACK);
        }

        // If space is pressed, pause the simulation
        if (GetKey(olc::Key::SPACE).bPressed) {
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

        // If paused, continue to the next frame
        if (paused) {
            return true;
        }

        // Tick
        auto tickRes = tick();

        // Update the board
        for (int x = 0; x < ScreenWidth(); x++)
            for (int y = 0; y < ScreenHeight(); y++)
                if (next[x][y] != grid[x][y])
                    Draw(x, y, next[x][y] ? olc::WHITE : olc::BLACK);

        // If tick, assign next to grid
        if (tickRes)
            std::memcpy(grid, next, sizeof(grid));

        return true;
    }

    bool tick() {
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

private:
    bool grid[ROWS][COLS]{};
    bool next[ROWS][COLS]{};
    bool paused = false;
};


#endif //GAMEOFLIFE_GAME_H
