#pragma once
//#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <sstream>
#include <string>



#include "smartProperty.h"

#include "tinyxml2.h"

using namespace tinyxml2;




// Author Johannes Tiefnig
// CFD TU Graz final project 2018
// Gives basic import and export functionality to XML file format
// Properties can be linked

template <typename T>
class PropertyContainer
{
private:
	std::vector<smartProperty<T>*> props;
public:

	PropertyContainer()
	{
		//props = vector<Property<T>*>();
	}

	void addProperty(smartProperty<T>* np)
	{
		props.push_back(np);
	}

	void createProperty(std::string n)
	{
		props.push_back(new smartProperty<T>(n));
	}

	void createProperty(std::string n, T val)
	{
		auto np = new smartProperty<T>(n);
		(*np) = val;
		props.push_back(np);
	}


	smartProperty<T>& operator[](int i)
	{
		return *(props[i]);
	}

	smartProperty<T>& operator[](std::string n)
	{
		for (int i = 0; i < props.size(); i++)
		{
			if (props[i]->getName().compare(n) == 0)
				return *(props[i]);
		}

	}

	int size()
	{
		return props.size();
	}

	void print()
	{
		for (int i = 0; i < props.size(); i++)
		{
			std::cout << props[i]->getName() << " - " << (double)(*props[i]) << std::endl;
		}
	}

	~PropertyContainer()
	{
		for (int i = 0; i < props.size(); i++)
		{
			delete props[i];
		}
	}



	void LoadFromFile(std::string path)
	{
		XMLDocument doc;

		doc.LoadFile(path.c_str());

		XMLElement* mainElm = doc.FirstChildElement("Parameter");
		if (mainElm == nullptr)
			throw "Load parameter error";


		XMLElement* par = mainElm->FirstChildElement();


		while (NULL != par)
		{

			// Should check for right format here.
			
			T val = par->DoubleAttribute("Value");

			if (par->Name() != NULL)
				this->addProperty(new smartProperty<T>(par->Attribute("Name"), val));

			par = par->NextSiblingElement();
		}

	}



	void SaveToFile(std::string path)
	{
		
		 


		// dox in stack..
		XMLDocument doc;

		XMLElement* mainElm = doc.NewElement("Parameter");
		doc.InsertFirstChild(mainElm);


		// Schreiben der 
		for (int i = 0; i < props.size(); i++)
		{
			XMLElement* valelm = doc.NewElement("Param");
			
			valelm->SetAttribute("Name", props[i]->getName().c_str());
			valelm->SetAttribute("Value", (props[i]->getValue()));

			mainElm->InsertEndChild(valelm);
		}


		// Write file
		doc.SaveFile(path.c_str());

	}


};