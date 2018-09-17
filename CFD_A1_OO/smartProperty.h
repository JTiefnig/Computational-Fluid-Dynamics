#pragma once

#include <string>



template <class T>
class smartProperty 
{
public:
	T value;
	std::string name;


	smartProperty();
	smartProperty(std::string n);
	smartProperty(std::string n, T val);
	~smartProperty();


	void operator=(smartProperty* temp);
	T& operator = (T& temp);
};
