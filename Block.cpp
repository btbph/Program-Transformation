#include "Block.h"
#include <iostream>

#ifdef PARALLEL
#include "omp.h"
#endif

using namespace std;

Block Block::operator*(const Block &a) const {
    const int N = this->blockSize;
    vector<dataType> res(this->matrixSize);
    dataType sumElements = 0;

#ifdef PARALLEL
#ifdef PARALLEL_BLOCK
#pragma omp parallel for private(sumElements)
#endif
#endif
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            for(int k = 0; k < N; k++) {
                sumElements += this->matrix[getIndex(i, k)] * a.matrix[a.getIndex(k, j)];
            }
            res[i*N+j] = sumElements;
            sumElements = 0;
        }
    }
    return Block(res, N);
}

Block Block::operator+(const Block &a) const {
    const int N = this->blockSize;
    vector<dataType> res;

    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            res.push_back(this->matrix[getIndex(i, j)] + a.matrix[a.getIndex(i, j)]);
    return Block(res, N);
}

void Block::operator+=(const Block& a) {
    auto tmp = *this + a;
    this->matrix = tmp.matrix;
}

