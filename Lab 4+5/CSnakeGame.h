#pragma once
#include "CBase4618.h"
#include "CControl.h"

/**
 * @class CSnakeGame
 * @brief Implements a simple snake game using OpenCV and microcontroller input.
 *
 * This class extends `CBase4618` to handle game logic, GPIO input,
 * and rendering the snake on an OpenCV canvas.
 */
class CSnakeGame : public CBase4618
{
public:
    /**
     * @brief Constructs a CSnakeGame object with specified canvas size and communication port.
     * @param canvas_size The size of the OpenCV canvas.
     * @param com_port The communication port for microcontroller interaction.
     */
    CSnakeGame(cv::Size canvas_size, int com_port);

    /**
     * @brief Destructor for CSnakeGame.
     */
    ~CSnakeGame();

    /**
     * @brief Handles GPIO interactions for button and joystick input.
     *
     * Reads inputs from the microcontroller to control snake movement
     * and game resets.
     */
    void gpio();

    /**
     * @brief Updates game logic, including snake movement and interactions.
     *
     * This function is responsible for handling the snake’s position,
     * detecting collisions, and updating the game state.
     */
    void update();

    /**
     * @brief Renders the game on the OpenCV canvas.
     *
     * This function draws the snake, updates the game UI, and handles
     * button-based interactions such as exiting and resetting the game.
     */
    void draw();

private:
    double elapsedTime = 0;
    double startTime = 0;

    int snakeStep = 20; ///< Snake movement step size.
    double snakeStepDelay; ///< Delay between snake movements (for speed control).
    double lastMoveTime; ///< Timestamp of last movement.
    int snakeLength = 5; ///< Length of the snake.
    int score = 0; ///< Player's score.
    std::string str_colour; ///< Color setting for GUI elements.

    std::vector<cv::Point> apples; ///< Vector storing positions of multiple apples.
    cv::Point applePosition; ///< Stores the position of the current apple.

    int gpioCycle = 0; ///< Tracks which input to check in the current cycle.
    int _stepDelayMs = 100; ///< Speed of the snake in milliseconds.

    double start_time = 0; ///< Time reference for FPS locking.
    double initial_time = 0; ///< Initial game start time.

    /// @name Push Button Handling
    /// @{
    cv::Scalar colour; ///< Stores the current LED color (cv::Scalar(Blue, Green, Red)).
    int ledCounter = 0; ///< Counter for cycling LED colors.
    bool _resetState = false; ///< Flag to track reset button state.

    int lastState1 = 0; ///< Last recorded state of BUTTON1.
    double startTime1 = 0.0; ///< Debounce timer for BUTTON1.

    int lastState2 = 0; ///< Last recorded state of BUTTON2.
    double startTime2 = 0.0; ///< Debounce timer for BUTTON2.
    /// @}

    /// @name Analog Input Handling
    /// @{
    float xInput; ///< X-axis joystick input.
    float yInput; ///< Y-axis joystick input.
    bool isMoving; ///< Flag to check if the snake is moving.
    std::string lastDirection; ///< Stores the last movement direction.
    std::vector<cv::Point> snakeBody; ///< Vector storing the snake's body segments.
    cv::Point startHead; ///< Initial position of the snake's head.
    /// @}

    /// @name Canvas Settings
    /// @{
    cv::Size _canvas_size; ///< Size of the OpenCV canvas.
    /// @}
};
