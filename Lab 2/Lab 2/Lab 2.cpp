// ELEX 4618 Lab 2.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Ken Malana, A01307120
// January 11, 2025

#include <iostream>
#include <string>
#include "CProduct.h" 
#include "CGrocery.h"


//BETTER VERSION!
int main()
{
	char command1;
	std::string command2;

	CGrocery Inventory;	//Create an object of CGrocery
	CProduct Product;	//Create an object of CProduct


	while (true)
	{
		Inventory.print_menu(); // Calling the print menu fuction 
		do
		{
			//std::cin >> command2;
			std::getline(std::cin, command2);
			if (command2.length() > 1)
			{
				std::cout << "\nInvalid Input. Please enter exatcly one character: \n\n";
				Inventory.print_menu();
			}

		} while (command2.length() != 1);

		command1 = command2[0];

		if (command1 == 'A' || command1 == 'a')
		{
			Inventory.add_product();
		}
		else if (command1 == 'E' || command1 == 'e')
		{
			Inventory.edit_product();
		}
		else if (command1 == 'P' || command1 == 'p')
		{
			Inventory.print_product();
		}
		else if (command1 == 'd' || command1 == 'D')
		{
			Inventory.delete_product();
		}
		else if (command1 == 'Q' || command1 == 'q')
		{
			break;
		}
		else
		{
			std::cout << "Invalid input, try again. \n\n";
		}

	}
	return 0;
}