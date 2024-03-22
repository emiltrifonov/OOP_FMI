#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cmath>
#include <cassert>
#pragma warning (disable : 4996)

namespace Constants {
    constexpr size_t MAX_NAME_LEN = 64;
    constexpr size_t MAX_LYRICS_LEN = 256;
    constexpr size_t MAX_SONGS_IN_PLAYLIST = 30;
    constexpr char UNDEFINED_GENRE_STR[] = "Undefined";
    constexpr char ROCK_GENRE_STR[] = "Rock";
    constexpr char POP_GENRE_STR[] = "Pop";
    constexpr char HIPHOP_GENRE_STR[] = "Hip-hop";
    constexpr char ELECTRONIC_MUSIC_GENRE_STR[] = "Electronic music";
    constexpr char JAZZ_GENRE_STR[] = "Jazz";
}

struct Song {
private:
    int length = 0; //in seconds
    size_t lyricsLen = 0;
    uint8_t lyrics[Constants::MAX_LYRICS_LEN]{ 0 };
    uint8_t genre = 0;
    char name[Constants::MAX_NAME_LEN + 1] = "Unknown";

    void printLengthConverted() const;
    void printGenres() const;
    void copyLyrics(const uint8_t* lyrics, size_t size);

public:
    Song() = default;
    Song(const char* name, int lengthInSecs, const char* genre, const char* lyricsFile);

    enum class Genre : uint8_t;

    void setLength(int length);
    void setLyrics(const char* lyrics);
    void setLyricAt(int ind, uint8_t lyric);
    void setName(const char* name);
    void setGenre(const char* genre);

    int getLength() const;
    const char* getName() const;
    uint8_t getGenre() const;
    const uint8_t* getLyrics() const;
    size_t getLyricsLen() const;

    void printSong() const;
    void printLyrics() const;
    bool isValid() const;
    void operator=(const Song& other);
};

enum class Song::Genre : uint8_t{
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
    case 'r': return (int)Song::Genre::ROCK;
        break;
    case 'p': return (int)Song::Genre::POP;
        break;
    case 'h': return (int)Song::Genre::HIPHOP;
        break;
    case 'e': return (int)Song::Genre::ELECTRONIC_MUSIC;
        break;
    case 'j': return (int)Song::Genre::JAZZ;
        break;
    default: return 0;
        break;
    }
}

Song::Song(const char* name, int lengthInSecs, const char* genre, const char* lyricsFile) {
    setLength(lengthInSecs);
    setLyrics(lyricsFile);
    setName(name);
    setGenre(genre);
}

void Song::setLength(int length) {
    if (length > 0) {
        this->length = length;
    }
}

void Song::setLyrics(const char* lyrics) {
    if (lyrics) {
        int lyricsLen = getFileSize(lyrics);
        if (lyricsLen > Constants::MAX_LYRICS_LEN) {
            return;
        }
    }

    std::ifstream ifs(lyrics, std::ios::binary);
    if (!ifs.is_open()) {
        return;
    }
    ifs.seekg(0);
    this->lyricsLen = lyricsLen;

    for (unsigned i = 0; i < lyricsLen; i++)
    {
        ifs.read((char*)&this->lyrics[i], sizeof(this->lyrics[i]));
    }

    ifs.close();
}

void Song::setLyricAt(int ind, uint8_t lyric) {
    if (ind < 0 || ind >= lyricsLen) {
        return;
    }
    else {
        lyrics[ind] = lyric;
    }
}

void Song::setName(const char* name) {
    if (!name || strlen(name) > Constants::MAX_NAME_LEN) {
        return;
    }
    else {
        strcpy(this->name, name);
    }
}

void Song::setGenre(const char* genre) {
    if (genre) {
        uint8_t result = 0;
        while (*genre)
        {
            result += getEnumFromChar(*genre);
            genre++;
        }

        this->genre = result;
    }
}

int Song::getLength() const {
    return length;
}

const char* Song::getName() const {
    return name;
}

uint8_t Song::getGenre() const {
    return genre;
}

const uint8_t* Song::getLyrics() const {
    return lyrics;
}

size_t Song::getLyricsLen() const {
    return lyricsLen;
}

void Song::copyLyrics(const uint8_t* lyrics, size_t size) {
    if (!lyrics) {
        return;
    }
    else {
        for (unsigned i = 0; i < size; i++)
        {
            this->lyrics[i] = lyrics[i];
        }

        if (this->getLyricsLen() > size) {
            for (unsigned i = this->getLyricsLen(); i < size; i++)
            {
                this->lyrics[i] = 0;
            }
        }

        this->lyricsLen = std::max(this->getLyricsLen(), size);
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
        std::cout << Constants::UNDEFINED_GENRE_STR;
        return;
    }
    if ((genre & (uint8_t)Genre::ROCK) != 0) {
        std::cout << Constants::ROCK_GENRE_STR;
        printSep = true;
    }
    if ((genre & (uint8_t)Genre::POP) != 0) {
        if (printSep) {
            std::cout << sep;
        }
        std::cout << Constants::POP_GENRE_STR;
        printSep = true;
    }
    if ((genre & (uint8_t)Genre::HIPHOP) != 0) {
        if (printSep) {
            std::cout << sep;
        }
        std::cout << Constants::HIPHOP_GENRE_STR;
        printSep = true;
    }
    if ((genre & (uint8_t)Genre::ELECTRONIC_MUSIC) != 0) {
        if (printSep) {
            std::cout << sep;
        }
        std::cout << Constants::ELECTRONIC_MUSIC_GENRE_STR;
        printSep = true;
    }
    if ((genre & (uint8_t)Genre::JAZZ) != 0) {
        if (printSep) {
            std::cout << sep;
        }
        std::cout << Constants::JAZZ_GENRE_STR;
    }
}

void Song::printSong() const {
    std::cout << "Name: " << name << ", ";
    std::cout << "Length: ";
    printLengthConverted();
    std::cout << ", ";
    std::cout << "Genre: ";
    printGenres();
    std::cout << std::endl;
}

void Song::printLyrics() const {
    for (unsigned i = 0; i < lyricsLen; i++)
    {
        std::cout << '\'' << lyrics[i] << "\' ";
    }
    std::cout << std::endl;
}

void Song::operator=(const Song & other) {
    this->setName(other.getName());
    this->genre = other.getGenre();
    this->setLength(other.getLength());
    copyLyrics(other.getLyrics(), other.getLyricsLen());
}

class Playlist {
private:
    
    Song songs[Constants::MAX_SONGS_IN_PLAYLIST];
    size_t songsCount = 0;

    int getSongIndex(const char* name) const;
    void sort(bool(*pred)(Song, Song));

public:
    Playlist() = default;
    void addSong(const char* name, int length, const char* genre, const char* lyricsFile);
    void printSongs() const;
    void findName (const char* name) const;
    void findGenre(char genre) const;
    void saveSong(const char* song, const char* file) const;
    enum class SortingOptions;
    void sort(SortingOptions sortType);
    void mix(const char* song, int k);              //Add rhythm to song
    void mix(const char* song1, const char* song2); //Mix 2 songs
};

enum class Playlist::SortingOptions{
    OTHER,
    NAME,
    LENGTH
};

int Playlist::getSongIndex(const char* name) const {
    if (!name) {
        return -1;
    }

    for (unsigned i = 0; i < songsCount; i++)
    {
        if (strcmp(songs[i].getName(), name) == 0) {
            return i;
        }
    }

    return -1;
}

void Playlist::addSong(const char* name, int lengthInSecs, const char* genre, const char* lyricsFile) {
    if (!lyricsFile || !genre) {
        return;
    }

    if (songsCount < Constants::MAX_SONGS_IN_PLAYLIST) {
        Song s(name, lengthInSecs, genre, lyricsFile);
        songs[songsCount] = s;
        songsCount++;
    }
}

void Playlist::printSongs() const {
    std::cout << "------------------------------------------------\n\n";
    for (unsigned i = 0; i < songsCount; i++)
    {
        songs[i].printSong();
        //songs[i].printLyrics();
        std::cout << std::endl;
    }
    std::cout << "------------------------------------------------\n\n";
}

void Playlist::findName(const char* name) const {
    songs[getSongIndex(name)].printSong();
}

void Playlist::findGenre(char genre) const {
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

void Playlist::sort(Playlist::SortingOptions sortType) {
    if (sortType == SortingOptions::NAME) {
        sort([](Song a, Song b) {return strcmp(a.getName(), b.getName()) < 0; }); //sort by name
    }
    else if (sortType == SortingOptions::LENGTH) {
        sort([](Song a, Song b) {return a.getLength() < b.getLength(); }); //sort by length
    }
    else {
        std::cout << "Cannot sort playlist.\n"; 
        return;
    }
}

void Playlist::saveSong(const char* song, const char* file) const {
    if (!song || !file) {
        return;
    }

    int index = getSongIndex(song);
    Song s = songs[index];

    std::ofstream ofs(file, std::ios::binary);
    if (!ofs.is_open()) {
        std::cout << "Cannot open file for writing.\n";
        return;
    }

    ofs.write((const char*)s.getLyrics(), s.getLyricsLen() * sizeof(s.getLyrics()[0]));
}

void Playlist::mix(const char* song, int addedRhythm) {
    if (addedRhythm < 0 || addedRhythm > 8 || !song) {
        return;
    }

    int index = getSongIndex(song);
    Song result = songs[index];
    unsigned powerOfTwo = addedRhythm - 1;

    for (int i = result.getLyricsLen() - 1; i >= 0; i--)
    {
        uint8_t newLyric = result.getLyrics()[i];

        while (powerOfTwo < 8)
        {
            int toAdd = (1 << powerOfTwo);

            newLyric = (newLyric | toAdd);
            result.setLyricAt(i, newLyric);

            powerOfTwo += addedRhythm;
        }
        powerOfTwo -= 8;
    }

    unsigned resultInd = getSongIndex(result.getName());

    songs[resultInd] = result;

    result.printLyrics();
}

void Playlist::mix(const char* song1, const char* song2) {
    if (!song1 || !song2) {
        return;
    }

    int resultInd = getSongIndex(song1);
    Song result = songs[resultInd];

    int otherInd = getSongIndex(song2);
    Song other = songs[otherInd];

    size_t lyricsToMix = std::min(result.getLyricsLen(), other.getLyricsLen());

    for (unsigned i = 0; i < lyricsToMix; i++)
    {
        uint8_t newLyric = (result.getLyrics()[i] ^ other.getLyrics()[i]);

        result.setLyricAt(i, newLyric);
    }

    songs[resultInd] = result;
}

int main()
{

}
