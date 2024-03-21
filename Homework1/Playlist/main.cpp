#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cmath>
#pragma warning (disable : 4996)

namespace Constants {
    constexpr size_t MAX_NAME_LEN = 64;
    constexpr uint8_t GENRE_MAX = 1 + 2 + 4 + 8 + 16; // 31
    constexpr short MAX_LYRICS_LEN = 256;
    constexpr short MAX_SONGS_IN_PLAYLIST = 30;
}

enum class Genre : uint8_t {
    UNDEFINED = 0,
    ROCK = 1,
    POP = 2,
    HIPHOP = 4,
    ELECTRONIC_MUSIC = 8,
    JAZZ = 16,
};

int getFileSize(const char* file) {
    int result = -1;
    if (!file) {
        return result;
    }

    std::ifstream ifs(file, std::ios::binary);
    if (!ifs.is_open()) {
        return result;
    }

    ifs.seekg(0, std::ios::end);
    result = ifs.tellg();
    ifs.close();

    return result;
}

uint8_t getEnumFromChar(char ch) {
    switch (ch)
    {
    case 'r': return (int)Genre::ROCK;
        break;
    case 'p': return (int)Genre::POP;
        break;
    case 'h': return (int)Genre::HIPHOP;
        break;
    case 'e': return (int)Genre::ELECTRONIC_MUSIC;
        break;
    case 'j': return (int)Genre::JAZZ;
        break;
    default: return 0;
        break;
    }
}

struct Song {
private:
    int length = 0; //in seconds
    short lyricsLen = 0;
    uint8_t lyrics[Constants::MAX_LYRICS_LEN]{ 0 };
    uint8_t genre = 0;
    char name[Constants::MAX_NAME_LEN + 1] = "Unknown";
    bool isValidSong = true;

    void printLengthConverted() const;
    void printGenres() const;

public:
    Song() = default;
    Song(const char* name, int lengthInSecs, const char* genre, const char* lyricsFile) {
        setLength(lengthInSecs);
        setLyrics(lyricsFile);
        setName(name);
        setGenre(genre);
    }
    void setLength(int length) {
        if (length > 0) {
            this->length = length;
        }
        else {
            isValidSong = false;
        }
    }
    void setLyrics(const char* lyrics) {
        if (!lyrics) {
            isValidSong = false;
            return;
        }

        int lyricsLen = getFileSize(lyrics); //binary stream o/c

        if (lyricsLen > Constants::MAX_LYRICS_LEN) {
            isValidSong = false;
            return;
        }

        std::ifstream ifs(lyrics, std::ios::binary);
        if (!ifs.is_open()) {
            isValidSong = false;
            return;
        }
        ifs.seekg(0);
        this->lyricsLen = lyricsLen;

        for (unsigned i = 0; i < lyricsLen; i++)
        {
            ifs.read((char*)&this->lyrics[i], sizeof(this->lyrics[i]));
            //std::cout << lyrics[i] << std::endl;
        }

        ifs.close();
    }
    void setName(const char* name) {
        if (!name || strlen(name) > Constants::MAX_NAME_LEN) {
            isValidSong = false;
            return;
        }
        else {
            strcpy(this->name, name);
        }
    }
    void setGenre(const char* genre) {
        if (!genre) {
            isValidSong = false;
            return;
        }

        uint8_t result = 0;
        while (*genre)
        {
            result += getEnumFromChar(*genre);
            genre++;
        }

        this->genre = result;
    }
    int getLength() const {
        return length;
    }
    const char* getName() const {
        return name;
    }
    uint8_t getGenre() const {
        return genre;
    }
    void addRhythm(int k);
    void mixSong(const Song& other);
    void printSong() const;
    bool isValid() const;
    void printLyrics() const;
};

void Song::addRhythm(int k) {
    if (k < 0 || k > 8) {
        return;
    }

    int powerOfTwo = k - 1;

    for (int i = lyricsLen; i >= 0; i--)
    {
        while (powerOfTwo < 8)
        {
            int toAdd = (1 << powerOfTwo);
            lyrics[i] += toAdd;
            powerOfTwo += k;
        }

        powerOfTwo -= 8;
    }
}

void Song::mixSong(const Song& other) {
    if (!isValidSong || !other.isValidSong) {
        return;
    }

    unsigned short bytesToMix = std::min(lyricsLen, other.lyricsLen);

    for (unsigned i = 0; i < bytesToMix; i++) {
        lyrics[i] = (lyrics[i] ^ other.lyrics[i]);
    }
}

void Song::printLengthConverted() const {
    int temp = length;

    int hours = 0;
    if (temp >= 3600) {
        hours = temp / 3600;
        temp %= 3600;
    }

    int minutes = 0;
    if (temp >= 60) {
        minutes = temp / 60;
        temp %= 60;
    }

    int seconds = temp;

    if (hours < 10) {
        std::cout << "0";
    }
    std::cout << hours << ":";

    if (minutes < 10) {
        std::cout << "0";
    }
    std::cout << minutes << ":";

    if (seconds < 10) {
        std::cout << "0";
    }
    std::cout << seconds;
}

void Song::printGenres() const {
    const char sep[] = "/";
    bool printSep = false;

    if (genre == 0) {
        std::cout << "Undefied";
        return;
    }
    if ((genre & (uint8_t)Genre::ROCK) != 0) {
        std::cout << "Rock";
        printSep = true;
    }
    if ((genre & (uint8_t)Genre::POP) != 0) {
        if (printSep) {
            std::cout << sep;
        }
        std::cout << "Pop";
        printSep = true;
    }
    if ((genre & (uint8_t)Genre::HIPHOP) != 0) {
        if (printSep) {
            std::cout << sep;
        }
        std::cout << "Hip-hop";
        printSep = true;
    }
    if ((genre & (uint8_t)Genre::ELECTRONIC_MUSIC) != 0) {
        if (printSep) {
            std::cout << sep;
        }
        std::cout << "Electronic music";
        printSep = true;
    }
    if ((genre & (uint8_t)Genre::JAZZ) != 0) {
        if (printSep) {
            std::cout << sep;
        }
        std::cout << "Jazz";
    }
}

void Song::printSong() const {
    if (!isValidSong) {
        return;
    }

    std::cout << "Name: " << name << ", ";
    std::cout << "Length: ";
    printLengthConverted();
    std::cout << ", ";
    std::cout << "Genre: ";
    printGenres();
    std::cout << std::endl;
}

bool Song::isValid() const {
    return isValidSong;
}

void Song::printLyrics() const {
    for (unsigned i = 0; i < lyricsLen; i++)
    {
        std::cout << (char)lyrics[i] << " ";
    }
}

class Playlist {
private:
    Song songs[Constants::MAX_SONGS_IN_PLAYLIST];
    unsigned short songsCount = 0;

public:
    Playlist() = default;
    void addSong(const char* name, int length, const char* genre, const char* lyricsFile);
    void printSongs() const;
    Song getSongByName(const char* name) const;
    void printAllSongsWithGenre(char genre) const;
    void sort(bool(*pred)(Song, Song));
    void saveSongToFile(const char* songName, const char* fileName) const;
};

void Playlist::addSong(const char* name, int lengthInSecs, const char* genre, const char* lyricsFile) {
    if (!lyricsFile || !genre) {
        return;
    }

    if (songsCount < Constants::MAX_SONGS_IN_PLAYLIST) {
        songs[songsCount].setName(name);
        songs[songsCount].setLength(lengthInSecs);
        songs[songsCount].setGenre(genre);
        songs[songsCount].setLyrics(lyricsFile);
        songsCount++;
    }
}

void Playlist::printSongs() const {
    for (unsigned i = 0; i < songsCount; i++)
    {
        songs[i].printSong();
        //songs[i].printLyrics();
        std::cout << std::endl;
    }
}

Song Playlist::getSongByName(const char* name) const {
    if (!name) {
        Song s;
        return s;
    }

    for (unsigned i = 0; i < songsCount; i++)
    {
        if (strcmp(songs[i].getName(), name) == 0) {
            return songs[i];
        }
    }
}

void Playlist::printAllSongsWithGenre(char genre) const {
    uint8_t genreNum = getEnumFromChar(genre);

    for (unsigned i = 0; i < songsCount; i++)
    {
        if ((songs[i].getGenre() & genreNum) != 0) {
            songs[i].printSong();
        }
    }
}

void Playlist::sort(bool(*pred)(Song, Song)) {
    for (unsigned i = 0; i < songsCount - 1; i++)
    {
        unsigned currMinInd = i;
        for (unsigned j = i + 1; j < songsCount; j++)
        {
            if (pred(songs[j], songs[currMinInd])) {
                currMinInd = j;
            }
        }
        if (currMinInd != i) {
            std::swap(songs[i], songs[currMinInd]);
        }
    }
}

void Playlist::saveSongToFile(const char* songName, const char* fileName) const {
    if (!songName || !fileName) {
        return;
    }

    std::ofstream ofs(fileName, std::ios::binary | std::ios::trunc);
    if (!ofs.is_open()) {
        return;
    }

    Song s = getSongByName(songName);

    ofs.write((const char*)&s, sizeof(s));
    ofs.close();
}

int main()
{
    // !!!!!!!!!!!! Reading the lyrics from a file means reading the whole song and then copying the lyrics (ig?)
    uint8_t lyrics[3];
    lyrics[0] = 67; // 0 1 0 0  0 0 1 1
    lyrics[1] = 111;// 0 1 1 0  1 1 1 1
    lyrics[2] = 119;// 0 1 1 1  0 1 1 1
    const char file[] = "song1.dat";
    std::ofstream ofs(file, std::ios::binary);
    ofs.write((const char*)lyrics, sizeof(lyrics));
    ofs.close();

    Playlist p;
    p.addSong("Pesen", 413, "jrp", file);
    p.addSong("103 kila", 146, "rh", file);
    p.addSong("tiktok short", 79, "e", file);
    p.addSong("rech na stalin", 314, "je", file);

    p.sort( [](Song a, Song b) {return a.getLength() < b.getLength(); }); //sort by length
    p.printSongs();
    std::cout << "-----------------------\n\n";
    p.sort([](Song a, Song b) {return strcmp(a.getName(), b.getName()) < 0; }); //sort by name
    p.printSongs();

    p.saveSongToFile("103 kila", "newFile.dat");
}
