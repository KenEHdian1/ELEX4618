#include <iostream>
#include <string>
#include <iomanip> //setting the decimal places
#include <regex>	//Patterns
#include <vector>	//Vectors
#include <fstream> //File Streams
//#include <nlohmann/json.hpp>
#include "CGrocery.h"

//Print Menu Function
void CGrocery::print_menu()
{
	std::cout << "****************************************\n";
	std::cout << "ELEX 4618 Grocery System, by Ken Malana\n";
	std::cout << "****************************************\n";
	std::cout << "(A)dd Product\n(E)dit Product\n(D)elete Product\n(P)rint Product\n(S)ave Inventory\n(L)oad Inventory\n(Q)uit\nCMD > ";
}

//Add Product Function 
void CGrocery::add_product()
{
		CProduct NewItem; // NewItem is the Object for CProduct
		std::string product_name;
		int quantityTemp0 = 0;
		float priceTemp0 = 0.00;
		bool validInput = false; // To track whether a valid input is received

		std::cout << "\nAdding Product\nProduct Name: ";
		while (!validInput) // Keep prompting until valid input is given
		{
			std::cin >> product_name;

			// Check if the product already exists in the Inventory
			bool exists = false;
			for (int i = 0; i < Inventory.size(); i++)
			{
				// Temporarily lowercase both names for comparison
				std::string existingName = Inventory[i].get_name(); 
				std::string temp_product_name = product_name;		

				std::transform(existingName.begin(), existingName.end(), existingName.begin(), ::tolower);	//lower case the inventory name 
				std::transform(temp_product_name.begin(), temp_product_name.end(), temp_product_name.begin(), ::tolower);	//lower case the user input name

				if (temp_product_name == existingName)
				{
					exists = true;
					std::cout << "\nThe product already exists as: " << Inventory[i].get_name();
					std::cout << "\nPlease enter a different product name: ";
					break;
				}
			}

			if (!exists)
			{
				NewItem.set_name(product_name); // Set the valid product name as entered by the user
				NewItem.set_price(quantityTemp0); //Temporarily setting the price of the added product to 0
				NewItem.set_quantity(priceTemp0); //Temporarily setting the quantity of the added product to 0
				validInput = true; // Break out of the loop
			}
		}

	//Adds the NewItem to the global Inventory Vector
	Inventory.push_back(NewItem);
};

//Edit Product Function
void CGrocery::edit_product() //need to work on this function
{
	std::regex trueInteger("[0-9]+");
	//std::regex truePrice("^[0-9]+?(\\.[0-9]+)?$"); //Patterns like 12 or 12.0
	std::regex truePrice("^([0-9]+\.?[0-9]*|\.?[0-9]+)$"); //Patterns like 12 or 12.0
	std::regex negOnePrice("^-1$"); //For some reason -1 worked so had to make another one
	//std::regex conditionPrice1("R(^[0-9]+\.?$)");	//Patterns that has a decimal at the end

	int item; //Variable for what item the user wants to edit
	std::string tempItem; //Will be used to check if user put an invalid item
	std::string tempQuantity; //Will be used to check if user put an invalid quantity
	std::string tempPrice; //Will be used to check if user put an invalid price

	//Does a check initally if there are nothing in the grocery list
	while (Inventory.size() == 0)
	{
		std::cout << "\nThere are currently nothing in the grocery. Please add an item first.\n\n";
		return;
	}

	//Will let user choose an item within the grocery
	std::cout << "\nPlease choose an item in the grocery (1 - " << Inventory.size() <<"): ";
	std::cin >> tempItem;

	do //Checks if the input is correct for when choosing what number to edit
	{
		if (regex_match(tempItem, trueInteger) == false)
		{
			std::cout << "\nInvalid Input, please put a postive whole number to choose what item to edit: ";
			std::cin >> tempItem;
		}
		else
		{
			item = stoi(tempItem); //"Store Int" converting string into an integer
			validInput = true;
		}
	} while (!validInput);

	while (item < 1 || item > Inventory.size()) //Checks if input is within the range and is not a negative value
	{
		std::cout << "\nInvalid item number. Please choose a valid item number (1 - " << Inventory.size() << "): ";
		std::cin >> tempItem;

		while (!regex_match(tempItem, trueInteger))
		{
			std::cout << "\nInvalid input, please put only positive integers: ";
			std::cin >> tempItem;
		}
		item = stoi(tempItem);
	}



	//Will Show the name of the product that was selected
	int index = item - 1; //Variable to match the index inside the memory
	std::cout << "\nProduct Name: " << Inventory[index].get_name() << "\n";



	//Will let user choose a price for the item
	validInput = false; //Must make it false for the Do-While Loop again
	std::cout << "\nProduct Price : ";
	std::cin >> tempPrice;

	do //Checks if the input is correct for the price
	{
		if (regex_match(tempPrice, truePrice) == false || regex_match(tempPrice, negOnePrice))
		{
			std::cout << "Invalid price, please put a valid price: ";
			std::cin >> tempPrice;
		}
		else
		{
			Inventory[index].set_price(stof(tempPrice)); //"Store Float" converts string into a float
			validInput = true;
		}

	} while (!validInput);



	//Will let user choose a quantity for the item
	validInput = false; //Must make it false for the Do-While Loop again
	std::cout << "\nProduct Quantity : ";
	std::cin >> tempQuantity;

	do //Checks if the input is correct for the quantity
	{
		if (regex_match(tempQuantity, trueInteger) == false) //Checks if input is negative, has a character, or has any fractional part
		{
			std::cout << "Invalid Quantity, please put a correct positive whole number: ";
			std::cin >> tempQuantity;
		}
		else
		{
			Inventory[index].set_quantity(stoi(tempQuantity)); //"Store Int" Converts string into an integer
			validInput = true;
		}
	} while (!validInput);

	std::cout << "\n";
}

//Print Product Function
void CGrocery::print_product()
{
	float total_value = 0; //Will be used at the end to sum the total cost
	std::cout << std::left
		<< std::setw(20) << "\nProduct"
		<< std::setw(10) << "Price"
		<< std::setw(15) << "Quantity"
		<< std::setw(10) << "Value" << std::endl;

	//Make for loop to focus on each items in the inventory
	for (int i = 0; i < Inventory.size(); i++)    //i will indicate which item in inventory will be focusing on at a time
	{
		Inventory[i].set_value(Inventory[i].get_price() * Inventory[i].get_quantity()); //Once item has been identified, make the price
		total_value += Inventory[i].get_value(); //Adds the initial price to the next item and displays the total cost of everything

		//Print it out
		std::cout << "(" << (i + 1) << ") " << std::left << std::setw(15) << Inventory[i].get_name();
		std::cout << "$" << std::left << std::setw(9)  << std::fixed << std::setprecision(2) << Inventory[i].get_price();
		std::cout << std::left << std::setw(15) << Inventory[i].get_quantity();
		std::cout << "$" << std::fixed << std::setprecision(2) << Inventory[i].get_value() << "\n";	
	}
	std::cout << "\n" << std::string(55, '-');
	std::cout << "\nTotal: $" << std::fixed << std::setprecision(2) << total_value << "\n";
	std::cout << "\n";
}

//Delete Product Function
void CGrocery::delete_product()
{
	CProduct DeleteItem;
	std::string tempItem; //Will be used to check if the user puts a valid item
	int item;
	std::regex trueInteger("[0-9]+");

	while (Inventory.size() == 0) //Checks if theres anything in the inventory
	{
		std::cout << "\nThere are currently nothing in the grocery. Please add an item first.\n\n";
		return;
	}

	std::cout << "\nThere are currently: " << Inventory.size() << " item(s) in the inventory.";
	std::cout << "\nChoose which item to delete: ";
	std::cin >> tempItem;

	
	while (!regex_match(tempItem, trueInteger))	//Checks if its a valid input
	{
		std::cout << "\nInvalid input, please put only positive integers: ";
		std::cin >> tempItem;
	}

	item = stoi(tempItem); //For when user puts the correct data

	while (item < 1 || item > Inventory.size()) //Checks if input is within the range and is not a negative value
	{
		std::cout << "\nInvalid item number. Please choose a valid item number (1 - " << Inventory.size() << "): ";
		std::cin >> tempItem;

		while (!regex_match(tempItem, trueInteger))
		{
			std::cout << "\nInvalid input, please put only positive integers: ";
			std::cin >> tempItem;
		}
		item = stoi(tempItem);
	}

	//Subtract 1 to adjust to the index bc its 0-based
	item--;

	//Removing the item
	std::cout << "\nDeleting Product: " << Inventory[item].get_name() << "\n";
	Inventory.erase(Inventory.begin() + item); //Erasing the element that the 'item' was in
}
