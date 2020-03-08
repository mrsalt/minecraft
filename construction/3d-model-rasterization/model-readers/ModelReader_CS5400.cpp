#include "ModelReader_CS5400.h"
#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;

bool ModelReader_CS5400::canParse(const std::string &path)
{
	ifstream input(path);
	if (!input)
	{
		cout << "Unable to open " << path << endl;
		return false;
	}
	const char *prefix = "#  Viewpoint Datalabs International, Inc.  Copyright 1996";
	char line[81];
	input.getline(line, sizeof(line) - 1);

	// OK, this is definitely a cheat, but this is only designed to recognize a single file.
	return memcmp(prefix, line, strlen(prefix)) == 0;
}

bool ModelReader_CS5400::parse(const std::string &path, ModelConsumer &consumer)
{
	typedef unsigned char BYTE;
	typedef unsigned short WORD;
	typedef unsigned long DWORD;
	typedef DWORD COLORREF;
#define RGB(r, g, b) ((COLORREF)(((BYTE)(r) | ((WORD)((BYTE)(g)) << 8)) | (((DWORD)(BYTE)(b)) << 16)))
#define GetRValue(rgb) (rgb & 0xff)
#define GetGValue(rgb) ((rgb & 0xff00) >> 8)
#define GetBValue(rgb) ((rgb & 0xff0000) >> 16)

	ifstream fin(path);
	char line[81];
	char type;
	int numbers[10];
	int wShape = 0;
	COLORREF color = RGB(0, 255, 0);
	int nColors = 9;
	char *colors[] = {"glass", "bone", "fldkdkgrey", "redbrick", "black", "brass", "dkdkgrey", "ltbrown", "redbrown"};
	COLORREF colorValues[] = {RGB(200, 200, 255), RGB(240, 240, 255), RGB(100, 100, 100),
							  RGB(200, 80, 80), RGB(50, 50, 50), RGB(100, 200, 200),
							  RGB(120, 120, 120), RGB(160, 120, 80), RGB(190, 90, 50)}; //RGB(190,90,50)};

	while (fin >> type)
	{
		if (type == 'v')
		{
			double x, y, z;
			fin >> x >> y >> z;
			consumer.verticeRead(x, y, z);
		}
		else if (type == 'f')
		{
			fin.getline(line, 80);
			int n = 0;
			int nNumbers = 0;

			for (int i = 1; line[i - 1] != '\0'; i++)
			{
				if (line[i] >= '0' && line[i] <= '9')
				{
					n *= 10;
					n += line[i] - '0';
				}
				else if (n != 0)
				{
					numbers[nNumbers] = n - 1;
					n = 0;
					nNumbers++;
				}
			}

			consumer.polygonRead(numbers, nNumbers);
		}
		else if (type == 'u')
		{
			fin.getline(line, 80);

			if (strncmp(line, "semtl ", 6) == 0)
			{
				for (int i = 0; i < nColors; i++)
				{
					if (strcmp(&line[6], colors[i]) == 0)
					{
						color = colorValues[i];
						consumer.setCurrentColor(GetRValue(color), GetGValue(color), GetBValue(color));
						break;
					}
				}
			}
		}
		else
		{
			fin.getline(line, 80);
		}
	}
	return true;
}