#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <vector>
#include <deque>

using namespace std;

enum kierunek
{
    gora,
    dol,
    prawo,
    lewo
};

const int kafalek = 32;
const int kolumny = 20;
const int wiersze = 15;

struct segment
{
    int x;
    int y;
};

int obecny_kierunek = gora;

int main()
{
    sf::RenderWindow okno(sf::VideoMode({640, 480}), "Snake");
    okno.setFramerateLimit(60);
    okno.clear();
    srand(time(NULL));

    // ustawienie obiektów i ich wstepne własności

    sf::CircleShape owoc(12.0f);
    owoc.setFillColor(sf::Color::White);

    sf::RectangleShape snake;
    snake.setSize({30.0f, 30.0f});
    snake.setFillColor(sf::Color::White);
    deque<segment> waz;
    waz.push_back({10, 7});
    waz.push_back({10, 8});
    waz.push_back({10, 9});

    sf::Texture plansza;
    plansza.loadFromFile("szachownica.png");
    sf::Sprite plansza1;
    plansza1.setTexture(plansza);

    sf::Clock stoper;

    int owoc_x = 2;
    int owoc_y = 1;

    bool miejsce; // zmienna czy jest wolne pole

    while (okno.isOpen())
    {
        okno.clear();

        while (const optional zdarzenie = okno.pollEvent()) // pobieranie akcji
        {

            if (zdarzenie->is<sf::Event::Closed>())
            {
                okno.close();
            }

            if (const auto *klawisz = zdarzenie->getIf<sf::Event::KeyPressed>())
            {
                if (klawisz->code == sf::Keyboard::Key::Escape)
                {
                    okno.close();
                }
            }
        }
        plansza1.setPosition({0, 0});
         okno.draw(plansza1);

        owoc.setPosition({owoc_x * kafalek + 4, owoc_y * kafalek + 4});
        // kierowanie
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            if (obecny_kierunek != dol)
            {
                obecny_kierunek = gora;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            if (obecny_kierunek != gora)
            {
                obecny_kierunek = dol;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            if (obecny_kierunek != prawo)
            {
                obecny_kierunek = lewo;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            if (obecny_kierunek != lewo)
            {
                obecny_kierunek = prawo;
            }
        }

        segment a = waz.front();
        // automatyczne chodzenie
        if (stoper.getElapsedTime().asSeconds() > 0.3f)
        {

            switch (obecny_kierunek)
            {

            case gora:
                a.y -= 1;
                break;

            case dol:
                a.y += 1;
                break;

            case prawo:
                a.x += 1;
                break;

            case lewo:
                a.x -= 1;
                break;

            default:
                break;
            }

            waz.push_front(a); // sprawdzanie czy owoc zjedzony
            if (a.x == owoc_x && a.y == owoc_y)
            {
                // nowa pozycja owocu
                miejsce = false;
                while (miejsce == false)
                {
                    owoc_x = rand() % 20;
                    owoc_y = rand() % 15;
                    miejsce = true;
                    for (int i = 0; i < waz.size(); i++)
                    {
                        segment b = waz[i];
                        if (owoc_x == b.x && owoc_y == b.y)
                        {
                            miejsce = false;
                            break;
                        }
                    }
                    owoc.setPosition({owoc_x * kafalek + 4, owoc_y * kafalek + 4});
                }
            }
            else
            {
                waz.pop_back();
            }

            stoper.restart();
        }
        // pokazywanie węża na ekranie
        for (int i = 0; i < waz.size(); i++)
        {
            segment b; // zmienna pomocna
            b = waz[i];
            b.x = b.x * kafalek + 1;
            b.y = b.y * kafalek + 1;

            snake.setPosition({b.x, b.y});
            okno.draw(snake);
        }
        // sprawdzanie smierci
        for (int i = 1; i < waz.size(); i++)
        {
            segment b = waz[i];
            if ((a.x == b.x && a.y == b.y) || (a.x < 0 || a.x > 20 || a.y < 0 || a.y > 15))
            {
                Sleep(1000);
                waz.clear();
                waz.push_back({10, 7});
                waz.push_back({10, 8});
                waz.push_back({10, 9});
                obecny_kierunek = gora;
                stoper.restart();
            }
        }
       
        okno.draw(owoc);
        okno.display();
    }

    return 0;
}