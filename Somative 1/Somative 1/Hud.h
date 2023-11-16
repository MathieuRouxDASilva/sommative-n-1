#pragma once
#include <SFML/Graphics.hpp>

class Hud {
public:
    Hud(sf::RenderWindow& window);
    void UpdateCoins(int collected_coins);
    void draw(sf::RenderWindow& window);

private:
    sf::Font font_;
    sf::Text coin_text_;
};

