#include "stdafx.h"
#include "propertyContainer.h"

template <class T>
propertyContainer<T>::propertyContainer()
{

}


template <class T>
propertyContainer<T>::~propertyContainer()
{
}
template <class T>
smartProperty<T> * propertyContainer<T>::getProperty(std::string name)
{
	const int s = props.size();
	for (int i = 0; i<s; i++)
	{
		if (props[i]->name.compare(name) == 0)
			return *props[i];
	}
	return nullptr; // exception ??
}

template <class T>
int propertyContainer<T>::size()
{
	return props.size();
}

template <class T>
void propertyContainer<T>::loadPropertiesFromFile(std::string path)
{
	// Todo // file read in an xml Serialization
}

template <class T>
void propertyContainer<T>::savePropertiesToFile(std::string path)
{
	// Todo
}


template <class T>
smartProperty<T>& propertyContainer<T>::operator[](std::string name)
{
	return this->getProperty(name);
}

template <class T>
smartProperty<T>& propertyContainer<T>::operator[](int i)
{
	return *(props[i]);
}