#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "Constants.h"
#include "Paddle.h"
#include "Ball.h"
#include <iostream>

int main()
{
	// New window.
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pong");
	window.setFramerateLimit(60);
	auto image = sf::Image{};
	if (!image.loadFromFile("Images/icon/icon.png"))
	{
		std::cout << "Nie zaladowano ikonki programu!";
	}

	window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());

	// Sound to be played when ball hits paddle
	sf::SoundBuffer boopBuffer;
	if (!boopBuffer.loadFromFile("Sounds/boop.wav")) {
		std::cout << "Nie zaladowano pliku boop.wav!";
	}
	sf::Sound boop;
	boop.setBuffer(boopBuffer);
	boop.setVolume(200.f);

	//Sound for left player wins
	sf::SoundBuffer leftWinBuffer;;
	if (!leftWinBuffer.loadFromFile("Sounds/left.wav")) {
		std::cout << "Nie zaladowano pliku left.wav!";
	}
	sf::Sound left;
	left.setBuffer(leftWinBuffer);
	left.setVolume(200.f);

	//Sound for right player wins
	sf::SoundBuffer rightWinBuffer;;
	if (!rightWinBuffer.loadFromFile("Sounds/right.wav")) {
		std::cout << "Nie zaladowano pliku right.aup3!";
	}
	sf::Sound right;
	right.setBuffer(rightWinBuffer);
	right.setVolume(200.f);



	// Sound to be played when ball goes offscreen to the left
	sf::SoundBuffer loseBuffer;
	if (!loseBuffer.loadFromFile("Sounds/lose.wav")) {
		std::cout << "Nie zaladowano pliku lose.wav!";
	}
	sf::Sound lose;
	lose.setBuffer(loseBuffer);
	lose.setVolume(80.f);


	Paddle leftPaddle(sf::Vector2f(WINDOW_WIDTH - (WINDOW_WIDTH - 25), (WINDOW_HEIGHT / 2) - 30)); // Player Paddle
	Paddle rightPaddle(sf::Vector2f(WINDOW_WIDTH - 30, (WINDOW_HEIGHT / 2) - 30)); // AI Paddle
	Ball ball(sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), 10);

	sf::Clock clock;


	sf::Font font;
	if (!font.loadFromFile("Fonts/rusq.ttf"))
	{
		std::cout << "Nie zaladowano czcionki!";
	}

	
	unsigned int leftScore = 0;
	sf::Text leftScoreText(std::to_string(leftScore), font, 30);
	leftScoreText.setPosition(sf::Vector2f(WINDOW_WIDTH / 4, 15));

	
	unsigned int rightScore = 0;
	sf::Text rightScoreText(std::to_string(rightScore), font, 30);
	rightScoreText.setPosition(sf::Vector2f(WINDOW_WIDTH * 3 / 4, 15));

	sf::Text introText("Nacisnij spacje aby rozpoczac!", font, 30);
	introText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 3));

	sf::Text rightWin("Prawy gracz wygral!", font, 30);
	rightWin.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 5));

	sf::Text leftWin("Lewy gracz wygral!", font, 30);
	leftWin.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 5));

	bool isPlaying = false;

	while (window.isOpen())
	{
		float dt = clock.restart().asSeconds();

		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			isPlaying = true;
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
			rightScore = 0;
			leftScore = 0;
			leftScoreText.setString(std::to_string(leftScore));
			rightScoreText.setString(std::to_string(rightScore));
			ball.reset();
			isPlaying = false;
			
		}

		
		if (isPlaying)
		{
			
			//Lewy gracz
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				leftPaddle.moveUp(dt);
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				leftPaddle.moveDown(dt);
			
			
			//Prawy gracz
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				rightPaddle.moveUp(dt);
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				rightPaddle.moveDown(dt);
			

			//AI
			/*
			if (ball.getDirection().x > 0.f)
			{
				if (ball.getPosition().y + (ball.getSize() / 2.f) >= rightPaddle.getPosition().y + (rightPaddle.getSize().y / 2.f))
					rightPaddle.moveDown(dt);
				if (ball.getPosition().y + (ball.getSize() / 2.f) <= rightPaddle.getPosition().y + (rightPaddle.getSize().y / 2.f))
					rightPaddle.moveUp(dt);
			}
			*/
			
			
			

			if ((ball.getPosition().x <= leftPaddle.getPosition().x + leftPaddle.getSize().x) &&
				(ball.getPosition().y <= leftPaddle.getPosition().y + leftPaddle.getSize().y) &&
				(ball.getPosition().y + ball.getSize() >= leftPaddle.getPosition().y))
			{
				ball.hit(leftPaddle);
				boop.play();
			}

			if ((ball.getPosition().x + ball.getSize() >= rightPaddle.getPosition().x) &&
				(ball.getPosition().y <= rightPaddle.getPosition().y + rightPaddle.getSize().y) &&
				(ball.getPosition().y + ball.getSize() >= rightPaddle.getPosition().y))
			{
				ball.hit(rightPaddle);
				boop.play();
			}


			
			ball.update(dt);

			
			if (ball.getPosition().x < 0.f)
			{
				rightScore++;

				if (rightScore == 10) {
					right.play();
					rightScore = 0;
					rightScoreText.setString(std::to_string(rightScore));
					isPlaying = false;
				}

				else {

					lose.play();
					leftPaddle.reset(sf::Vector2f(WINDOW_WIDTH - (WINDOW_WIDTH - 25), (WINDOW_HEIGHT / 2) - 30));
					rightPaddle.reset(sf::Vector2f(WINDOW_WIDTH - 30, (WINDOW_HEIGHT / 2) - 30));
					ball.reset();
					isPlaying = false;
				}

			}

			
			if (ball.getPosition().x + ball.getSize() > WINDOW_WIDTH)
			{
				leftScore++;

				if (leftScore == 10) 
				{
					left.play();
					leftScore = 0;
					leftScoreText.setString(std::to_string(rightScore));
					isPlaying = false;
				}

				else {

					lose.play();
					leftPaddle.reset(sf::Vector2f(WINDOW_WIDTH - (WINDOW_WIDTH - 25), (WINDOW_HEIGHT / 2) - 30));
					rightPaddle.reset(sf::Vector2f(WINDOW_WIDTH - 30, (WINDOW_HEIGHT / 2) - 30));
					ball.reset();
					isPlaying = false;

				}
			}


			leftScoreText.setString(std::to_string(leftScore));
			rightScoreText.setString(std::to_string(rightScore));

			leftPaddle.setVelocity(0.f);
			rightPaddle.setVelocity(0.f);
		}

		window.clear();

		if (!isPlaying) 

			window.draw(introText);
		

			window.draw(leftScoreText);
			window.draw(rightScoreText);
			window.draw(leftPaddle);
			window.draw(rightPaddle);
			window.draw(ball);
			window.display();

	}

	return EXIT_SUCCESS;
}