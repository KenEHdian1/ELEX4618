#include "stdafx.h"
#include "CSnakeGame.h"

#include "cvui.h"
#include <sstream>
#include <ctime>


	CSnakeGame::CSnakeGame(cv::Size canvas_size, int com_port)
	{
		Controller.init_com(com_port); // Initializes the COM Port
		gameName = "Snake Game";

		_canvas = cv::Mat::zeros(canvas_size, CV_8UC3); //Creates a black canvas
		_canvas_size = canvas_size;
		applePosition = cv::Point(-1, -1);  // Initialize apple position with an invalid value

		lastMoveTime = (cv::getTickCount() / cv::getTickFrequency()) * 1000; // Set to current time in ms

		// Initialize other game elements
		apples.clear();  // Ensure apples vector is empty at the start

		// Seed the random number generator at the start of the game
		std::srand(static_cast<unsigned int>(std::time(0)));

		// Drawing a circle in the centre
		for (int i = 0; i < snakeLength; i++)
		{
			snakeBody.push_back(cv::Point(800, 400 - (i * 10)));
		}

		startHead = snakeBody[0];
	}

	void CSnakeGame::gpio()
	{
		// Timers
		double start_tic = cv::getTickCount();
		double freq = cv::getTickFrequency();

		static int prevLedCounter = -1;  // Store the last LED state
		static bool firstButtonCheck = true; // Ignore the first button press

		// Only update LED if it actually changed
		if (ledCounter != prevLedCounter)
		{
			prevLedCounter = ledCounter;

			// Define color mappings for LEDs
			static const cv::Scalar colors[] = {
				cv::Scalar(0, 0, 255),  // Red
				cv::Scalar(0, 255, 0),  // Green
				cv::Scalar(255, 0, 0)   // Blue
			};

			static const char* colorNames[] = { "RED", "GREEN", "BLUE" };

			// Digital output values
			static const int ledStates[][3] = {
				{1, 0, 0}, // Red0
				{0, 1, 0}, // Green
				{0, 0, 1}  // Blue
			};

			// Update LED color and text
			colour = colors[ledCounter];
			str_colour = colorNames[ledCounter];

			// Update LED states efficiently
			Controller.set_data(DIGITAL, RED, ledStates[ledCounter][0]);
			Controller.set_data(DIGITAL, GREEN, ledStates[ledCounter][1]);
			Controller.set_data(DIGITAL, BLUE, ledStates[ledCounter][2]);
		}

		switch (gpioCycle)
		{
		case 0:
			// Read joystick inputs
			xInput = Controller.get_analog(HORZ);
			yInput = Controller.get_analog(VERT);
			break;

		case 1:
			// Read reset button
			if (Controller.get_button(BUTTON2, lastState2, startTime2))
			{
				_reset = true;
			}
			break;

		case 2:
			// Read Button 1 for color change
			if (isMoving && Controller.get_button(BUTTON1, lastState1, startTime1))
			{
				if (firstButtonCheck)
				{
					firstButtonCheck = false;  // Ignore first button press
				}
				else
				{
					ledCounter = (ledCounter + 1) % 3; // Cycle through 0,1,2
					std::cout << "ledCounter changed to: " << ledCounter << std::endl;
				}
			}
			break;

		case 3:
			// Do Nothing
			break;

		}

		// Increment cycle index
		gpioCycle = (gpioCycle + 1) % 4;

		// Measure time taken for GPIO processing
		_gpioTime = ((cv::getTickCount() - start_tic) / freq);
	}

	void CSnakeGame::update() // The logic of the game
	{
		// Timers
		double freq2 = cv::getTickFrequency(); //Get tick frequency
		double start_tic2 = cv::getTickCount(); //Get number of ticks since event

		// Snake Movement Logic:
		if (_version == 1) //For Snake Game Version1
		{
			isMoving = false; // Reset isMoving at start
			// Movement logic for version 1 (without delay)
			if (xInput < 20 || xInput > 80)
			{
				if (xInput >= 80) // Move right
				{
					if (startHead.x < _canvas.cols - 1) // Prevent crossing boundary
					{
						startHead.x += 10;
						isMoving = true;
					}
				}
				else if (xInput <= 20) // Move left
				{
					if (startHead.x > 0) // Prevent crossing boundary
					{
						startHead.x -= 10;
						isMoving = true;
					}
				}
			}
			else if (yInput < 20 || yInput > 80)
			{
				if (yInput >= 80) // Move up
				{
					if (startHead.y > 0) // Prevent crossing boundary
					{
						startHead.y -= 10;
						isMoving = true;
					}
				}
				else if (yInput <= 20) // Move down
				{
					if (startHead.y < _canvas.rows - 1) // Prevent crossing boundary
					{
						startHead.y += 10;
						isMoving = true;
					}
				}
			}
		}
		else if (_version == 2 && isMoving)
		{
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Draw Apples 
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			static double lastAppleTime = 0;   // Store the time of the last apple spawn

			double currentTime = (cv::getTickCount() / cv::getTickFrequency()) * 1000;  // Current time in ms
			double appleInterval = 5000; // 5 seconds interval

			// Spawn apples every 5 seconds
			if (currentTime - lastAppleTime >= appleInterval)
			{
				// Update the last spawn time
				lastAppleTime = currentTime;

				// Generate a new random apple position
				int appleX = rand() % (_canvas.cols - 20);  // Random X coordinate within canvas, 20px margin
				int appleY = rand() % (_canvas.rows - 20);  // Random Y coordinate within canvas, 20px margin

				// Ensure apple does not overlap with the snake body
				bool overlap = false;
				for (const auto& bodyPart : snakeBody)
				{
					if (cv::norm(bodyPart - cv::Point(appleX, appleY)) < 10)  // Ensure no overlap (10px radius check)
					{
						overlap = true;
						break;
					}
				}

				// If overlap is found, retry the random generation
				if (overlap)
				{
					appleX = rand() % (_canvas.cols - 20);
					appleY = rand() % (_canvas.rows - 20);
				}

				// Add the new apple position to the apples vector
				apples.push_back(cv::Point(appleX, appleY));
			}

			// Handle apple collision
			for (size_t i = 0; i < apples.size(); ++i)
			{
				if (cv::norm(snakeBody.front() - apples[i]) < 20)  // Collision check between head and apple
				{
					score++; //Increment the score to show the points

					// Apple eaten, remove it from the vector
					apples.erase(apples.begin() + i);

					// Increase snake length
					snakeLength++;

					// Add a new segment to the snake (you can adjust this behavior for game design)
					snakeBody.push_back(snakeBody.back());  // Append a new body segment at the tail

					// Exit the loop once we find the eaten apple
					break;
				}
			}


			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Snake Logic 
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////

			// Get the current time in milliseconds
			double currentTime1 = (cv::getTickCount() / cv::getTickFrequency()) * 1000;
			isMoving = true;

			// Check if enough time has passed for the next move based on stepDelayMS
			if ((currentTime1 - lastMoveTime) >= (_stepDelayMs) && isMoving)
			{
				lastMoveTime = currentTime1;

				// Change direction only when analog is moved
				if ((xInput < 30) && (lastDirection != "RIGHT") )
				{
					lastDirection = "LEFT";
					isMoving = true;
				}
				else if ((xInput > 70) && (lastDirection != "LEFT"))
				{
					lastDirection = "RIGHT";
					isMoving = true;
				}
				else if ((yInput < 30) && (lastDirection != "UP"))
				{
					lastDirection = "DOWN";
					isMoving = true;
				}
				else if ((yInput > 70) && (lastDirection != "DOWN"))
				{
					lastDirection = "UP";
					isMoving = true;
				}

				// Move the snake in the last direction but prevent boundary crossing
				if (lastDirection == "LEFT")
				{
					if (startHead.x > snakeStep - 1)
					{
						startHead.x -= snakeStep;
					}
					else 
					{
						isMoving = false;
					}
				}
				else if (lastDirection == "RIGHT")
				{
					if (startHead.x < _canvas.cols - snakeStep + 1)
					{
						startHead.x += snakeStep;
					}
					else
					{
						isMoving = false;
					}
				}
				else if (lastDirection == "UP")
				{
					if (startHead.y > snakeStep - 1)
					{
						startHead.y -= snakeStep;
					}
					else
					{
						isMoving = false;
					}
				}
				else if (lastDirection == "DOWN")
				{
					if (startHead.y < _canvas.rows - snakeStep + 1)
					{
						startHead.y += snakeStep;
					}
					else
					{
						isMoving = false;
					}

				}

				// Check if the head collides with the body
				for (size_t i = 1; i < snakeBody.size(); i++)
				{
					if (snakeBody[i] == startHead)
					{
						isMoving = false; // Stop movement
						return; // Exit function
					}
				}
			}
		}

		// Insert the new head at the front of the vector if the snake is moving
		if (startHead != snakeBody[0]) //Will update if a change is made
		{
			snakeBody.insert(snakeBody.begin(), startHead);

			// Remove the last segment to keep the snake's length constant (20 units)
			if (snakeBody.size() > snakeLength)
			{
				snakeBody.pop_back(); // Remove the last segment (tail)
			}
		}

		_updateTime = ((cv::getTickCount() - start_tic2) / freq2);
	}

	void CSnakeGame::draw()
	{
		// Timers
		double freq3 = cv::getTickFrequency(); //Get tick frequency
		double start_tic3 = cv::getTickCount(); //Get number of ticks since event
		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Initialize GUI system
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		_canvas = cv::Mat::zeros(_canvas_size, CV_8UC3); //Creates a black canvas

		cv::Point gui_position;
		gui_position = cv::Point(10, 10);

		cvui::window(_canvas, gui_position.x, gui_position.y, 200, 300, "Snake");

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Exit Button
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		gui_position = cv::Point(42, 278);
		if (cvui::button(_canvas, gui_position.x, gui_position.y, "Exit"))
		{
			_quit = true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Reset Button
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		gui_position = cv::Point(112, 278);
		if (cvui::button(_canvas, gui_position.x, gui_position.y, "Reset") || _reset)
		{
			score = 0; //Reset the score back to 0

			//Clear the image
			snakeBody.clear(); // Clears the vector
			snakeLength = 5;

			for (int i = 0; i < snakeLength; i++)
			{
				snakeBody.push_back(cv::Point(_canvas.cols / 2, _canvas.rows / 2 - (i * 10))); //From ChatGPT
			}
			startHead = snakeBody[0];  // Reset the head to the first segment
			ledCounter = 0; // Reset the ledCounter


			//Clear the apples
			apples.clear();

			//Reset the _reset flag
			_reset = false;

			//Resets the snake back to moving
			lastDirection = "DOWN"; //Have it move down initially
			isMoving = true;
		}
		
		if (_version == 1)
		{
			// Draw the snake body
			for (int i = 0; i < snakeBody.size() - 1; i++)
			{
				cv::circle(_canvas, snakeBody[i], (10 / 2), colour, -1, cv::LINE_AA);
			}
		}
		else if (_version == 2)
		{
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Display Score
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			gui_position = cv::Point(12, 108);
			cvui::text(_canvas, gui_position.x, gui_position.y, "Score: " + std::to_string(score));

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Display the Colour of the Snake
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			gui_position = cv::Point(12, 88);
			cvui::text(_canvas, gui_position.x, gui_position.y, "Colour: " + str_colour);


			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Snake Step Size Track Bar
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			gui_position = cv::Point(12, 168);
			cvui::trackbar(_canvas, gui_position.x, gui_position.y, 180, &snakeStep, 10, 30, true);
			gui_position = cv::Point(52, 168);
			cvui::text(_canvas, gui_position.x, gui_position.y, "Snake Step Size");


			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Snake Speed Track Bar
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Trackbar to adjust snake step delay (10 - 500 ms)
			gui_position = cv::Point(62, 218);
			cvui::text(_canvas, gui_position.x, gui_position.y, "Snake Speed");
			gui_position = cv::Point(12, 218);
			cvui::trackbar(_canvas, gui_position.x, gui_position.y, 180, &_stepDelayMs, 10, 500);


			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Draw the snake body
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Adjust snake body size based on step size
			int segmentSize = snakeStep / 2;  // Set segment size to step size and ensures the size is at least 10 pixels

			for (int i = 0; i < snakeBody.size() - 1; i++)
			{
				cv::circle(_canvas, snakeBody[i], (segmentSize), colour, 1, cv::LINE_AA);
			}


			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Draw Apples 
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			for (const auto& apple : apples) //(ChatGPT)
			{
				cv::circle(_canvas, apple, 10, cv::Scalar(0, 0, 255), -1, cv::LINE_AA);  // Red apple
			}

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Display Game Over 
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (isMoving == false)
			{
				cvui::printf(_canvas, 800, 400, "Game Over");
			}
		
			elapsedTime = (cv::getTickCount() - startTime) / cv::getTickFrequency();
			while (elapsedTime <= (0.033333))
			{
				double buffer = cv::getTickCount();
				elapsedTime = (buffer - startTime) / cv::getTickFrequency(); //Reset the time
			}

			startTime = cv::getTickCount();



			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Show the FPS with Exponential Moving Average (More Adaptive, No Fixed 30-Frame Limit)
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			static double smoothedFPS = 0.0;
			const double alpha = 0.1; // Smoothing factor (adjust between 0 and 1, lower = smoother FPS)


			double fps = 1.0 / elapsedTime;

			// Apply exponential moving average smoothing
			smoothedFPS = (alpha * fps) + (1.0 - alpha) * smoothedFPS;

			// Display the smoothed FPS
			gui_position = cv::Point(110, 15);
			std::stringstream _fpsString;
			_fpsString.precision(4);
			_fpsString << "FPS: " << smoothedFPS << " Hz";
			cvui::text(_canvas, gui_position.x, gui_position.y, _fpsString.str());

			//Get the Draw Time
			_drawTime = ((cv::getTickCount() - start_tic3) / freq3);
		}
	
		// Ensure cvui updates properly
		cvui::update();  
		cv::imshow(gameName, _canvas);
	}

	CSnakeGame::~CSnakeGame()
	{

	}