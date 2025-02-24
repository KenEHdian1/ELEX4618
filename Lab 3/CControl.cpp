#include "stdafx.h"
#include "CControl.h"
#include <iostream>
#include <string>
#include <conio.h>
#include <iomanip>

#define BAUD_RATE 115200

void CControl::init_com(int comport) 
{
	std::string portName = "COM" + std::to_string(comport);
	_com.open(portName, BAUD_RATE);
}

bool CControl::get_data(int type, int channel, int& result)
{

	// Will write the cmd to the uController 
	// Will receieve the Acknowledge string "A 0 33\n" 
	// Parse the 'value' data from the string 
	// store the 'data' into the variable result
	// once the data has been read susccesfully, it will return true
	bool readData = FALSE;
	std::string rx_str;

	char buff[2];
	std::string cmd = "g " + std::to_string(type) + " " + std::to_string(channel) + '\n';

	if (_com.is_open()) 
	{
		_com.write(cmd.c_str(), cmd.length());
		Sleep(10);

		rx_str = "";
		// start timeout count
		double start_time = cv::getTickCount();

		buff[0] = 0;
		// Read 1 byte and if an End Of Line then exit loop
		// Timeout after 1 second, if debugging step by step this will cause you to exit the loop
		while (buff[0] != '\n' && (cv::getTickCount() - start_time) / cv::getTickFrequency() < 1.0)
		{
			if (_com.read(buff, 1) > 0)
			{
				rx_str = rx_str + buff[0];
				readData = true;
			}
		}

		size_t pos = rx_str.find(" \n"); // Find the position of " \n"

		size_t last_space = rx_str.rfind(' ', pos - 1); // Find last space before " \n"

		std::string recieved = rx_str.substr(last_space + 1, pos - last_space - 1);  // Extract full number before " \n"
		result = stoi(recieved);

		return readData;
	}
	else 
	{
		std::cout << "It's not connected!" << std::endl; //Will return once the _com isnt open
		return false;
	}
}

bool CControl::set_data(int type, int channel, int val)
{
	std::string cmd = "s " + std::to_string(type) + " " + std::to_string(channel) + " " + std::to_string(val) + "\n";
	if (_com.is_open()) 
	{
		_com.write(cmd.c_str(), cmd.length());
		Sleep(10); // wait for ADC conversion, etc. May not be needed?
		return true;
	}
	else std::cout << "It's not connected!" << std::endl;
	

}

float CControl::get_analog(int channel)
{
	CControl::get_data(ANALOG, channel, value);
	return ((static_cast<float> (value) * 100) / 4096);
}

void CControl::analogTest()
{
	while (!_kbhit()) 
	{
		float x_value = CControl::get_analog(HORZ); // Get raw X data
		int rawX = value; //Converts into x pecentage

		float y_value = CControl::get_analog(VERT); // Get Y Percentage
		int rawY = value; //Converts into y percentage

		std::cout << std::fixed << std::setprecision(1) << "\nANALOG TEST: CH 11 = " << rawX << " (" << x_value << "%) CH 4 = " << rawY << " (" << y_value << "%)" << std::endl;
	}
	_getch();

}

void CControl::digitalTest()
{
	while (!_kbhit())
	{
		CControl::get_data(DIGITAL, BUTTON1, value);
		//Sleep(10);
		std::cout << "\nDIGITAL TEST: CH 33 = " << value;

		if (value == 0)
		{
			CControl::set_data(DIGITAL, RED, 1);
		}
		else CControl::set_data(DIGITAL, RED, 0);
	}
	_getch();
	CControl::set_data(DIGITAL, RED, 0); //Turn of the LED when quit
}

bool CControl::get_button(int BUTTON, int& lastState, double& startTime)
{
	double debounce = cv::getTickFrequency() / 100
		;
	int value = 0;

	if (get_data(DIGITAL, BUTTON, value))
	{
		double time = cv::getTickCount();
		if (value == 1 && lastState == 0) 
		{
			if ((time - startTime) > debounce)
			{
				startTime = time;
				lastState = value;
				return true;
			}
		}
		else
		{
			lastState = value;
			return false;
		}
	}
}

void CControl::buttonTest()
{
	int count = 0;
	int lastState1 = 1;
	double startTime1 = 0.0;

	int lastState2 = 1;
	double startTime2 = 0.0;
	std::cout << "BUTTON TEST: 0" << std::endl;

	while (!_kbhit())
	{
		// Check BUTTON1
		if (get_button(BUTTON1, lastState1, startTime1))
		{
			count++;
			std::cout << "BUTTON TEST: " << count << std::endl;
		}

		// Check BUTTON2
		if (get_button(BUTTON2, lastState2, startTime2))
		{
			count++;
			std::cout << "BUTTON TEST: " << count << std::endl;
		}
	}
	_getch();
}

void CControl::servoTest()
{
	get_data(SERVO, MOTOR, value); // Grabs the previous data

	while (!_kbhit())
	{
		//Continuing where the servo left off
		if (cycle == false)
		{
			for (value; value < 180; value++)
			{
				set_data(SERVO, MOTOR, value);
				std::cout << "SERVO TEST CH 19 POS: " << value << std::endl;
				if (_kbhit())
				{
					break;
				}
			}
			if (value >= 180)
			{
				cycle = true;
			}
		}
		else
		{
			for (value; value > 0; value--)
			{
				set_data(SERVO, MOTOR, value);
				std::cout << "SERVO TEST CH 19 POS: " << value << std::endl;

				if (_kbhit())
				{
					break;
				}
			}

			if (value <= 0)
			{
				cycle = false;
			}
		}
	}
	_getch();

}

void CControl::print_menu()
{
	std::cout << "\n********************";
	std::cout << "\n ELEX 4618 Lab 3 by Ken Malana";
	std::cout << "\n********************";
	std::cout << "\n(1) Analog Test";
	std::cout << "\n(2) Digital Test";
	std::cout << "\n(3) Button Test";
	std::cout << "\n(4) Servo Test";
	std::cout << "\n(0) To exit" << std::endl;
}

///////////////////////////////
CControl::CControl() 
{
	//_com.open("COM3");
}
CControl::~CControl() 
{
	
}