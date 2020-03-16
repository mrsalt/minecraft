#include "ModelReader_Wavefront.h"
#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;

bool ModelReader_Wavefront::parse(const std::string &path, ModelConsumer &consumer)
{
	ifstream fin(path);
	char line[81];
	char type;
	int numbers[10];
	int wShape = 0;

	// TODO: replace these hard coded colors with code to read the .mtl file, which defines the colors.
	int nColors = 9;
	char *colors[] = {"glass", "bone", "fldkdkgrey", "redbrick", "black", "brass", "dkdkgrey", "ltbrown", "redbrown"};
	Color colorValues[] = {{200, 200, 255}, {240, 240, 255}, {100, 100, 100},
							  {200, 80, 80}, {50, 50, 50}, {100, 200, 200},
							  {120, 120, 120}, {160, 120, 80}, {190, 90, 50} }; //RGB(190,90,50)};

	while (fin >> type)
	{
		if (type == 'v') // Geometric vertex
		{
			Point p;
			fin >> p.x >> p.y >> p.z;
			consumer.verticeRead(p);
		}
		else if (type == 'f') //Vertex indices
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
						consumer.setCurrentColor(colorValues[i]);
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