#include "Polynomial.h"

#include <cmath>


Polynomial::Polynomial()
{
}

Polynomial::Polynomial(int size)
{
	m_poly.resize(size, 0);
}


Polynomial::~Polynomial(void)
{
}

int Polynomial::size()
{
	return m_poly.size();
}

void Polynomial::resize(int n)
{
	m_poly.resize(n, 0);
}

int &Polynomial::operator[](int n)
{
	return m_poly[n];
}

const int &Polynomial::operator[] (int n) const
{
	return m_poly[n];
}

std::string Polynomial::toString()
{
	std::string res;
	for (int i = 0; i < m_poly.size(); i++)
	{
		if (m_poly[i])
		{
			if (i == 0)
				res += std::to_string(m_poly[i]);
			else
			{
				res += " + " + std::to_string(m_poly[i]) + "x^" + std::to_string(i);
			}
		}
	}
	return res;
}

Polynomial Polynomial::add(Polynomial &left, Polynomial &right)
{
	//Sumos polinomo dydis yra toks pat kaip didesnio is sudedamuju
	int sz = std::max(left.size(), right.size());
	Polynomial res(sz); 
	int l, r;
	
	//Atskirai sudedam visus polinomo narius
	for (int i = 0; i < sz; i++)
	{
		l = i < left.size() ? left[i] : 0;
		r = i < right.size() ? right[i] : 0;
		res[i] = l + r;
	}
	
	return res;
}

Polynomial Polynomial::mul(Polynomial &left, Polynomial &right)
{
	//Sandaugos polinomo maksimalus dydis yra dauginamuju dydziu suma
	Polynomial res(right.size() + left.size() - 1);
	for (int i = 0; i < right.size(); i++)
	{
		for (int j = 0; j < left.size(); j++)
		{
			int newExp = i + j;
			res[newExp] = (res[newExp] + left[j]*right[i]);
		}
	}
	return res;
}

Polynomial Polynomial::mul(Polynomial &el, int value)
{
	Polynomial res(el.size());
	for (int i = 0; i < el.size(); i++)
	{
		res[i] = el[i]*value;
	}
	
	//std::cout << left[0] << "+" << right[0] << " = " << res[0] << std::endl;
	return res;
}

Polynomial Polynomial::pow(Polynomial &el, int power)
{
	//Kelimas laipsniu igyvendinamas daugyba is saves
	Polynomial res(1);
	res[0] = 1;
	for (int i = 0; i < power; i++)
		res = mul(res, el);
	return res;
}