#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

int ConvertCharArrayToInt(unsigned char* array, int LengthOfArray);

int ReverseInt(int i);

bool read_Mnist_Label(string filename, vector<char>&labels);

bool read_Mnist_Images(string filename, vector<vector<char>>&images);
