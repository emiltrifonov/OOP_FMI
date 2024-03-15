#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <cassert>

namespace Constants {
    const unsigned MIN_POWER = 10;
    const unsigned MAX_POWER = 1000;
    const unsigned MAX_NAME_LEN = 50;
    const unsigned BUFF_SIZE = 1024;
    const unsigned MIN_ENUM_VALUE = 1;
    const unsigned MAX_ENUM_VALUE = 7;
    const unsigned INDEX_LINE_LEN = 25;
    const char SEP = ';';
}

enum class Type {
    NORMAL = 1, //changed to 1 because std::cin converts invalid integer input to 0
    FIRE,
    WATER,
    GRASS,
    ELECTRIC,
    GHOST,
    FLYING,
    UNDEFINED
};

struct Pokemon {
    char name[Constants::MAX_NAME_LEN];
    Type type;
    unsigned power;
};

struct PokemonHandler {
    char binaryFileName[Constants::BUFF_SIZE];
};

void sortPokemonsInFileByPower(const PokemonHandler& ph);
int size(const PokemonHandler& ph);
void swapPokemons(const PokemonHandler& ph, int i, int j);
void emptyFile(const char* fileName);

void resetCin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printTypeFromNum(int num) {
    switch (num)
    {
    case Constants::MIN_ENUM_VALUE:
        std::cout << "Normal";
        break;
    case Constants::MIN_ENUM_VALUE + 1:
        std::cout << "Fire";
        break;
    case Constants::MIN_ENUM_VALUE + 2:
        std::cout << "Water";
        break;
    case Constants::MIN_ENUM_VALUE + 3:
        std::cout << "Grass";
        break;
    case Constants::MIN_ENUM_VALUE + 4:
        std::cout << "Electric";
        break;
    case Constants::MIN_ENUM_VALUE + 5:
        std::cout << "Ghost";
        break;
    case Constants::MIN_ENUM_VALUE + 6:
        std::cout << "Flying";
        break;
    default:
        break;
    }
}

void printPokemonTypes() {
    for (unsigned i = Constants::MIN_ENUM_VALUE; i <= Constants::MAX_ENUM_VALUE; i++)
    {
        std::cout << i << ": ";
        printTypeFromNum(i);
        std::cout << std::endl;
    }
}

Type getTypeFromNum(int num) {
    Type result = Type::UNDEFINED;
    switch (num)
    {
    case Constants::MIN_ENUM_VALUE:
        result = Type::NORMAL;
        break;
    case Constants::MIN_ENUM_VALUE + 1:
        result = Type::FIRE;
        break;
    case Constants::MIN_ENUM_VALUE + 2:
        result = Type::WATER;
        break;
    case Constants::MIN_ENUM_VALUE + 3:
        result = Type::GRASS;
        break;
    case Constants::MIN_ENUM_VALUE + 4:
        result = Type::ELECTRIC;
        break;
    case Constants::MIN_ENUM_VALUE + 5:
        result = Type::GHOST;
        break;
    case Constants::MIN_ENUM_VALUE + 6:
        result = Type::FLYING;
        break;
    default:
        break;
    }

    return result;
}

void setPokemonName(Pokemon& p) {
    std::cout << "Choose your pokemon's name(max. length 50): ";
    char buff[Constants::BUFF_SIZE];
    while(true)
    {
        std::cin.getline(buff, Constants::BUFF_SIZE);
        if (strlen(buff) > Constants::MAX_NAME_LEN)
        {
            resetCin();
        }
        else {
            break;
        }
    }

    strcpy_s(p.name, buff);
}

Type getPokemonType() {
    Type result;

    unsigned choice = 0;

    printPokemonTypes();

    while (true)
    {
        std::cout << "Choose your pokemon's type(1 to 7): ";
        std::cin >> choice;
        resetCin();
        if (choice >= Constants::MIN_ENUM_VALUE && choice <= Constants::MAX_ENUM_VALUE)
        {
            result = getTypeFromNum(choice);
            break;
        }
    }

    return result;
}

unsigned getPokemonPower() {
    unsigned result = 0;
    std::cout << "Choose your pokemon's power(10 to 1000): ";

    while (result < Constants::MIN_POWER || result > Constants::MAX_POWER)
    {
        std::cin >> result;
        resetCin();
    }

    return result;
}

Pokemon getPokemonFromConsole() {
    Pokemon result{};
    setPokemonName(result);
    result.type = getPokemonType();
    result.power = getPokemonPower();

    std::cout << std::endl;

    return result;
}

void insert(const PokemonHandler& ph, const Pokemon& pokemon) { }
//used a different name
void addPokemonToBinary(const PokemonHandler& ph, const Pokemon& pokemon) {
    std::ofstream ofs(ph.binaryFileName, std::ios::binary | std::ios::app);
    if (!ofs.is_open()) {
        std::cout << "Can't save pokemon, file is not open.\n";
        return;
    }

    ofs.write((const char*)&pokemon, sizeof(pokemon));
    ofs.close();
}

unsigned getNumLen(unsigned num) {
    unsigned result = 1;
    if (num > 9) {
        result += log10(num);
    }

    return result;
}

void printPokemonIndexLine(unsigned index) {
    std::cout << index << ". ";

    const unsigned indexLen = getNumLen(index);

    for (unsigned i = 0; i < Constants::INDEX_LINE_LEN - indexLen; i++)
    {
        std::cout << '-';
    }

    std::cout << std::endl;
}

void printPokemon(const Pokemon& pokemon, unsigned index) {
    printPokemonIndexLine(index);
    std::cout << "Pokemon name: " << pokemon.name << std::endl;
    std::cout << "Pokemon type: ";
    printTypeFromNum((int)pokemon.type);
    std::cout << std::endl;
    std::cout << "Pokemon power: " << pokemon.power << std::endl;
}

void printAllPokemons(const PokemonHandler& ph) {
    const unsigned pokemonsCount = size(ph);
    std::ifstream ifs(ph.binaryFileName, std::ios::binary);
    if (!ifs.is_open()) {
        return;
    }
    
    std::cout << "Current pokemon collection:\n\n";

    if (pokemonsCount == 0) {
        std::cout << "(it's empty)\n";
    }

    for (unsigned i = 0; i < pokemonsCount; i++)
    {
        Pokemon temp;
        ifs.read((char*)&temp, sizeof(temp));
        printPokemon(temp, i);
    }

    std::cout << std::endl;

    ifs.close();
}

void sortPokemonsInFileByPower(const PokemonHandler& ph) {
    const unsigned pokemonsCount = size(ph);//binary file stream o/c
    
    for (unsigned i = 0; i < pokemonsCount; i++)
    {
        std::ifstream ifs(ph.binaryFileName, std::ios::binary);//binary file stream o
        if (!ifs.is_open()) {
            return;
        }

        unsigned currMaxInd = i;
        for (unsigned j = i + 1; j < pokemonsCount; j++)
        {
            ifs.seekg(currMaxInd * sizeof(Pokemon));
            Pokemon pokCurrMax;
            ifs.read((char*)&pokCurrMax, sizeof(Pokemon));

            ifs.seekg(j * sizeof(Pokemon));
            Pokemon pokJ;
            ifs.read((char*)&pokJ, sizeof(Pokemon));

            if (pokCurrMax.power < pokJ.power) {
                currMaxInd = j;
            }
        }

        ifs.close();//binary file stream c

        if (currMaxInd != i) {
            swapPokemons(ph, i, currMaxInd);//binary file stream o/c
        }
    }
}

PokemonHandler newPokemonHandler(const char* fileName) {
    if (!fileName) {
        assert(fileName);
    }

    //In case file doesn't exist
    std::ofstream ofs(fileName, std::ios::binary | std::ios::in);
    ofs.open(fileName);
    ofs.close();

    PokemonHandler result;
    strcpy_s(result.binaryFileName, fileName);

    if (size(result) > 0) {
        sortPokemonsInFileByPower(result);
    }

    return result;
}

int size(const PokemonHandler& ph) {
    std::ifstream ifs(ph.binaryFileName, std::ios::binary);
    if (!ifs.is_open()) {
        return -1;
    }

    ifs.seekg(0, std::ios::end);
    const unsigned fileSize = ifs.tellg();

    ifs.close();

    return (fileSize / sizeof(Pokemon));
}

Pokemon at(const PokemonHandler& ph, int i) {
    const unsigned pokemonsCount = size(ph);
    
    Pokemon result = { "", Type::UNDEFINED, 0 };

    if (i < 0 || i > pokemonsCount - 1) {
        return result;
    }

    std::ifstream ifs(ph.binaryFileName, std::ios::binary);
    if (!ifs.is_open()) {
        return result;
    }
    ifs.seekg(i * sizeof(Pokemon));
    ifs.read((char*)&result, sizeof(result));

    ifs.close();

    return result;
}

void swapPokemons(const PokemonHandler& ph, int i, int j) {
    {
        const unsigned pokemonsCount = size(ph);
        if (i < 0 || i > pokemonsCount - 1 || j < 0 || j > pokemonsCount - 1 || i == j) {
            return;
        }
    }

    Pokemon pokI = at(ph, i);
    Pokemon pokJ = at(ph, j);

    std::ofstream ofs(ph.binaryFileName, std::ios::binary | std::ios::in);
    if (!ofs.is_open()) {
        return;
    }

    ofs.seekp(i * sizeof(Pokemon));
    ofs.write((const char*)&pokJ, sizeof(pokJ));
    ofs.seekp(j * sizeof(Pokemon));
    ofs.write((const char*)&pokI, sizeof(pokI));

    ofs.close();
}

void writePokemonToTextFile(std::ofstream& ofs, const Pokemon& pokemon, bool addNewLine) {
    ofs << pokemon.name << Constants::SEP << int(pokemon.type) << Constants::SEP << pokemon.power;
    if (addNewLine) {
        ofs << '\n';
    }
}

void textify(const PokemonHandler& ph, const char* fileName, int skipAtIndex = -1) {
    if (!fileName) {
        return;
    }

    std::ofstream ofs(fileName, std::ios::trunc);//text file stream o
    if (!ofs.is_open()) {
        return;
    }

    const unsigned pokemonsCount = size(ph);//binary file stream o/c

    for (unsigned i = 0; i < pokemonsCount; i++)
    {
        Pokemon temp = at(ph, i);//binary file stream o/c

        if (i == skipAtIndex) {
            continue;
        }

        bool addNewLine = (i != pokemonsCount - 1 && !(i == pokemonsCount - 2 && skipAtIndex == pokemonsCount - 1));

        writePokemonToTextFile(ofs, temp, addNewLine);
    }

    ofs.close();//text file stream c
}

unsigned getDigitFromChar(char ch) {
    return ch - '0';
}

unsigned getNumFromStr(const char* str) {
    if (!str) {
        return 0;
    }
    unsigned result = 0;

    while (*str)
    {
        result *= 10;
        result += getDigitFromChar(*str);

        str++;
    }

    return result;
}

Pokemon parseRow(const char* row) {
    if (!row) {
        return { "", Type::UNDEFINED, 0 };
    }
    Pokemon result;

    std::stringstream ss(row);

    char name[Constants::BUFF_SIZE];
    ss.getline(name, Constants::BUFF_SIZE, Constants::SEP);
    strcpy_s(result.name, name);

    char typeStr[Constants::BUFF_SIZE];
    ss.getline(typeStr, Constants::BUFF_SIZE, Constants::SEP);
    const unsigned typeNum = getNumFromStr(typeStr);
    result.type = getTypeFromNum(typeNum);

    char powerStr[Constants::BUFF_SIZE];
    ss.getline(powerStr, Constants::BUFF_SIZE, '\n');
    const unsigned powerNum = getNumFromStr(powerStr);
    result.power = powerNum;

    return result;
}

void emptyFile(const char* fileName) {
    if (!fileName) {
        return;
    }
    std::ofstream ofs(fileName, std::ios::trunc);
    ofs.open(fileName);
    ofs.close();
}

void untextify(const PokemonHandler& ph, const char* fileName) {
    if (!fileName) {
        return;
    }

    std::ifstream ifs(fileName);//text file stream o
    if (!ifs.is_open()) {
        return;
    }

    emptyFile(ph.binaryFileName);//binary file stream o/c

    while (true)
    {
        if (ifs.eof()) {
            break;
        }

        char buff[Constants::BUFF_SIZE];
        ifs.getline(buff, Constants::BUFF_SIZE);

        Pokemon temp = parseRow(buff);

        if (temp.type != Type::UNDEFINED) {
            addPokemonToBinary(ph, temp);//binary file stream o/c
        }
    }

    sortPokemonsInFileByPower(ph);//binary file stream o/c
    ifs.close();//text file stream c
}

void readPokemonsFromConsole(const PokemonHandler& ph, const char* textFileName) {
    if (!textFileName) {
        return;
    }

    int pokemonsCount;
    while(true)
    {
        std::cout << "Choose how many pokemons to create: ";
        std::cin >> pokemonsCount;

        if (std::cin.fail()) {
            resetCin();
            continue;
        }

        resetCin();

        if (pokemonsCount >= 0) {
            break;
        }
    }

    std::cout << std::endl;

    for (unsigned i = 0; i < pokemonsCount; i++)
    {
        Pokemon p = getPokemonFromConsole();
        addPokemonToBinary(ph, p);
    }

    if (size(ph) != 0) {
        sortPokemonsInFileByPower(ph);
        textify(ph, textFileName);
    }
}

void deletePokemonAt(const PokemonHandler& ph, int index, const char* textFileName) {
    if (!textFileName) {
        return;
    }
    textify(ph, textFileName, index);
    untextify(ph, textFileName);
}

void deletePokemonsFromConsole(const PokemonHandler& ph, const char* textFileName) {
    if (!textFileName) {
        return;
    }

    int index;
    
    while (true)
    {
        const unsigned pokemonsCount = size(ph);

        if (pokemonsCount == 0) {
            std::cout << "\nPokemon collection is already empty.\n\n";
            break;
        }

        std::cout << "Choose which pokemon to delete or -1 to stop: ";
        std::cin >> index;

        if (std::cin.fail()) {
            resetCin();
            continue;
        }

        resetCin();

        if (index == -1) {
            std::cout << std::endl;
            break;
        }
        else if (index < 0 || index > pokemonsCount - 1) {
            std::cout << "Incorrect input!\n";
            continue;
        }
        else if (pokemonsCount == 1) {
            emptyFile(ph.binaryFileName);
            break;
        }
        else {
            deletePokemonAt(ph, index, textFileName);
            printAllPokemons(ph);
        }
    }

    textify(ph, textFileName);
}

int main()
{
    const char binaryFileName[] = "newPokemonDatabase.dat";
    const char textFileName[] = "newPokemonDatabase.txt";
    PokemonHandler ph = newPokemonHandler(binaryFileName);

    emptyFile(ph.binaryFileName);
    textify(ph, textFileName);

    //Sinhronizaciq za kogato si igraq s pokemonite v textoviq file
    //untextify(ph, textFileName);
    //textify(ph, textFileName);

    printAllPokemons(ph);

    readPokemonsFromConsole(ph, textFileName);
    printAllPokemons(ph);

    deletePokemonsFromConsole(ph, textFileName);

    printAllPokemons(ph);
}
