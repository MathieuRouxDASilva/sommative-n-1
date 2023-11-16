#include "Hud.h"

Hud::Hud(sf::RenderWindow& window)
{

    font_.loadFromFile("font/font_style.fon");

    coin_text_.setFont(font_);
    coin_text_.setCharacterSize(12);
    coin_text_.setPosition(60.0f, 60.0f);
}

void Hud::UpdateCoins(const int collected_coins)
{
    coin_text_.setString("Coins: " + std::to_string(collected_coins));
}

void Hud::draw(sf::RenderWindow& window)
{
    window.draw(coin_text_);
}