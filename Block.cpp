#include "Block.h"
#include <iostream>

using namespace std;

Block Block::operator*(const Block &a) const {
    const int N = this->blockSize;
    vector<int> res;
    int sumElements = 0;

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            for(int k = 0; k < N; k++) {
                sumElements += this->matrix[getIndex(i, k)] * a.matrix[a.getIndex(k, j)];
            }
            res.push_back(sumElements);
            sumElements = 0;
        }
    }
    return Block(res, N);
}

Block Block::operator+(const Block &a) const {
    const int N = this->blockSize;
    vector<int> res;

    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            res.push_back(this->matrix[getIndex(i, j)] + a.matrix[a.getIndex(i, j)]);
    return Block(res, N);
}

void Block::operator+=(const Block& a) {
    auto tmp = *this + a;
    this->matrix = tmp.matrix;
}

