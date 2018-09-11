#pragma once

#include <vector>
#include <string>

#include "smartProperty.h"


template<class T>
class propertyContainer
{
private:
	std::vector<smartProperty<T>*> props;
public:
	propertyContainer();
	~propertyContainer();

	smartProperty<T>* getProperty(std::string name);

	smartProperty<T>& operator[](std::string name);

	smartProperty<T>& operator[](int i);
	int size();	

	void loadPropertiesFromFile(std::string path);
	void savePropertiesToFile(std::string path);

};

