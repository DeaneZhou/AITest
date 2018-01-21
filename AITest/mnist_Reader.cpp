#include "stdafx.h"
#include "mnist_Reader.h"

extern int total_imageCnt;

int ConvertCharArrayToInt(unsigned char* array, int LengthOfArray)
{
	if (LengthOfArray < 0)
	{
		return -1;
	}
	int result = static_cast<signed int>(array[0]);
	for (int i = 1; i < LengthOfArray; i++)
	{
		result = (result << 8) + array[i];
	}
	return result;
}

int ReverseInt(int i)
{
	unsigned char ch1, ch2, ch3, ch4;

	ch1 = i & 255; // Get 1st byte 134777631
	ch2 = (i >> 8) & 255; // Get 2nd byte
	ch3 = (i >> 16) & 255; // Get 3rd byte
	ch4 = (i >> 24) & 255; // Get 4th byte
	return ((int)ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + (int)ch4;
}

bool read_Mnist_Label(string filename, vector<char>&labels)
{
	ifstream file(filename, ios::binary);
	if (file.is_open())
	{
		int magic_number = 0;
		int number_of_images = 0;
		file.read((char*)&magic_number, sizeof(magic_number));
		file.read((char*)&number_of_images, sizeof(number_of_images));
		//cout << magic_number << endl;
		magic_number = ReverseInt(magic_number);
		number_of_images = ReverseInt(number_of_images);
		cout << "magic number = " << magic_number << endl;
		cout << "number of images = " << number_of_images << endl;

		for (int i = 0; i < number_of_images; i++)
		{
			unsigned char label = 0;
			file.read((char*)&label, sizeof(label));
			labels.push_back((char)label);
		}
	}
	if (labels.empty())
		return false;
	else
		return true;
}

bool read_Mnist_Images(string filename, vector<vector<char>>&images)
{
	ifstream file(filename, ios::binary);
	if (file.is_open())
	{
		int magic_number = 0;
		int number_of_images = 0;
		int n_rows = 0;
		int n_cols = 0;

		file.read((char*)&magic_number, sizeof(magic_number));
		file.read((char*)&number_of_images, sizeof(number_of_images));
		file.read((char*)&n_rows, sizeof(n_rows));
		file.read((char*)&n_cols, sizeof(n_cols));

		magic_number = ReverseInt(magic_number);
		number_of_images = ReverseInt(number_of_images);
		total_imageCnt = number_of_images; // Get total images
		n_rows = ReverseInt(n_rows);
		n_cols = ReverseInt(n_cols);

		cout << "magic number = " << magic_number << endl;
		cout << "number of images = " << number_of_images << endl;
		cout << "rows = " << n_rows << endl;
		cout << "cols = " << n_cols << endl;

		for (int i = 0; i < number_of_images; i++)
		{
			vector< char >tp;
			for (int r = 0; r < n_rows; r++)
			{
				for (int c = 0; c < n_cols; c++)
				{
					unsigned char image = 0;
					file.read((char*)&image, sizeof(image));
					tp.push_back(image);
				}
			}
			images.push_back(tp);
		}
	}
	if (images.empty())
		return false;
	else
		return true;
}
