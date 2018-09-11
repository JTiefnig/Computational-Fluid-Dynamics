#include "stdafx.h"
#include "smartProperty.h"



template<class T>
smartProperty<T>::smartProperty()
{
}

template<class T>
smartProperty<T>::~smartProperty()
{
}

template <class T>
smartProperty<T>::smartProperty(std::string n)
	:name(n)
{

}

template <class T>
smartProperty<T>::smartProperty(std::string n, T val)
	:name(n), value(val)
{
	
}

template <class T>
void smartProperty<T>::operator= (smartProperty* temp)
{

}

template <class T>
T& smartProperty<T>::operator= (T& temp)
{

	return this->value;
}