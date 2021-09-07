#ifndef PLATFORMER_CASSETS_H
#define PLATFORMER_CASSETS_H

#include <SFML/Graphics.hpp>

const std::string RESOURCE_DIR_PATH = "../Resources/";

// singleton
class CAssets {
public:
    sf::Font m_Font;
public:
    static CAssets& GetInstance() {
        static CAssets SingleInstance;
        return SingleInstance;
    }
    void Load();

private:
    CAssets() {};
    ~CAssets() {};
    CAssets(const CAssets&) = delete;
    CAssets& operator=(const CAssets&) = delete;
};

#endif //PLATFORMER_CASSETS_H
