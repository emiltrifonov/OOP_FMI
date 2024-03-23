#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <sstream>
#pragma warning (disable:4996)

namespace Constants {
    constexpr size_t MAX_TEXT_SIZE = 50;
    constexpr size_t MAX_COL_COUNT = 15;
    constexpr size_t MAX_ROW_COUNT = 100;
    constexpr char CMD_PRINT[] = "print";
    constexpr char CMD_ADD[] = "add";
    constexpr char CMD_EDIT[] = "edit";
    constexpr char CMD_REMOVE[] = "remove";
    constexpr char SEP = ';';
    constexpr char ROW_NUM_ERROR[] = "Invalid row number!";
    constexpr char COL_NUM_ERROR[] = "Invalid collumn number!";
    constexpr char TEXT_LEN_ERROR[] = "Text is too large to fit in cell!";
}

struct Cell {
private:
    char text[Constants::MAX_TEXT_SIZE + 1] = "";

public:
    void setText(const char* text) {
        strcpy(this->text, text);
    }
    const char* getText() const {
        return text;
    }

    void print(int colWidth) const {
        std::cout << "| " << text;
        size_t len = strlen(text);
        for (int i = 0; i < colWidth - len; i++)
        {
            std::cout << " ";
        }
        std::cout << " | ";
        //std::cout << "| " << text << " | ";
    }
};

struct Row {
private:
    size_t cellCount = 0;
    Cell cells[Constants::MAX_COL_COUNT];

public:
    Row() = default;

    size_t getCellCount() const {
        return cellCount;
    }

    Cell getCellAt(int ind) const {
        return cells[ind];
    }

    void setCellAt(int ind, const char* str);
    void setCellCount(size_t size) {
        if (size > 0 && size <= Constants::MAX_COL_COUNT) {
            cellCount = size;
        }
    }
    void print(const unsigned* widths) const;
};

void Row::setCellAt(int ind, const char* str) {
    if (ind < 0 || ind >= Constants::MAX_COL_COUNT || !str) {
        return;
    }
    else {
        cells[ind].setText(str);
    }
}

void Row::print(const unsigned* widths) const {
    if (!widths) {
        return;
    }
    for (unsigned i = 0; i < cellCount; i++)
    {
        cells[i].print(widths[i]);
    }
}

class Table {
private:
    size_t colCount = 0;
    size_t rowCount = 0;
    Row rows[150];
    unsigned maxWidthPerCollumn[Constants::MAX_COL_COUNT]{ 0 }; //ToDo later

    int getNum(const char*& str) const;
    bool isRowValid(int rowNum, bool isAddingNewRow) const;
    bool isColValid(int colNum) const;
    void shiftRowsFrom(int ind);
    void updateColCounts();
    void updateColAfterEditing(int col, size_t newWordLen);

public:
    Table() = default;

    void processInput(const char* str);
    void addRow(const char* str);
    void removeRow(const char* str);
    void editCell(const char* str);
    void print() const;
};

bool isDigit(char ch) {
    return ch > '0' && ch < '9';
}
int getDigit(char ch) {
    return ch - '0';
}

void Table::updateColAfterEditing(int col, size_t newWordLen) {
    if (maxWidthPerCollumn[col] < newWordLen) {
        maxWidthPerCollumn[col] = newWordLen;
        return;
    }

    int currMax = maxWidthPerCollumn[col];
    int tempMax = 0;

    for (unsigned i = 0; i < rowCount; i++)
    {
        int currLen = strlen(rows[i].getCellAt(col).getText());
        if (currLen == currMax) {
            return;
        }
        else if (currLen > tempMax) {
            tempMax = currLen;
        }
    }

    maxWidthPerCollumn[col] = tempMax;
}

void Table::updateColCounts() {
    for (unsigned i = 0; i < rowCount; i++)
    {
        rows[i].setCellCount(colCount);
    }
}

bool Table::isRowValid(int rowNum, bool isAddingNewRow = false) const {
    switch (isAddingNewRow)
    {
    case true: return (rowNum >= 1 && rowNum <= rowCount + 1);
    case false: return (rowNum >= 1 && rowNum <= rowCount);
    }
}

bool Table::isColValid(int colNum) const {
    return (colNum >= 1 && colNum <= colCount);
}

int Table::getNum(const char*& str) const {
    if (!str) {
        return -1;
    }

    int result = 0;

    while (*str && *str != ' ')
    {
        if (isDigit(*str)) {
            result *= 10;
            result += getDigit(*str);
        }
        else {
            result = -1;
            break;
        }

        str++;
    }

    str++;

    if (result == 0) {
        result = -1;
    }

    return result;
}
//tova ne e ok
void Table::processInput(const char* str) {
    if (!str) {
        return;
    }

    std::stringstream ss(str);

    constexpr size_t BUFF_SIZE = 16;
    char buff[BUFF_SIZE];
    ss.getline(buff, BUFF_SIZE, ' ');

    if (strcmp(buff, Constants::CMD_PRINT) == 0) {
        print();
        return;
    }
    else if (strcmp(buff, Constants::CMD_ADD) == 0) {
        int toShift = strlen(Constants::CMD_ADD);
        addRow(str += toShift);
        return;
    }
    else if (strcmp(buff, Constants::CMD_EDIT) == 0) {
        int toShift = strlen(Constants::CMD_EDIT);
        addRow(str += toShift);
        return;
    }
    else if (strcmp(buff, Constants::CMD_REMOVE) == 0) {
        int toShift = strlen(Constants::CMD_REMOVE);
        addRow(str += toShift);
        return;
    }
    else {
        std::cout << "Incorrect input!\n";
        return;
    }
}

void Table::shiftRowsFrom(int ind) {
    for (int i = rowCount - 1; i >= ind; i--)
    {
        std::swap(rows[i], rows[i + 1]);
    }
}

size_t getColCount(const char* str) {
    if (!str) {
        return 0;
    }

    int result = 0;
    int temp = 0;

    while (*str)
    {
        if (*str == Constants::SEP) {
            result++;
            std::cout << "Temp: " << temp << std::endl;
            if (temp > Constants::MAX_TEXT_SIZE) {
                return 0;
            }
            temp = 0;
        }
        else {
            temp++;
        }

        str++;
    }

    std::cout << "Temp: " << temp << std::endl;
    if (temp > Constants::MAX_TEXT_SIZE) {
        return 0;
    }

    return result + 1;
}

//TO DO
void Table::addRow(const char* str) {
    if (!str) {
        return;
    }

    if (rowCount == Constants::MAX_ROW_COUNT) {
        std::cout << "Table is full!\n";
        return;
    }

    int rowNum = getNum(str);
    if (!isRowValid(rowNum, true)) {
        std::cout << Constants::ROW_NUM_ERROR << std::endl;
        return;
    }

    size_t colCount = getColCount(str);
    if (colCount < 1 || colCount > Constants::MAX_COL_COUNT) {
        std::cout << Constants::TEXT_LEN_ERROR << std::endl;
        return;
    }

    if (rowNum != rowCount + 1) {
        shiftRowsFrom(rowNum - 1);
    }

    std::stringstream ss(str);

    for (unsigned i = 0; i < colCount; i++)
    {
        char buff[1024];
        ss.getline(buff, 1024, Constants::SEP);
        size_t buffLen = strlen(buff);
        if (buffLen > Constants::MAX_TEXT_SIZE) {
            //removeRow(); //ToDo make removeRow() command generator
            return;
        }
        else {
            rows[rowNum - 1].setCellAt(i, buff);
            if (buffLen > maxWidthPerCollumn[i]) {
                maxWidthPerCollumn[i] = buffLen;
            }
        }
    }

    rowCount++;

    if (this->colCount < colCount) {
        this->colCount = colCount;
        updateColCounts();
    }
    else {
        rows[rowNum - 1].setCellCount(this->colCount);
    }
}


void Table::removeRow(const char* str) {
    if (!str) {
        return;
    }

    int rowNum = getNum(str);

    if (!isRowValid(rowNum)) {
        std::cout << Constants::ROW_NUM_ERROR << std::endl;
        return;
    }
    else {
        std::swap(rows[rowNum - 1], rows[rowCount - 1]);
        rowCount--;
    }

    for (unsigned i = 0; i < colCount; i++)
    {
        updateColAfterEditing(i, 0);
    }
}

void Table::editCell(const char* str) {
    if (!str) {
        return;
    }

    int rowNum = getNum(str);
    if (!isRowValid(rowNum)) {
        std::cout << Constants::ROW_NUM_ERROR << std::endl;
        return;
    }

    int colNum = getNum(str);
    if (!isColValid(colNum)) {
        std::cout << Constants::COL_NUM_ERROR << std::endl;
        return;
    }

    size_t len = strlen(str);

    if (len > Constants::MAX_TEXT_SIZE) {
        std::cout << Constants::TEXT_LEN_ERROR << std::endl;
        return;
    }

    rows[rowNum - 1].setCellAt(colNum - 1, str);

    updateColAfterEditing(colNum - 1, len);
}

void Table::print() const {
    for (unsigned i = 0; i < rowCount; i++)
    {
        rows[i].print(maxWidthPerCollumn);
        std::cout << std::endl;
    }
    std::cout << "---------------------------------------------------------------------\n";
}

int main()
{
    Table t;
    t.addRow("1 sashko;bratle;znam;che;chetesh;tova");
    t.addRow("2 dimitriev;me;dupi;qko");
    t.addRow("2 ne;e;gotino");
    t.print();
    t.editCell("2 6 pasta");
    t.editCell("1 5 pasta");
    t.editCell("2 1 achkata");
    t.editCell("3 4 az");
    t.print();
    t.removeRow("2");
    t.print();
}
