#ifndef CGROCERY_H
#define CGROCERY_H

#include "CProduct.h"
#include <vector>


class CGrocery //The CGrocery Class
{
public:	//Access Specifier 
	//Declaring Functions
	void add_product();
	void edit_product();
	void print_product();
	void delete_product();

	bool validInput = false;
	// "vector<CProduct::Product>" is a type and "get_inventory()" is the GETTER function
	//std::vector<CProduct> get_inventory();

	// "set_inventory" is my SETTER function. Inside the brackets are what i want to set. 
	// "CProduct::Product product_ip" => Which class its in :: the struct 'Prodcut' and "product_ip" is the variable name (product input)
	//void set_inventory(CProduct::Product product_ip);
	
	// Declaring a global Inventory vector to store Product objects
private:
	std::vector<CProduct> Inventory; //The data type is in CProduct Class, with the Structure Product
};

#endif CGROCERY_H

