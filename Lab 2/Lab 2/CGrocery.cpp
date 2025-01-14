#include <iostream>
#include <string>
#include <iomanip> //setting the decimal places
#include <regex>	//Patterns
#include <vector>	//Vectors
#include <fstream> //File Streams
#include "CGrocery.h"


//Add Product Function 
void CGrocery::add_product() // need to figure out a way to put it in an array with a max total of 100 (size)
{
	using namespace std;
	CProduct NewItem; //Calling Product Struct to add a "NewItem"
	string product_name;

	//Letting user input the name of the Item
	cout << "\nAdding Product\nProduct Name: ";
	cin >> product_name;
	NewItem.set_name(product_name); //"product_name" is getting passed by to SETTER function "set_name"
	cout << "\n";

	//Need to add a function where it checks if the item that the user typed out is already in the inventory
	//Need to do a for loop and check i < Inventory[i].size();
	//If the user input is == Inventory[i].product_name then error message, input another name.
	//You can display the name that was originally in the inventory 
	//Might have to convert to either lower case or upper case so the string is exactly the same

	//Adds the NewItem to the global Inventory Vector
	Inventory.push_back(NewItem);
	
	// Testing line
	//cout << Inventory[0].get_name();
}

//Edit Product Function
void CGrocery::edit_product() //need to work on this function
{
	using namespace std;
	regex trueInteger("[0-9]+");
	regex truePrice("^[0-9]+(\\.[0-9]+)?$");
	//vector<CProduct::Product> Inventory_copy = buff.get_inventory();

	int item; //Variable for what item the user wants to edit
	string tempItem; //Will be used to check if user put an invalid item
	string tempQuantity; //Will be used to check if user put an invalid quantity
	string tempPrice; //Will be used to check if user put an invalid price


	std::cout << "\nProduct to Edit: ";
	cin >> tempItem;

	do //Checks if the input is correct for when choosing what number to edit
	{
		if (regex_match(tempItem, trueInteger) == false)
		{
			std::cout << "Invalid Input, please put a postive whole number to choose what item to edit: ";
			cin >> tempItem;
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
		cin >> tempItem;

		while (!regex_match(tempItem, trueInteger))
		{
			std::cout << "\nInvalid input, please put only positive integers: ";
			cin >> tempItem;
		}
		item = stoi(tempItem);
	}

	int index = item - 1; //Variable to match the index inside the memory
	std::cout << "\nProduct Name: " << Inventory[index].get_name();

	validInput = false; //Must make it false for the Do-While Loop again
	std::cout << "\nProduct Price : ";
	cin >> tempPrice;

	do //Checks if the input is correct for the price
	{
		if (regex_match(tempPrice, truePrice) == false)
		{
			std::cout << "Invalid price, please put a valid price: ";
			cin >> tempPrice;
		}
		else
		{
			Inventory[index].set_price(stof(tempPrice)); //"Store Float" converts string into a float
			validInput = true;
		}

	} while (!validInput);

	validInput = false; //Must make it false for the Do-While Loop again
	std::cout << "\nProduct Quantity : ";
	cin >> tempQuantity;

	do //Checks if the input is correct for the quantity
	{
		if (regex_match(tempQuantity, trueInteger) == false) //Checks if input is negative, has a character, or has any fractional part
		{
			std::cout << "Invalid Quantity, please put a correct positive whole number: ";
			cin >> tempQuantity;
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
	using namespace std;
	float total_value = 0; //Will be used at the end to sum the total cost
	cout << "\nProduct     Price     Quantity     Value\n";

	//Make for loop to focus on each items in the inventory
	for (int i = 0; i < Inventory.size(); i++)    //i will indicate which item in inventory will be focusing on at a time
	{
		Inventory[i].set_value(Inventory[i].get_price() * Inventory[i].get_quantity()); //Once item has been identified, make the price
		total_value += Inventory[i].get_value(); //Adds the initial price to the next item and displays the total cost of everything

		//Print it out
		cout << "(" << (i + 1) << ") " << Inventory[i].get_name()
			<< "     $" << fixed << setprecision(2) << Inventory[i].get_price()
			<< "     " << Inventory[i].get_quantity()
			<< "          $" << fixed << setprecision(2) << Inventory[i].get_value() << "\n";
	}
	cout << "Total: $" << fixed << setprecision(2) << total_value << "\n";
	cout << "\n";
}

//Delete Product Function
void CGrocery::delete_product()
{
	CProduct DeleteItem;
	using namespace std;
	string tempItem; //Will be used to check if the user puts a valid item
	int item;
	regex trueInteger("[0-9]+");

	cout << "\nThere are currently: " << Inventory.size() << " item(s) in the inventory.";
	cout << "\nChoose which item to delete: ";
	cin >> tempItem;

	while (!regex_match(tempItem, trueInteger))	//Checks if its a valid input
	{
		cout << "\nInvalid input, please put only positive integers: ";
		cin >> tempItem;
	}

	item = stoi(tempItem); //For when user puts the correct data

	while (item < 1 || item > Inventory.size()) //Checks if input is within the range and is not a negative value
	{
		cout << "\nInvalid item number. Please choose a valid item number (1 - " << Inventory.size() << "): ";
		cin >> tempItem;

		while (!regex_match(tempItem, trueInteger))
		{
			cout << "\nInvalid input, please put only positive integers: ";
			cin >> tempItem;
		}
		item = stoi(tempItem);
	}

	//Subtract 1 to adjust to the index bc its 0-based
	item--;

	//Removing the item
	cout << "\nDeleting Product: " << Inventory[item].get_name() << "\n";
	Inventory.erase(Inventory.begin() + item); //Erasing the element that the 'item' was in
}
