#ifndef GRID_HPP_INCLUDED
#define GRID_HPP_INCLUDED

#include "Block.hpp"
#include "Wave.h"

#include <list>

class Grid
{
public:
    Grid(unsigned int arg_width, unsigned int arg_height);

    int& get(unsigned int x, unsigned int y);
    const int& get(unsigned int x, unsigned int y) const;
    bool getStatus() const {return _gameOver;}

    void validBlock(Block& block);
    void checkPlayerMoveCollision(Block& block);
    void checkRotationCollision(Block& block);
    void checkDownCollision(Block& block);
    void draw(sf::RenderTarget* target, double scale) const;

    void update();

private:
    std::vector<int> _blocks;
    unsigned int _width, _height;
    bool _gameOver;

    std::list<Wave> _waves;
    std::vector<WavePoint> _map;

    void checkFullLines();
    void eraseLine(unsigned int lineNumber);
    void explode();

    bool checkCollision(Block& block);
};

#endif // GRID_HPP_INCLUDED
