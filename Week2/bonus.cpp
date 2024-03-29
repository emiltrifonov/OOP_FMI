#include<iostream>
#include<cstring>
#include<fstream>

namespace Constants {
    const int INVALID_VALUE = -1;
    constexpr size_t BUFF_SIZE = 1024;
}

enum class ErrorInCatalog {
    catalog_not_open,
    read_from_empty_catalog,
    movie_not_in_catalog,
    no_error_occurred
};

struct SafeAnswer {
    int number;
    ErrorInCatalog error;
};

struct Movie {
    char name[128];
    unsigned int price;
};

void setIfsToPos(std::ifstream& ifs, int pos) {
    ifs.clear();
    ifs.seekg(pos);
}

bool isCatalogEmpty(std::ifstream& ifs) {
    const int currPos = ifs.tellg();

    ifs.seekg(0, std::ios::end);
    const int lastPos = ifs.tellg();

    setIfsToPos(ifs, currPos);

    return !lastPos;
}

int getMovieCount(std::ifstream& ifs) {
    const int currPos = ifs.tellg();
    int result = 0;

    while (true)
    {
        if (ifs.eof()) {
            break;
        }

        char buff[Constants::BUFF_SIZE];
        ifs.getline(buff, Constants::BUFF_SIZE);
        result++;
    }

    setIfsToPos(ifs, currPos);

    return result;
}

ErrorInCatalog checkForStreamErrors(std::ifstream& ifs) {
    if (!ifs.is_open()) {
        return ErrorInCatalog::catalog_not_open;
    }

    if (isCatalogEmpty(ifs)) {
        return ErrorInCatalog::read_from_empty_catalog;
    }

    return ErrorInCatalog::no_error_occurred;
}

SafeAnswer getNumberOfMovies(const char* catalogName) {
    std::ifstream ifs(catalogName);
    ErrorInCatalog noError = ErrorInCatalog::no_error_occurred;

    int result = Constants::INVALID_VALUE;

    {
        ErrorInCatalog potentialError = checkForStreamErrors(ifs);

        if (noError != potentialError) {
            return { result, potentialError };
        }
    }

    result = getMovieCount(ifs);

    return { result, noError };
}

double getAverageMoviePrice(std::ifstream& ifs) {
    const int currPos = ifs.tellg();
    double result = 0;
    int movieCount = 0;

    while (true)
    {
        if (ifs.eof()) {
            break;
        }

        char buff[Constants::BUFF_SIZE];
        int currentPrice;
        ifs >> buff >> currentPrice;
        result += currentPrice;
        movieCount++;
    }

    result /= movieCount;

    setIfsToPos(ifs, currPos);

    return result;
}

SafeAnswer averagePrice(const char* catalogName) {
    std::ifstream ifs(catalogName);
    ErrorInCatalog noError = ErrorInCatalog::no_error_occurred;

    int result = Constants::INVALID_VALUE;

    {
        ErrorInCatalog potentialError = checkForStreamErrors(ifs);

        if (noError != potentialError) {
            return { result, potentialError };
        }
    }

    result = getAverageMoviePrice(ifs);

    return { result, noError };
}

int getPriceOfMovie(std::ifstream& ifs, const char* movie) {
    const int currPos = ifs.tellg();
    int resultNum = Constants::INVALID_VALUE;

    while (true)
    {
        if (ifs.eof()) {
            break;
        }

        char buff[Constants::BUFF_SIZE];
        ifs >> buff;
        if (strcmp(buff, movie) == 0) {
            ifs >> resultNum;
            break;
        }
        else {
            ifs >> buff;
        }
    }

    setIfsToPos(ifs, currPos);

    return resultNum;
}

SafeAnswer getMoviePrice(const char* catalogName, const char* movieName) {
    std::ifstream ifs(catalogName);
    ErrorInCatalog noError = ErrorInCatalog::no_error_occurred;

    int result = Constants::INVALID_VALUE;

    {
        ErrorInCatalog potentialError = checkForStreamErrors(ifs);
        if (noError != potentialError) {
            return { result, potentialError };
        }
    }

    result = getPriceOfMovie(ifs, movieName);

    if (result == Constants::INVALID_VALUE) {
        return { result, ErrorInCatalog::movie_not_in_catalog };
    }
    else {
        return { result, noError };
    }

}

Movie readMovie(std::ifstream& ifs) { //добавете аргумент - файлов поток за четене
    Movie movie;

    ifs >> movie.name;
    ifs >> movie.price;

    return movie;
}

Movie* saveMoviesInArray(std::ifstream& ifs, int numberOfMovies) {
    const int currPos = ifs.tellg();
    Movie* arr = new Movie[numberOfMovies];

    for (unsigned i = 0; i < numberOfMovies; i++)
    {
        arr[i] = readMovie(ifs);
    }

    setIfsToPos(ifs, currPos);

    return arr;
}

void freeMoviesFromArray(Movie*& arr) { // добавете нужните аргументи
    delete[] arr;
    arr = nullptr;
}

void sortMoviesInArray(Movie* arr, size_t moviesCount) { // добавете нужните аргументи
    //Selection sort
    for (unsigned i = 0; i < moviesCount - 1; i++)
    {
        unsigned currMinIndex = i;
        for (unsigned j = i + 1; j < moviesCount; j++)
        {
            if (arr[j].price < arr[currMinIndex].price) {
                currMinIndex = j;
            }
        }
        if (currMinIndex != i) {
            std::swap(arr[i], arr[currMinIndex]);
        }
    }
}

void writeSortedMovies(std::ofstream& ofs, const Movie* arr, int moviesCount) {
    const char SEP = ' ';
    for (unsigned i = 0; i < moviesCount; i++)
    {
        ofs << arr[i].name << SEP << arr[i].price << '\n';
    }
}

ErrorInCatalog saveMoviesSorted(const char* catalogName, const char* catalogSortedName) {
    // Ако файл, отворен с файлов поток за писане, не съществува, то той се създава
    // Първо намерете колко филма има във файла с име catalogName
    // след това продължете с имплементацията на функцията
    std::ifstream ifs(catalogName);
    ErrorInCatalog noError = ErrorInCatalog::no_error_occurred;

    {
        ErrorInCatalog potentialError = checkForStreamErrors(ifs);

        if (noError != potentialError) {
            return potentialError;
        }
    }

    int movieCount = getMovieCount(ifs);
    Movie* movies = saveMoviesInArray(ifs, movieCount);
    sortMoviesInArray(movies, movieCount);

    std::ofstream ofs(catalogSortedName);

    // Погледнете примера за писане във файл
    writeSortedMovies(ofs, movies, movieCount);

    freeMoviesFromArray(movies);

    return noError;
}

void printSortedMovies(const char* catalogSortedName) {
    std::ifstream ifs(catalogSortedName);

    {
        ErrorInCatalog potentialError = checkForStreamErrors(ifs);

        if (potentialError != ErrorInCatalog::no_error_occurred) {
            return;
        }
    }

    while (true)
    {
        if (ifs.eof()) {
            break;
        }

        char buff[Constants::BUFF_SIZE];
        ifs.getline(buff, Constants::BUFF_SIZE);
        std::cout << buff << std::endl;
    }
}

int main() {
    SafeAnswer safeNumberOfMovies = getNumberOfMovies("movieCatalog.txt");
    if (safeNumberOfMovies.error == ErrorInCatalog::no_error_occurred) {
        std::cout << "The number of movies is: " << safeNumberOfMovies.number << std::endl;
    }
    SafeAnswer safeAveragePrice = averagePrice("movieCatalog.txt");
    if (safeAveragePrice.error == ErrorInCatalog::no_error_occurred) {
        std::cout << "The average price is: " << safeAveragePrice.number << std::endl;
    }

    SafeAnswer safePrice = getMoviePrice("movieCatalog.txt", "FullMetal-Alchemist-Brotherhood");
    if (safePrice.error == ErrorInCatalog::no_error_occurred) {
        std::cout << "The price for the FMAB movies is: " << safePrice.number << std::endl;
    }

    ErrorInCatalog errorSorting = saveMoviesSorted("movieCatalog.txt", "movieCatalogSorted.txt");
    if (errorSorting == ErrorInCatalog::no_error_occurred) {
        std::cout << "Look the content of the movieCatalogSorted.txt file" << std::endl << std::endl;
        printSortedMovies("movieCatalogSorted.txt");
    }
}
