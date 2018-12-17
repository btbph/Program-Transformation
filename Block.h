#ifndef BLOCKMULTIPLICATION_MATRIX_H
#define BLOCKMULTIPLICATION_MATRIX_H

#include <vector>

#define PARALLEL
//#define PARALLEL_BLOCK

using namespace std;
using dataType = double;

class Block {
public:
    Block() { }
    explicit Block(const int& inputBlockSize) {
        matrixSize = inputBlockSize*inputBlockSize;
        matrix = vector<dataType>(matrixSize);
        blockSize = inputBlockSize;
    }
    Block(const vector<dataType>& initialMatrix, const int& blockSize) {
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
    }

    int getIndex(int i, int j) const{
        return this->isTranspose ? j * this->blockSize + i : i * this->blockSize + j;
    }

private:
    int matrixSize;
    vector<dataType> matrix;
    int blockSize;
    bool isTranspose = false;
};


#endif //BLOCKMULTIPLICATION_MATRIX_H
