#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <cmath>

using namespace std;

int main()
{
    sf::RenderWindow okno(sf::VideoMode({800, 600}), "Ping_pong");
    okno.setFramerateLimit(60);
    okno.clear();
    srand(time(NULL));
    // ustawienie obiektów i ich wstepne własności
    sf::RectangleShape paletka_r;
    sf::RectangleShape paletka_l;
    paletka_l.setSize(sf::Vector2f(30.f, 100.f));
    paletka_r.setSize(sf::Vector2f(30.f, 100.f));
    paletka_l.setFillColor(sf::Color::White);
    paletka_r.setFillColor(sf::Color::White);
    paletka_l.setPosition({20.f, 250.f});
    paletka_r.setPosition({750.f, 250.f});

    sf::RectangleShape pilka;
    pilka.setFillColor(sf::Color::White);
    pilka.setSize(sf::Vector2f(20.f, 20.f));
    pilka.setPosition({390.f, 290.f});

    sf::RectangleShape linia;
    linia.setFillColor(sf::Color::White);
    linia.setSize(sf::Vector2f(6.f, 550.f));
    linia.setPosition({397.f, 25.f});

    okno.draw(paletka_l);
    okno.draw(paletka_r);
    okno.draw(pilka);
    okno.draw(linia);

    okno.display();
    auto pal_l = paletka_l.getGlobalBounds();
    auto pal_r = paletka_r.getGlobalBounds();
    auto ramka_pk = pilka.getGlobalBounds();

    float predkoscY = 4.5f, predkoscX = 4.5f;  //prędkośc piłki

    // tekst
    sf::Font czcionka;
    if (!czcionka.openFromFile("PressStart2P-Regular.ttf"))
    {
        okno.close();
    }

    sf::Text prawy(czcionka); // prawa punktacja
    prawy.setString("0");
    prawy.setCharacterSize(40);
    prawy.setFillColor(sf::Color::White);
    prawy.setPosition({470.f, 20.f});

    sf::Text lewy(czcionka); // lewa punktacja
    lewy.setString("0");
    lewy.setCharacterSize(40);
    lewy.setFillColor(sf::Color::White);
    lewy.setPosition({300.f, 20.f});

    int p_lewy = 0;
    int p_prawy = 0;

    // pętla akcji
    while (okno.isOpen())
    {

        while (p_prawy < 10 && p_lewy < 10)
        {

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

            pilka.move({predkoscX, predkoscY});
            auto pal_l = paletka_l.getGlobalBounds();
            auto pal_r = paletka_r.getGlobalBounds();
            auto ramka_pk = pilka.getGlobalBounds();

            // fizyka piłki

            if (pilka.getPosition().y <= 0.f || pilka.getPosition().y >= 595.f)
            {
                predkoscY = -predkoscY;
            }

            if (ramka_pk.findIntersection(pal_l).has_value())
            {
                predkoscX = abs(predkoscX);
            }

            if (ramka_pk.findIntersection(pal_r).has_value())
            {
                predkoscX = -abs(predkoscX);
            }

            // sterowanie paletkami
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            {
                if (paletka_l.getPosition().y > 0.f)
                {
                    paletka_l.move(sf::Vector2f(0.f, -5.f));
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            {
                if (paletka_l.getPosition().y < 500.f)
                {
                    paletka_l.move(sf::Vector2f(0.f, 5.f));
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            {
                if (paletka_r.getPosition().y > 0.f)
                {
                    paletka_r.move(sf::Vector2f(0.f, -5.f));
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            {
                if (paletka_r.getPosition().y < 500.f)
                {
                    paletka_r.move(sf::Vector2f(0.f, 5.f));
                }
            }

            // punktacja

            if (pilka.getPosition().x >= 800.f)
            {
                p_lewy++;
                lewy.setString(to_string(p_lewy));
                pilka.setPosition({390.f, 290.f});
                if(rand()%2 == 0)
                {
                    predkoscX = -predkoscX;

                }
                if(rand()%2 == 0)
                {
                    predkoscY = -predkoscY;
                    
                }
            }

            if (pilka.getPosition().x <= 0.f)
            {
                p_prawy++;
                prawy.setString(to_string(p_prawy));
                pilka.setPosition({390.f, 290.f});

                if(rand()%2 == 0)
                {
                    predkoscX = -predkoscX;

                }
                if(rand()%2 == 0)
                {
                    predkoscY = -predkoscY;
                    
                }

                if(p_lewy >5||p_prawy >5)
                {
                    predkoscX = 5.f;
                    predkoscY = 5.f;
                }
            }

            okno.clear();
            okno.draw(paletka_l);
            okno.draw(paletka_r);
            okno.draw(pilka);
            okno.draw(linia);
            okno.draw(prawy);
            okno.draw(lewy);
            okno.display();
        }
       
       
        paletka_l.setPosition({20.f, 250.f});
        paletka_r.setPosition({750.f, 250.f});
        pilka.setPosition({390.f, 290.f});

        p_lewy = 0;
        p_prawy = 0;
        lewy.setString(to_string(p_lewy));
        prawy.setString(to_string(p_prawy));

        okno.clear();
        okno.draw(paletka_l);
        okno.draw(paletka_r);
        okno.draw(pilka);
        okno.draw(linia);
        okno.draw(prawy);
        okno.draw(lewy);
        okno.display();
    }

    return 0;
}