#include "Actor.h"
#include <string>
#include <iostream>
using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp


Actor::Actor(StudentWorld* sw, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth) : GraphObject(imageID, startX, startY, dir, size, depth) {
    m_world = sw;
    m_isAlive = true;
}

Actor::~Actor() {

}
bool Actor::isAlive() {
    return m_isAlive;
}
void Actor::setDead() {
    m_isAlive = false;
}


StudentWorld* Actor::getWorld() {
    return m_world;
}
//void Actor::setWorld(StudentWorld *world) {
//    m_world = world;
//}

Earth::Earth(StudentWorld* sw, int startX, int startY) : Actor(sw, TID_EARTH, startX, startY, right, .25, 3) {
    setVisible(true);
}
void Earth::doSomething() {
    return;
}
Earth::~Earth() {

}


Tunnelman::Tunnelman(StudentWorld* sw) : Actor(sw, TID_PLAYER, 30, 60, right, 1, 0) {
    setVisible(true);
    m_hp = 10;
    m_waterUnits = 5;
    m_numSonar = 1;
    m_numGold = 0;

}
Tunnelman::~Tunnelman() {

}

int Tunnelman::hp() const {
    return m_hp;
}
int Tunnelman::numWater() const {
    return m_waterUnits;
}
int Tunnelman::numSonar() const {
    return m_numSonar;
}
int Tunnelman::numGold() const {
    return m_numGold;
}

Boulder::Boulder(StudentWorld* sw, int startX, int startY) : Actor(sw, TID_BOULDER, startX, startY, down, 1, 1) {
    setVisible(true);
    m_state = stable;
    waitingNum = 30;
 
}

Boulder::~Boulder() {
}

void Boulder::setState(string state) {
    m_state = state;
}

void Boulder::doSomething() {
    if (!isAlive()) {
        return;
    }
    int x = getX();
    int y = getY();
    //if it's stable, set to waiting.
    if (getState() == stable) {
        if (!getWorld()->isthereEarth(x, y - 1)) {
            setState(waiting);
        }
    }
    if (waitingNum == 0) {
        setState(falling);
        getWorld()->playSound(SOUND_FALLING_ROCK);
        waitingNum = -1;
    }

    if (getState() == falling) {
        // As long as there isn't earth below and y is valid we can move down
        // TODO: check for a bolder
        if (y == -1 || getWorld()->isthereEarth(x, y - 1)) {
            setDead();
        }
        if (!getWorld()->isthereEarth(x, y - 1)) {
            moveTo(x, y - 1);
        }

        // Need to figure out how to not run into other boulders. I think something like we had in Zion like nRobotsAt?? like a function to check if a boulder/object is at that location. Maybe it could take in a class type so we can template it for other classess???

    }
    // TODO: check if it's near a Protestor or Tunnelman & annoy them.
    // Decrement each tick if waiting to fall
    if (getState() == waiting) {
        waitingNum--;
    }
}

Barrel::Barrel(StudentWorld* sw, int startX, int startY, Tunnelman* p) : Actor(sw, TID_BARREL, startX, startY, right, 1, 2) {
    m_x = startX;
    m_y = startY;
    playerInGame = p;
    //count++;
}

void Barrel::doSomething() {
    if (!isAlive()) {
        return;
    } 
    int x = playerInGame->getX();
    int y = playerInGame->getY();
   
    if (found != true) {
        if (m_x - 4 <= x && x <= m_x + 4) {
            if (m_y - 4 <= y && y <= m_y + 4) {

                setVisible(true);
                found = true;
                return;
            }

        }
    }
    if (m_x - 3 <= x && x <= m_x + 3) {
        if (m_y - 3 <= y && y <= m_y + 3) {
            setDead();
            getWorld()->playSound(SOUND_FOUND_OIL);
            getWorld()->increaseScore(1000);
            getWorld()->decrementBarrelCount();
            
        }

    }
   

   
}
void Barrel::makeVisible() {
    found = true;

}
Barrel::~Barrel() {
}
void Barrel::setState(string state) {
    m_state = state;
}

void Tunnelman::doSomething() {

    int ch;

    if (getWorld()->getKey(ch) == true)
    {
        // user hit a key this tick!
        switch (ch)
        {
        case KEY_PRESS_LEFT:
            if (this->getDirection() != left) {
                this->setDirection(left);
                break;
            }
            if (getX() != 0) {
                moveTo(getX() - 1, getY());
            }

            break;
        case KEY_PRESS_RIGHT:
            if (this->getDirection() != right) {
                this->setDirection(right);
                break;
            }
            if (getX() != 60) {
                moveTo(getX() + 1, getY());
                //                if (m_world->isthereEarth(getX()+1, getY())) {
                //                    m_world->digField(getX()+1, getY());
                //                    m_world->digField(getX()+2, getY());
                //                    m_world->playSound(SOUND_DIG);
                //                }
            }
            break;
        case KEY_PRESS_UP:
            if (this->getDirection() != up) {
                this->setDirection(up);
                break;
            }

            if (getY() != 60) {
                moveTo(getX(), getY() + 1);
                //                if (m_world->isthereEarth(getX(), getY())) {
                //                    m_world->digField(getX(), getY());
                //                    m_world->playSound(SOUND_DIG);
                //                }
            }

            break;
        case KEY_PRESS_DOWN:
            if (this->getDirection() != down) {
                this->setDirection(down);
                break;
            }
            if (getY() != 0) {
                moveTo(getX(), getY() - 1);
                //                if (m_world->isthereEarth(getX(), getY())) {
                //                    m_world->digField(getX(), getY());
                //                    m_world->playSound(SOUND_DIG);
                //                }
            }
            break;
        case KEY_PRESS_SPACE:

            break;
        case KEY_PRESS_ESCAPE:
            setDead();
            break;
        case none:
            return;
            // etcÖ
        }
        //        Direction d = getDirection();
        //        int x = getX();
        //        int y = getY();

        if (getWorld()->isthereEarth(getX(), getY())) {
            getWorld()->digField(getX(), getY());
            getWorld()->playSound(SOUND_DIG);
           
        }
        
    }
}
void Tunnelman::getLocation() {
    x = getX();
    y = getY();
}