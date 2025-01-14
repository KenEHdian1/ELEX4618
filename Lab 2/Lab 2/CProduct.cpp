#include<iostream>
#include<string>
#include <vector>
#include "CProduct.h"

// Getters
float CProduct::get_price()
{
	return price;
}
float CProduct::get_value()
{
	return value;
}
int CProduct::get_quantity()
{
	return quantity;
}
std::string CProduct::get_name()
{
	return product_name;
}

// Setters
void CProduct::set_price(float price_ip)
{
	price = price_ip;
}
void CProduct::set_value(float value_ip)
{
	value = value_ip;
}
void CProduct::set_quantity(int quantity_ip)
{
	quantity = quantity_ip;
}
void CProduct::set_name(std::string name_ip)
{
	product_name = name_ip;
}