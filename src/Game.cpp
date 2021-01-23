//
// Created by logan on 21.1.21..
//

#include <iostream>
#include "Game.h"

void Game::gameTick(float dt, glm::mat4 projection, glm::mat4 view) {
    timeElapsed += dt;
    previousFire -= dt;
    bool tickUpdate = (timeElapsed-previousTick) >= tickSec;
    auto p = objArray.begin();
    for(; p != objArray.end(); ++p) {
        if((*p)->shouldDelete)
            continue;
        // tick the movement and check for collisions
        // TODO: complexity is O(n^2) at the moment, find a better way to do this
        if(tickUpdate && (*p)->checkMove()) {
            (*p)->move();
            auto other = objArray.begin();
            for(; other != objArray.end(); ++other) {
                if((*other)->shouldDelete) continue;
                if(p != other) {
                    if((*p)->priorityLevel >= (*other)->priorityLevel) continue;
                    if(BoundingBox::boxesIntersect(*(*p)->hitbox, *(*other)->hitbox)) {
                        std::cout << "object collision" << std::endl;
                        (*p)->shouldDelete = true;
                    }
                }
            }
        }

        // we have to redraw them every render loop iteration
        (*p)->draw(projection, view);
    }

    for(; p!= objArray.end(); ++p) {
        if((*p)->shouldDelete) {
            objArray.erase(p);
            delete *p;
        }
    }

    if((timeElapsed - logicTick) >= 1) {
        logicTick = timeElapsed;
        levelLogic();
    }
}

void Game::levelLogic() {
    // this function ticks every second, easier to work with for spawning etc
    secondsCounter++;
    std::cout << "seconds: " << secondsCounter << std::endl;
    switch(secondsCounter) {
        case 5: {
            // spawn a zombie or something
            // don't forget to add it to objArray
        }
    }

}

void Game::addMoveable(MoveableObject* mo) {
    objArray.push_back(mo);
}

Game::~Game() {
    for(auto &p: objArray) {
        delete p;
    }
}

void Game::shoot(glm::vec3 position, glm::vec3 direction) {
    if(previousFire <= 0) {
        float speed = 0.1;
        auto testBullet = new MoveableObject(position + 5.0f*direction, speed*direction, 0.5f, 0.5f, 1, "red");
        testBullet->setShow(true);
        addMoveable(testBullet);
        previousFire = fireRate;
    }
}
