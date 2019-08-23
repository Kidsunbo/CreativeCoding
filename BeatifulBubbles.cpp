#include <SFML/Graphics.hpp>
#include <random>
#include <vector>
#include <chrono>

class MoveCircle:public sf::Drawable{

    static std::default_random_engine e;
    static std::uniform_int_distribution<int> colorRan;
    static std::uniform_int_distribution<int> posXRan;
    static std::uniform_int_distribution<int> posYRan;


    sf::CircleShape circleShape;
    float minRad = 0;
    float maxRad = 50.0f;
    float h;
    float v;
    float changeSpeed = 5.0f;
    float moveSpeed = 1.0f;
public:

    MoveCircle(){
        minRad = static_cast<float>((colorRan(e))/80.0f)*2.0f+1;
        circleShape.setRadius(minRad);
        circleShape.setFillColor(sf::Color(colorRan(e),colorRan(e),colorRan(e)));
        circleShape.setPosition(posXRan(e),posYRan(e));
        h = colorRan(e)%2==0?moveSpeed:-moveSpeed;
        v = colorRan(e)%2==0?moveSpeed:-moveSpeed;
    }

    void update(float x,float y,sf::RenderWindow& window){
        circleShape.setOrigin(circleShape.getRadius(),circleShape.getRadius());
        if((std::abs(x-circleShape.getPosition().x)<=100&& std::abs(y-circleShape.getPosition().y)<=100)&&circleShape.getRadius()<=maxRad){
            circleShape.setRadius(circleShape.getRadius()+changeSpeed);
        }
        if((std::abs(x-circleShape.getPosition().x)>100|| std::abs(y-circleShape.getPosition().y)>100)&&circleShape.getRadius()>minRad){
            circleShape.setRadius(circleShape.getRadius()-changeSpeed);
        }
        if((circleShape.getPosition().x+circleShape.getRadius())>=window.getSize().x) h = -moveSpeed;
        else if((circleShape.getPosition().x-circleShape.getRadius())<=0) h = moveSpeed;
        if((circleShape.getPosition().y+circleShape.getRadius())>=window.getSize().y) v= -moveSpeed;
        else if((circleShape.getPosition().y-circleShape.getRadius())<=0) v=moveSpeed;
        circleShape.move(h,v);
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        target.draw(circleShape,states);
    }

};

std::default_random_engine MoveCircle::e = std::default_random_engine(static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count()));
std::uniform_int_distribution<int> MoveCircle::colorRan = std::uniform_int_distribution<int>(0,255);
std::uniform_int_distribution<int> MoveCircle::posXRan = std::uniform_int_distribution<int>(0,1280);
std::uniform_int_distribution<int> MoveCircle::posYRan = std::uniform_int_distribution<int>(0,960);


int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 960), "BeatifulBubbles",sf::Style::Close|sf::Style::Titlebar);
    window.setPosition({10,10});
    std::vector<MoveCircle> circles(250);
    window.setFramerateLimit(30);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        for(auto& circle:circles){
            circle.update(event.mouseMove.x,event.mouseMove.y,window);
            window.draw(circle);
        }


        window.display();
    }

    return 0;
}