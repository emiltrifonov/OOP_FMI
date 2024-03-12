#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>

namespace Constants {
    const unsigned short MIN_POWER = 10;
    const unsigned short MAX_POWER = 1000;
    const unsigned short MAX_NAME_LEN = 50;
    const unsigned short BUFF_SIZE = 1024;
    const unsigned short MIN_ENUM_VALUE = 1;
    const unsigned short MAX_ENUM_VALUE = 7;
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
    unsigned int power;
};

struct PokemonHandler {
    char binaryFileName[Constants::BUFF_SIZE];
};

void sortPokemonsInFileByPower(const PokemonHandler& ph);
int size(const PokemonHandler& ph);
void swapPokemons(const PokemonHandler& ph, int i, int j);

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

    unsigned short choice = USHRT_MAX;

    printPokemonTypes();
    std::cout << "Choose your pokemon's type(1 to 7): ";

    while (true)
    {
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

unsigned short getPokemonPower() {
    unsigned short result = 0;
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

    return result;
}

void addPokemonToBinary(const PokemonHandler& ph, const Pokemon& pokemon) {
    std::ofstream ofs(ph.binaryFileName, std::ios::binary | std::ios::app);
    if (!ofs.is_open()) {
        std::cout << "Can't save pokemon, file is not open." << std::endl;
        return;
    }

    ofs.write((const char*)&pokemon, sizeof(pokemon));
    ofs.close();

    sortPokemonsInFileByPower(ph);
}

void printPokemon(const Pokemon& pokemon) {
    std::cout << "Pokemon name: " << pokemon.name << std::endl;
    std::cout << "Pokemon type: ";
    printTypeFromNum((int)pokemon.type);
    std::cout << std::endl;
    std::cout << "Pokemon power: " << pokemon.power << std::endl;
    std::cout << "-------------------------\n";
}

void printAllPokemons(const PokemonHandler& ph) {
    std::ifstream ifs(ph.binaryFileName, std::ios::binary);
    if (!ifs.is_open()) {
        return;
    }

    for (unsigned i = 0; i < size(ph); i++)
    {
        Pokemon temp;
        ifs.read((char*)&temp, sizeof(temp));
        printPokemon(temp);
    }
}

void sortPokemonsInFileByPower(const PokemonHandler& ph) {
    std::fstream fstr(ph.binaryFileName, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    if (!fstr.is_open()) {
        return;
    }
    int pokemonsCount = size(ph);
    fstr.seekg(0);
    fstr.seekp(0);
    for (unsigned i = 0; i < pokemonsCount - 1; i++)
    {
        unsigned currMaxInd = i;
        for (unsigned j = i + 1; j < pokemonsCount; j++)
        {
            fstr.seekg(i * sizeof(Pokemon));
            Pokemon pokI;
            fstr.read((char*)&pokI, sizeof(Pokemon));

            fstr.seekg(j * sizeof(Pokemon));
            Pokemon pokJ;
            fstr.read((char*)&pokJ, sizeof(Pokemon));

            if (pokI.power < pokJ.power) {
                currMaxInd = j;
            }
        }
        if (currMaxInd != i) {
            swapPokemons(ph, i, currMaxInd);
        }
    }
}

PokemonHandler newPokemonHandler(const char* fileName) {
    if (!fileName) {
        return {};
    }

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
    int fileSize = ifs.tellg();

    ifs.close();

    return (fileSize / sizeof(Pokemon));
}

Pokemon at(const PokemonHandler& ph, int i) {
    int pokemonsCount = size(ph);
    
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
        int pokemonsCount = size(ph);
        if (i < 0 || i > pokemonsCount - 1 || j < 0 || j > pokemonsCount - 1 || i == j) {
            return;
        }
    }

    Pokemon pokI = at(ph, i);
    Pokemon pokJ = at(ph, j);

    std::ofstream ofs(ph.binaryFileName, std::ios::binary || std::ios::ate);
    if (!ofs.is_open()) {
        return;
    }

    ofs.seekp(i * sizeof(Pokemon));
    ofs.write((const char*)&pokJ, sizeof(pokJ));
    ofs.seekp(j * sizeof(Pokemon));
    ofs.write((const char*)&pokI, sizeof(pokI));

    ofs.close();
}

void insert(const PokemonHandler& ph, const Pokemon& pokemon) {
    addPokemonToBinary(ph, pokemon);
    sortPokemonsInFileByPower(ph);
}

void writePokemonToTextFile(std::ofstream& ofs, const Pokemon& pokemon) {
    ofs << pokemon.name << Constants::SEP << int(pokemon.type) << Constants::SEP << pokemon.power;
}

void textify(const PokemonHandler& ph, const char* fileName, int skipAtIndex = -1) {
    if (!fileName) {
        return;
    }

    std::ofstream ofs(fileName, std::ios::trunc);
    if (!ofs.is_open()) {
        return;
    }
    int pokemonsCount = size(ph);

    for (unsigned i = 0; i < pokemonsCount; i++)
    {
        Pokemon temp = at(ph, i);
        if (i == skipAtIndex) {
            
        }
        else {
            writePokemonToTextFile(ofs, temp);
        }

        if (i != pokemonsCount - 1 && i != skipAtIndex) {
            ofs << std::endl;
        }
    }

    ofs.close();
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

    int type;
    ss >> type;
    result.type = getTypeFromNum(type);
    ss.ignore();

    ss >> result.power;

    return result;
}

void emptyBinaryFile(const PokemonHandler& ph) {
    std::ofstream ofs(ph.binaryFileName, std::ios::trunc);
    ofs.close();
}

void untextify(const PokemonHandler& ph, const char* fileName) {
    if (!fileName) {
        return;
    }

    std::ifstream ifs(fileName);
    if (!ifs.is_open()) {
        return;
    }

    emptyBinaryFile(ph);

    while (true)
    {
        if (ifs.eof()) {
            break;
        }

        char buff[Constants::BUFF_SIZE];
        ifs.getline(buff, Constants::BUFF_SIZE);
        Pokemon temp = parseRow(buff);
        addPokemonToBinary(ph, temp);
    }

    ifs.close();

    sortPokemonsInFileByPower(ph);
}

void readPokemonsFromConsole(const PokemonHandler& ph, const char* textFileName) {
    if (!textFileName) {
        return;
    }

    int pCount;
    //pCount = 0;
    std::cout << "Choose how many pokemons to create: ";
    std::cin >> pCount;
    resetCin();
    for (unsigned short i = 0; i < pCount; i++)
    {
        Pokemon p = getPokemonFromConsole();
        addPokemonToBinary(ph, p);
    }

    sortPokemonsInFileByPower(ph);
    textify(ph, textFileName);
}

void deletePokemonAt(const PokemonHandler& ph, int index, const char* textFileName) {
    textify(ph, textFileName, index);
    untextify(ph, textFileName);
}

int main()
{
    const char binaryFileName[] = "pokemonDatabase.dat";
    const char textFileName[] = "pokemonDatabase.txt";
    PokemonHandler ph = newPokemonHandler(binaryFileName);

    readPokemonsFromConsole(ph, textFileName);

    //deletePokemonAt(ph, 7, textFileName);

    int pokemonsCount = size(ph);
    std::cout << "Size of pokemon collection: " << pokemonsCount << std::endl;
    printAllPokemons(ph);

    //textify(ph, textFileName);
    //untextify(ph, textFileName);
}
