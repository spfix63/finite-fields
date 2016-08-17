#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "WordMatrix.h"
#include "FiniteField.h"
#include "Util.h"


WordMatrix::WordMatrix(int rows, int columns, int base)
	: m_rows(rows), m_columns(columns), m_base(base)
{
	/* Surandam pagrindo pirmine dali ir laipsni */
	if (Util::is_prime(m_base))
	{
		m_ellen = 1;
		m_primebase = m_base;
	}
	else
	{
		int i;
		for (i = 0; i < 24; i++)
		{
			if (Util::is_prime(i))
			{
				float exponent = Util::logn(i, m_base);
				if (floor(exponent) == exponent) //if integer
				{
					m_ellen = exponent;
					m_primebase = i;
					break;
				}
			}
		}
		if (i == 24)
			throw std::invalid_argument("The finite field doesn't exist. Range is p=2..23, m >= 1, where q = p^m");
	}
	
	m_matrix.resize(m_rows);
	for (int i = 0; i < m_rows; i++)
	{
		m_matrix[i].resize(m_columns);
		for (int j = 0; j < m_columns; j++)
		{
			m_matrix[i][j].resize(m_ellen);
		}
	}
}

WordMatrix::~WordMatrix(void)
{
	
}

void WordMatrix::set(int rowindex, int columnindex, int value)
{
	m_matrix[rowindex][columnindex][0] = value;

}

void WordMatrix::set(int rowindex, int columnindex, Polynomial &el)
{
	for (int i = 0; i < el.size(); i++)
	{
		m_matrix[rowindex][columnindex][i] = el[i] % m_primebase;
	}
}

void WordMatrix::set(int rowindex, int columnindex, std::string &el)
{
	std::vector<std::string> params;
	Util::split(el, ' ', params);
	if (params.size() < m_ellen)
		throw std::invalid_argument("Element isnt part of the field");

	for (int i = 0; i < m_ellen; i++)
	{
		m_matrix[rowindex][columnindex][i] = std::stoi(params[i]) % m_primebase;
	}
	
	//std::cout << "[" << rowindex << "," << columnindex << "] = " << el[0] << std::endl;
}

Polynomial WordMatrix::get(int rowindex, int columnindex)
{
	Polynomial &res = m_matrix[rowindex][columnindex];
	return res;
}

int WordMatrix::getRowCount()
{
	return m_rows;
}

int WordMatrix::getColumnCount()
{
	return m_columns;
}

int WordMatrix::getElementLength()
{
	return m_ellen;
}

int WordMatrix::getElementBase()
{
	return m_primebase;
}

void WordMatrix::randomize()
{
	/* matrica turi buti tiesiskai nepriklausoma
	todel ji generuojama laiptuotu pavidalu:
	1 r r r r..
	0 1 r r r..
	0 0 1 r r..
	*/
	srand(time(NULL));
	for (int i = 0; i < m_rows; i++)
	{
		/* irasom nulinius elementus eilutes pradzioje */
		for (int j = 0; j < i; j++)
		{
			for (int k = 0; k < m_ellen; k++)
			{
				m_matrix[i][j][k] = 0;
			}
		}
		
		/* irasom vieneta */
		m_matrix[i][i][0] = 1;
		for (int k = 1; k < m_ellen; k++)
		{
			m_matrix[i][i][k] = 0;
		}
		
		/* irasom likusius elementus atsitiktinai */
		for (int j = i + 1; j < m_columns; j++)
		{
			for (int k = 0; k < m_ellen; k++)
			{
				m_matrix[i][j][k] = rand() % m_primebase;
			}
		}
	}
}

void WordMatrix::standardize()
{
	//http://www.maths.manchester.ac.uk/~pas/code/notes/part6.pdf page2
	for (int j = 0; j < m_rows; j++)
	{
		//step1
		if (FiniteField::isZero(get(j, j)))
		{
			int i = j + 1;
			for (; i < m_rows; i++)
			{
				if (!FiniteField::isZero(get(i, j)))
				{
					switchRows(i, j);
					break;
				}
			}
			if (i == m_rows)
			{
				for (int h = j + 1; h < m_columns; h++)
				{
					if (!FiniteField::isZero(get(j, h)))
					{
						switchColumns(j, h);
						break;
					}
				}
			}
		}
		
		//step2
		FiniteField ff(m_primebase, m_ellen);
		if (!FiniteField::isOne(get(j, j)))
		{
			multiplyRow(j, ff.inverse(get(j, j)) );
		}

		//step3
		for (int i = 0; i < m_rows; i++)
		{
			if (i == j)
				continue;

			addPremultipliedRow(i, j, ff.negate(get(i, j)));
			/*while (!FiniteField::isZero(get(i, j)))
			{
				addRow(i, j);
			}*/
		}
	}

	//print();
}

void WordMatrix::switchRows(int r1, int r2)
{
	std::vector<Polynomial > row = m_matrix[r1];
	m_matrix[r1] = m_matrix[r2];
	m_matrix[r2] = row;
}

void WordMatrix::switchColumns(int c1, int c2)
{
	for (int i = 0; i < m_rows; i++)
	{
		Polynomial temp = m_matrix[i][c1];
		m_matrix[i][c1] = m_matrix[i][c2];
		m_matrix[i][c2] = temp;
	}
}

void WordMatrix::multiplyRow(int r, Polynomial &el)
{
	FiniteField ff(m_primebase, m_ellen);
	for (int i = 0; i < m_columns; i++)
	{
		m_matrix[r][i] = ff.mul(m_matrix[r][i], el);
	}
}

void WordMatrix::addRow(int destRow, int sourceRow)
{
	FiniteField ff(m_primebase, m_ellen);
	for (int i = 0; i < m_columns; i++)
	{
		m_matrix[destRow][i] = ff.add(m_matrix[destRow][i], m_matrix[sourceRow][i]);
	}
}


void WordMatrix::addPremultipliedRow(int destRow, int sourceRow, Polynomial &multiplier)
{
	FiniteField ff(m_primebase, m_ellen);
	for (int i = 0; i < m_columns; i++)
	{
		m_matrix[destRow][i] = ff.add(m_matrix[destRow][i], ff.mul(m_matrix[sourceRow][i], multiplier));
	}
}


void WordMatrix::print()
{
	for (int i = 0; i < m_rows; i++)
	{
		for (int j = 0; j < m_columns; j++)
		{
			for (int k = 0; k < m_ellen; k++)
			{
				std::cout << m_matrix[i][j][k];
			}
			std::cout << "  ";
		}
		std::cout << std::endl;
	}
	std::cout << "---------------" << std::endl;
	std::cout.flush();
}
