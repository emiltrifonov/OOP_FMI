#include <iostream>
#include <cstring>

class GradeReport {
private:
    int* grades = nullptr;
    size_t gradesCount = 0;
    char* name = nullptr;

    void free();
    void copyFrom(const GradeReport& other);
    bool isValidGrade(int grade) const;

public:
    GradeReport();
    GradeReport(const int* grades, size_t gradesCount, const char* name);
    GradeReport(const GradeReport& other);

    void setGrades(const int* grades, size_t gradesCount);
    void setGradesCount(size_t gradesCount);
    void setName(const char* name);

    const int* getGrades() const;
    size_t getGradesCount() const;
    const char* getName() const;

    GradeReport& operator=(const GradeReport& other);

    ~GradeReport();
};

bool GradeReport::isValidGrade(int grade) const {
    return grade >= 0 && grade <= 100;
}

GradeReport::GradeReport(const GradeReport& other) {
    copyFrom(other);
}
GradeReport::GradeReport() = default;
GradeReport::GradeReport(const int* grades, size_t gradesCount, const char* name) {
    setGrades(grades, gradesCount);
    setGradesCount(gradesCount);
    setName(name);
}

void GradeReport::setGrades(const int* grades, size_t gradesCount) {
    if (!grades) {
        return;
    }

    for (unsigned i = 0; i < gradesCount; i++)
    {
        if (isValidGrade(grades[i])) {
            this->grades[i] = grades[i];
        }
    }
}
void GradeReport::setGradesCount(size_t gradesCount) {
    this->gradesCount = gradesCount;
}
void GradeReport::setName(const char* name) {
    if (!name) {
        return;
    }
    delete[] this->name;
    this->name = new char[strlen(name) + 1];
    strcpy(this->name, name);
}

const int* GradeReport::getGrades() const {
    return grades;
}
size_t GradeReport::getGradesCount() const {
    return gradesCount;
}
const char* GradeReport::getName() const {
    return name;
}

GradeReport& GradeReport::operator=(const GradeReport& other) {
    free();
    copyFrom(other);
    return *this;
}

void GradeReport::free() {
    delete[] name;
    name = nullptr;
    delete[] grades;
    grades = nullptr;
}
void GradeReport::copyFrom(const GradeReport& other) {
    if (this != &other) {
        grades = new int[other.getGradesCount()];
        setGradesCount(other.getGradesCount());
        setGrades(other.getGrades(), gradesCount);
        setName(other.getName());
    }
}

GradeReport::~GradeReport() {
    free();
}

int main()
{
    
}
