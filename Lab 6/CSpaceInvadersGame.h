#pragma once

#include <vector>
#include "CBase4618.h"
#include "CGameObject.h"
#include "CShip.h"
#include "CInvader.h"
#include "CMissile.h"

class CSpaceInvadersGame : public CBase4618
{
public:
    CSpaceInvadersGame(cv::Size canvas_size, int com_port);
    ~CSpaceInvadersGame();

    void gpio();
    void update();
    void draw();
    void init();


private:
    CShip playerShip;
    std::vector<CInvader> invaders;
    bool _missiles = false;
    std::vector<CMissile> missiles;
    cv::Mat _canvas;
    bool _quit = false;
    bool _reset = false;

    int _score = 0;
    int _lives = 0;
    bool gameOver = false;

    bool _disableShooting = false;

    //For Invaders
    // Set the number of rows and columns for the invaders
    int rows = 3;
    int cols = 10;  // 10 invaders per row

    // Define the size of each invader
    float invaderWidth = 30.0f;  // Width of each invader
    float invaderHeight = 20.0f; // Height of each invader

    // Define the spacing between invaders
    float spacing = 50.0f; // Adjust spacing between invaders as needed

    float startX = 10.0f;  // X position for the first invader
    float startY = 55.0f;  // Y position for the first invader

    std::vector<CMissile> invaderMissiles; // Vector to store invader missiles
    std::vector<int> invaderFireTimers; // Vector to track the timers for each invader (for random intervals)

    // For Controller
    float xInput;
    int lastState1 = 0; ///< Last recorded state of BUTTON1.
    double startTime1 = 0.0; ///< Debounce timer for BUTTON1.

    int lastState2 = 0; ///< Last recorded state of BUTTON2.
    double startTime2 = 0.0; ///< Debounce timer for BUTTON2.

    bool button1 = false;

    cv::Mat im;

    double _gpioTime;
    double _updateTime;
    double _drawTime;
};



