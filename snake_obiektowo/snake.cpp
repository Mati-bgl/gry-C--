#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <vector>
#include <deque>
#include <string>

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

class owoc
{
private:
    int x;
    int y;
    sf::Texture tekstura;
    sf::Sprite sprite;

public:
    owoc(string plik, int start_x, int start_y)
    {
        if (!tekstura.loadFromFile(plik))
        {
            cout << "bład wczytania";
        }
        sprite = sf::Sprite(tekstura);
        ustaw_pozycje(start_x, start_y);
    }

    int getx() { return x; }
    int gety() { return y; }

    void ustaw_pozycje(int n_x, int n_y)
    {
        x = n_x;
        y = n_y;
        sprite.setPosition({x * kafalek, y * kafalek});
    }

    void rysuj(sf::RenderWindow &okno)
    {
        okno.draw(sprite);
    }
};

void kolizja_owoc(deque<segment>& waz_f, owoc *&owoc_w, owoc *o1, owoc *o2, owoc *o3)
{
    segment a = waz_f[0];

    bool miejsce = false;
    while (miejsce == false)
    {
        int x = rand() % 20;
        int y = rand() % 15;
        miejsce = true;
        switch (rand() % 3)
        {
        case 0:
            owoc_w = o1;
            break;
        case 1:
            owoc_w = o2;
            break;
        case 2:
            owoc_w = o3;
            break;

        default:
            break;
        }
        for (int i = 0; i < waz_f.size(); i++)
        {
            segment b = waz_f[i];
            if (x == b.x && y == b.y)
            {
                miejsce = false;
                break;
            }
        }
        if(miejsce==true)owoc_w->ustaw_pozycje(x, y);
      
    }
    
}

int main()
{
    sf::RenderWindow okno(sf::VideoMode({640, 480}), "Snake");
    okno.setFramerateLimit(60);
    okno.clear();
    srand(time(NULL));

    // ustawienie obiektów i ich wstepne własności

    owoc borowka("borowki.png", 3, 2);
    owoc wisnia("wisnia.png", 3, 2);
    owoc jablko("jablko.png", 3, 2);

    owoc *owoc1 = &borowka;
    owoc *owoc2 = &wisnia;

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

        owoc1->ustaw_pozycje(owoc1->getx(), owoc1->gety());
        owoc2->ustaw_pozycje(owoc2->getx(), owoc1->gety());
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

            // pojawianie sie po drugiej stronie ekranu
            if (a.x < 0)
            {
                a.x = 19;
            }
            if (a.x > 20)
            {
                a.x = 0;
            }
            if (a.y < 0)
            {
                a.y = 14;
            }
            if (a.y > 15)
            {
                a.y = 0;
            }

            waz.push_front(a);
            // sprawdzanie czy owoc zjedzony
            if (a.x == owoc1->getx() && a.y == owoc1->gety())
            {
                kolizja_owoc(waz, owoc1, &borowka, &wisnia, &jablko);
            }
        
        else if (a.x == owoc2->getx() && a.y == owoc2->gety())
        {
            kolizja_owoc(waz, owoc2, &borowka, &wisnia, &jablko);
        }

        else
        {
            waz.pop_back();
        }

        stoper.restart();
    }
    // pokazywanie węża na ekranie

    glowa_s.setPosition({a.x * kafalek + 16.0f, a.y * kafalek + 16.0f});
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

    owoc1->rysuj(okno);
    owoc2->rysuj(okno);
    okno.display();
}

return 0;
}