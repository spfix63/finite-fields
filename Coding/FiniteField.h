#pragma once

#include <vector>
#include <map>

#include "Polynomial.h"

/* Klase modeliuojanti baigtinius kunus */
class FiniteField
{
public:
	/* prime^exp sudaro kuno pagrinda */
	FiniteField(int prime, int exp);
	~FiniteField(void);
	
	/* Polinomu sudetis baigtiniame kune */
	Polynomial add(Polynomial &left, Polynomial &right);
	
	/* Polinomu daugyba baigtiniame kune */
	Polynomial mul(Polynomial &left, Polynomial &right);
	
	/* Grazina el^(-1) baigtiniame kune */
	Polynomial inverse(Polynomial &el);
	
	/* Grazina -el baigtiniame kune */
	Polynomial negate(Polynomial &el);

	/* Padidina el vienetu. Grazina true, jeigu el perzengia kuno ribas ir tampa 0 */
	bool increment(Polynomial &el); //true if carry/overflow, false if not

	/* Grazina kuno elementu skaiciu */
	int getElementCount();
	
	/* 
	Statine dalis skirta darbui su polinomais, 
	kurie naudojami baigtiniams kunams su nepirminiu pagrindu 
	*/
	/* nurodomas pagrindas prime^degree, ir polinomas kuri taikyti esant tokiam kunui */
	static void setPolynomial(int prime, int degree, Polynomial &poly);
	/* nurodomas pagrindas prime^degree, ir polinomas kuri taikyti esant tokiam kunui.
	Polinomas isreikstas int masyvu, kurio dydis nustatomas pagal degree. */
	static void setPolynomial(int prime, int degree, int *poly);
	/* Tikrina ar polinomas lygus 0 */
	static bool isZero(Polynomial &el);
	/* Tikrina ar polinomas lygus 1 */
	static bool isOne(Polynomial &el);

private:
	static std::map<std::pair<int, int>, Polynomial > s_polyTable;

	int m_prime;
	int m_exp;
};

