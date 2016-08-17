#include "LinearCode.h"

#include "Polynomial.h"
#include "WordMatrix.h"
#include "FiniteField.h"


#include <iostream>
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;

LinearCode::LinearCode(WordMatrix *g)
	: m_generatingMatrix(g)
{

}


LinearCode::~LinearCode(void)
{
}


WordMatrix *LinearCode::getCheckMatrix()
{
	m_generatingMatrix->standardize();
	/*
	http://en.wikipedia.org/wiki/Linear_code
	If C is a code with a generating matrix G in standard form, G = (Ik | A), 
	then H = (-At | In - k) is a check matrix for C.
	*/
	/* Skaiciuojam naujos matricos dimensijas */
	int rows = m_generatingMatrix->getColumnCount() - m_generatingMatrix->getRowCount(); 
	int cols = m_generatingMatrix->getColumnCount();
	int base = pow(m_generatingMatrix->getElementBase(), m_generatingMatrix->getElementLength());
	WordMatrix *check = new WordMatrix(rows, cols, base);

	/* Nustatom vienetine matricos dali */
	for (int i = cols - rows, j = 0; i < cols; i++, j++)
	{
		check->set(j, i, 1);
	}

	/* Nustatom transponuota dali ir atliekam neigima */
	FiniteField ff(m_generatingMatrix->getElementBase(), m_generatingMatrix->getElementLength());
	for (int dc = 0, sr = 0;dc < cols - rows;dc++, sr++)
	{
		for (int dr = 0, sc = m_generatingMatrix->getRowCount(); dr < rows;dr++, sc++)
		{	
			check->set(dr, dc, ff.negate(m_generatingMatrix->get(sr, sc)));
		}
	}
	
	return check;
}


Polynomial LinearCode::getWeightDistribution()
{
	Polynomial res(m_generatingMatrix->getColumnCount()+1);
	/* Svorio pasiskirstymas skaiciuojamas generuojant visas tiesines kombinacijas is baigtinio kuno
	elementu ir generuojancios matricos eiluciu, tada didinant atitinkamo svorio skaiciuokli */

	FiniteField ff(m_generatingMatrix->getElementBase(), m_generatingMatrix->getElementLength());
	/* kiek kodo zodziu reikes surasti */
	unsigned long long wordCount = pow(ff.getElementCount(), m_generatingMatrix->getRowCount());
	
	//std::vector<std::vector<Polynomial> > res(wordCount);
	
	/* Apsibreziam baigtinio kuno elementu rinkini, 
	kuri keisim skaiciuodami kiekviena atskira zodi */
	std::vector<Polynomial> counter(m_generatingMatrix->getRowCount());
	for (int i = 0; i < m_generatingMatrix->getRowCount(); i++)
	{
		counter[i].resize(m_generatingMatrix->getElementLength());
	}
	//steady_clock::time_point start = steady_clock::now();
	
	std::vector<Polynomial> word(m_generatingMatrix->getColumnCount());
	for (int i = 0; i < wordCount; i++)
	{
		/* Sudauginam generuojancios matricos eilutes elementus su counter elementais ir sudedam gautus
		vektorius gaudami nauja kodo zodi */
		for (int j = 0; j < m_generatingMatrix->getColumnCount(); j++)
		{
			word[j].resize(m_generatingMatrix->getElementLength());
			for (int l = 0; l < word[j].size(); l++)
				word[j][l] = 0;

			for (int k = 0; k < m_generatingMatrix->getRowCount(); k++)
			{
				word[j] = ff.add(word[j], ff.mul(m_generatingMatrix->get(k, j), counter[k]));
			}
		}

		/*if ((i+1) % 200000 == 0)
		{
			steady_clock::time_point end = steady_clock::now();
			
			float pleft = ((float)i)/wordCount;
			int elapsed = duration_cast<milliseconds>(end - start).count();
			long milli = elapsed / pleft - elapsed;
			std::cout << "minutes: " << milli/1000/60 << " seconds: " << milli/1000%60 << std::endl;
		}*/
		//res[i] = word;

		/*
		Suskaiciuojam zodzio svori
		*/
		int weight = 0;
		for (int j = 0; j < word.size(); j++)
		{
			if (!FiniteField::isZero(word[j]))
				weight++;
		}
		res[weight]++;

		/* Pakeiciam counter sekancio zodzio skaiciavimui. Ciklo metu, counter igaus visas galimas reiksmes */
		int indexToIncrement = counter.size() - 1;
		while (indexToIncrement >= 0 && ff.increment(counter[indexToIncrement]))
			indexToIncrement--;
	}
	
	return res;
	/*auto code = getCodewords();
	for (int i = 0; i < code.size(); i++)
	{
		int weight = 0;
		for (int j = 0; j < code[i].size(); j++)
		{
			if (!FiniteField::isZero(code[i][j]))
				weight++;
		}
		res[weight]++;
	}



	return res;*/
}


Polynomial LinearCode::getWeightDistributionByMacWilliams()
{
	/* 
	Gaunam dualaus kodo generuojancia matrica ir
	tiesiniu budu suskaiciuojam svoriu pasiskirstyma. 
	Tada issprendziam MacWilliams tapatybe:
	wC(x,y) = wC' (x+(q-1)y, x-y) / |C'|.
	*/

	int q, n, k;
	k = m_generatingMatrix->getRowCount();
	n = m_generatingMatrix->getColumnCount();
	q = pow(m_generatingMatrix->getElementBase(), m_generatingMatrix->getElementLength());
	
	WordMatrix *check = getCheckMatrix();
	LinearCode lcd(check);
	Polynomial dualWD = lcd.getWeightDistribution();
	Polynomial rightSide;
	int side = q - 1;
	
	/* skaiciuojam desine nelygybes puse
	wC' (x+(q-1)y, x-y)*/
	for (int i = 0; i <= n; i++)
	{
		Polynomial m1(2);
		m1[0] = 1;
		m1[1] = side;
		m1 = Polynomial::pow(m1, n - i);
		Polynomial m2(2);
		m2[0] = 1;
		m2[1] = -1;
		m2 = Polynomial::pow(m2, i);
		Polynomial tmp = Polynomial::mul(m1, m2);
		tmp = Polynomial::mul(tmp, dualWD[i]);
		rightSide = Polynomial::add(rightSide, tmp);
	}

	//Suskaiciuojam kiek zodziu priklauso kodui ir padalinam desine puse.
	int codeLength = pow(pow(check->getElementBase(), check->getElementLength()), check->getRowCount());
	for (int i = 0; i <= n; i++)
	{
		rightSide[i] = rightSide[i] / codeLength;
	}

	return rightSide;
}

std::vector<std::vector<Polynomial> > LinearCode::getCodewords()
{
	// Tiesiniu budu generuojami visi kodo zodziai. Sis metodas darbe nenaudojamas, taciau jis buvo
	// svoriu skirstinio metodo prekursorius, ir didzioji jo kodo dalis atsispindi ten.
	FiniteField ff(m_generatingMatrix->getElementBase(), m_generatingMatrix->getElementLength());
	int wordCount = pow(ff.getElementCount(), m_generatingMatrix->getRowCount());

	std::vector<std::vector<Polynomial> > res(wordCount);

	std::vector<Polynomial> counter(m_generatingMatrix->getRowCount());
	for (int i = 0; i < m_generatingMatrix->getRowCount(); i++)
	{
		counter[i].resize(m_generatingMatrix->getElementLength());
	}
	steady_clock::time_point start = steady_clock::now();
	
	std::vector<Polynomial> word(m_generatingMatrix->getColumnCount());
	for (int i = 0; i < wordCount; i++)
	{
		for (int j = 0; j < m_generatingMatrix->getColumnCount(); j++)
		{
			word[j].resize(m_generatingMatrix->getElementLength());
			for (int l = 0; l < word[j].size(); l++)
				word[j][l] = 0;

			for (int k = 0; k < m_generatingMatrix->getRowCount(); k++)
			{
				word[j] = ff.add(word[j], ff.mul(counter[k], m_generatingMatrix->get(k, j)));
			}
		}
		res[i] = word;
		int indexToIncrement = counter.size() - 1;
		while (indexToIncrement >= 0 && ff.increment(counter[indexToIncrement]))
			indexToIncrement--;
	}
	
	return res;
}

