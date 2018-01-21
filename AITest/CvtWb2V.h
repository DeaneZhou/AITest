#pragma once
#ifndef _TEST_CVTWB2V
#define _TEST_CVTWB2V
#include <windows.h>
#include <vector>
#include <iostream>

using namespace std;

class CvtWb2V
{
public:
	CvtWb2V(LPCSTR filename);
	~CvtWb2V();
	int get_ndim();
	int get_vectorSize();
	int get_magicWord();
	vector<float>* get_vector();

private:
	vector<float>* loadBinaryData(LPCSTR filename);
	FILE *pFile;
	int ndim;
	int *dlist;
	int magic_word;
	vector<float>* rslt;
};
#endif
