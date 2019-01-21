#pragma once

#include <string>




// Autor Johannes Tiefnig
// CFD TU Graz final project 2018

class smartProperty 
{
public:
	smartProperty()
	{
		name = "";

	}

	smartProperty(std::string n)
	{
		name = n;

	}

	smartProperty(std::string n, double v)
	{
		value = v;
		name = n;
	}



	virtual ~smartProperty()
	{
		this->unlink();
	}

	virtual double & operator = (const double&f) noexcept
	{
		value = f;
		this->PropagateChange();
		return value;
	}

	void operator = (smartProperty& chain) noexcept
	{
		this->link(chain);
	}



	virtual operator double const & () const
	{
		return this->getValue();
	}


	friend double operator +(const smartProperty& a, const smartProperty& b)
	{
		return a.getValue() + b.getValue();
	}

	friend double operator *(const smartProperty& a, const smartProperty& b)
	{
		return a.getValue() * b.getValue();
	}

	friend double operator /(const smartProperty& a, const smartProperty& b)
	{
		return a.getValue() / b.getValue();
	}


	


	virtual double getValue() const
	{
		return this->value;
	}

	std::string getName() const
	{
		return this->name;
	}

	void setName(std::string n)
	{
		name = n;
	}


	void link(smartProperty& chain)
	{
		if (lowerLink != nullptr || upperLink != nullptr)
			this->unlink();

		this->value = chain.value; // Value übertragen


								   // in Kette von properties einfädeln 
		this->lowerLink = chain.lowerLink;
		this->upperLink = &chain;
		if (chain.lowerLink != nullptr)
			chain.lowerLink->upperLink = this;

		chain.lowerLink = this;
	}


	void unlink()
	{
		if (lowerLink != nullptr)
			lowerLink->upperLink = this->upperLink;

		if (upperLink != nullptr)
			upperLink->lowerLink = this->lowerLink;

		this->upperLink = nullptr;
		this->lowerLink = nullptr;

	}

	void PropagateUp()
	{
		if (upperLink != nullptr)
		{
			upperLink->setValue(value);
			upperLink->PropagateUp();
		}
	}


	void PropagateDown()
	{
		if (lowerLink != nullptr)
		{
			lowerLink->setValue(value);
			lowerLink->PropagateDown();
		}
	}


	void PropagateChange()
	{
		this->PropagateDown();
		this->PropagateUp();
	}

	void setValue(double v)
	{
		value = v;
	}



	friend double operator -(const smartProperty& a, const smartProperty& b)
	{
		return a.getValue() - b.getValue();
	}

protected:

	double value;
	std::string name;

	mutable smartProperty* upperLink = nullptr;
	mutable smartProperty* lowerLink = nullptr;
private:

	smartProperty(const smartProperty & temp) {}
};


