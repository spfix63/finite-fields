#pragma once

#include <vector>
#include <string>

/* Klase modeliuojanti polinomus ir veiksmus su jais. std::vector<int> wrapperis. */
class Polynomial
{
public:
	Polynomial();

	/*size: polinomo nariu skaicius. Pvz: x^2 + x + 1, size = 3*/
	Polynomial(int size);
	~Polynomial(void);

	/*grazina polinomo nariu skaiciu*/
	int size();
	/*pakeicia polinomo nariu skaiciu i n*/
	void resize(int n);

	/*grazina koeficienta prie x^n*/
	int &operator[](int n);
	const int &operator[] (int n) const;

	std::string toString();
	
	/*polinomu sudetis*/
	static Polynomial add(Polynomial &left, Polynomial &right);
	
	/*polinomu daugyba*/
	static Polynomial mul(Polynomial &left, Polynomial &right);
	
	/*polinomo daugyba is skaiciaus*/
	static Polynomial mul(Polynomial &el, int value);

	/*polinomo kelimas laipsniu*/
	static Polynomial pow(Polynomial &el, int power);

private:
	std::vector<int> m_poly;
};

