// ELEX 4618 Lab 2.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Ken Malana, A01307120
// January 11, 2025

#include <iostream>
#include <string>
#include "CProduct.h" // Hi git hub test
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
//	CGrocery buff;
//	// Declaring a global Inventory vector to store Product objects
//	//vector<Product> Inventory;
//	//^^^ this needs to be in CGrocery and is PRIVATE
//
//	//Declaring Functions
//	void add_product();
//	void edit_product();
//	void print_product();
//	void delete_product();
//	
//};

//class CGrocery //The CGrocery Class
//{
//public:	//Access Specifier 
//	void print_menu();	//Method/Function Declaration
//	vector<CProduct::Product> get_inventory();
//	void set_inventory(CProduct::Product product_ip);
//	//Needs to hold a private dynamic vector for all of products "vector method size()"
//	//Also a function that can save and load the inventory 
//
//	// Declaring a global Inventory vector to store Product objects
//private:
//	vector<CProduct::Product> Inventory;
//};

//Print Menu Function
void print_menu()
{
	cout << "****************************************\n";
	cout << "ELEX 4618 Grocery System, by Ken Malana\n";
	cout << "****************************************\n";
	cout << "(A)dd Product\n(E)dit Product\n(D)elete Product\n(P)rint Product\n(S)ave Inventory\n(L)oad Inventory\n(Q)uit\nCMD > ";
}


int main()
{
	char command1;
	string command2;

	CGrocery Inventory;	//Create an object of CGrocery
	CProduct Product;	//Create an object of CProduct


	while (true)
	{
		print_menu(); // Calling the print menu fuction 
		do
		{
			cin >> command2;

			if (command2.length() > 1)
			{
				cout << "\nInvalid Input. Please enter exatcly one character: \n\n";
				print_menu();
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
			cout << "Invalid input, try again. \n\n";
		}

	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// Functions Below
//////////////////////////////////////////////////////////////////////////////////

