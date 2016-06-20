#include "Grid.hpp"

#include <iostream>

Grid::Grid(unsigned int arg_width, unsigned int arg_height) :
    _width(arg_width),
    _height(arg_height),
    _gameOver(false)
{
    _blocks.resize(_width*_height);
    for (unsigned int i(0); i<_width*_height; ++i) _blocks[i] = 0;

    for (int i(0); i<20; ++i)
    {
        for (int j(4); j<44; ++j) {_map.push_back(WavePoint(i, j));}
    }
    Wave::surround = &_map;
}

int& Grid::get(unsigned int x, unsigned int y)
{
    return _blocks[x + _width*y];
}

const int& Grid::get(unsigned int x, unsigned int y) const
{
    return _blocks[x +_width*y];
}

void Grid::validBlock(Block& block)
{
    for (unsigned int ix(0); ix<block.width; ++ix)
    {
        for (unsigned int iy(0); iy<block.width; ++iy)
        {
            if (block(ix, iy))
            {
                get(block.x+ix, block.y+iy) = block(ix, iy);
            }
        }
    }

    block.validated = true;
}

bool Grid::checkCollision(Block& block)
{
    for (unsigned int ix(0); ix<block.width; ++ix)
    {
        for (unsigned int iy(0); iy<block.width; ++iy)
        {
            if (block(ix, iy))
            {
                if (block.x+int(ix) < 0)
                    return true;

                if (block.x+int(ix) > int(_width)-1)
                    return true;

                if (block.y+int(iy) > int(_height)-1)
                    return true;

                if (get(block.x+ix, block.y+iy))
                    return true;
            }
        }
    }

    _waves.push_front(Wave(block.x*2+block.width, block.y*2, 5, 1));
    _waves.front().ampl = 20;

    return false;
}

void Grid::checkPlayerMoveCollision(Block& block)
{
    if (checkCollision(block))
        block.restoreOldPosition();
}

void Grid::checkRotationCollision(Block& block)
{
    if (checkCollision(block))
        block.rotate(false);
    else
    {
        _waves.push_front(Wave(block.x*2+block.width, block.y*2, 20, 0.75));
        _waves.front().ampl = 50;

        _waves.push_front(Wave(block.x*2+block.width, block.y*2, 10, 2));
        _waves.front().ampl = 100;
    }
}

void Grid::checkDownCollision(Block& block)
{
    if (checkCollision(block))
    {
        block.restoreOldPosition();
        validBlock(block);
        checkFullLines();

        if (block.y == 0)
        {
            _gameOver = true;
            explode();
        }
    }
}

void Grid::update()
{
    for (WavePoint& wp : _map)
    {
        wp.color = 25;
        wp.delta = 0;
    }

    for (Wave& wave : _waves) { wave.apply(); }

    _waves.remove_if([](Wave& v){ return v.done; });
}

void Grid::draw(sf::RenderTarget* target, double scale) const
{
    int n_points = _map.size();
    sf::VertexArray background(sf::Quads, n_points*4);
    for (int i=0; i<n_points; i++)
    {
        const WavePoint& wp = _map[i];
        sf::Color col = sf::Color(wp.color, wp.color, wp.color);
        double x = 0.5*wp.x*scale-wp.delta;
        double y = 0.5*wp.y*scale-wp.delta+2*scale;
        int g = 1;
        if (wp.delta > 4) g = wp.delta/4.0;

        background[i*4  ].position = sf::Vector2f(x-g, y-g);
        background[i*4+1].position = sf::Vector2f(x+g, y-g);
        background[i*4+2].position = sf::Vector2f(x+g, y+g);
        background[i*4+3].position = sf::Vector2f(x-g, y+g);

        background[i*4  ].color = col;
        background[i*4+1].color = col;
        background[i*4+2].color = col;
        background[i*4+3].color = col;
    }
    target->draw(background);

    sf::RectangleShape inner(sf::Vector2f(scale, scale));
    inner.setFillColor(sf::Color(50, 50, 50));

    for (unsigned int ix(0); ix<_width; ++ix)
    {
        for (unsigned int iy(0); iy<_height; ++iy)
        {
            inner.setPosition(scale*(ix), scale*(iy));
            if (get(ix, iy))
                target->draw(inner);
        }
    }
}

void Grid::checkFullLines()
{
    for (unsigned int i(0); i<_height; ++i)
    {
        bool full = true;
        for (unsigned int j(0); j<_width; ++j)
        {
            if (!get(j, i))
            {
                full = false;
                break;
            }
        }

        if (full) eraseLine(i);
    }
}

void Grid::eraseLine(unsigned int lineNumber)
{
    for (unsigned int j(0); j<_width; ++j)
    {
        _waves.push_front(Wave(j*2, lineNumber*2, 50, 0.75));
        _waves.front().ampl = 50;

        _waves.push_front(Wave(j*2, lineNumber*2, 20, 4));
        _waves.front().ampl = 25;
    }

    for (unsigned int i(lineNumber); i>0; --i)
        for (unsigned int j(0); j<_width; ++j)
            get(j, i) = get(j, i-1);

    for (unsigned int j(0); j<_width; ++j)
        get(j, 0) = 0;
}

void Grid::explode()
{
    for (unsigned int i(0); i<_height; ++i)
    {
        for (unsigned int j(0); j<_width; ++j)
        {
            if (get(j, i))
            {
                get(j, i) = 0;
                _waves.push_front(Wave(j*2, i*2, 10, 1));
                _waves.front().ampl = 50;
            }
        }
    }
}

