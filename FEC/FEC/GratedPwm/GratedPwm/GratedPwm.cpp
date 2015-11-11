// GratedPwm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <bitset>
#include <iostream>
#include <fstream>

int _tmain(int argc, _TCHAR* argv[])
{
	char byteArray[101][100];
	float step;

	int i;

	memset(byteArray, 0, 101*100);
	std::bitset<100> bitsetArray[101];
	std::bitset<100> bitsetMask;
	bitsetMask = 0x1f;

	float f;
	for (i = 1; i < 101; ++i)
	{
		step = 100.0 / (float)i;
		for (f = 0; f < 100; f += step)
		{
			byteArray[i][(int)f] = 1;
			bitsetArray[i][f] = 1;
		}
	}

	std::bitset<5> bs[101][20];
	int j;
	std::fstream lutFile;
	lutFile.open("GeneratedGratedPwm.h", std::ios_base::out);

	lutFile << "static const uint32_t gratedPwm[101][4] = {" << std::endl;
	for (i = 0; i < 101; ++i)
	{
		for (j = 0; j < 20; ++j)
		{
			bs[i][j] = (bitsetArray[i] & bitsetMask).to_ulong();
			bitsetArray[i] >>= 5;
			std::cout << std::hex << bs[i][j].to_ulong() << " ";
		}
		std::cout << std::endl;

		lutFile << "{ ";
		for (j = 19; j >= 0; --j)
		{
			lutFile << "0x" << std::hex << bs[i][j].to_ulong() << ", ";
		}
		lutFile << " }," << std::endl;


		/*
		bs[0] = 0;
		bs[1] = 0;
		bs[2] = 0;
		bs[3] = 0;
		for (j = 0; j < 32; ++j)
		{
			bs[0][j] = byteArray[i][j];
		}
		for (j = 32; j < 64; ++j)
		{
			bs[1][j-32] = byteArray[i][j];
		}
		for (j = 64; j < 96; ++j)
		{
			bs[2][j-64] = byteArray[i][j];
		}
		for (j = 96; j < 100; ++j)
		{
			bs[3][j-96] = byteArray[i][j];
		}
		std::cout << std::hex << bs[3].to_ulong() << bs[2].to_ulong() << bs[1].to_ulong() << bs[0].to_ulong() << std::endl;
		lutFile << "{ ";
		lutFile << "0x" << std::hex << bs[0].to_ulong() << ", ";
		lutFile << "0x" << std::hex << bs[1].to_ulong() << ", ";
		lutFile << "0x" << std::hex << bs[2].to_ulong() << ", ";
		lutFile << "0x" << std::hex << bs[3].to_ulong() << " }," << std::endl;
		*/
	}
	lutFile << "};" << std::endl;
	lutFile.close();

	return 0;
}

