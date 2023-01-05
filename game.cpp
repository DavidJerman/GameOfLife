//
// Created by pinkynyte on 1/4/23.
//

#include "game.h"

game::game() {
    sAppName = "Game Of Life";

}


bool game::OnUserCreate() {
    // Called once at the start, so create things here

    // Setup
    ConsoleCaptureStdOut(true);
    pauseSimulation(true);
    loadConfig("config.cfg");
    pauseSimulation(true);
    // Rand setup
    rng = std::mt19937(rd());
    dist = std::make_shared<std::uniform_int_distribution<std::mt19937::result_type>>(0, 100);
    // Seed the grid with random values
    newState();

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

        // If right mouse clicked
        if (GetMouse(1).bHeld) {
            return removeCells();
        }

        if (GetKey(olc::Key::N).bPressed && !classicMode) {
            calculateNewState();
            updateBoard();
        }
    }

    // If space is pressed, pause the simulation
    if (!IsConsoleShowing())
        if (GetKey(olc::Key::SPACE).bPressed)
            pauseSimulation(true);

    // Show console
    if (GetKey(olc::Key::TAB).bPressed)
        ConsoleShow(olc::Key::TAB, false);

    // If paused, continue to the temp frame
    if (paused)
        return true;

    // Tick
    if (classicMode)
        calculateNewState();

    // Update the board
    updateBoard();

    return true;
}


bool game::addCells() {// Get the mouse position
    auto mousePos = GetMousePos();
    // Get the cell position
    saveOldState();
    grid[mousePos.x][mousePos.y] = true;
    updateBoard();
    // Skip the rest of the loop
    return true;
}


bool game::removeCells() {
    auto mousePos = GetMousePos();
    // Get the cell position
    saveOldState();
    grid[mousePos.x][mousePos.y] = false;
    updateBoard();
    // Skip the rest of the loop
    return true;
}


void game::copyTemp() {
    std::memcpy(grid, temp, sizeof(grid));
}


void game::updateBoard() {
    for (int x = 0; x < ScreenWidth(); x++)
        for (int y = 0; y < ScreenHeight(); y++)
            if (prev[x][y] != grid[x][y])
                Draw(x, y, grid[x][y] ? aliveCellColor : deadCellColor);
}


void game::pauseSimulation(bool mute) {
    paused = !paused;
    if (paused) {
        DrawString(0, 0, "Paused", olc::BLUE, 1);
        if (!mute)
            std::cout << "Paused the simulation" << std::endl;
    } else {
        Clear(game::deadCellColor); // Is this necessary?
        if (!mute)
            std::cout << "Resumed the simulation" << std::endl;
        // Draw the grid
        for (int x = 0; x < ROWS; x++)
            for (int y = 0; y < COLS; y++)
                if (grid[x][y])
                    Draw(x, y, grid[x][y] ? aliveCellColor : deadCellColor);
    }
}


void game::clearState() {
    std::memset(grid, false, sizeof(grid));
    std::memset(temp, false, sizeof(temp));
    std::memset(prev, false, sizeof(prev));
    // Clear the screen
    Clear(game::deadCellColor);
}


void game::newState() {
    clearState();
    for (auto &row: grid)
        for (auto &cell: row)
            if (dist->operator()(rng) < (unsigned long) (randomizeChance * 100))
                cell = true;
}


bool game::calculateNewState() {
    saveOldState();
    // Calculate the temp board - V1
    for (int x = 0; x < ScreenWidth(); x++) {
        for (int y = 0; y < ScreenHeight(); y++) {
            // Same code for all algorithms
            int neighbors = 0;
            if (border) {
                // If border enabled
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
            } else {
                // If border not enabled
                for (int i = -1; i < 2; i++) {
                    for (int j = -1; j < 2; j++) {
                        if (i == 0 && j == 0)
                            continue;
                        if (grid[(x + i + COLS) % COLS][(y + j + ROWS) % ROWS])
                            neighbors++;
                    }
                }
            }
            // Generalized code for all algorithms
            if (grid[x][y]) {
                if (survival[neighbors])
                    temp[x][y] = true;
                else
                    temp[x][y] = false;
            } else {
                if (birth[neighbors])
                    temp[x][y] = true;
                else
                    temp[x][y] = false;
            }
        }
    }
    copyTemp();
    return true;
}


void game::setRandomizationChance(float chance) {
    if (chance < 0 || chance > 100)
        return;
    randomizeChance = chance / 100;
}

bool game::OnConsoleCommand(const std::string &command) {
    return parseCommand(command);
}

float game::getRandomizationChance() const {
    return randomizeChance;
}

void game::setClassicMode(bool _classicMode) {
    game::classicMode = _classicMode;
}

void game::setBorders(bool _borders) {
    game::border = _borders;
}

void game::loadConfig(const std::string &configPath) {
    std::fstream file(configPath);
    if (!file.is_open()) {
        std::cout << "Failed to open config file" << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty())
            continue;
        if (line[0] == '#')
            continue;
        parseCommand(line);
    }
    std::cout << "Loaded config file" << std::endl;
}

bool game::parseCommand(const std::string &command) {
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
        else if (var == "mode") {
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
        else if (var == "border") {
            std::string borderState;
            ss >> borderState;
            if (borderState == "true") {
                setBorders(true);
                std::cout << "Borders set to on" << std::endl;
                return true;
            }
            if (borderState == "false") {
                setBorders(false);
                std::cout << "Borders set to off" << std::endl;
                return true;
            }
        }
        else if (var == "cell") {
            std::string cellType;
            ss >> cellType;
            bool alive;
            if (cellType == "alive") {
                alive = true;
            } else if (cellType == "dead") {
                alive = false;
            } else {
                std::cout << "Invalid cell type" << std::endl;
                return false;
            }
            std::string color;
            ss >> color;
            if (color == "red") {
                if (alive)
                    aliveCellColor = olc::RED;
                else
                    deadCellColor = olc::RED;
                std::cout << "Cell color set to red" << std::endl;
            } else if (color == "green") {
                if (alive)
                    aliveCellColor = olc::GREEN;
                else
                    deadCellColor = olc::GREEN;
                std::cout << "Cell color set to green" << std::endl;
            } else if (color == "blue") {
                if (alive)
                    aliveCellColor = olc::BLUE;
                else
                    deadCellColor = olc::BLUE;
                std::cout << "Cell color set to blue" << std::endl;
            } else if (color == "yellow") {
                if (alive)
                    aliveCellColor = olc::YELLOW;
                else
                    deadCellColor = olc::YELLOW;
                std::cout << "Cell color set to yellow" << std::endl;
            } else if (color == "white") {
                if (alive)
                    aliveCellColor = olc::WHITE;
                else
                    deadCellColor = olc::WHITE;
                std::cout << "Cell color set to white" << std::endl;
            } else if (color == "black") {
                if (alive)
                    aliveCellColor = olc::BLACK;
                else
                    deadCellColor = olc::BLACK;
                std::cout << "Cell color set to black" << std::endl;
            } else {
                std::cout << "Invalid color" << std::endl;
                return false;
            }
            fullUpdateBoard();
            return true;
        }
        else if (var == "algo") {
            std::string algo;
            ss >> algo;
            if (!paused) {
                std::cout << "Cannot change algorithm while game is running" << std::endl;
                return false;
            }
            auto res = parseAlgo(algo);
            if (!res) {
                std::cout << "Invalid algorithm, setting to B3/S23" << std::endl;
                parseAlgo("B3/S23");
                return false;
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
        pauseSimulation(false);
        return true;
    }
    if (cmd == "next" || cmd == "n") {
        calculateNewState();
        updateBoard();
        copyTemp();
        std::cout << "Calculated the temp state" << std::endl;
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

void game::fullUpdateBoard() {
    for (int x = 0; x < ScreenWidth(); x++) {
        for (int y = 0; y < ScreenHeight(); y++) {
            Draw(x, y, grid[x][y] ? aliveCellColor : deadCellColor);
        }
    }
}

void game::saveOldState() {
    std::memcpy(prev, grid, sizeof(grid));
}

bool game::parseAlgo(const std::string &algorithm) {
    std::memset(birth, false, sizeof(birth));
    std::memset(survival, false, sizeof(survival));
    if (tolower(algorithm[0]) != 'b') {
        std::cout << "Invalid algorithm" << std::endl;
        return false;
    }
    for (int i = 1; i < algorithm.length(); i++) {
        if (algorithm[i] == '/') {
            break;
        }
        if (algorithm[i] < '0' || algorithm[i] > '8') {
            std::cout << "Invalid algorithm" << std::endl;
            return false;
        } else {
            birth[algorithm[i] - '0'] = true;
        }
    }
    for (auto i = algorithm.length() - 1; ; i--) {
        if (algorithm[i] == '/') {
            break;
        }
        if (algorithm[i] < '0' || algorithm[i] > '8') {
            if (tolower(algorithm[i]) == 's')
                continue;
            std::cout << "Invalid algorithm" << std::endl;
            return false;
        } else {
            survival[algorithm[i] - '0'] = true;
        }
    }
    std::cout << "Algorithm set to " << algorithm << std::endl;
    return true;
}
