#include<iostream>
#include<string>
#include <vector>

class CProduct
{
public:
	// Defining Struct for a product
	struct Product
	{
		string product_name; // Variable for the product
		float price = 0; // Variable for pricing
		float value = 0; // Variable for value of the item
		int quantity = 0; // Variable for total quantity at the end
	};

	// Declaring a global Inventory vector to store Product objects
	vector<Product> Inventory;

	//Declaring Functions
	void add_product();
	void edit_product();
	void print_product();
	void delete_product();
};