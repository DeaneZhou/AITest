#include "CvtWb2V.h"
#include <fstream>
//#include "stdafx.h"


CvtWb2V::CvtWb2V(LPCSTR filename) :magic_word(0), ndim(0), dlist(nullptr), pFile(nullptr), rslt(nullptr)
{
	rslt=loadBinaryData(filename);
}

CvtWb2V::~CvtWb2V()
{
	fclose(pFile);
}

int CvtWb2V::get_ndim()
{
	return ndim;
}

int CvtWb2V::get_vectorSize()
{
	int vs = 1;
	for (int i = 0; i < ndim; i++)
	{
		vs = vs*dlist[i];
	}

	return vs;
}

int CvtWb2V::get_magicWord()
{
	return magic_word;
}

vector<float>* CvtWb2V::get_vector()
{

	return rslt;
}



vector<float>* CvtWb2V::loadBinaryData(LPCSTR filename)
{
	errno_t er;
	er = fopen_s(&pFile, filename, "rb");

	int tmp;
	fread(&tmp, 4, 1, pFile);
	magic_word = tmp; //Get magic word

	fread(&tmp, 4, 1, pFile);
	ndim = tmp; //Get total dim

	int *buf =(int*) malloc(ndim*sizeof(int)); //Allocate memory for the tuple for dim, as dim shape is unknown.

	for (int i = 0; i < ndim; i++)
	{
		fread(&tmp, 4, 1, pFile);
		buf[i] = tmp;
	}
	dlist = buf; //Refer to dim, a private member of this class, will exist forever

	vector<float>* wb_vec = new vector<float>[2]; //Apply two vector to store W and b, will be deleted after using.

	for (int i = 0; i < get_vectorSize(); i++)  //Get the Weight vector
	{
		float tmp;
		fread(&tmp, 4, 1, pFile);
		wb_vec[0].push_back(tmp);
	}
	
	for (int i = 0; i < dlist[ndim - 1]; i++) //Get the bias vector
	{
		float tmp;
		fread(&tmp, 4, 1, pFile);
		wb_vec[1].push_back(tmp);
	}

	return wb_vec;
}


