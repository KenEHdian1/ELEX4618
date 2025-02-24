#include "stdafx.h"
#include "cvui.h"
#include "CSpaceInvadersGame.h"

CSpaceInvadersGame::CSpaceInvadersGame(cv::Size canvas_size, int com_port)
{
    srand(0);
	gameName = "Space Invaders";
	_canvas = cv::Mat::zeros(canvas_size, CV_8UC3); //Creates a black canvas
	im = cv::imread("background.jpg"); // Place the background
    cv::resize(im, im, canvas_size);

	Controller.init_com(com_port); // Initializes the COM Port
	init();
}

void CSpaceInvadersGame::init()
{
    // Clear any previous invaders
    invaders.clear();

    // Loop to create invaders in a grid of 3 rows and 10 columns
    for (int i = 0; i < rows; ++i) 
    {
        for (int j = 0; j < cols; ++j) 
        {
            // Calculate the position of each invader based on row and column
            cv::Point2f position(startX + j * (invaderWidth + spacing), startY + i * (invaderHeight + spacing));

            // Create an invader object, using the constructor that takes position and velocity
            CInvader invader(position, cv::Point2f(1, 0), cv::Scalar(127, 127, 127)); // Set position and velocity directly

            invaders.push_back(invader);
        }
    }
}

void CSpaceInvadersGame::gpio()
{
    xInput = Controller.get_analog(HORZ); //Get the X Percentage

    //Reset Button
    _reset = (Controller.get_button(BUTTON2, lastState2, startTime2));

    _missiles = (Controller.get_button(BUTTON1, lastState1, startTime1));

    //For cheats LOL, just holding button will infinitely shoot missiles
	//button1 = Controller.get_button(BUTTON1, lastState1, startTime1);
	//if (lastState1 == 0)
	//{
	//		_missiles = true;
	//}
	//else
	//	_missiles = false;
	
}

void CSpaceInvadersGame::update()
{
    if (!gameOver)
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Missiles Logic
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (_missiles)
        {
            // Create a new missile at the player's ship position
            missiles.push_back(CMissile(cv::Point2f(playerShip.get_pos().x + playerShip.get_shape().width / 2, playerShip.get_pos().y), cv::Point(0, -10), cv::Scalar(255, 0, 0)));
        }

        // Update missile positions
        for (int i = 0; i < missiles.size(); i++)
        {
            missiles[i].move();  // Move missile

            bool hit = false;

            // Check for missile collisions with invaders
            for (int j = 0; j < invaders.size(); j++)
            {
                if (missiles[i].collide(invaders[j]))  // Only check actual collision
                {
                    invaders[j].hit();  // Decrease invader's life

                    if (invaders[j].get_lives() <= 0)  // If invader's life reaches 0
                    {
                        invaders.erase(invaders.begin() + j);
                        j--;  // Adjust index to avoid skipping next invader
                    }

                    // Mark missile as hit
                    hit = true;
                    _score += 10;
                    break;  // Stop checking further invaders for this missile
                }
            }

            // If missile collides or goes off-screen, remove it
            if (hit || missiles[i].get_pos().y + missiles[i].get_shape().height < 0)
            {
                missiles.erase(missiles.begin() + i);
                i--;  // Adjust index after removal
            }
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Invader Missiles Logic - Handle Invaders Firing Missiles
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Loop through each invader in the last row to check if it can fire a missile
        int lastRowStart = (rows - 1) * cols;  // Index of first invader in last row
        int lastRowEnd = lastRowStart + cols;  // Index of last invader in last row

        for (int i = lastRowStart; i < lastRowEnd; ++i)
        {
            if (i >= invaders.size()) break; // To ensure we don't go out of bounds if invaders are removed

            // Random chance to fire a missile (e.g., 1 in 100 chance every frame)
            if (rand() % 100 < 2)  // 2% chance to fire missile per frame
            {
                // Create a new missile with the starting position at the center of the bottom of the invader
                cv::Point2f missileStartPos(
                    invaders[i].get_pos().x + invaders[i].get_shape().width / 2,  // Center horizontally
                    invaders[i].get_pos().y + invaders[i].get_shape().height     // Just below the bottom
                );

                // Create a new missile with the starting position of the invader
                invaderMissiles.push_back(CMissile(missileStartPos, cv::Point(0, 5), cv::Scalar(0, 0, 255)));
            }
        }

        // Update invader missiles
        for (int i = 0; i < invaderMissiles.size(); i++)
        {
            invaderMissiles[i].move(); // Move the missile

            bool missileRemoved = false; // Flag to check if missile was removed

            // Check if invader missile hits the player's ship
            if (invaderMissiles[i].collide(playerShip))
            {

                _score -= 5;
                playerShip.hit();  // Reduce player's life

                invaderMissiles.erase(invaderMissiles.begin() + i); // Remove missile if it hits
                missileRemoved = true;  // Set the flag indicating missile is removed
            }

            // Remove missile if it goes off-screen or was removed due to collision
            if (!missileRemoved && invaderMissiles[i].get_pos().y > _canvas.rows)
            {
                invaderMissiles.erase(invaderMissiles.begin() + i);
                i--;  // Adjust index after removal
            }

            // Adjust index only if missile was removed
            if (missileRemoved)
            {
                i--;  // Adjust index after missile removal
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Invader Movement Logic
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool hitRightWall = false;
        bool hitLeftWall = false;
        int counter = 0;

        // Check if any invader hits the right or left boundary
        for (auto& invader : invaders)
        {
            if (invader.get_pos().x + invader.get_shape().width >= _canvas.cols)
            {
                hitRightWall = true;
            }
            else if (invader.get_pos().x <= 0)
            {
                hitLeftWall = true;
            }

        }

        // Move invaders based on wall hit
        if (hitRightWall || hitLeftWall)
        {
            for (auto& invader : invaders)
            {
                // Reverse velocity
                invader.set_velocity(cv::Point2f(-invader.get_velocity().x, invader.get_velocity().y));
                // Move down by 20 units
                invader.set_pos(cv::Point2f(invader.get_pos().x, invader.get_pos().y + 20));
            }

            if (hitLeftWall)
            {
                for (int i = 0; i < cols; ++i)
                {
                    // New invader at the top with the same initial X position and Y position for new rows
                    CInvader newInvader(cv::Point2f((startX - 10) + i * (invaderWidth + spacing), startY - 20), cv::Point2f(1, 0), cv::Scalar(127, 127, 127));   // Moving right initially
                    invaders.push_back(newInvader);
                }
            }
        }

        // Apply movement to all invaders
        for (auto& invader : invaders)
        {
            invader.set_pos(cv::Point2f(invader.get_pos().x + invader.get_velocity().x, invader.get_pos().y));
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Ship Movement
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        if (xInput < 40) // Moving Left
        {
            playerShip.set_velocity(cv::Point2f(-100, 0)); // Set velocity left
        }
        else if (xInput > 60) // Moving Right
        {
            playerShip.set_velocity(cv::Point2f(100, 0)); // Set velocity right
        }
        else
        {
            playerShip.set_velocity(cv::Point2f(0, 0)); // Stop if within dead zone
        }

        // Move the ship
        playerShip.move();

        // If ship collides with the wall, stop movement
        if (playerShip.collide_wall(_canvas.size()))
        {
            if (playerShip.get_pos().x <= 0)
            {
                playerShip.set_pos(cv::Point2f(0, playerShip.get_pos().y));
            }
            else if (playerShip.get_pos().x + playerShip.get_shape().width >= _canvas.cols)
            {
                playerShip.set_pos(cv::Point2f(_canvas.cols - playerShip.get_shape().width, playerShip.get_pos().y));
            }
        }

    }
    
}

void CSpaceInvadersGame::draw()
{
	_canvas.setTo(cv::Scalar(0, 0, 0));
    cv::Rect picture(0, 0, 800, 1000);

    im.copyTo(_canvas(picture));

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Player
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    playerShip.draw(_canvas);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Player Missiles
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (auto& missile : missiles)
    {
        missile.draw(_canvas);  // Draw each missile
    }

    // Remove missiles that move off-screen (e.g., above the screen) - From ChatGPT
    missiles.erase(std::remove_if(missiles.begin(), missiles.end(),
        [this](CMissile& missile) {
            return missile.get_pos().y + missile.get_shape().height < 0;  // If missile goes above the top of the screen
        }),
        missiles.end());


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Invaders 
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (auto& invader : invaders)  // Use reference to avoid copying each invader
    {
        invader.draw(_canvas);  // Call draw on each individual invader
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Invader Missiles
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (auto& missile : invaderMissiles)
    {
        missile.draw(_canvas);  // Draw each invader missile
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Initialize GUI system
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    cv::Point gui_position;
    gui_position = cv::Point(1, 1);

    cvui::window(_canvas, gui_position.x, gui_position.y, 800, 49, "Space Invaders Game");

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Exit Button
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    gui_position = cv::Point(680, 22);
    if (cvui::button(_canvas, gui_position.x, gui_position.y, "Exit"))
    {
        setQuit(true);  // <-- Tell `CBase4618` to quit
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Reset Button
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    gui_position = cv::Point(735, 22);
    if (cvui::button(_canvas, gui_position.x, gui_position.y, "Reset") || _reset)
    {
        gameOver = false;
        _score = 0; //Reset the score
        missiles.clear(); //Clears the missile
        invaderMissiles.clear(); //Clears the invaders missile

        init(); //Recall the init function
        playerShip.set_pos(cv::Point2f(_canvas.cols / 2 - playerShip.get_shape().width / 2, _canvas.rows - 50)); // Reset the players posotion
        playerShip.set_lives(9);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Player's Score
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    gui_position = cv::Point(150, 30);
    cvui::text(_canvas, gui_position.x, gui_position.y, "SCORE:  " + std::to_string(_score));

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Player's Lives
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    gui_position = cv::Point(25, 30);
    cvui::text(_canvas, gui_position.x, gui_position.y, "Ships Lifes: " + std::to_string(playerShip.get_lives()));

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Missiles Fired
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    gui_position = cv::Point(550, 30);
    cvui::text(_canvas, gui_position.x, gui_position.y, "Missiles Fired: " + std::to_string(missiles.size()));

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Game Over
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (playerShip.get_lives() == 0)
    {
        gameOver = true;
        gui_position = cv::Point(400, 500);
        cvui::printf(_canvas, gui_position.x, gui_position.y, "YOU LOSE");
    }




	cvui::update();
	cv::imshow(gameName, _canvas); // Print the canvas
}

CSpaceInvadersGame::~CSpaceInvadersGame()
{

}