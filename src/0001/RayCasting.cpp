#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>


class Wall:public sf::Drawable{
    static std::default_random_engine e;
    static std::uniform_real_distribution<float> xRan;
    static std::uniform_real_distribution<float> yRan;

public:
    float x1,x2,y1,y2;
    Wall(){

        x1=xRan(e);
        x2=xRan(e);
        y1=yRan(e);
        y2=yRan(e);
    }
    Wall(float x1,float y1,float x2,float y2):x1(x1),x2(x2),y1(y1),y2(y2){}

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        sf::Vertex line[] = {sf::Vertex{{x1,y1}},sf::Vertex{{x2,y2}}};
        target.draw(line,2,sf::Lines,states);
    }
};
std::default_random_engine Wall::e =std::default_random_engine{static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count())} ;
std::uniform_real_distribution<float> Wall::xRan =std::uniform_real_distribution<float>{0,1280} ;
std::uniform_real_distribution<float> Wall::yRan=std::uniform_real_distribution<float>{0,960};


int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 960), "RayCasting");
    window.setPosition({10,10});
    std::vector<Wall> walls;
    for(int i=0;i<5;i++){
        walls.emplace_back();
    }
    walls.emplace_back(0,0,0,1280);
    walls.emplace_back(0,1280,1280,960);
    walls.emplace_back(1280,960,0,960);
    walls.emplace_back(0,960,0,0);

    //Light
    sf::CircleShape light;
    light.setRadius(10);
    light.setFillColor(sf::Color::White);
    light.setOrigin(10,10);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        //Draw walls
        for(auto& i:walls){
            window.draw(i);
        }

        //Draw light point
        light.setPosition(event.mouseMove.x,event.mouseMove.y);
        //window.draw(light);


        //Draw light
        for(int i=0;i<360;i+=1){
            float length = std::numeric_limits<float>::max();
            for(auto& wall : walls) {
                auto x1 = wall.x1;
                auto x2 =wall.x2;
                auto y1 = wall.y1;
                auto y2 = wall.y2;
                auto x3 = light.getPosition().x;
                auto y3 =light.getPosition().y;
                auto x4 = light.getPosition().x+std::cos(static_cast<float>(i)/180*M_PI);
                auto y4 = light.getPosition().y+std::sin(static_cast<float>(i)/180*M_PI);
                double den = (x1-x2)*(y3-y4)-(y1-y2)*(x3-x4);
                if(den==0) continue;
                auto t = ((x1-x3)*(y3-y4)-(y1-y3)*(x3-x4))/den;
                auto u = -((x1-x2)*(y1-y3)-(y1-y2)*(x1-x3))/den;

                if(t>=0&&t<=1&&u>0) {
                    auto px = x1+t*(x2-x1);
                    auto py = y1+t*(y2-y1);
                    length = std::min(length,static_cast<float>(std::sqrt((px-x3)*(px-x3)+(py-y3)*(py-y3))));
                }
            };
            sf::RectangleShape line({length, 1});
            line.rotate(static_cast<float>(i));
            line.setPosition(light.getPosition());
            window.draw(line);
        }


        window.display();
    }

    return 0;
}