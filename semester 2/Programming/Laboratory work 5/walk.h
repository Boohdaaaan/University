#ifndef WALK_H
#define WALK_H

#include "matrix.h"
#include <vector>

void traverseLowerSectorSpiral(const Matrix& matrix, std::vector<int>& sequence);
void traverseRightSectorSnake(const Matrix& matrix, std::vector<int>& sequence);

#endif
