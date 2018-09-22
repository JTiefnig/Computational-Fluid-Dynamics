#pragma once
//#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <sstream>
#include <string>



#include "smartProperty.h"

#include "tinyxml2.h"

using namespace tinyxml2;




// Autor Johannes Tiefnig
// CFD TU Graz final project 2018
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

			std::stringstream valss;

			if (par->GetText() != NULL)
				valss << par->GetText();
			T val;
			valss >> val;

			if (par->Name() != NULL)
				this->addProperty(new smartProperty<T>(par->Name(), val));

			par = par->NextSiblingElement();
		}

	}


	void SaveToFile(std::string path)
	{
		XMLDocument doc;

		XMLElement* mainElm = doc.NewElement("Parameter");
		doc.InsertFirstChild(mainElm);


		for (int i = 0; i < props.size(); i++)
		{
			std::stringstream valss;
			valss << (T)props[i]->getValue();
			XMLElement* valelm = doc.NewElement(props[i]->getName().c_str());


			valelm->SetText(valss.str().c_str());
			mainElm->InsertEndChild(valelm);
		}


		doc.SaveFile(path.c_str());

	}


};