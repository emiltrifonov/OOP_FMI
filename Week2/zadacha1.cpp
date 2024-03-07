#include <iostream>
#include <fstream>
#include <sstream>

namespace Constants {
    const char rowSep = '|';
    const char elemSep = ',';
}

struct Row {
    int* elements = nullptr;
};

struct Matrix {
    int rowsCount = 0;
    int colsCount = 0;
    Row* rows = nullptr;
};

void updateMatrixRowAndColCount(std::ifstream& ifs, int& rowsCount, int& colsCount) {
    rowsCount = colsCount = 1;

    while (true)
    {
        if (ifs.eof()) {
            break;
        }

        char ch = ifs.get();
        if (ch == Constants::rowSep) {
            rowsCount++;
        }
        else if (ch == Constants::elemSep) {
            if (rowsCount == 1) {
                colsCount++;
            }
        }
    }

    ifs.clear();
    ifs.seekg(0);
}

void initRowsAndCols(Matrix& m) {
    m.rows = new Row[m.rowsCount];
    for (unsigned i = 0; i < m.colsCount; i++)
    {
        m.rows[i].elements = new int[m.colsCount];
    }
}

int getDigitFromChar(char ch) {
    return ch - '0';
}

int getNumFromStr(const char* str) {
    if (!str)
        return 0;

    int result = 0;

    int strLen = strlen(str);

    for (int i = strLen - 1; i >= 0; i--)
    {
        int digit = getDigitFromChar(str[i]);
        result *= 10;
        result += digit;
    }

    return result;
}

void parseRow(Matrix& m, const char* buff, int rowIndex) {
    if (!buff)
        return;

    std::stringstream ss(buff);

    for (unsigned i = 0; i < m.colsCount; i++)
    {
        char numBuff[16];
        ss.getline(numBuff, 16, Constants::elemSep);
        m.rows[rowIndex].elements[i] = getNumFromStr(numBuff);
    }
}

Matrix getMatrixFromFile(std::ifstream& ifs) {
    Matrix result;
    updateMatrixRowAndColCount(ifs, result.rowsCount, result.colsCount);

    std::cout << "Matrix is " << result.rowsCount << "x" << result.colsCount << std::endl;

    initRowsAndCols(result);

    for (unsigned i = 0; i < result.rowsCount; i++)
    {
        char buff[1024];
        ifs.getline(buff, 1024, Constants::rowSep);
        //std::cout << buff << std::endl;
        parseRow(result, buff, i);
    }

    return result;
}

void freeRows(Row* r) {
    delete[] r;
}

void freeCols(int* a) {
    delete[] a;
}

void freeMatrix(Matrix& m) {
    for (unsigned i = 0; i < m.colsCount; i++)
    {
        freeCols(m.rows[i].elements);
    }
    freeRows(m.rows);
}

void printMatrix(Matrix& m) {
    for (unsigned i = 0; i < m.rowsCount; i++)
    {
        for (unsigned j = 0; j < m.colsCount; j++)
        {
            std::cout << m.rows[i].elements[j] << " ";
        }
        std::cout << std::endl;
    }
}

void writeProductToFile(const char* file1, const char* file2, const char* fileRes) {
    std::ifstream ifs1(file1);
    std::ifstream ifs2(file2);

    if (!ifs1.is_open() || !ifs2.is_open()) {
        return;
    }

    Matrix matrix1 = getMatrixFromFile(ifs1);
    Matrix matrix2 = getMatrixFromFile(ifs2);

    std::cout << "M1 is " << matrix1.rowsCount << "x" << matrix1.colsCount << std::endl;
    std::cout << "M1 is " << matrix2.rowsCount << "x" << matrix2.colsCount << std::endl;

    printMatrix(matrix1);
    printMatrix(matrix2);

    freeMatrix(matrix1);
    freeMatrix(matrix2);

    ifs1.close();
    ifs2.close();
}

int main()
{
    writeProductToFile("matrix1.txt", "matrix2.txt", "matrixResult.txt");
}
