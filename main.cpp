#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

#include "Block.h"

using namespace std;
using namespace chrono;

vector<Block> readData(const string &filePath, const int &N) {
    ifstream fin(filePath);
    auto valuesInBlock = new vector<dataType>();
    vector<Block> res;
    string data;
    int i = 1;
    int countElemsInBlock = N * N;
    while (!fin.eof()) {
        fin >> data;
        if (typeid(dataType) == typeid(double))
            valuesInBlock->push_back(stod(data));
        if (typeid(dataType) == typeid(float))
            valuesInBlock->push_back(stof(data));
        if (i != 0 && i % countElemsInBlock == 0) {
            res.emplace_back(*valuesInBlock, N);
            valuesInBlock->clear();
        }
        i++;
    }
    fin.close();
    return res;
}

vector<int> readInfovector() {
    ifstream fin("/Users/btbph/Desktop/GenerateMatrix/info.txt");
    int countOfParametersInFile = 3;
    vector<int> res(countOfParametersInFile);
    string param;
    for(int i = 0; i < countOfParametersInFile; i++) {
        fin >> param;
        res[i] = stoi(param);
    }
    fin.close();
    return res;
}

vector<int> createOffsetVector(int countBlocks, int N) { // из ряда в колонну
    auto counter = 1;
    auto step = 1;
    auto count_row_handled = 0;
    auto first_column_index = 1;
    auto count_added_elems = 1;

    vector<int> res;

    for(int i = 1; i < countBlocks + 1; i++) {
        if (count_added_elems % (N - count_row_handled + 1) == 0) {
            count_row_handled ++;
            step = 1 + count_row_handled;
            first_column_index += step;
            counter = first_column_index;
            count_added_elems = 1;
        }
        res.push_back(counter - 1); // -1
        count_added_elems++;
        counter += step;
        step++;
    }
    return res;
}

vector<int> swapKeysAndValues(const vector<int>& vec) {
    vector<int> res(vec.size());
    for(int i = 0; i < vec.size();i++) {
        res[vec[i]] = i;
    }
    return res;
}

vector<Block> multyplyBlockMatrices(const vector<Block>& A, vector<Block>& B, const int& N, const int& sizeOfBlock, const vector<int>& toRowOffset) {
    vector<Block> res;
    Block sum(sizeOfBlock);
    int countIterations = N / sizeOfBlock;
    int countUnStoredBlocks;

#ifdef PARALLEL
    #ifndef PARALLEL_BLOCK
    #pragma omp parallel for
    #endif
#endif
    for(int i = 0; i < countIterations; i++) {
        for(int j = 0; j < countIterations; j++) {
            countUnStoredBlocks = 0;
            for(int k = 0; k < i+1; k++) {
                int indexA = ((i+1)*i)/2+k;
                int indexB = k*(k+1)/2+j;

                if (j > k) {
                    indexB = ((j+1)*j/2 + k); // хорошо работает с первом рядом, учитывать j - k
                    sum += (A[toRowOffset[indexA]] * B[indexB].T());
                    B[indexB].T();
                    countUnStoredBlocks++;
                }
                else sum += (A[toRowOffset[indexA]] * B[indexB]);
            }
            res.push_back(sum);
            sum.clear();
        }
    }
    return res;
}

int main() {
// production
    auto info = readInfovector();
    auto countElems = info[0];
    auto N = info[1];
    auto blockSize = info[2];

    cout << "Start of reading files" << endl;
    auto aBlocks = readData("/Users/btbph/Desktop/GenerateMatrix/linear_a.txt", blockSize);
    auto bBlocks = readData("/Users/btbph/Desktop/GenerateMatrix/linear_b.txt", blockSize);
    cout << "End of reading files" << endl;

    const int countBlocks = (int)aBlocks.size();
    auto toColumnOffset = createOffsetVector(countBlocks, N / blockSize);
    auto toRowOffset = swapKeysAndValues(toColumnOffset);

    cout << "Multiplication start!" << endl;
    auto timer = duration<double>();
    auto start_time = steady_clock::now();
    auto res = multyplyBlockMatrices(aBlocks, bBlocks, N, blockSize, toRowOffset);
    timer = steady_clock::now() - start_time;

    cout << "Matrixes multiplication time: " << timer.count() << " sec." << endl;
    ofstream fout("result.txt");
    fout << timer.count() << endl;
    fout << blockSize << endl;
    fout.close();

    return 0;
}