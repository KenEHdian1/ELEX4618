// ELEX 4618 Lab 2.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Ken Malana, A01307120
// January 11, 2025

#include <iostream>
#include <string>
#include <iomanip> //setting the decimal places
#include <regex>	//Patterns
#include <vector>	//Vectors
#include <fstream> //File Streams
#include "CProduct.h"
#include "CGrocery.h"




using namespace std;
bool validInput = false;

//class CProduct
//{
//public:
//	// Defining Struct for a product
//	struct Product
//	{
//		string product_name; // Variable for the product
//		float price = 0; // Variable for pricing
//		float value = 0; // Variable for value of the item
//		int quantity = 0; // Variable for total quantity at the end
//	};
//
//	// Declaring a global Inventory vector to store Product objects
//	vector<Product> Inventory;
//	//^^^ this needs to be in CGrocery and is PRIVATE
//
//	//Declaring Functions
//	void add_product();
//	void edit_product();
//	void print_product();
//	void delete_product();
//};
//
//class CGrocery //The CGrocery Class
//{
//public:	//Access Specifier 
//	void print_menu();	//Method/Function Declaration
//
//	//Needs to hold a private dynamic vector for all of products "vector method size()"
//	//Also a function that can save and load the inventory 
//
//	// Declaring a global Inventory vector to store Product objects
//	//vector<CProduct> Inventory;
//};

int main()
{
	char command1;
	string command2;

	CGrocery Inventory;	//Create an object of CGrocery
	CProduct Product;	//Create an object of CProduct


	while (true)
	{
		Inventory.print_menu(); // Calling the print menu fuction 
		do
		{
			cin >> command2;

			if (command2.length() > 1)
			{
				cout << "\nInvalid Input. Please enter exatcly one character: \n\n";
				Inventory.print_menu();
			}

		} while (command2.length() != 1);

		command1 = command2[0];

		if (command1 == 'A' || command1 == 'a')
		{
			Product.add_product();
		}
		else if (command1 == 'E' || command1 == 'e')
		{
			Product.edit_product();
		}
		else if (command1 == 'P' || command1 == 'p')
		{
			Product.print_product();
		}
		else if (command1 == 'd' || command1 == 'D')
		{
			Product.delete_product();
		}
		else if (command1 == 'Q' || command1 == 'q')
		{
			break;
		}
		else
		{
			cout << "Invalid input, try again. \n\n";
		}

	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// Functions Below
//////////////////////////////////////////////////////////////////////////////////

//Print Menu Function
void CGrocery::print_menu()
{
	cout << "****************************************\n";
	cout << "ELEX 4618 Grocery System, by Ken Malana\n";
	cout << "****************************************\n";
	cout << "(A)dd Product\n(E)dit Product\n(D)elete Product\n(P)rint Product\n(S)ave Inventory\n(L)oad Inventory\n(Q)uit\nCMD > ";
}

//Add Product Function 
void CProduct::add_product() // need to figure out a way to put it in an array with a max total of 100 (size)
{
	Product NewItem; //Calling Product Struct to add a "NewItem"

	//Letting user input the name of the Item
	cout << "\nAdding Product\nProduct Name: ";
	cin >> NewItem.product_name;
	cout << "\n";

	//Need to add a function where it checks if the item that the user typed out is already in the inventory
	//Need to do a for loop and check i < Inventory[i].size();
	//If the user input is == Inventory[i].product_name then error message, input another name.
	//You can display the name that was originally in the inventory 
	//Might have to convert to either lower case or upper case so the string is exactly the same

	//Adds the NewItem to the global Inventory Vector
	Inventory.push_back(NewItem);
}

//Edit Product Function
void CProduct::edit_product() //need to work on this function
{
	regex trueInteger("[0-9]+");
	regex truePrice("^[0-9]+(\\.[0-9]+)?$");

	int item; //Variable for what item the user wants to edit
	string tempItem; //Will be used to check if user put an invalid item
	string tempQuantity; //Will be used to check if user put an invalid quantity
	string tempPrice; //Will be used to check if user put an invalid price
	

	cout << "\nProduct to Edit: ";
	cin >> tempItem;

	do //Checks if the input is correct for when choosing what number to edit
	{
		if (regex_match(tempItem, trueInteger) == false)
		{
			cout << "Invalid Input, please put a postive whole number to choose what item to edit: ";
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
		cout << "\nInvalid item number. Please choose a valid item number (1 - " << Inventory.size() << "): ";
		cin >> tempItem;

		while (!regex_match(tempItem, trueInteger))
		{
			cout << "\nInvalid input, please put only positive integers: ";
			cin >> tempItem;
		}
		item = stoi(tempItem);
	}

	int index = item - 1; //Variable to match the index inside the memory
	cout << "\nProduct Name: " << Inventory[index].product_name;
	
	validInput = false; //Must make it false for the Do-While Loop again
	cout << "\nProduct Price : ";
	cin >> tempPrice;

	do //Checks if the input is correct for the price
	{
		if (regex_match(tempPrice, truePrice) == false)
		{
			cout << "Invalid price, please put a valid price: ";
			cin >> tempPrice;
		}
		else
		{
			Inventory[index].price = stof(tempPrice); //"Store Float" converts string into a float
			validInput = true;
		}

	} while (!validInput);

	validInput = false; //Must make it false for the Do-While Loop again
	cout << "\nProduct Quantity : ";
	cin >> tempQuantity;

	do //Checks if the input is correct for the quantity
	{
		if (regex_match(tempQuantity, trueInteger) == false) //Checks if input is negative, has a character, or has any fractional part
		{
			cout << "Invalid Quantity, please put a correct positive whole number: ";
			cin >> tempQuantity;
		}
		else
		{
			Inventory[index].quantity = stoi(tempQuantity); //"Store Int" Converts string into an integer
			validInput = true;
		}
	} while (!validInput);

	cout << "\n";
}

//Print Product Function
void CProduct::print_product()
{
	float total_value = 0; //Will be used at the end to sum the total cost
	cout << "\nProduct     Price     Quantity     Value\n";

	//Make for loop to focus on each items in the inventory
	for (int i = 0; i < Inventory.size(); i++)    //i will indicate which item in inventory will be focusing on at a time
	{
		Inventory[i].value = Inventory[i].price * Inventory[i].quantity; //Once item has been identified, make the price
		total_value += Inventory[i].value; //Adds the initial price to the next item and displays the total cost of everything

		//Print it out
		cout << "(" << (i + 1) << ") " << Inventory[i].product_name
			<< "     $" << fixed << setprecision(2) << Inventory[i].price
			<< "     " << Inventory[i].quantity
			<< "          $" << fixed << setprecision(2) << Inventory[i].value << "\n";
	}
	cout << "Total: $" << fixed << setprecision(2) << total_value << "\n";
	cout << "\n";
}

//Delete Product Function
void CProduct::delete_product()
{
	string tempItem; //Will be used to check if the user puts a valid item
	int item; 
	regex trueInteger("[0-9]+");

	Product DeleteItem; //Calling Product Struct to 'DeleteItem
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
	cout << "\nDeleting Product: " << Inventory[item].product_name << "\n";
	Inventory.erase(Inventory.begin() + item); //Erasing the element that the 'item' was in
}