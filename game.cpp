#include "game.h"
#include <iostream>

GameState::GameState()
    : board(GRID_SIZE, std::vector<Player>(GRID_SIZE, Player::None)),
      currentPlayer(Player::X),
      gameOver(false),
      winner(Player::None) {}

void drawGrid(sf::RenderWindow &window)
{
    for (int i = 1; i < GRID_SIZE; ++i)
    {
        sf::RectangleShape horizontalLine(sf::Vector2f(WINDOW_SIZE, 5));
        horizontalLine.setFillColor(sf::Color::Black);
        horizontalLine.setPosition(0, HEADER_HEIGHT + i * CELL_SIZE);
        window.draw(horizontalLine);

        sf::RectangleShape verticalLine(sf::Vector2f(5, WINDOW_SIZE));
        verticalLine.setFillColor(sf::Color::Black);
        verticalLine.setPosition(i * CELL_SIZE, HEADER_HEIGHT);
        window.draw(verticalLine);
    }
}

void drawBoard(sf::RenderWindow &window, const GameState &gameState)
{
    sf::Font font;
    if (!font.loadFromFile("Text/arial.ttf"))
    {
        std::cout << "Failed to load font!" << std::endl;
        return;
    }

    for (int row = 0; row < GRID_SIZE; ++row)
    {
        for (int col = 0; col < GRID_SIZE; ++col)
        {
            if (gameState.board[row][col] == Player::None)
                continue;

            sf::Text symbol;
            symbol.setFont(font);
            symbol.setCharacterSize(100);
            symbol.setFillColor(sf::Color::Black);
            symbol.setString(gameState.board[row][col] == Player::X ? "X" : "O");
            symbol.setPosition(col * CELL_SIZE + 60, HEADER_HEIGHT + row * CELL_SIZE + 40);
            window.draw(symbol);
        }
    }
}

void drawTurnIndicator(sf::RenderWindow &window, const GameState &gameState)
{
    sf::Font font;
    if (!font.loadFromFile("Text/arial.ttf"))
    {
        std::cout << "Failed to load font!" << std::endl;
        return;
    }

    sf::Text turnText;
    turnText.setFont(font);
    turnText.setCharacterSize(40);
    turnText.setFillColor(sf::Color::Black);
    turnText.setPosition(10, 10);

    if (gameState.gameOver)
    {
        turnText.setString(gameState.winner == Player::None ? "It's a Tie!" : (gameState.winner == Player::X ? "Player X Wins!" : "Player O Wins!"));
    }
    else
    {
        turnText.setString(gameState.currentPlayer == Player::X ? "Player X's Turn" : "Player O's Turn");
    }

    window.draw(turnText);
}

bool checkWin(GameState &gameState)
{
    const auto &board = gameState.board;

    for (int i = 0; i < GRID_SIZE; ++i)
    {
        if (board[i][0] != Player::None && board[i][0] == board[i][1] && board[i][1] == board[i][2])
        {
            gameState.winner = board[i][0];
            gameState.winningLine = {{i, 0}, {i, 1}, {i, 2}};
            return true;
        }
        if (board[0][i] != Player::None && board[0][i] == board[1][i] && board[1][i] == board[2][i])
        {
            gameState.winner = board[0][i];
            gameState.winningLine = {{0, i}, {1, i}, {2, i}};
            return true;
        }
    }

    if (board[0][0] != Player::None && board[0][0] == board[1][1] && board[1][1] == board[2][2])
    {
        gameState.winner = board[0][0];
        gameState.winningLine = {{0, 0}, {1, 1}, {2, 2}};
        return true;
    }
    if (board[0][2] != Player::None && board[0][2] == board[1][1] && board[1][1] == board[2][0])
    {
        gameState.winner = board[0][2];
        gameState.winningLine = {{0, 2}, {1, 1}, {2, 0}};
        return true;
    }

    return false;
}

bool isBoardFull(const GameState &gameState)
{
    for (const auto &row : gameState.board)
    {
        for (Player cell : row)
        {
            if (cell == Player::None)
                return false;
        }
    }
    return true;
}

void restartGame(GameState &gameState)
{
    gameState = GameState();
}

void drawWinningLine(sf::RenderWindow &window, const GameState &gameState)
{
    const auto &board = gameState.board;
    sf::RectangleShape line;

    for (int i = 0; i < GRID_SIZE; ++i)
    {
        if (board[i][0] != Player::None && board[i][0] == board[i][1] && board[i][1] == board[i][2])
        {
            line.setSize(sf::Vector2f(WINDOW_SIZE, 7));                         
            line.setFillColor(sf::Color::Red);                                  
            line.setPosition(0, HEADER_HEIGHT + i * CELL_SIZE + CELL_SIZE / 2); 
            window.draw(line);
            return;
        }
    }

    for (int i = 0; i < GRID_SIZE; ++i)
    {
        if (board[0][i] != Player::None && board[0][i] == board[1][i] && board[1][i] == board[2][i])
        {
            line.setSize(sf::Vector2f(7, WINDOW_SIZE));                    
            line.setFillColor(sf::Color::Red);                              
            line.setPosition(i * CELL_SIZE + CELL_SIZE / 2, HEADER_HEIGHT); 
            window.draw(line);
            return;
        }
    }

    if (board[0][0] != Player::None && board[0][0] == board[1][1] && board[1][1] == board[2][2])
    {
        line.setSize(sf::Vector2f(static_cast<float>(sqrt(2) * GRID_SIZE * CELL_SIZE), 7)); 
        line.setFillColor(sf::Color::Red);                                                  
        line.setRotation(45);                                                               
        line.setPosition(HEADER_HEIGHT / 99999, HEADER_HEIGHT); 
        window.draw(line);                                      
        return;
    }

    if (board[0][2] != Player::None && board[0][2] == board[1][1] && board[1][1] == board[2][0])
    {
        const float lineThickness = 7.0f;

        std::vector<sf::Vertex> thickLine;

        for (int i = -lineThickness / 2; i <= lineThickness / 2; ++i)
        {
            thickLine.push_back(sf::Vertex(sf::Vector2f(WINDOW_SIZE + i, HEADER_HEIGHT), sf::Color::Red));
            thickLine.push_back(sf::Vertex(sf::Vector2f(i, HEADER_HEIGHT + (GRID_SIZE * CELL_SIZE)), sf::Color::Red));
        }

        window.draw(&thickLine[0], thickLine.size(), sf::Lines);

        return;
    }
}

void drawButtons(sf::RenderWindow &window, GameState &gameState, sf::Texture &restartTexture, sf::Texture &closeTexture, sf::Sprite &restartButton, sf::Sprite &closeButton)
{
    if (gameState.gameOver)
    {
        restartButton.setTexture(restartTexture);
        restartButton.setScale(0.1f, 0.1f);
        restartButton.setPosition(WINDOW_SIZE - 70, 10);

        closeButton.setTexture(closeTexture);
        closeButton.setScale(0.1f, 0.1f);
        closeButton.setPosition(WINDOW_SIZE - 125, 10);

        window.draw(restartButton);
        window.draw(closeButton);
    }
}