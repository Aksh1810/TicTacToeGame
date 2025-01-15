#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

const int GRID_SIZE = 3;
const int CELL_SIZE = 200;
const int HEADER_HEIGHT = 100;
const int WINDOW_SIZE = GRID_SIZE * CELL_SIZE;

enum class Player
{
    None,
    X,
    O
};

struct GameState
{
    std::vector<std::vector<Player>> board;
    Player currentPlayer;
    bool gameOver;
    Player winner;
    std::vector<sf::Vector2i> winningLine;

    GameState();
};

void drawGrid(sf::RenderWindow &window);
void drawBoard(sf::RenderWindow &window, const GameState &gameState);
void drawTurnIndicator(sf::RenderWindow &window, const GameState &gameState);
bool checkWin(GameState &gameState);
bool isBoardFull(const GameState &gameState);
void restartGame(GameState &gameState);
void drawWinningLine(sf::RenderWindow &window, const GameState &gameState);
void drawButtons(sf::RenderWindow &window, GameState &gameState, sf::Texture &restartTexture, sf::Texture &closeTexture, sf::Sprite &restartButton, sf::Sprite &closeButton);

#endif