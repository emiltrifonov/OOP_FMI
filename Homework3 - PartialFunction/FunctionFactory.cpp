#include <iostream>
#include <exception>
#include <fstream>
#include "FunctionFactory.h"
#include "FunctionType0.h"
#include "FunctionType1.h"
#include "FunctionType2.h"
#include "FunctionMaximum.h"
#include "FunctionMinimum.h"
#include "FunctionContainer.h"
#include "PartialFunctionByCriteria.hpp"
#include "MyString.h"

using std::cout;
using std::endl;

const static int MAX_SIZE = 32;
const static int MAX_N = 32;
const static int MAX_T = 32;

static st getFileSize(std::ifstream& ifs) {
    st curPos = ifs.tellg();
    ifs.seekg(0, std::ios::end);
    st fileSize = ifs.tellg();

    ifs.seekg(curPos);
    return fileSize;
}

static void validate(unsigned short N, unsigned short T) {
    if (N > MAX_N || T > MAX_T) {
        throw std::out_of_range("Incorrect data in file!");
    }
}

static PartialFunction* handle0(int N, std::ifstream& ifs, const int* args) {
    int values[MAX_SIZE]{ 0 };
    ifs.read(reinterpret_cast<char*>(values), sizeof(int) * N);

    return new PartialFunctionByCriteria<FunctionType0>(FunctionType0(args, values, N));
}

static PartialFunction* handle1(int N, std::ifstream& ifs, const int* args) {
    return new PartialFunctionByCriteria<FunctionType1>(FunctionType1(args, N));
}

static PartialFunction* handle2(int N, std::ifstream& ifs, const int* args) {
    return new PartialFunctionByCriteria<FunctionType2>(FunctionType2(args, N));
}

static PartialFunction* handle3(FunctionContainer&& fc) {
    return new FunctionMaximum(std::move(fc));
}

static PartialFunction* handle4(FunctionContainer&& fc) {
    return new FunctionMinimum(std::move(fc));
}

static PartialFunction* handle012(int N, int T, std::ifstream& ifs) {
    int args[MAX_SIZE]{ 0 };
    ifs.read(reinterpret_cast<char*>(args), sizeof(int) * N);

    switch (T)
    {
    case 0: return handle0(N, ifs, args);
    case 1: return handle1(N, ifs, args);
    case 2: return handle2(N, ifs, args);
    }
}

static PartialFunction* handle34(int N, int T, std::ifstream& ifs, st fileSize) {
    char* fileContent = new char[fileSize];
    st curPos = ifs.tellg();
    ifs.read(fileContent, fileSize);
    ifs.seekg(curPos);

    FunctionContainer fc;

    for (st i = 0; i < N; i++)
    {
        MyString str(fileContent);
        fc.add(factory(str.c_str()));
        fileContent += str.getSize() + 1;
    }

    switch (T)
    {
    case 3: return handle3(std::move(fc));
    case 4: return handle4(std::move(fc));
    }
}

static PartialFunction* handle(int N, int T, std::ifstream& ifs, st fileSize) {
    switch (T)
    {
    case 0: 
    case 1: 
    case 2: return handle012(N, T, ifs);
    case 3: 
    case 4: return handle34(N, T, ifs, fileSize);
    }

    return nullptr;
}

PartialFunction* factory(const char* file)
{
    if (!file) {
        throw std::runtime_error("Empty file name!");
    }

    std::ifstream ifs(file, std::ios::binary);
    if (!ifs.is_open()) {
        throw std::runtime_error("Could not open file!");
    }

    unsigned short N = 0, T = 0;
    ifs.read(reinterpret_cast<char*>(&N), sizeof(N));
    ifs.read(reinterpret_cast<char*>(&T), sizeof(T));
    validate(N, T);

    st fileSize = getFileSize(ifs);
    PartialFunction* result = handle(N, T, ifs, fileSize);

    ifs.close();

    return result;
}