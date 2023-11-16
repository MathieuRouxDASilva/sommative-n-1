#include <iostream>
#include <SFML/Graphics.hpp>
#include "functions.h"
#include "Hud.h"


bool grounded;
bool sprint;
int number_of_coins_collected = 0;

//bools to manage all 6 coins
bool coin1 = false;
bool coin2 = false;
bool coin3 = false;
bool coin4 = false;
bool coin5 = false;
bool coin6 = false;

bool eye_of_god = false;
bool is_double_jump_active = false;
bool jumped_once = false;
bool is_speed_active = false;

sf::Vector2f player_pos(0.0f, 0.0f);
sf::Vector2f player_vel(0.0f, 0.0f);
sf::Vector2f player_sprint_speed(0.0f, 0.0f);

bool in_bounds(sf::Vector2i coords)
{
	return coords.x >= 0
		&& coords.x < TILEMAP_WIDTH
		&& coords.y >= 0
		&& coords.y < TILEMAP_HEIGHT;
}

//bool qui permet de savoir si collision ou non
bool collision_check(sf::Vector2f position)
{
	sf::Vector2i coord;
	coord.x = position.x / TILE_SIZE_PX;
	coord.y = position.y / TILE_SIZE_PX;

	if (coord.x < 0 || coord.y < 0 || coord.x >= TILEMAP_WIDTH || coord.y >= TILEMAP_HEIGHT)
	{
		return true;
	}

	int index = coord.y * TILEMAP_WIDTH + coord.x;
	bool tile_solid = box[index];

	return tile_solid;
}

bool tile_at(sf::Vector2i tile_coord)
{
	if (tile_coord.x < 0 || tile_coord.y < 0 || tile_coord.x >= TILEMAP_WIDTH || tile_coord.y >= TILEMAP_HEIGHT)
	{
		return true;
	}
	const int index = tile_coord.y * TILEMAP_WIDTH + tile_coord.x;
	return box[index];
}

sf::Vector2i pos_to_coords(sf::Vector2f world_position)
{
	sf::Vector2i coord;
	coord.x = std::floor(world_position.x / TILE_SIZE_PX);
	coord.y = std::floor(world_position.y / TILE_SIZE_PX);
	return coord;
}

int main()
{
	// partit sfml et boucle éditeur de niveau
	sf::RenderWindow SFMLWindow(sf::VideoMode(1200, 1200), "Windows");
	SFMLWindow.setFramerateLimit(30);


	//set all textures
	sf::Texture plateform_texture;
	plateform_texture.loadFromFile("sprite/plateform.png");
	sf::Texture right;
	sf::Texture left;
	right.loadFromFile("sprite/mob_Right.png");
	left.loadFromFile("sprite/mob_Left.png");
	sf::Texture coin_texture;
	coin_texture.loadFromFile("sprite/coin.png");
	sf::Texture air;
	air.loadFromFile("sprite/test4.png");
	sf::Texture ground_surface_texture;
	ground_surface_texture.loadFromFile("sprite/ground_surface.png");
	sf::Texture ground;
	ground.loadFromFile("sprite/ground.png");
	sf::Texture start_texture;
	start_texture.loadFromFile("sprite/portail_start.png");
	sf::Texture end_texture;
	end_texture.loadFromFile("sprite/end.png");
	sf::Texture eye_texture;
	eye_texture.loadFromFile("sprite/eye_boost.png");
	sf::Texture illusion_left_texture;
	illusion_left_texture.loadFromFile("sprite/left_arrow.png");
	sf::Texture illusion_down_texture;
	illusion_down_texture.loadFromFile("sprite/down_arrow.png");
	sf::Texture double_jump_texture;
	double_jump_texture.loadFromFile("sprite/double_jump_texture.png");
	sf::Texture speed_power_texture;
	speed_power_texture.loadFromFile("sprite/speed_texture.png");
	sf::Texture backgorund_texture;
	backgorund_texture.loadFromFile("sprite/background.png");


	//regular tile shape 
	sf::RectangleShape tile_shape(sf::Vector2f(TILE_SIZE_PX, TILE_SIZE_PX));
	tile_shape.setFillColor(sf::Color(209, 147, 67));
	tile_shape.setTexture(&plateform_texture);


	//start block
	sf::RectangleShape start(sf::Vector2f(TILE_SIZE_PX, 75.0f));
	start.setFillColor(sf::Color(255, 255, 255));
	start.setTexture(&start_texture);
	start.setPosition(0.0f, 0.0f);


	//tp block
	sf::RectangleShape teleportation(sf::Vector2f(TILE_SIZE_PX, 75.0f));
	teleportation.setFillColor(sf::Color(0, 255, 0));
	teleportation.setTexture(&start_texture);
	teleportation.setPosition(2500.0f, 705.0f);


	//end block
	sf::RectangleShape end(sf::Vector2f(TILE_SIZE_PX, 75.0f));
	end.setFillColor(sf::Color(255, 0, 0));
	end.setTexture(&start_texture);
	end.setPosition(4774.0f, 100.0f);


	//eye power up block
	sf::RectangleShape eye(sf::Vector2f(40.0f, 20.0f));
	eye.setFillColor(sf::Color(255, 255, 255));
	eye.setTexture(&eye_texture);
	eye.setPosition(3650.0f, 150.0f);


	//double jump power up
	sf::RectangleShape double_jump(sf::Vector2f(TILE_SIZE_PX, TILE_SIZE_PX));
	double_jump.setFillColor(sf::Color(255, 255, 255));
	double_jump.setTexture(&double_jump_texture);
	double_jump.setPosition(14.0f, 800.0f);


	//speed power up
	sf::RectangleShape speed_power(sf::Vector2f(TILE_SIZE_PX, TILE_SIZE_PX));
	speed_power.setFillColor(sf::Color(255, 255, 255));
	speed_power.setTexture(&speed_power_texture);
	speed_power.setPosition(3925.0f, 1400.0f);


	//arrow named illusions that goes to the left (lore)
	sf::RectangleShape illusions_left(sf::Vector2f(TILE_SIZE_PX, 50.0f));
	illusions_left.setFillColor(sf::Color(0, 0, 0, 0));
	illusions_left.setTexture(&illusion_left_texture);
	illusions_left.setPosition(3627.0, 50.0f);


	//arrow named illusions that goes down (lore)
	sf::RectangleShape illusion_down(sf::Vector2f(TILE_SIZE_PX, 50.0f));
	illusion_down.setFillColor(sf::Color(0, 0, 0, 0));
	illusion_down.setTexture(&illusion_down_texture);
	illusion_down.setPosition(997.0f, 50.0f);



	// Create cursor shape
	sf::RectangleShape cursor_shape(sf::Vector2f(TILE_SIZE_PX, TILE_SIZE_PX));
	cursor_shape.setFillColor(sf::Color(209, 147, 67, 0));
	cursor_shape.setOutlineColor(sf::Color(250, 250, 250));
	cursor_shape.setOutlineThickness(-3);


	//player square shape
	sf::RectangleShape the_square(sf::Vector2f(25, 25));
	the_square.setFillColor(sf::Color(255, 255, 255));
	the_square.setOrigin(the_square.getSize().x / 2, the_square.getSize().y);
	the_square.setTexture(&right);

	//create a coin
	sf::RectangleShape coin(sf::Vector2f(30.0f, 50.0f));
	coin.setFillColor(sf::Color(255, 255, 255));
	coin.setTexture(&coin_texture);
	coin.setPosition(100.0f, 100.0f);
	coin.setOrigin(coin.getSize().x / 2, coin.getSize().y / 2);


	//background setupp
	sf::RectangleShape background(sf::Vector2f(EDGE_MAP_LENGTH,EDGE_MAP_HEIGHT));
	background.setPosition(0.0f, -10.0f);

	background.setTexture(&backgorund_texture);

	load();

	Hud hud(SFMLWindow);


	while (SFMLWindow.isOpen())
	{
		sf::Event event;

		system("cls");
		std::cout << "number of coins you got : " << number_of_coins_collected << "\n";


		sf::View view(sf::Vector2f(the_square.getPosition().x, the_square.getPosition().y), sf::Vector2f(SFMLWindow.getSize().x, SFMLWindow.getSize().y));
		SFMLWindow.setView(view);

		if (grounded)
		{
			player_vel.y = 0;
		}

		while (SFMLWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				SFMLWindow.close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::U)
				{
					save();
				}
				if (event.key.code == sf::Keyboard::Space)
				{
					if (grounded)
					{
						player_vel.y = -15;
						jumped_once = true;
					}
					if (jumped_once)
					{
						if (is_double_jump_active && !grounded)
						{
							player_vel.y = -15;
							jumped_once = false;
						}
					}
				}
			}
		}

		float limit_x_low = -1000000000.f;
		float limit_x_high = 1000000000.f;
		float limit_y_low = -1000000000.f;
		float limit_y_high = 1000000000.f;

		sf::Vector2i player_coords = pos_to_coords(player_pos);
		constexpr int margin = 1;

		if (tile_at(player_coords + sf::Vector2i(1, 0)))
		{
			limit_x_high = (player_coords.x + 1) * TILE_SIZE_PX - margin;
		}
		if (tile_at(player_coords + sf::Vector2i(-1, 0)))
		{
			limit_x_low = (player_coords.x) * TILE_SIZE_PX + margin;
		}
		if (tile_at(player_coords + sf::Vector2i(0, 1)))
		{
			limit_y_high = (player_coords.y + 1) * TILE_SIZE_PX - margin;
		}
		if (tile_at(player_coords + sf::Vector2i(0, -1)))
		{
			limit_y_low = (player_coords.y) * TILE_SIZE_PX + margin;
		}

		// moving the player!
		sf::Vector2f delta(0, 0);
		constexpr float player_speed = 5;
		constexpr float player_boost_speed = 10.0f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			the_square.setTexture(&left);

			if (!is_speed_active)
			{
				delta += sf::Vector2f(-player_speed, 0);
			}
			else if (is_speed_active)
			{
				delta += sf::Vector2f(-player_boost_speed, 0);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			the_square.setTexture(&right);

			if (!is_speed_active)
			{
				delta += sf::Vector2f(player_speed, 0);
			}
			else if (is_speed_active)
			{
				delta += sf::Vector2f(player_boost_speed, 0);
			}
		}

		// Falling speed limit
		if (player_vel.y > 10.0f)
		{
			player_vel.y = 10.0f;
		}

		// Gravity
		player_vel.y += 1.0f;


		delta += sf::Vector2f(0, player_vel.y);

		player_pos += delta;
		if (player_pos.x >= limit_x_high - (the_square.getSize().x / 2))
		{
			player_pos.x = limit_x_high - (the_square.getSize().x / 2);
		}
		if (player_pos.x <= limit_x_low + (the_square.getSize().x / 2))
		{
			player_pos.x = limit_x_low + (the_square.getSize().x / 2);
		}
		grounded = false;
		if (player_pos.y >= limit_y_high)
		{
			player_pos.y = limit_y_high;
			grounded = true;
		}
		if (player_pos.y <= limit_y_low + the_square.getSize().y) {
			player_pos.y = limit_y_low + the_square.getSize().y;
			player_vel.y = 0;
		}



		// Determine tile coordinates that the mouse is hovering
		sf::Vector2i mouse_pos = sf::Mouse::getPosition(SFMLWindow);
		sf::Vector2i mouse_tile((mouse_pos.x + the_square.getPosition().x - SFMLWindow.getSize().x / 2) / TILE_SIZE_PX, (mouse_pos.y + the_square.getPosition().y - SFMLWindow.getSize().y / 2) / TILE_SIZE_PX);


		//quand je clique gauche -> colision, clique droit -> pas colision.
		bool mouse_left = sf::Mouse::isButtonPressed(sf::Mouse::Left);
		bool mouse_right = sf::Mouse::isButtonPressed(sf::Mouse::Right);
		if (mouse_left || mouse_right)
		{

			box[mouse_tile.y * TILEMAP_WIDTH + mouse_tile.x] = mouse_left;


			if (in_bounds(mouse_tile))
			{

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad0))
				{
					tiles[mouse_tile.y * TILEMAP_WIDTH + mouse_tile.x] = 0;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1))
				{
					tiles[mouse_tile.y * TILEMAP_WIDTH + mouse_tile.x] = 1;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
				{
					tiles[mouse_tile.y * TILEMAP_WIDTH + mouse_tile.x] = 2;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3))
				{
					tiles[mouse_tile.y * TILEMAP_WIDTH + mouse_tile.x] = 3;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
				{
					tiles[mouse_tile.y * TILEMAP_WIDTH + mouse_tile.x] = 4;
				}
			}
		}

		// clear the window with black color
		SFMLWindow.clear(sf::Color::Black);

		SFMLWindow.draw(background);

		// draw the tilemap
		for (int y = 0; y < TILEMAP_HEIGHT; y++)
		{
			for (int x = 0; x < TILEMAP_WIDTH; x++)
			{
				int index = y * TILEMAP_WIDTH + x;

				if (tiles[index] == 0)
				{
					tile_shape.setPosition(x * TILEMAP_HEIGHT, y * TILEMAP_HEIGHT);
					tile_shape.setTexture(&air);
					tile_shape.setFillColor(sf::Color(255, 255, 255, 0));
					SFMLWindow.draw(tile_shape);
				}
				if (tiles[index] == 1)
				{
					tile_shape.setFillColor(sf::Color(255, 255, 255, 244));
					tile_shape.setPosition(x * TILEMAP_HEIGHT, y * TILEMAP_HEIGHT);
					tile_shape.setTexture(&plateform_texture);
					tile_shape.setOrigin(0.0f, PLATEFORM_HEIGHT_SETUPP);
					SFMLWindow.draw(tile_shape);
				}
				if (tiles[index] == 2)
				{
					tile_shape.setFillColor(sf::Color(255, 255, 255, 244));

					tile_shape.setPosition(x * TILEMAP_HEIGHT, y * TILEMAP_HEIGHT);
					tile_shape.setTexture(&ground_surface_texture);
					SFMLWindow.draw(tile_shape);
				}
				if (tiles[index] == 3)
				{
					tile_shape.setFillColor(sf::Color(255, 255, 255, 244));

					tile_shape.setPosition(x * TILEMAP_HEIGHT, y * TILEMAP_HEIGHT);
					tile_shape.setTexture(&ground);
					SFMLWindow.draw(tile_shape);
				}
			}
		}

		// draw selection cursor
		cursor_shape.setPosition(mouse_tile.x * TILE_SIZE_PX, mouse_tile.y * TILE_SIZE_PX);
		SFMLWindow.draw(cursor_shape);

#pragma region coin_managment
		if (!coin1)
		{
			coin.setPosition(721.0f, 50.0f);
			SFMLWindow.draw(coin);
			if (the_square.getPosition().y <= coin.getPosition().y + coin.getSize().y / 2 && the_square.getPosition().y >= coin.getPosition().y - coin.getSize().y / 2)
			{
				if (the_square.getPosition().x <= coin.getPosition().x + coin.getSize().x / 2 && the_square.getPosition().x >= coin.getPosition().x - coin.getSize().x / 2)
				{
					coin.setPosition(-100.0f, -100.0f);
					number_of_coins_collected++;
					coin1 = true;
				}
			}

		}
		if (!coin2)
		{
			coin.setPosition(1000.0f, 150.0f);
			SFMLWindow.draw(coin);
			if (the_square.getPosition().y <= coin.getPosition().y + coin.getSize().y / 2 && the_square.getPosition().y >= coin.getPosition().y - coin.getSize().y / 2)
			{
				if (the_square.getPosition().x <= coin.getPosition().x + coin.getSize().x / 2 && the_square.getPosition().x >= coin.getPosition().x - coin.getSize().x / 2)
				{
					coin.setPosition(-100.0f, -100.0f);
					number_of_coins_collected++;
					coin2 = true;

				}
			}
		}
		if (!coin3)
		{
			coin.setPosition(1500.0f, 150.0f);
			SFMLWindow.draw(coin);
			if (the_square.getPosition().y <= coin.getPosition().y + coin.getSize().y / 2 && the_square.getPosition().y >= coin.getPosition().y - coin.getSize().y / 2)
			{
				if (the_square.getPosition().x <= coin.getPosition().x + coin.getSize().x / 2 && the_square.getPosition().x >= coin.getPosition().x - coin.getSize().x / 2)
				{
					coin.setPosition(-100.0f, -100.0f);
					number_of_coins_collected++;
					coin3 = true;

				}
			}
		}
		if (!coin4)
		{
			coin.setPosition(3830.0f, 300.0f);
			SFMLWindow.draw(coin);
			if (the_square.getPosition().y <= coin.getPosition().y + coin.getSize().y / 2 && the_square.getPosition().y >= coin.getPosition().y - coin.getSize().y / 2)
			{
				if (the_square.getPosition().x <= coin.getPosition().x + coin.getSize().x / 2 && the_square.getPosition().x >= coin.getPosition().x - coin.getSize().x / 2)
				{
					coin.setPosition(-100.0f, -100.0f);
					number_of_coins_collected++;
					coin4 = true;

				}
			}
		}
		if (!coin5)
		{
			coin.setPosition(2770.0f, 1350.0f);
			SFMLWindow.draw(coin);
			if (the_square.getPosition().y <= coin.getPosition().y + coin.getSize().y / 2 && the_square.getPosition().y >= coin.getPosition().y - coin.getSize().y / 2)
			{
				if (the_square.getPosition().x <= coin.getPosition().x + coin.getSize().x / 2 && the_square.getPosition().x >= coin.getPosition().x - coin.getSize().x / 2)
				{
					coin.setPosition(-100.0f, -100.0f);
					number_of_coins_collected++;
					coin5 = true;

				}
			}
		}
		if (!coin6)
		{
			coin.setPosition(128.0f, 1050.0f);
			SFMLWindow.draw(coin);
			if (the_square.getPosition().y <= coin.getPosition().y + coin.getSize().y / 2 && the_square.getPosition().y >= coin.getPosition().y - coin.getSize().y / 2)
			{
				if (the_square.getPosition().x <= coin.getPosition().x + coin.getSize().x / 2 && the_square.getPosition().x >= coin.getPosition().x - coin.getSize().x / 2)
				{
					coin.setPosition(-100.0f, -100.0f);
					number_of_coins_collected++;
					coin6 = true;

				}
			}
		}
#pragma endregion

		hud.UpdateCoins(number_of_coins_collected);

		//collision of eye power up
		if (the_square.getPosition().y <= eye.getPosition().y + eye.getSize().y / 2 && the_square.getPosition().y >= eye.getPosition().y - eye.getSize().y / 2)
		{
			if (the_square.getPosition().x <= eye.getPosition().x + eye.getSize().x / 2 && the_square.getPosition().x >= eye.getPosition().x - eye.getSize().x / 2)
			{
				eye.setPosition(-100.0f, -2000.0f);
				eye_of_god = true;
			}
		}

		//collision of double jump power up
		if (the_square.getPosition().y <= double_jump.getPosition().y + double_jump.getSize().y / 2 && the_square.getPosition().y >= double_jump.getPosition().y - double_jump.getSize().y / 2)
		{
			if (the_square.getPosition().x <= double_jump.getPosition().x + double_jump.getSize().x / 2 && the_square.getPosition().x >= double_jump.getPosition().x - double_jump.getSize().x / 2)
			{
				double_jump.setPosition(-100.0f, -2000.0f);
				is_double_jump_active = true;
			}
		}

		//collision for the speed boost
		if (the_square.getPosition().y <= speed_power.getPosition().y + speed_power.getSize().y / 2 && the_square.getPosition().y >= speed_power.getPosition().y - speed_power.getSize().y / 2)
		{
			if (the_square.getPosition().x <= speed_power.getPosition().x + speed_power.getSize().x / 2 && the_square.getPosition().x >= speed_power.getPosition().x - speed_power.getSize().x / 2)
			{
				speed_power.setPosition(-100.0f, -2000.0f);
				is_speed_active = true;
			}
		}
#pragma region end_game
		if (the_square.getPosition().y <= end.getPosition().y + end.getSize().y / 2 && the_square.getPosition().y >= end.getPosition().y - end.getSize().y / 2)
		{
			if (the_square.getPosition().x <= end.getPosition().x + end.getSize().x / 2 && the_square.getPosition().x >= end.getPosition().x - end.getSize().x / 2)
			{
				SFMLWindow.close();
			}
		}

		//collision of double jump power up
		if (the_square.getPosition().y <= teleportation.getPosition().y + teleportation.getSize().y / 2 && the_square.getPosition().y >= teleportation.getPosition().y - teleportation.getSize().y / 2)
		{
			if (the_square.getPosition().x <= teleportation.getPosition().x + teleportation.getSize().x / 2 && the_square.getPosition().x >= teleportation.getPosition().x - teleportation.getSize().x / 2)
			{
				the_square.setPosition(50.0f, 50.0f);
				player_pos = sf::Vector2f(0.0f, 0.0f);
			}
		}
#pragma endregion


		the_square.setPosition(player_pos);


		//place to manage if you fall
		if (the_square.getPosition().y >= EDGE_MAP_RESPAWN)
		{
			the_square.setPosition(50.0f, 50.0f);
			player_pos = sf::Vector2f(0.0f, 0.0f);
			coin1 = false;
			coin2 = false;
			coin3 = false;
			coin4 = false;
			coin5 = false;
			coin6 = false;
			number_of_coins_collected = 0;
		}

		//effect of the eye of god power up -> makes you see things unexpected
		if (eye_of_god)
		{
			illusions_left.setFillColor(sf::Color(255, 255, 255, 244));
			illusion_down.setFillColor(sf::Color(255, 255, 255, 244));
		}

		std::cout << the_square.getPosition().x << "\n";
		std::cout << the_square.getPosition().y;

		hud.draw(SFMLWindow);
		SFMLWindow.draw(speed_power);
		SFMLWindow.draw(teleportation);
		SFMLWindow.draw(double_jump);
		SFMLWindow.draw(illusions_left);
		SFMLWindow.draw(illusion_down);
		SFMLWindow.draw(end);
		SFMLWindow.draw(eye);
		SFMLWindow.draw(the_square);
		SFMLWindow.draw(start);
		SFMLWindow.display();
	}
	return 0;

}

