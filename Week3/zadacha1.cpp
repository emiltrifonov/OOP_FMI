#include <iostream>
#include <fstream>
#include <sstream>

namespace Constants {
    const char ROW_SEP = '|';
    const char ELEM_SEP = ',';
    constexpr size_t BUFF_SIZE = 1024;
}

struct Row {
    int* elements = nullptr;
};

struct Matrix {
    int rowsCount = 0;
    int colsCount = 0;
    Row* rows = nullptr;
};

void setIfsToPos(std::ifstream& ifs, int pos) {
    ifs.clear();
    ifs.seekg(pos);
}

void updateMatrixRowAndColCount(std::ifstream& ifs, int& rowsCount, int& colsCount) {
    const int currPos = ifs.tellg();
    rowsCount = colsCount = 1;

    while (true)
    {
        if (ifs.eof()) {
            break;
        }

        char ch = ifs.get();
        if (ch == Constants::ROW_SEP) {
            rowsCount++;
        }
        else if (ch == Constants::ELEM_SEP) {
            if (rowsCount == 1) {
                colsCount++;
            }
        }
    }

    setIfsToPos(ifs, currPos);
}

void initRowsAndCols(Matrix& m) {
    m.rows = new Row[m.rowsCount];
    for (unsigned i = 0; i < m.rowsCount; i++)
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
        char numBuff[Constants::BUFF_SIZE];
        ss.getline(numBuff, Constants::BUFF_SIZE, Constants::ELEM_SEP);
        m.rows[rowIndex].elements[i] = getNumFromStr(numBuff);
    }
}

Matrix getMatrixFromFile(std::ifstream& ifs) {
    Matrix result;
    updateMatrixRowAndColCount(ifs, result.rowsCount, result.colsCount);

    //std::cout << "Matrix is " << result.rowsCount << "x" << result.colsCount << std::endl;

    initRowsAndCols(result);

    for (unsigned i = 0; i < result.rowsCount; i++)
    {
        char buff[Constants::BUFF_SIZE];
        ifs.getline(buff, Constants::BUFF_SIZE, Constants::ROW_SEP);
        //std::cout << "Current row: \"" << buff << "\"" << std::endl;
        //std::cout << buff << std::endl;
        parseRow(result, buff, i);
    }

    return result;
}

void freeRows(Row*& r) {
    if (!r) {
        delete[] r;
        r = nullptr;
    }
}

void freeCols(int*& a) {
    if (!a) {
        delete[] a;
        a = nullptr;
    }
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
    std::cout << std::endl;
}

bool canMatrixesBeMultiplied(const Matrix& first, const Matrix& second) {
    return (first.colsCount == second.rowsCount);
}

Row getProductRow(const Matrix& first, const Matrix& second, int productRowInd, int productColCount) {
    Row result;
    result.elements = new int[productColCount];

    for (unsigned i = 0; i < productColCount; i++)
    {
        int currentElement = 0;
        for (unsigned j = 0; j < first.colsCount; j++)
        {
            currentElement += first.rows[productRowInd].elements[j] * second.rows[j].elements[i];
        }
        result.elements[i] = currentElement;
    }

    return result;
}

Matrix getProduct(const Matrix& first, const Matrix& second) {
    Matrix result;
    result.rowsCount = first.rowsCount;
    result.colsCount = second.colsCount;
    initRowsAndCols(result);

    for (unsigned i = 0; i < result.rowsCount; i++)
    {
        result.rows[i] = getProductRow(first, second, i, result.colsCount);
    }

    return result;
}

void writeProductToFile(std::ofstream& ofs, const Matrix& product) {
    if (!ofs.is_open()) {
        return;
    }

    for (unsigned i = 0; i < product.rowsCount; i++)
    {
        for (unsigned j = 0; j < product.colsCount; j++)
        {
            ofs << product.rows[i].elements[j];
            if (j != product.colsCount - 1) {
                ofs << ',';
            }
        }
        if (i != product.rowsCount - 1) {
            ofs << '|';
        }
    }
}

void writeProductToFile(const char* file1, const char* file2, const char* fileRes) {
    std::ifstream ifs1(file1);
    std::ifstream ifs2(file2);

    if (!ifs1.is_open() || !ifs2.is_open()) {
        return;
    }

    Matrix matrix1 = getMatrixFromFile(ifs1);
    printMatrix(matrix1);
    //3 3 3
    //1 1 3
    //3 3 1
    Matrix matrix2 = getMatrixFromFile(ifs2);
    printMatrix(matrix2);
    //1 2
    //3 4
    //5 6

    if (!canMatrixesBeMultiplied(matrix1, matrix2)) {
        return;
    }

    Matrix product = getProduct(matrix1, matrix2);
    printMatrix(product);
    //27 36
    //19 24
    //17 24

    std::ofstream ofs(fileRes);
    writeProductToFile(ofs, product);

    freeMatrix(matrix1);
    freeMatrix(matrix2);
    freeMatrix(product);
    ifs1.close();
    ifs2.close();
}

int main()
{
    writeProductToFile("matrix1.txt", "matrix2.txt", "matrixResult.txt");
}
