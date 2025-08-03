#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <SFML\Audio.hpp>


//some important variables
float blockSpeed = 6.0f;
float spawnTime = 0.5f;
int highScore = 0;
int score = 0;
bool isGameOver = false;

int main() {
	sf::RenderWindow window(sf::VideoMode(400, 500),"OMO");
	window.setFramerateLimit(60);

	//my music
	sf::Music bgm;
	if (!bgm.openFromFile("C:\\Users\\Admin\\OneDrive\\Desktop\\ola\\ConsoleApplication1\\Ove - Earth Is All We Have .ogg")) {
		std::cout << "failed\n";
		return -1;
	}
	bgm.setLoop(true);
	bgm.play();
	//music wen u die
	sf::SoundBuffer gob;
	sf::Sound gos;
	if (!gob.loadFromFile("C:\\Users\\Admin\\OneDrive\\Desktop\\ola\\ConsoleApplication1\\GameOver.ogg")) {
		std::cout << "failed3\n";
	}
	gos.setBuffer(gob);

	//create my player
	sf::RectangleShape player(sf::Vector2f(40.0f, 40.0f));
	player.setFillColor(sf::Color::Blue);
	player.setPosition(200.0f, 450.0f);

	//blocks creation
	std::vector<sf::RectangleShape>blocks;
	sf::Clock blockSpawnClock;
	std::srand(static_cast<unsigned>(std::time(nullptr)));


	sf::Font font;
	if (!font.loadFromFile("C:\\Users\\Admin\\OneDrive\\Desktop\\ola\\ConsoleApplication1\\OpenSans-Bold.ttf")) {
		std::cout << " failed\n";
		return -1;
	}
	//scoring system
		sf::Text scoreText;
		scoreText.setFont(font);
		scoreText.setCharacterSize(20);
		scoreText.setFillColor(sf::Color::White);
		scoreText.setPosition(10.f, 10.f);
		scoreText.setString("Score: 0");
		//restarting
		sf::Text restart;
		restart.setFillColor(sf::Color::White);
		restart.setPosition(80.f, 300.f);
		restart.setFont(font);
		restart.setString("Press R to restart");
		restart.setCharacterSize(20);
		//game loop
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		//restarting 2.0
		if (isGameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
			blocks.clear();
			score = 0;
			blockSpeed = 6.0f;
			spawnTime = 0.5f;
			isGameOver = false;
			player.setPosition(200.f, 450.0f);
			scoreText.setString("Score: 0");
			blockSpawnClock.restart();
		}
		//player movement
		if (!isGameOver) {
			bool moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
			bool moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
			sf::Vector2f playerPos = player.getPosition();
			float width = player.getSize().x;
			float playerSpeed = 7.0f;
			if (moveLeft && playerPos.x > 0) {
				player.move(-playerSpeed, 0);
			}
			if (moveRight && playerPos.x + width < 400) {
				player.move(playerSpeed, 0);
			}
		
			//spanwing blocks
			if (blockSpawnClock.getElapsedTime().asSeconds() > spawnTime) {
				for (int i = 0; i < 1; i++) {
					sf::RectangleShape block(sf::Vector2f(50.f, 50.f));
					block.setFillColor(sf::Color::Red);
					float x = static_cast<float>(std::rand() % (400 - 40));
					block.setPosition(x, 0.f);
					blocks.push_back(block);
					blockSpawnClock.restart();
				}
			}
			//checking collision
			for (int i = 0; i < blocks.size(); i++) {
				blocks[i].move(0, blockSpeed);
				if (blocks[i].getGlobalBounds().intersects(player.getGlobalBounds())) {
					gos.play();
					isGameOver = true;
				}
				if (blocks[i].getPosition().y > 600) {
					blocks.erase(blocks.begin() + i);
					i--;
					score += 1;
					scoreText.setString("Score: " + std::to_string(score));
				}
				if (score > 20) {
					blockSpeed = 7.0f;
					spawnTime = 0.4;
				}
				if (score > 40) {
					blockSpeed = 8.0f;
					spawnTime = 0.3;
				}
				if (score > 60) {
					spawnTime = 0.2f;
				}
				if (score > 80) {
					spawnTime = 0.1f;
				}
			}
		}

		//draw all things
		window.clear();
		window.draw(player);
		window.draw(scoreText);
		for (const auto& block : blocks) {
			window.draw(block);
		}
		if (isGameOver) {
			sf::Text gameOverText;
			gameOverText.setFont(font);
			gameOverText.setFillColor(sf::Color::White);
			gameOverText.setCharacterSize(40);
			gameOverText.setString("GAME OVER");
			gameOverText.setPosition(100, 200);
			window.draw(gameOverText);
			window.draw(restart);
		}
		window.display();
	}
	return 0;
}