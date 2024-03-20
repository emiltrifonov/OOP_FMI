#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#pragma warning (disable : 4996)

struct Balloon {
private:
    enum class Color {
        UNDEFINED,
        WHITE,
        RED,
        GREEN
    };
    double price = 0;
    Color color = Color::UNDEFINED;

    Color getColorFromStr(const char* str) {
        if (!str) {
            return Color::UNDEFINED;
        }
        else if (strcmp(str, "White") == 0){
            return color = Color::WHITE;
        }
        else if (strcmp(str, "Red") == 0) {
            return color = Color::RED;
        }
        else if (strcmp(str, "Green") == 0) {
            return color = Color::GREEN;
        }
        else {
            return color = Color::UNDEFINED;
        }
    }

public:
    Balloon() = default;
    Balloon(double price, const char* color) {
        setPrice(price);
        setColor(color);
    }
    void setPrice(double price) {
        if (price > 0) {
            this->price = price;
        }
    }
    void setColor(const char* color) {
        if (color) {
            this->color = getColorFromStr(color);
        }
    }
    double getPrice() const {
        return price;
    }
    const char* getColor() const {
        if (color == Color::GREEN) {
            return "Green";
        }
        else if (color == Color::WHITE) {
            return "White";
        }
        else if (color == Color::RED) {
            return "Red";
        }
        else {
            return "Undefined";
        }
    }
};

struct Shisha {
private:
    enum class Taste {
        UNDEFINED,
        BLUEBERRY,
        QUEEN,
        MANGO,
        NESHTO_SVEJO,
        SHPEK_OREHITE
    };
    double price = 0;
    char brand[1024] = "Unknown";
    Taste taste = Taste::UNDEFINED;

    Taste getTasteFromStr(const char* str) {
        if (!str) {
            return Taste::UNDEFINED;
        }
        else if (strcmp(str, "Blueberry") == 0) {
            return Taste::BLUEBERRY;
        }
        else if (strcmp(str, "Queen") == 0) {
            return Taste::QUEEN;
        }
        else if (strcmp(str, "Mango") == 0) {
            return Taste::MANGO;
        }
        else if (strcmp(str, "Neshto svejo") == 0) {
            return Taste::NESHTO_SVEJO;
        }
        else if (strcmp(str, "Shpek orehite") == 0) {
            return Taste::SHPEK_OREHITE;
        }
        else {
            return Taste::UNDEFINED;
        }
    }

public:
    Shisha() = default;
    Shisha(double price, const char* brand, const char* taste) {
        setPrice(price);
        setBrand(brand);
        setTaste(taste);
    }
    void setPrice(double price) {
        if (price > 0) {
            this->price = price;
        }
    }
    void setBrand(const char* brand) {
        if (brand) {
            strcpy(this->brand, brand);
        }
    }
    void setTaste(const char* taste) {
        if (taste) {
            this->taste = getTasteFromStr(taste);
        }
    }
    double getPrice() const {
        return price;
    }
    const char* getBrand() const {
        return brand;
    }
    const char* getTaste() const {
        if (taste == Taste::BLUEBERRY) {
            return "Blueberry";
        }
        else if (taste == Taste::QUEEN) {
            return "Queen";
        }
        else if (taste == Taste::MANGO) {
            return "Mango";
        }
        else if (taste == Taste::NESHTO_SVEJO) {
            return "Neshto svejo";
        }
        else if (taste == Taste::SHPEK_OREHITE) {
            return "Shpek orehite";
        }
        else {
            return "Unknown";
        }
    }
    void read() {

    }
};

int main()
{

}
