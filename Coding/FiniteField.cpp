#include "FiniteField.h"
#include <iostream>

std::map<std::pair<int, int>, Polynomial > FiniteField::s_polyTable;

FiniteField::FiniteField(int prime, int exp)
	: m_prime(prime), m_exp(exp)
{
}

FiniteField::~FiniteField(void)
{
}

Polynomial FiniteField::add(Polynomial &left, Polynomial &right)
{
	Polynomial res(left.size()); 
	for (int i = 0; i < left.size(); i++)
	{
		res[i] = (left[i] + right[i]) % m_prime;
	}
	
	//std::cout << left[0] << "+" << right[0] << " = " << res[0] << std::endl;
	return res;
}

Polynomial FiniteField::mul(Polynomial &left, Polynomial &right)
{
	/*
		Iprasta polinomu daugyba, bet tarpiniai skaiciai
		modinami is pirmines pagrindo dalies.
	*/
	Polynomial temp(m_exp + m_exp - 1);
	for (int i = 0; i < right.size(); i++)
	{
		for (int j = 0; j < left.size(); j++)
		{
			int newExp = i + j;
			temp[newExp] = (temp[newExp] + left[j]*right[i]) % m_prime;
		}
	}
	
	/*
		Atliekama mod operacija is neredukuojamo polinomo
		tokiu prinpu:                
          11111101111110 (mod) 100011011
         ^100011011     
           1110000011110
          ^100011011    
            110110101110
           ^100011011   
             10101110110
            ^100011011  
              0100011010
              ^100011011 
                00000001
	*/
	if (m_exp != 1)
	{
		Polynomial &modulo = s_polyTable[std::make_pair(m_prime, m_exp)];
		for (int i = temp.size() - modulo.size(); i >= 0; i--)
		{
			while (temp[i + modulo.size() - 1] != 0)
			{
				for (int j = modulo.size() - 1; j >= 0; j--)
				{
					temp[i + j] = temp[i + j] - modulo[j];
					if (temp[i + j] < 0)
						temp[i + j] += m_prime;
				}
			}
		}
		temp.resize(m_exp);
	}
	
	//std::cout << "(" << left.toString() << ") * (" << right.toString() << ") = " << temp.toString() << std::endl;
	
	return temp;
}


Polynomial FiniteField::inverse(Polynomial &el)
{
	Polynomial inv(m_exp);
	/* Tikrinam visus galimus kuno narius, kol randome toki, 
	is kurio padauginus el gautume 1*/
	bool overflow = false;
	while (!isOne(mul(el, inv)) && !overflow)
		overflow = increment(inv);
	if (overflow)
		throw(std::invalid_argument("No inverse found"));
	return inv;
}


Polynomial FiniteField::negate(Polynomial &el)
{
	/* randame toki polinoma, kad el + negate(el) == 0 */
	Polynomial neg(m_exp);
	for (int i = 0; i < m_exp; i++)
	{
		neg[i] = m_prime - el[i];
	}
	
	return neg;
}


bool FiniteField::increment(Polynomial &el)
{
	//std::cout << el[0] << "++";
	/* Didinam maziausio laipsnio komponenta. Jeigu sis patampa 0, 
	didinam sekancio laipsnio komponenta, jeigu nepatampa griztam. 
	*/
	bool carry = false;
	int indexToIncrement = 0;
	while (indexToIncrement < el.size())
	{
		el[indexToIncrement]++;
		if (el[indexToIncrement] % m_prime)
			break;
		el[indexToIncrement] = 0;
		indexToIncrement++;
	}
	if (indexToIncrement == el.size())
		carry = true;
	
	//std::cout << " = " << el[0] << " ^ " << carry << std::endl;
	return carry;
}

int FiniteField::getElementCount()
{
	return pow(m_prime, m_exp);
}


void FiniteField::setPolynomial(int prime, int degree, Polynomial &poly)
{
	s_polyTable[std::make_pair(prime, degree)] = poly;
}

void FiniteField::setPolynomial(int prime, int degree, int *poly)
{
	/*paverciam poly i polinoma ir issaugom*/
	Polynomial vp(degree + 1);
	for (int i = 0; i <= degree; i++)
		vp[i] = poly[i];
	s_polyTable[std::make_pair(prime, degree)] = vp;
}


bool FiniteField::isZero(Polynomial &el)
{
	/* Jeigu bent viena komponente nera lygi 0, 
	tai ir elementas nelygus 0 */
	bool zero = true;
	for (int i = 0; i < el.size(); i++)
	{
		if (el[i] != 0) 
		{
			zero = false; 
			break;
		}
	}
	return zero;
}


bool FiniteField::isOne(Polynomial &el)
{
	/* Pirma komponente turi buti lygi 1, visos kitos 0 */
	if (el[0] != 1)
		return false;
	
	bool one = true;
	for (int i = 1; i < el.size(); i++)
	{
		if (el[i]) 
		{
			one = false; 
			break;
		}
	}
	return one;
}