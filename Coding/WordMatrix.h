#pragma once

#include <vector>

#include "Polynomial.h"

/* Matrica polinomams saugoti. */
class WordMatrix
{
public:
	/* eilutes, stulpeliai ir kuno, kurio elementus saugosim pagrindas */
	WordMatrix(int rows, int columns, int base = 10);
	~WordMatrix(void);
	
	/* elementu irasymas ir gavimas */
	void set(int rowindex, int columnindex, int val);
	void set(int rowindex, int columnindex, Polynomial &el);
	void set(int rowindex, int columnindex, std::string &el);
	Polynomial get(int rowindex, int columnindex);

	void print();

	int getRowCount();
	int getColumnCount();

	/* Grazina elemento komponenciu skaiciu */
	int getElementLength();
	/* Grazina elemento pirmini pagrinda */
	int getElementBase();

	/* Sudaro atsitiktine matrica */
	void randomize();

	/* Suveda i standartini pavidala */
	void standardize();

private:
	/* Pagalbiniai suvedimo i standartini pavidala metodai */
	/* Nurodomi eiluciu/stulpeliu indeksai */
	void switchRows(int r1, int r2);
	void switchColumns(int c1, int c2);
	void multiplyRow(int r, Polynomial &el);
	void addRow(int destRow, int sourceRow);
	void addPremultipliedRow(int destRow, int sourceRow, Polynomial &multiplier);

	std::vector<std::vector<Polynomial> > m_matrix;
	//int ***m_matrix;
	int m_rows; //k
	int m_columns; //n
	int m_base; //q

	int m_ellen; //m where q == p ^ m
	int m_primebase; //p where q == p ^ m
};

