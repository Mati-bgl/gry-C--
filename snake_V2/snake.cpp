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
  sf::Texture borowki;
    borowki.loadFromFile("borowki.png");
sf::Sprite owoc1(borowki);
  int borowki_x = 2;
    int borowki_y = 1;

  sf::Texture jablka;
    jablka.loadFromFile("jablka.png");

  sf::Texture wisnia;
    wisnia.loadFromFile("wisnia.png");
sf::Sprite owoc2(wisnia);
  int owoc2_x = 9;
    int owoc2_y = 9;


    sf::Texture glowa;
    glowa.loadFromFile("glowa.png");
    sf::Sprite glowa_s(glowa);
    glowa_s.setOrigin({16.0f, 16.0f});

    sf::Texture cialo;
    cialo.loadFromFile("cialo.png");
    sf::Sprite cialo_s(cialo);


    deque<segment> waz;
    waz.push_back({10, 7});
    waz.push_back({10, 8});
    waz.push_back({10, 9});

    sf::Texture plansza;
    plansza.loadFromFile("szachownica.png");
sf::Sprite plansza1(plansza);

    sf::Clock stoper;

  

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

        owoc1.setPosition({borowki_x * kafalek, borowki_y * kafalek});
        owoc2.setPosition({owoc2_x * kafalek, owoc2_y * kafalek});
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
                glowa_s.setRotation(sf::degrees(0.f));
                break;

            case dol:
                a.y += 1;
                glowa_s.setRotation(sf::degrees(180.f));
                break;

            case prawo:
                a.x += 1;
                glowa_s.setRotation(sf::degrees(90.f));
                break;

            case lewo:
                a.x -= 1;
                glowa_s.setRotation(sf::degrees(270.f));
                break;

            default:
                break;
            }

             //pojawianie sie po drugiej stronie ekranu
        if(a.x <0)
        {
            a.x =19;

        }
         if(a.x >=20)
        {
            a.x = 0;
        }
         if(a.y <0)
        {
            a.y = 14;
            
        }
         if(a.y >=15)
        {
            a.y = 0;
            
        }


            waz.push_front(a);
             // sprawdzanie czy owoc zjedzony
            if (a.x == borowki_x && a.y == borowki_y)
            {
                // nowa pozycja owocu
                miejsce = false;
                while (miejsce == false)
                {
                    borowki_x = rand() % 20;
                    borowki_y = rand() % 15;
                    miejsce = true;
                    switch (rand() % 3)
                    {
                    case 0:
                        owoc1.setTexture(borowki);
                        break;
                        case 1:
                        owoc1.setTexture(jablka);
                        break;
                        case 2:
                        owoc1.setTexture(wisnia);
                        break;
                    
                    default:
                        break;
                    }
                    for (int i = 0; i < waz.size(); i++)
                    {
                        segment b = waz[i];
                        if (borowki_x == b.x && borowki_y == b.y)
                        {
                            miejsce = false;
                            break;
                        }
                    }
                    owoc1.setPosition({borowki_x * kafalek, borowki_y * kafalek});
                }
            }
            else if (a.x == owoc2_x && a.y == owoc2_y)
            {
                 // nowa pozycja owocu
                miejsce = false;
                while (miejsce == false)
                {
                    owoc2_x = rand() % 20;
                    owoc2_y = rand() % 15;
                    miejsce = true;

                    switch (rand() % 3)
                    {
                    case 0:
                        owoc2.setTexture(borowki);
                        break;
                        case 1:
                        owoc2.setTexture(jablka);
                        break;
                        case 2:
                        owoc2.setTexture(wisnia);
                        break;
                    
                    default:
                        break;
                    }
                    for (int i = 0; i < waz.size(); i++)
                    {
                        segment b = waz[i];
                        if (owoc2_x == b.x && owoc2_y == b.y)
                        {
                            miejsce = false;
                            break;
                        }
                    }
                    owoc2.setPosition({
                        owoc2_x * kafalek, owoc2_y * kafalek});
            }
        }
            
            else
            {
                waz.pop_back();
            }

            stoper.restart();
        }
        // pokazywanie węża na ekranie

     

glowa_s.setPosition({a.x* kafalek +16.0f, a.y* kafalek +16.0f});
            okno.draw(glowa_s);

        for (int i = 1; i < waz.size(); i++)
        {
            segment b; // zmienna pomocna
            b = waz[i];
            b.x = b.x * kafalek;
            b.y = b.y * kafalek;

            cialo_s.setPosition({b.x, b.y});
            okno.draw(cialo_s);
        }
        // sprawdzanie smierci
        for (int i = 1; i < waz.size(); i++)
        {
            segment b = waz[i];
            if (a.x == b.x && a.y == b.y)
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

        okno.draw(owoc1);
        okno.draw(owoc2);
        okno.display();
    }

    return 0;
}