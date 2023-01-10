#pragma once

#include "Coordinate.h"
#include <array>

// Define direction to expand a node in 2-dimensions grid

static std::array<Coordinate, 4> four_principle_directions = {Coordinate(1, 0), // down
                                                       Coordinate(-1, 0), // up
                                                       Coordinate(0, 1), // right
                                                       Coordinate(0, -1)}; // left

static std::array<Coordinate, 8> eight_principle_directions = {Coordinate(1, 0), // down
                                        Coordinate(-1, 0), // up
                                        Coordinate(0, 1), // right
                                        Coordinate(0, -1), // left
                                        Coordinate(1, 1), // down-right diagonal move
                                        Coordinate(-1, 1), // // up-right diagonal move
                                        Coordinate(1, -1), // down-left diagonal move
                                        Coordinate(-1, -1)}; // up-left diagonal move