#include "StudentWorld.h"
#include "Actor.h"
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir)
{
}

StudentWorld::~StudentWorld() {


}

int StudentWorld::init()
{
    for (int x = 0; x < VIEW_WIDTH; x++) {
        for (int y = 0; y < VIEW_HEIGHT; y++) {
            if (y < 4 || x != 30 && x != 31 && x != 32 && x != 33 && y < 60) {
                earthPtrs[x][y] = new Earth(this, x, y);
            }
            else {
                earthPtrs[x][y] = nullptr;
            }
        }
    }
    player = new Tunnelman(this);

    // Create boulders where # of boulders  min(current_level_number/2+2, 9)
    // Boulders must be distributed between x=0,y=20 and x=60,y=56, inclusive (so
    // they have room to fall).
    //revist the loop that doesnt include the shaft
    int numBoulders = std::min((static_cast<int>(getLevel()) / 2) + 2, 9);
    for (int i = 0; i < numBoulders; i++) {
        int x = rand() % 61;
        if (x >= 28 && x <= 35) {
            x += 8;
        }
        int y = rand() % (57 - 20 + 1) + 20;
       while (isBoulderthere(x, y) == true) {
           int x = rand() % 61;
           if (x >= 30 && x <= 33) {
               x += 8;
           }
           int y = rand() % (57 - 20 + 1) + 20;
        }
        actorPtrs.push_back(new Boulder(this, x, y));
        setLocation(x, y);
        digField(x, y);
    }
  
    int numBarrels = std::min((static_cast<int>(2+getLevel())), 21);
    for (int i = 0; i < numBarrels; i++) {
        int x = rand() % 61;
        if (x >= 28 && x <= 35) {
            x += 8;
        }
        int y = rand() % (57 - 20 + 1) + 20;
        while (isBoulderthere(x, y) == true) {
            int x = rand() % 61;
            if (x >= 30 && x <= 33) {
                x += 8;
            }
            int y = rand() % (57 - 20 + 1) + 20;
        }
        actorPtrs.push_back(new Barrel(this, x, y, player));
        barrelCount++;
        setLocation(x, y);
        digField(x, y);
    }
    return GWSTATUS_CONTINUE_GAME;
}
void StudentWorld::setLocation(int xPos, int yPos) {
    for (int k = xPos; k <= xPos + 5 && (k >= 0 && k < VIEW_WIDTH); k++) {
        for (int j = yPos; j <= yPos + 5 && (j >= 0 && j < VIEW_HEIGHT); j++) {
            BoulderPtrs[xPos][yPos] = true;
        }
    }
 
}

bool StudentWorld::isBoulderthere(int xPos, int yPos) {

    for (int k = xPos; k <= xPos + 5 && (k >= 0 && k < VIEW_WIDTH); k++) {
        for (int j = yPos; j <= yPos + 5 && (j >= 0 && j < VIEW_HEIGHT); j++) {
            if (BoulderPtrs[k][j] == true) {
                return true;
            }
              
        }
    }
    for (int k = xPos; k <= xPos - 5 && (k >= 0 && k < VIEW_WIDTH); k--) {
        for (int j = yPos; j <= yPos - 5 && (j >= 0 && j < VIEW_HEIGHT); j--) {
            if (BoulderPtrs[k][j] == true) {
                return true;
            }

        }
    }
    return false;
   /* if (BoulderPtrs[xPos][yPos] == true) {
        return false;
    }
    return true;*/
    //Trying to make squares into a 2D array, and turn the values into the arguments
    //into the the x,y coordinates 

}
void StudentWorld::setDisplayText() {
    int level = getLevel();
    int lives = getLives();
    int health = player->hp();
    int squirts = player->numWater();
    int gold = player->numGold();
    int barrelsLeft = 111;
    int sonar = player->numSonar();
    int score = getScore();
    string s;
    s = "BC: " + std::to_string(barrelCount) + "Time: " + std::to_string(tick) + " Lvl: " + std::to_string(level) + " Lives: " + std::to_string(lives) + " Hlth: " + std::to_string(health) + "% Wtr: " + std::to_string(squirts) + " Gld: " + std::to_string(gold) + " Oil Left: " + std::to_string(barrelsLeft) + " Sonar: " + std::to_string(sonar) + " Scr: " + std::to_string(score);
    setGameStatText(s);
}
int StudentWorld::move()
{
    /// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    //
    if (barrelCount == 0) {
        advanceToNextLevel();
        return GWSTATUS_FINISHED_LEVEL;
    }
    tick++;
    
    setDisplayText();
   //decLives();
    player->doSomething();
 
    vector<Actor*>::iterator it;
    it = actorPtrs.begin();
    while (it != actorPtrs.end()) {
        (*it)->doSomething();
       
        it++;
    }
    it = actorPtrs.begin();
    while (it != actorPtrs.end()) {
       
        if (!(*it)->isAlive()) {
           delete (*it);
           it = actorPtrs.erase(it);
        }
        else {
            it++;
        }
    }

    

    return GWSTATUS_CONTINUE_GAME;
   // return barrelCount;
}


void StudentWorld::cleanUp()
{
    delete player;
    for (int x = 0; x < VIEW_WIDTH; x++) {
        for (int y = 0; y < VIEW_HEIGHT; y++) {
            delete earthPtrs[x][y];
        }
    }
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
/*Remove/destroy the Earth objects from the 4x4 area occupied by
the Tunnelman (from x, y to x+3,y+3 inclusive)*/
void StudentWorld::digField(int x, int y) {
    //    if (earthPtrs[x][y] != nullptr) {
    for (int k = x; k <= x + 3 && (k >= 0 && k < VIEW_WIDTH); k++) {
        for (int j = y; j <= y + 3 && (j >= 0 && j < VIEW_HEIGHT); j++) {
            delete earthPtrs[k][j];
            earthPtrs[k][j] = nullptr;
        }
    }
    //    }
}

bool StudentWorld::isthereEarth(int x, int y) {
    for (int k = x; k <= x + 3 && (k >= 0 && k < VIEW_WIDTH); k++) {
        for (int j = y; j <= y + 3 && (j >= 0 && j < VIEW_HEIGHT); j++) {
            if (earthPtrs[k][j] != nullptr)
                return true;
        }
    }
    return false;


}

void StudentWorld::decrementBarrelCount() {
    if (barrelCount != 0) {
        barrelCount--;
    }
}