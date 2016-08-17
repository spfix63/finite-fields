#pragma once

#include <vector>

#include "Polynomial.h"

class WordMatrix;

/* Modeliuoja tiesini koda */
class LinearCode
{
public:
	/* Perduodame generuojancia kodo matrica */
	LinearCode(WordMatrix *gen);
	~LinearCode(void);

	/* Grazina dualaus kodo generuojancia matrica */
	WordMatrix *getCheckMatrix();

	/* Grazina svoriu pasiskirstyma suskaiciuota tiesinio perrinkimo budu */
	Polynomial getWeightDistribution();
	/* Grazina svoriu pasiskirstyma suskaiciuota naudojantis MacWilliams tapatybe */
	Polynomial getWeightDistributionByMacWilliams();
	/* Grazina visus kodo zodzius */
	std::vector<std::vector<Polynomial> > getCodewords();

private:
	WordMatrix *m_generatingMatrix;
};

