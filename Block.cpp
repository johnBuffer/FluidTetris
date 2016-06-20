#include "Block.hpp"

Block::Block(int arg_x, int arg_y, unsigned arg_width) : x(arg_x), y(arg_y), width(arg_width)
{
    shape.resize(width*width);
    rotatedShape.resize(width*width);

    for (unsigned int i(0); i<width; ++i) shape[i] = 0;
}

int& Block::operator()(unsigned int ix, unsigned int iy)
{
    return shape[ix+iy*width];
}

int& Block::get(unsigned int ix, unsigned int iy)
{
    return shape[ix+iy*width];
}

const int& Block::get(unsigned int ix, unsigned int iy) const
{
    return shape[ix+iy*width];
}

void Block::draw(sf::RenderTarget* target, double scale) const
{
    if (validated) return;

    sf::RectangleShape inner(sf::Vector2f(scale, scale));
    inner.setFillColor(sf::Color(50, 50, 50));

    for (unsigned int ix(0); ix<width; ++ix)
    {
        for (unsigned int iy(0); iy<width; ++iy)
        {
            inner.setPosition(scale*(x+ix), scale*(y+iy));
            if (get(ix, iy))
                target->draw(inner);
        }
    }
}

void Block::rotate(bool way)
{
    for (unsigned int ix(0); ix<width; ++ix)
    {
        for (unsigned int iy(0); iy<width; ++iy)
        {
            if (way)
                rotatedShape[(width-1)-iy+ix*width] = get(ix, iy);
            else
                rotatedShape[iy+(width-1-ix)*width] = get(ix, iy);
        }
    }

    shape = rotatedShape;
}

void Block::move(int vx, int vy)
{
    lastX = x;
    lastY = y;

    x += vx;
    y += vy;
}

void Block::restoreOldPosition()
{
    x = lastX;
    y = lastY;
}

Block buildTri(unsigned int x, unsigned int y)
{
    Block b(x, y, 3);
    b(0, 0) = 1;
    b(1, 0) = 1;
    b(2, 0) = 1;
    b(1, 1) = 1;

    return b;
}

Block buildSquare(unsigned int x, unsigned int y)
{
    Block b(x, y, 2);
    b(0, 0) = 1;
    b(1, 0) = 1;
    b(0, 1) = 1;
    b(1, 1) = 1;

    return b;
}

Block buildLine(unsigned int x, unsigned int y)
{
    Block b(x, y, 4);
    b(0, 0) = 1;
    b(1, 0) = 1;
    b(2, 0) = 1;
    b(3, 0) = 1;

    return b;
}

Block buildL1(unsigned int x, unsigned int y)
{
    Block b(x, y, 3);
    b(0, 0) = 1;
    b(1, 0) = 1;
    b(2, 0) = 1;
    b(0, 1) = 1;

    return b;
}

Block buildL2(unsigned int x, unsigned int y)
{
    Block b(x, y, 3);
    b(0, 0) = 1;
    b(1, 0) = 1;
    b(2, 0) = 1;
    b(2, 1) = 1;

    return b;
}

Block buildRandom(unsigned int x, unsigned int y)
{
    int type = rand()%5;
    switch (type)
    {
    case 0:
        return buildL1(x, y);
    case 1:
        return buildL2(x, y);
    case 2:
        return buildLine(x, y);
    case 3:
        return buildSquare(x, y);
    case 4:
        return buildTri(x, y);
    }

    return buildSquare(x, y);
}
