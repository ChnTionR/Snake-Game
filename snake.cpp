#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

sf::RenderWindow gameWindow(sf::VideoMode(500, 500), "snake");

sf::Clock timer;

class Snake{

    std::vector<sf::Vector2f> snakePartsPos = {sf::Vector2f(10,25), sf::Vector2f(-1,0)};

    sf::RectangleShape snakePart;

public:

    Snake(){
        drawSnake();
    }

    void drawSnake(){
        for(int i = 0; i < snakePartsPos.size(); i++){
            snakePart.setSize(sf::Vector2f(8,8));
            snakePart.setPosition((snakePartsPos[i].x*10)+1, (snakePartsPos[i].y*10)+1); //apply pos changes
            snakePart.setFillColor(sf::Color::Green); //set color
            gameWindow.draw(snakePart); //draw snake


            //connector parts
            if(snakePartsPos[i].x-1 == snakePartsPos[i+1].x || snakePartsPos[i].x-1 == snakePartsPos[i-1].x){
                snakePart.setSize(sf::Vector2f(1,8));
                snakePart.setPosition(snakePartsPos[i].x*10, (snakePartsPos[i].y*10)+1);
                gameWindow.draw(snakePart);
            }   

            if(snakePartsPos[i].x+1 == snakePartsPos[i+1].x || snakePartsPos[i].x+1 == snakePartsPos[i-1].x){
                snakePart.setSize(sf::Vector2f(1,8));
                snakePart.setPosition((snakePartsPos[i].x*10)+9, (snakePartsPos[i].y*10)+1);
                gameWindow.draw(snakePart);
            }


            if(snakePartsPos[i].y-1 == snakePartsPos[i-1].y || snakePartsPos[i].y-1 == snakePartsPos[i+1].y){
                snakePart.setSize(sf::Vector2f(8,1));
                snakePart.setPosition((snakePartsPos[i].x*10)+1, snakePartsPos[i].y*10);
                gameWindow.draw(snakePart);
            }

            if(snakePartsPos[i].y+1 == snakePartsPos[i-1].y || snakePartsPos[i].y+1 == snakePartsPos[i+1].y){
                snakePart.setSize(sf::Vector2f(8,1));
                snakePart.setPosition((snakePartsPos[i].x*10)+1, (snakePartsPos[i].y*10)+9);
                gameWindow.draw(snakePart);
            }
            
        }

    }

    std::vector<sf::Vector2f> getSnakePos(){
        return snakePartsPos;
    }



    void updatePos(float x, float y){


        //update other parts
        for(int i = snakePartsPos.size()-1; i > 0; i--){
            snakePartsPos[i].x = snakePartsPos[i-1].x;
            snakePartsPos[i].y = snakePartsPos[i-1].y;
        //std::cout <<snakePartsPos[i].x<<"\n";
        }


        //update head
        snakePartsPos[0].x = x;
        snakePartsPos[0].y = y;
    }

    void move(sf::Keyboard::Key& input){

        switch (input){

            case sf::Keyboard::W: //subtract one from yPos
                updatePos(snakePartsPos[0].x, snakePartsPos[0].y-1);
                break;

            case sf::Keyboard::S: //add one to yPos
                updatePos(snakePartsPos[0].x, snakePartsPos[0].y+1);
                break;

            case sf::Keyboard::A: //subtract one from xPos
                updatePos(snakePartsPos[0].x-1, snakePartsPos[0].y);
                break;

            case sf::Keyboard::D: //add one to xPos
                updatePos(snakePartsPos[0].x+1, snakePartsPos[0].y);
                break;
        }

        if(snakePartsPos[0].x >= 50){
            updatePos(0, snakePartsPos[0].y);
        }
        if(snakePartsPos[0].x < 0){
            updatePos(49, snakePartsPos[0].y);
        }
        if(snakePartsPos[0].y >= 50){
            updatePos(snakePartsPos[0].x, 0);
        }
        if(snakePartsPos[0].y < 0){
            updatePos(snakePartsPos[0].x, 49);
        }
    }

    bool checkMove(sf::Keyboard::Key& key){
        switch (key){

            case sf::Keyboard::W:
                if(snakePartsPos[0].y-1 == snakePartsPos[1].y){
                    return false;
                }
                return true;

            case sf::Keyboard::S:
                if(snakePartsPos[0].y+1 == snakePartsPos[1].y){
                    return false;
                }
                return true;

            case sf::Keyboard::A:
                if(snakePartsPos[0].x-1 == snakePartsPos[1].x){
                    return false;
                }
                return true;

            case sf::Keyboard::D:
                if(snakePartsPos[0].x+1 == snakePartsPos[1].x){
                    return false;
                }
                return true;
        }
        return false;
    }

    void grow(){
        snakePartsPos.push_back(snakePartsPos.back());
    }

    sf::Vector2f getHeadPos(){
        return snakePartsPos[0];
    }

    bool checkBodyCollision(){
        for(int i = 1; i<snakePartsPos.size(); i++){
            if(snakePartsPos[0] == snakePartsPos[i]){
                return true;
            }
        }
        return false;
    }

};

class Apple{
    sf::Vector2f pos;
    sf::RectangleShape apple;
public:
    Apple(){
        std::srand(static_cast<unsigned int>(std::time(0)));
        apple.setSize(sf::Vector2f(10, 10));
        randomizePos();
    }
    void randomizePos(){
        pos.x = static_cast<float>(std::rand() % 50) * 10;
        pos.y = static_cast<float>(std::rand() % 50) * 10;
        apple.setPosition(pos);
    }

    void draw(){
        apple.setFillColor(sf::Color::Red);
        gameWindow.draw(apple);
    }

    sf::Vector2f getPos(){
        return sf::Vector2f(pos.x/10, pos.y/10);
    }
};

int main(){
    gameWindow.setFramerateLimit(30);
    Snake snake;
    Apple apple;
    std::vector<sf::Keyboard::Key> keyList = {sf::Keyboard::D};

    snake.drawSnake();
    apple.draw();
    gameWindow.display();

    while(gameWindow.isOpen()){
        sf::Event event;

        while(gameWindow.pollEvent(event)){
            sf::Keyboard::Key key = event.key.code;
            if(event.type == sf::Event::Closed || key == sf::Keyboard::Escape){
                gameWindow.close();
            }

            if(event.type == sf::Event::KeyPressed){
                if(snake.checkMove(key)){
                    keyList[0] = key;
                }
            }
        }

        //std::cout << pressedKey << "\n";

        if(timer.getElapsedTime().asMilliseconds() >= 100){
            snake.move(keyList[0]);

            if(snake.checkBodyCollision()){
                gameWindow.close();
            }

            if (apple.getPos() == snake.getHeadPos()){
                snake.grow();
                apple.randomizePos();
            }

            gameWindow.clear();
            snake.drawSnake();
            for(int i = 1; i < snake.getSnakePos().size(); i++){
                if(apple.getPos() == snake.getSnakePos()[i]){
                    apple.randomizePos();
                }
            }
            apple.draw();
            gameWindow.display();
            timer.restart();

        }
        
      
    }

    return 0;
}