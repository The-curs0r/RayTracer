#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <Windows.h>
#include <glm/glm.hpp>

#include "ray.hpp"

using namespace std;

const int WIDTH = 600;
const int HEIGHT = 600;
const int MAXDEPTH = 5;

int main()
{
	ofstream Output_Image("Output.ppm");
	if (Output_Image.is_open())
	{
		Output_Image << "P3\n" << WIDTH << " " << HEIGHT << " 255\n";
		for (int i = 0; i < HEIGHT; i++)
		{
			for (int j = 0; j < WIDTH; j++)
			{
				glm::dvec3 pixColor = glm::dvec3(.200, .197, .184);
				Output_Image << (int)(255 * pixColor[0]) << ' ' << (int)(255 * pixColor[0]) << ' ' << (int)(255 * pixColor[0]) << "\n";
			}
		}
	}
	Output_Image.close();
	WinExec("cd ..", 1);
	WinExec("magick \"./Output.ppm\" \"./Output.png\"", 1);
	return 0;
}