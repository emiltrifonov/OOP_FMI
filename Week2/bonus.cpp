#include<iostream>
#include<cstring>
#include<fstream>

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
    int currPos = ifs.tellg();
    ifs.seekg(0, std::ios::end);
    int lastPos = ifs.tellg();

    setIfsToPos(ifs, currPos);

    return !lastPos;
}

int getMovieCount(std::ifstream& ifs) {
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

    setIfsToPos(ifs, currPos);

    return result;
}

ErrorInCatalog checkForErrors(std::ifstream& ifs) {
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

    {
        ErrorInCatalog potentialError = checkForErrors(ifs);

        if (noError != potentialError) {
            return { -1, potentialError };
        }
    }

    int result = getMovieCount(ifs);

    return { result, noError };
}

double getAverageMoviePrice(std::ifstream& ifs) {
    int currPos = ifs.tellg();
    double result = 0;
    int movieCount = 0;

    while (true)
    {
        if (ifs.eof()) {
            break;
        }

        char buff[1024];
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

    {
        ErrorInCatalog potentialError = checkForErrors(ifs);

        if (noError != potentialError) {
            return { -1, potentialError };
        }
    }

    int result = getAverageMoviePrice(ifs);

    return { result, noError };
}

int getPriceOfMovie(std::ifstream& ifs, const char* movie) {
    int currPos = ifs.tellg();
    int result = -1;

    while (true)
    {
        if (ifs.eof()) {
            break;
        }
        char buff[1024];
        ifs >> buff;
        if (strcmp(buff, movie) == 0) {
            ifs >> result;
            break;
        }
        else {
            ifs >> buff;
        }
    }

    setIfsToPos(ifs, currPos);

    return result;
}

SafeAnswer getMoviePrice(const char* catalogName, const char* movieName) {
    std::ifstream ifs(catalogName);
    ErrorInCatalog noError = ErrorInCatalog::no_error_occurred;

    {
        ErrorInCatalog potentialError = checkForErrors(ifs);

        if (noError != potentialError) {
            return { -1, potentialError };
        }
    }

    int result = getPriceOfMovie(ifs, movieName);

    if (result < 0) {
        noError = ErrorInCatalog::movie_not_in_catalog;
    }

    return { result, noError };
}

Movie readMovie(std::ifstream& ifs) { //добавете аргумент - файлов поток за четене
    Movie movie;

    ifs >> movie.name;
    ifs >> movie.price;

    return movie;
}

Movie* saveMoviesInArray(std::ifstream& ifs, int numberOfMovies) {
    int currPos = ifs.tellg();
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
    const char sep = ' ';
    for (unsigned i = 0; i < moviesCount; i++)
    {
        ofs << arr[i].name << sep << arr[i].price << std::endl;
    }
}

ErrorInCatalog saveMoviesSorted(const char* catalogName, const char* catalogSortedName) {
    // Ако файл, отворен с файлов поток за писане, не съществува, то той се създава 
    // Първо намерете колко филма има във файла с име catalogName 
    // след това продължете с имплементацията на функцията
    std::ifstream ifs(catalogName);
    ErrorInCatalog noError = ErrorInCatalog::no_error_occurred;

    {
        ErrorInCatalog potentialError = checkForErrors(ifs);

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
        ErrorInCatalog potentialError = checkForErrors(ifs);

        if (potentialError != ErrorInCatalog::no_error_occurred) {
            return;
        }
    }

    int currPos = ifs.tellg();

    while (true)
    {
        if (ifs.eof()) {
            break;
        }

        constexpr size_t SIZE = 1024;
        char buff[SIZE];
        ifs.getline(buff, SIZE);
        std::cout << buff << std::endl;
    }

    setIfsToPos(ifs, currPos);
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

    SafeAnswer safePrice = getMoviePrice("movieCatalog.txt", "Black-bullet");
    if (safePrice.error == ErrorInCatalog::no_error_occurred) {
        std::cout << "The price for the Black bullet movies is: " << safePrice.number << std::endl;
    }

    ErrorInCatalog errorSorting = saveMoviesSorted("movieCatalog.txt", "movieCatalogSorted.txt");
    if (errorSorting == ErrorInCatalog::no_error_occurred) {
        std::cout << "Look the content of the movieCatalogSorted.txt file" << std::endl;
        printSortedMovies("movieCatalogSorted.txt");
    }
}
