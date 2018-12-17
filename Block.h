#ifndef BLOCKMULTIPLICATION_MATRIX_H
#define BLOCKMULTIPLICATION_MATRIX_H

#include <vector>

using namespace std;

class Block {
public:
    explicit Block(const int& inputBlockSize) {
        matrixSize = inputBlockSize*inputBlockSize;
        matrix = vector<int>(matrixSize);
        blockSize = inputBlockSize;
    }
    Block(const vector<int>& initialMatrix, const int& blockSize) {
        matrix = initialMatrix;
        matrixSize = (int)matrix.size();
        this->blockSize = blockSize;
    }

    Block T() {
        this->isTranspose = !this->isTranspose;
        return *this;
    }

    Block operator*(const Block& a) const;
    Block operator+(const Block& a) const;
    void operator+=(const Block& a);

    void clear() {
        matrix.assign(this->matrixSize, 0);
        matrixSize = (int)matrix.size();
    }

    int getIndex(int i, int j) const{
        return this->isTranspose ? j * this->blockSize + i : i * this->blockSize + j;
    }

private:
    int matrixSize;
    vector<int> matrix;
    int blockSize;
    bool isTranspose = false;
};


#endif //BLOCKMULTIPLICATION_MATRIX_H
