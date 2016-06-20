#include <iostream>

#include "Grid.hpp"

std::vector<WavePoint>* Wave::surround = NULL;

int main()
{
    srand(time(0));

    int scale = 15;

    sf::RenderWindow window(sf::VideoMode(scale*10, scale*22), "FT");
    window.setFramerateLimit(60);

    Grid grid(10, 10);
    Block b = buildRandom(2, 0);

    bool keyPressed = false;

    long tick = 0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::Space && !keyPressed)
                {
                    keyPressed = true;
                    b.rotate(true);
                    grid.checkRotationCollision(b);

                }
                else if (event.key.code == sf::Keyboard::Left)
                {
                    b.move(-1, 0);
                    grid.checkPlayerMoveCollision(b);
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    b.move(1, 0);
                    grid.checkPlayerMoveCollision(b);
                }
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    keyPressed = false;
                }
            }
        }

        if (tick%10==0)
        {
            b.move(0, 1);
            grid.checkDownCollision(b);
        }

        if (b.validated && !grid.getStatus())
        {
            std::cout << grid.getStatus() << std::endl;
            b = buildRandom(2, 0);
        }

        tick++;

        grid.update();

        window.clear();

        grid.draw(&window, scale);
        b.draw(&window, scale);

        window.display();
    }

    return 0;
}
