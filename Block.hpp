#ifndef BLOCK_HPP_INCLUDED
#define BLOCK_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>

struct Block
{
    int x, y, lastX, lastY;
    bool validated = false;
    unsigned int width;
    std::vector<int> shape, rotatedShape;

    Block(int arg_x, int arg_y, unsigned arg_width);

    int& operator()(unsigned int ix, unsigned int iy);
    int& get       (unsigned int ix, unsigned int iy);

    const int& get(unsigned int ix, unsigned int iy) const;
    void       draw(sf::RenderTarget* target, double scale) const;
    void       rotate(bool way);
    void       move(int vx, int vy);
    void       restoreOldPosition();
};

Block buildTri   (unsigned int x, unsigned int y);
Block buildSquare(unsigned int x, unsigned int y);
Block buildLine  (unsigned int x, unsigned int y);
Block buildL1    (unsigned int x, unsigned int y);
Block buildL2    (unsigned int x, unsigned int y);
Block buildRandom(unsigned int x, unsigned int y);

#endif // BLOCK_HPP_INCLUDED
