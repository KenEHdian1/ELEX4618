/**
 * @file CControl.h
 * @brief Header file for the CControl class.
 * 
 * This file contains the declaration of the CControl class, which provides 
 * functionality for initializing a serial communication port, retrieving data, 
 * and setting data via serial communication.
 */

#pragma once
#include "Serial.h"

/**
 * @class CControl
 * @brief A class to manage serial communication for data retrieval and control.
 * 
 * The CControl class encapsulates a Serial object and provides methods to 
 * initialize the communication port, retrieve data, and send control data.
 */
class CControl
{
private:
    /**
    *
    * @brief The Serial object used for communication.
    */
    Serial _com;

public:
    /** 
     * @brief Default constructor for CControl.
     * 
     * Initializes the CControl object.
     */
    CControl();

    /**
     * @brief Destructor for CControl.
     * 
     * Cleans up resources used by the CControl object.
     */
    ~CControl();

    /**
     * @brief Initializes the serial communication port.
     * 
     * @param comport The number of the communication port to initialize.
     */
    void init_com(int comport);

    /**
     * @brief Retrieves data from the communication channel.
     * 
     * @param type The type of data to retrieve.
     * @param channel The channel number to retrieve data from.
     * @param result A reference to an integer where the retrieved result will be stored.
     * @return true if the operation is successful, false otherwise.
     */
    bool get_data(int type, int channel, int &result);

    /**
     * @brief Sends data to the communication channel.
     * 
     * @param type The type of data to set.
     * @param channel The channel number to send data to.
     * @param val The value to send to the specified channel.
     * @return true if the operation is successful, false otherwise.
     */
    bool set_data(int type, int channel, int val);

    /////////////////////////////////////////////////////////
    
    int value;//Will be used everytime for get

    float get_analog(int channel); //Returns Analog Input as a % of the full scale (0 - 4096)

    void analogTest();

    void digitalTest();

    bool get_button(int BUTTON, int& lastState,  double& startTime); // Reads digital input and debounces it (use a 1 second timeout)

    void buttonTest();
    
    void servoTest();

    void print_menu(); //Prints the menu for lab 3

    enum { DIGITAL = 0, ANALOG, SERVO }; // For channels
    enum { BUTTON1 = 33, BUTTON2 = 32 }; //For push button
    enum { BLUE = 37, GREEN, RED }; //For Blue LED
    enum { HORZ = 11, VERT = 4 };   //For X and Y axis
    enum { MOTOR = 19}; //For motor for servo

    bool cycle = false; //False menaing it didnt finish the first cycle
};
