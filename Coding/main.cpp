#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Util.h"
#include "WordMatrix.h"
#include "FiniteField.h"
#include "LinearCode.h"
#include "Polynomial.h"

//Globalus steko kintamieji, kuriuose saugoma vartotojo ivestis
int method, q, n, k;
WordMatrix *wm;

/* 
parseParams - nuskaito parametrus is perduoto input filestream
input:
- filestream'as is kurio bus skaitoma
output: 
 uzpildyti kintamieji: method, q, n, k.
- true: pavyko nuskaityti
- false: nepavyko nuskaityti
*/
bool parseParams(std::ifstream &);

/* 
parseMatrix - nuskaito generuojancia matrica is perduoto input filestream
input:
- filestream'as is kurio bus skaitoma
output: 
 uzpildytas kintamasis: wm
- true: pavyko nuskaityti
- false: nepavyko nuskaityti
*/
bool parseMatrix(std::ifstream &);

/* 
setupFiniteFields - nustatu baigtiniu lauku naudojamu polinomu sarasa
input:
output: 
*/
void setupFiniteFields();

/*
argv[1] - input failo vardas
*/
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: Coding.exe input.txt" << std::endl;
		return -1;
	}
	
	char *fn = argv[1];
	
	std::ifstream file(fn);
	if (!file.is_open())
	{
		std::cout << "Unable to open file" << std::endl; 
		return -1;
	}
	
	if (!parseParams(file))
	{
		std::cout << "Bad file format. First line should be: q n k" << std::endl; 
		file.close();
		return -1;
	}

	if (!parseMatrix(file))
	{
		std::cout << "Bad file format. Bad matrix" << std::endl; 
		return -1;
	}
	file.close();

	setupFiniteFields();

	/*FiniteField ff(2, 3);
	Polynomial a(3);
	Polynomial b(3);
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			ff.mul(a, b);
			ff.increment(b);
		}
		ff.increment(a);
	}
	return 1;*/

	std::cout << "Praktine uzduotis B1. Svoriu skirstinio radimas." << std::endl;
	std::cout << "Autorius: Jonas Incius." << std::endl << std::endl;
	std::cout << "Ivestis: q=" << q << " n=" << n << " k=" << k << std::endl;
	std::cout << "Matrica: " << std::endl;
	wm->print();
	std::cout << "Metodas: " << (method ? "MacWilliams" : "Tiesinis") << std::endl;
	std::cout << "Rezultatas: " << std::endl;

	LinearCode lc(wm);
	//Jeigu metodas == 0 gauname svori tiesiniu budu, jei ne - naudojam MacWilliams tapatybe
	if (method == 0)
	{
		auto wd = lc.getWeightDistribution();
		for (int k = 0; k < wd.size(); k++)
			std::cout << "w[" << k << "] = " << wd[k] << std::endl;
		std::cout.flush();
	}
	else
	{
		auto wd = lc.getWeightDistributionByMacWilliams();
		for (int k = 0; k < wd.size(); k++)
			std::cout << "w[" << k << "] = " << wd[k] << std::endl;
		std::cout.flush();
	}
	delete wm;
}

bool parseParams(std::ifstream &file)
{
	std::string line;
	std::getline(file, line);

	std::vector<std::string> params;
	Util::split(line, ' ', params);

	if (params.size() != 4)
		return false;
	
	try
	{
		method = std::stoi(params[0]);
		q = std::stoi(params[1]);
		n = std::stoi(params[2]);
		k = std::stoi(params[3]);
	}
	catch (std::exception e) 
	{
		return false;
	};

	return true;
}

bool parseMatrix(std::ifstream &file)
{
	bool randomize = false;
	std::string line;
	
	try
	{
		//Read first line of G matrix. If it's not there, randomize the matrix, 
		//else read the rest
		if (!std::getline(file, line))
			randomize = true;
		wm = new WordMatrix(k, n, q);
	
		if (randomize)
		{
			wm->randomize();
		}
		else
		{
			std::vector<std::string> params;
			for (int i = 0; i < k; i++)
			{
				if (!file) //ended prematurely
					return false;
				Util::split(line, ',', params);
				if (params.size() < n)
					return false;

				for (int j = 0; j < n; j++)
				{
					wm->set(i, j, params[j]);
				}
				std::getline(file, line);
			}


		}

	}
	catch (std::exception e)
	{
		return false;
	}

	
	return true;
}

void setupFiniteFields()
{
	//Nustatome baigtiniu kunu polinomus.
	{ int a[] = {1,1,1}; FiniteField::setPolynomial(2, 2, a); }
	{ int a[] = {1,1,0,1}; FiniteField::setPolynomial(2, 3, a); } //Pakeisti atgal i 1101
	{ int a[] = {1,1,0,0,1}; FiniteField::setPolynomial(2, 4, a); }
	{ int a[] = {1,0,1,0,0,1}; FiniteField::setPolynomial(2, 5, a); }
	{ int a[] = {1,1,0,0,0,0,1}; FiniteField::setPolynomial(2, 6, a); }
	{ int a[] = {1,1,0,0,0,0,0,1}; FiniteField::setPolynomial(2, 7, a); }
	{ int a[] = {1,0,1,1,1,0,0,0,1}; FiniteField::setPolynomial(2, 8, a); }
	{ int a[] = {1,0,0,0,1,0,0,0,0,1}; FiniteField::setPolynomial(2, 9, a); }
	{ int a[] = {1,0,0,1,0,0,0,0,0,0,1}; FiniteField::setPolynomial(2, 10, a); }
	{ int a[] = {1,0,1,0,0,0,0,0,0,0,0,1}; FiniteField::setPolynomial(2, 11, a); }
	{ int a[] = {1,1,0,0,1,0,1,0,0,0,0,0,1}; FiniteField::setPolynomial(2, 12, a); }
	{ int a[] = {1,1,0,1,1,0,0,0,0,0,0,0,0,1}; FiniteField::setPolynomial(2, 13, a); }
	{ int a[] = {1,1,0,1,0,1,0,0,0,0,0,0,0,0,1}; FiniteField::setPolynomial(2, 14, a); }
	{ int a[] = {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1}; FiniteField::setPolynomial(2, 15, a); }
	{ int a[] = {1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,1}; FiniteField::setPolynomial(2, 16, a); }
	{ int a[] = {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1}; FiniteField::setPolynomial(2, 17, a); }
	{ int a[] = {1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1}; FiniteField::setPolynomial(2, 18, a); }
	{ int a[] = {1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1}; FiniteField::setPolynomial(2, 19, a); }
	{ int a[] = {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}; FiniteField::setPolynomial(2, 20, a); }
	{ int a[] = {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}; FiniteField::setPolynomial(2, 21, a); }
	{ int a[] = {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}; FiniteField::setPolynomial(2, 22, a); }
	{ int a[] = {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}; FiniteField::setPolynomial(2, 23, a); }
	{ int a[] = {1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}; FiniteField::setPolynomial(2, 24, a); }
	{ int a[] = {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}; FiniteField::setPolynomial(2, 25, a); }
	{ int a[] = {1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}; FiniteField::setPolynomial(2, 26, a); }
	{ int a[] = {1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}; FiniteField::setPolynomial(2, 27, a); }
	{ int a[] = {1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}; FiniteField::setPolynomial(2, 28, a); }
	{ int a[] = {1,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}; FiniteField::setPolynomial(2, 29, a); }
	{ int a[] = {1,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}; FiniteField::setPolynomial(2, 30, a); }

	
	{ int a[] = {2,1,1}; FiniteField::setPolynomial(3, 2, a); }
	{ int a[] = {1,0,2,1}; FiniteField::setPolynomial(3, 3, a); }
	{ int a[] = {2,0,0,1,1}; FiniteField::setPolynomial(3, 4, a); }
	{ int a[] = {1,0,1,0,1,1}; FiniteField::setPolynomial(3, 5, a); }
	{ int a[] = {2,0,0,0,0,1,1}; FiniteField::setPolynomial(3, 6, a); }
	{ int a[] = {1,1,0,1,0,0,0,1}; FiniteField::setPolynomial(3, 7, a); }
	{ int a[] = {1,0,0,1,0,0,0,0,2}; FiniteField::setPolynomial(3, 8, a); }
	{ int a[] = {1,0,0,0,0,1,0,1,0,1}; FiniteField::setPolynomial(3, 9, a); }
	{ int a[] = {2,0,0,0,0,0,0,1,0,1,1}; FiniteField::setPolynomial(3, 10, a); }
	{ int a[] = {1,0,0,0,0,0,0,0,0,0,1,1}; FiniteField::setPolynomial(3, 11, a); }
	{ int a[] = {2,0,0,0,0,0,0,1,0,0,0,1,1}; FiniteField::setPolynomial(3, 12, a); }
	{ int a[] = {1,0,0,0,0,0,1,0,0,0,0,0,1,1}; FiniteField::setPolynomial(3, 13, a); }
	{ int a[] = {2,0,0,0,0,0,0,0,0,0,0,0,0,1,1}; FiniteField::setPolynomial(3, 14, a); }
	{ int a[] = {1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1}; FiniteField::setPolynomial(3, 15, a); }
	
	{ int a[] = {2,1,1}; FiniteField::setPolynomial(5, 2, a); }
	{ int a[] = {2,0,1,1}; FiniteField::setPolynomial(5, 3, a); }
	{ int a[] = {3,1,0,1,1}; FiniteField::setPolynomial(5, 4, a); }
	{ int a[] = {2,0,0,1,0,1}; FiniteField::setPolynomial(5, 5, a); }
	{ int a[] = {2,0,0,0,0,1,1}; FiniteField::setPolynomial(5, 6, a); }
	
	{ int a[] = {7,1,1}; FiniteField::setPolynomial(7, 2, a); }
	{ int a[] = {2,1,1,1}; FiniteField::setPolynomial(7, 3, a); }
	{ int a[] = {3,0,1,1,1}; FiniteField::setPolynomial(7, 4, a); }
	{ int a[] = {4,0,0,0,1,1}; FiniteField::setPolynomial(7, 5, a); }
	
	{ int a[] = {3,1,1}; FiniteField::setPolynomial(11, 2, a); }
	{ int a[] = {5,0,1,1}; FiniteField::setPolynomial(11, 3, a); }
	{ int a[] = {2,1,0,0,1}; FiniteField::setPolynomial(11, 4, a); }
	
	{ int a[] = {2,1,1}; FiniteField::setPolynomial(13, 2, a); }
	{ int a[] = {7,10,0,1}; FiniteField::setPolynomial(13, 3, a); }
	
	{ int a[] = {10,1,1}; FiniteField::setPolynomial(17, 2, a); }
	{ int a[] = {2,1,1}; FiniteField::setPolynomial(19, 2, a); }
	{ int a[] = {19,22,1}; FiniteField::setPolynomial(23, 2, a); }
}


