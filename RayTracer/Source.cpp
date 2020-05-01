#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <Windows.h>
#include <glm/glm.hpp>

#include "ray.hpp"
#include "object.hpp"
#include "scene.hpp"

using namespace std;

const int WIDTH = 600;
const int HEIGHT = 600;
const int MAXDEPTH = 5;
double CAMERA[] = { -4, 0, 0, 1, 0, 0, 0, 1, 0, 30 };

int main()
{

	Scene* scene = new Scene();

	sphere* sphere1 = new sphere();
	sphere1->center = glm::dvec3(10, 0, -2);
	sphere1->radius = 2;
	sphere1->diffuse = glm::dvec3(.6, .3, .9);
	sphere1->ambient = glm::dvec3(.6, .3, .9);
	sphere1->emission = glm::dvec3(.3, .5, .6);
	sphere1->shininess = .2;
	sphere1->specular = glm::dvec3(.3, .3, .3);

	sphere* sphere2 = new sphere();
	sphere2->center = glm::dvec3(5, 0, -2);
	sphere2->ambient = glm::dvec3(.3, .5, .1);
	sphere2->radius = 1;
	sphere2->diffuse = glm::dvec3(.6, .3, .9);
	sphere2->emission = glm::dvec3(.3, .5, .6);
	sphere2->shininess = .2;
	sphere2->specular = glm::dvec3(.3, .3, .3);

	scene->add(sphere1);
	scene->add(sphere2);

	ofstream Output_Image("Output.ppm");
	if (Output_Image.is_open())
	{
		Output_Image << "P3\n" << WIDTH << " " << HEIGHT << " 255\n";
		for (int i = 0; i < HEIGHT; i++)
		{
			for (int j = 0; j < WIDTH; j++)
			{
				ray* temp = new ray();
				temp->raythrough(CAMERA, i + .5, j + .5, WIDTH, HEIGHT);
				glm::dvec3 pixColor = scene->intersectray(*temp);
				Output_Image << (int)(255 * pixColor[0]) << ' ' << (int)(255 * pixColor[1]) << ' ' << (int)(255 * pixColor[2]) << "\n";
			}
		}
	}
	Output_Image.close();
	WinExec("cd ..", 1);
	WinExec("magick \"./Output.ppm\" \"./Output.png\"", 1);
	return 0;
}