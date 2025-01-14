#ifndef CPRODCUT_H
#define CPRODCUT_H

#include<iostream>
#include<string>

class CProduct
{
public:
	// Getters
	float get_price();
	float get_value();
	int get_quantity();
	std::string get_name();

	// Setters
	void set_price(float price_ip);
	void set_value(float value_ip);
	void set_quantity(int quantity_ip);
	void set_name(std::string name_ip);

private:
	//Private Variables
	std::string product_name;
	float price = 0;
	float value = 0;
	int quantity = 0;

};

#endif CPRODCUT_H
