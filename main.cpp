#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "game.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE + HEADER_HEIGHT), "Tic Tac Toe");
    GameState gameState;

    sf::SoundBuffer popBuffer;
    if (!popBuffer.loadFromFile("Sound/click.wav"))
    {
        std::cout << "Failed to load sound!" << std::endl;
        return -1;
    }

    sf::Sound popSound;
    popSound.setBuffer(popBuffer);

    sf::Texture restartTexture, closeTexture;
    if (!restartTexture.loadFromFile("images/restart.png"))
    {
        std::cout << "Failed to load restart.png!" << std::endl;
        return -1;
    }
    if (!closeTexture.loadFromFile("images/close.png"))
    {
        std::cout << "Failed to load close.png!" << std::endl;
        return -1;
    }

    sf::Sprite restartButton, closeButton;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!gameState.gameOver && event.type == sf::Event::MouseButtonPressed)
            {
                int row = (event.mouseButton.y - HEADER_HEIGHT) / CELL_SIZE;
                int col = event.mouseButton.x / CELL_SIZE;

                if (row >= 0 && row < GRID_SIZE && col < GRID_SIZE && gameState.board[row][col] == Player::None)
                {
                    gameState.board[row][col] = gameState.currentPlayer;
                    popSound.play();

                    if (checkWin(gameState))
                    {
                        gameState.gameOver = true;
                    }
                    else if (isBoardFull(gameState))
                    {
                        gameState.gameOver = true;
                    }
                    else
                    {
                        gameState.currentPlayer = (gameState.currentPlayer == Player::X ? Player::O : Player::X);
                    }
                }
            }

            if (gameState.gameOver && event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (restartButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    restartGame(gameState);
                }

                if (closeButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    window.close(); 
                }
            }
        }

        window.clear(sf::Color(155, 155, 155));                                                  
        drawTurnIndicator(window, gameState);                                                    
        drawGrid(window);                                                                         
        drawBoard(window, gameState);                                                             
        drawWinningLine(window, gameState);                                                       
        drawButtons(window, gameState, restartTexture, closeTexture, restartButton, closeButton); 
        window.display();                                                                         
    }

    return 0;
}