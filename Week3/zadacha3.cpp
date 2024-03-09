#include <iostream>
#include <fstream>
#include <sstream>

struct Book {
    char name[128];
    uint16_t releaseDate;
    float price;
};

int getFileSize(std::ifstream& ifs) {
    int currPos = ifs.tellg();
    ifs.seekg(0, std::ios::end);
    int fileSize = ifs.tellg();
    ifs.clear();
    ifs.seekg(currPos);

    return fileSize;
}

int getLineCount(std::ifstream& ifs) {
    int currPos = ifs.tellg();
    int result = 0;

    while (true)
    {
        if (ifs.eof()) {
            break;
        }

        char buff[1024];
        ifs.getline(buff, 1024);
        result++;
    }

    ifs.clear();
    ifs.seekg(currPos);

    return result;
}

Book* getBooks(std::ifstream& ifs, size_t& booksSize) {
    if (!ifs.is_open()) {
        return nullptr;
    }

    size_t arrSize = getLineCount(ifs);
    Book* result = new Book[arrSize];

    for (unsigned i = 0; i < arrSize; i++)
    {
        ifs >> result[i].name >> result[i].releaseDate >> result[i].price;
    }

    booksSize = arrSize;
    return result;
}

int* getYears(std::ifstream& ifs, size_t& yearsSize) {
    if (!ifs.is_open()) {
        return nullptr;
    }

    size_t fileSize = getFileSize(ifs);
    size_t arrSize = fileSize / sizeof(int);
    //std::cout << "Years arr size: " << arrSize << std::endl;
    int* result = new int[arrSize];

    for (unsigned i = 0; i < arrSize; i++)
    {
        ifs.read((char*)&result[i], sizeof(result[i]));
    }

    yearsSize = arrSize;
    return result;
}

void saveBooksFromYearInBinary(const int* years, size_t yearsSize, const Book* books, size_t booksSize, std::ofstream& ofs) {
    for (unsigned i = 0; i < yearsSize; i++)
    {
        for (unsigned j = 0; j < booksSize; j++)
        {
            if (books[j].releaseDate == years[i]) {
                ofs.write((const char*)&books[j], sizeof(Book));
            }
        }
    }
}

void printBook(const Book& book) {
    const char SEP = ' ';
    std::cout << book.name << SEP << book.releaseDate << SEP << book.price << std::endl;
}

void printBooksFromBinary(std::ifstream& ifs, int booksCount) {
    while(true) {
        ifs.get();
        if (ifs.eof()) {
            break;
        }
        ifs.seekg(-1, std::ios::cur);

        Book book;
        ifs.read((char*)&book, sizeof(Book));
        printBook(book);
    }
}

void printYears(const int* years, size_t size) {
    if (!years) {
        return;
    }
    for (unsigned i = 0; i < size; i++)
    {
        std::cout << years[i] << ' ';
    }
    std::cout << std::endl;
}

int main()
{
    /*constexpr size_t SIZE = 5;
    int years[SIZE] = { 2008, 2010, 2015, 2020, 2023 };
    std::ofstream ofs("years.dat", std::ios::binary);
    if (!ofs.is_open()) {
        return -1;
    }
    ofs.write((const char*)years, SIZE * sizeof(int));*/

    std::ifstream booksIfs("books.txt");
    size_t bookSize;
    Book* books = getBooks(booksIfs, bookSize);
    if (!books || !booksIfs.is_open()) {
        return -1;
    }
    booksIfs.close();
    /*for (unsigned i = 0; i < bookSize; i++)
    {
        printBook(books[i]);
    }*/
    std::ifstream yearsIfs("years.dat");
    size_t yearsSize;
    int* years = getYears(yearsIfs, yearsSize);
    if (!years || !yearsIfs.is_open()) {
        return -1;
    }
    yearsIfs.close();

    std::ofstream ofs("books.dat");
    if (!ofs.is_open()) {
        return -1;
    }
    saveBooksFromYearInBinary(years, yearsSize, books, bookSize, ofs);
    ofs.close();

    //printYears(years, yearsSize);
    
    std::ifstream ifs("books.dat");
    if (!ifs.is_open()) {
        return -1;
    }
    printBooksFromBinary(ifs, bookSize);
    ifs.close();

    delete[] books;
    delete[] years;
}
