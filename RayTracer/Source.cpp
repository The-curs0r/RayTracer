#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <Windows.h>
#include <glm/glm.hpp>

#include <omp.h>
#include <cmath>

#include <chrono>

#include "ray.hpp"
#include "object.hpp"
#include "scene.hpp"
#include "light.hpp"
#include "progressbar.hpp"
#include "objImporter.hpp"
#include "initScene.hpp"

using namespace std;

const int WIDTH = 2048;
const int HEIGHT = 1024;
const int MAXDEPTH = 1;
double CAMERA[] = { -4,0 , -4, 0, 0, 0, 0, 1, 0, 45 };
glm::dvec3 Color[HEIGHT][WIDTH];

int main()
{

	Scene* scene = new Scene();

	init(scene,3); 

	sphere* sphere2 = new sphere();
	sphere2->center = glm::dvec3(1.5, 1, -0.5);
	sphere2->radius = 1;
	sphere2->diffuse = glm::dvec3(.6, .3, .9);
	sphere2->emission = glm::dvec3(0,0,0);
	sphere2->shininess = .2;
	sphere2->specular = glm::dvec3(.3, .3, .3);
	sphere2->transparency = 0;
	sphere2->reflectivity = 0;
	//scene->add(sphere2);

	sphere* sphere3 = new sphere();
	sphere3->center = glm::dvec3(3, 0,0);
	sphere3->radius = 2;
	sphere3->diffuse = glm::dvec3(.6, .3, .9);
	sphere3->emission = glm::dvec3(.1,.1,.4);
	sphere3->shininess = .2;
	sphere3->specular = glm::dvec3(.43, .13, .32);
	sphere3->reflectivity = 0;
	sphere3->refractiveIndex = 1.65;
	sphere3->transparency = 1;
	//scene->add(sphere3);

	sphere* sphere4 = new sphere();
	sphere4->center = glm::dvec3(3, 0,1.5);
	sphere4->radius = 1;
	sphere4->diffuse = glm::dvec3(.54, .75, .69);
	sphere4->emission = glm::dvec3(.1, .1, .4);
	sphere4->shininess = .5;
	sphere4->specular = glm::dvec3(.23, .43, .13);
	sphere4->reflectivity = 0;
	sphere4->transparency = 0;
	//scene->add(sphere4);

	/*triangle* tri1 = new triangle();
	tri1->ambient = glm::dvec3(0.321, 0.898, 0.368);
	tri1->diffuse = glm::dvec3(0.298, 0.941, 0.784);
	tri1->emission = glm::dvec3(0,0,0);
	tri1->shininess = .2;
	tri1->specular = glm::dvec3(.3, .3, .3);
	tri1->reflectivity = 0;
	tri1->v1 = glm::dvec3(25, -1,5 );
	tri1->v2 = glm::dvec3(25, -1,-5 );
	tri1->v3 = glm::dvec3(-5, -1,5 );
	tri1->normal = glm::normalize((glm::cross(tri1->v2 - tri1->v1, tri1->v3 - tri1->v1)));

	triangle* tri2 = new triangle();
	tri2->ambient = glm::dvec3(0.321, 0.898, 0.368);
	tri2->diffuse = glm::dvec3(.6, .3, .9);
	tri2->emission = glm::dvec3(0.321, 0.898, 0.368);
	tri2->shininess = .2;
	tri2->specular = glm::dvec3(.3, .3, .3);
	tri2->reflectivity = 0;
	tri2->v1 = glm::dvec3(-5, -1,-5 );
	tri2->v2 = glm::dvec3(-5, -1,5 );
	tri2->v3 = glm::dvec3(25 ,-1,-5 );
	tri2->normal = glm::normalize((glm::cross(tri2->v2 - tri2->v1, tri2->v3 - tri2->v1)));

	

	light* light2 = new light();
	light2->attenuation = glm::dvec3(1, .05, .005);
	light2->source = glm::dvec3(10, -4, 0);
	light2->type = 1;
	light2->color = glm::dvec3(0.321, 0.898, 0.368);
	light2->intensity = 1.0;

	light* light3 = new light();
	light3->attenuation = glm::dvec3(1, .05, .005);
	light3->source = glm::dvec3(10, 0,-4);
	light3->type = 1;
	light3->color = glm::dvec3(0.321, 0.898, 0.368);
	light3->intensity = 1.0;

	light* light4 = new light();
	light4->attenuation = glm::dvec3(1, .05, .005);
	light4->source = glm::dvec3(10, 0, 4);
	light4->type = 1;
	light4->color = glm::dvec3(0.321, 0.898, 0.368);
	light4->intensity = 1.0;

	//scene->add(sphere2);
	//scene->add(sphere4);
	//scene->add(sphere3);
	

	//scene->add(tri1);
	//scene->add(tri2);

	//scene->add(light2);
	//scene->add(light3);
	//scene->add(light4);*/

	//SSAA Setup
	double jitterMatrix[5 * 2] = {
	0, 0,
	-1.0 / 4.0,  3.0 / 4.0,
	 3.0 / 4.0,  1.0 / 3.0,
	-3.0 / 4.0, -1.0 / 4.0,
	 1.0 / 4.0, -3.0 / 4.0,
	};
	const int samples = 1;

	double focallength = 7.5;
	double aperture = 0.05;
	double planedis = glm::length(glm::dvec3(CAMERA[0], CAMERA[1], CAMERA[2]) - glm::dvec3(CAMERA[3], CAMERA[4], CAMERA[5]));

	//ProgressBar
	progresscpp::ProgressBar progressBar(HEIGHT, 70, '#', '-');

	ofstream Output_Image("Output.ppm");
	if (Output_Image.is_open())
	{
		Output_Image << "P3\n" << WIDTH << " " << HEIGHT << " 255\n";

		auto start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < HEIGHT; i++)
		{
			++progressBar;
			progressBar.display();
#pragma omp parallel for
			for (int j = 0; j < WIDTH; j++)
			{
				glm::dvec3 pixColor = glm::dvec3(0, 0, 0);
				//std::cout << omp_get_num_threads()<<"\n";
				scene->cur_i = i;
				scene->cur_j = j;
				//Simple
				ray* temp = new ray();
				temp->raythrough(CAMERA, i , j , WIDTH, HEIGHT);
				pixColor = scene->intersectray(*temp, MAXDEPTH);
				delete temp;

				//Implemeted (Fixed) SSAA (Super Sampling Anti Aliasing)       //Randomize Later
				/*for (int sample = 0; sample < 5; ++sample) {

					ray* temp = new ray();
					temp->raythrough(CAMERA, i + jitterMatrix[2 * sample], j + jitterMatrix[2 * sample + 1], WIDTH, HEIGHT);
					//pixColor += scene->intersectray(*temp, 2);
					//std::cout << &temp<<"\n";
					glm::dvec3 focalPt = temp->origin + (focallength)*temp->direction;

					//Depth Of Field
					for (int divs = 0;divs < samples;divs++) {

						ray* rayInt = new ray();
						glm::dvec3 shift = glm::dvec3(((double)rand() / (RAND_MAX)) - 0.5, ((double)rand() / (RAND_MAX)) - 0.5, ((double)rand() / (RAND_MAX)) - 0.5);
						rayInt->origin = temp->origin + aperture * shift;
						rayInt->direction = glm::normalize(focalPt - rayInt->origin);
						//std::cout << &rayInt << "\n";
						//return 0;
						pixColor += scene->intersectray(*rayInt, MAXDEPTH);
						delete rayInt;
					}
					delete temp;
				}

				pixColor /= 5;*/
				Color[i][j] = pixColor;
				//Output_Image << (int)(255 * pixColor[0] / samples) << ' ' << (int)(255 * pixColor[1] / samples) << ' ' << (int)(255 * pixColor[2] / samples) << "\n";
			}
		}
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

		for (int i =0;i <HEIGHT;i++)
		{
			for (int j = 0;j < WIDTH;j++)
			{
				Output_Image << (int)(255 * Color[i][j][0] / samples) << ' ' << (int)(255 * Color[i][j][1] / samples) << ' ' << (int)(255 * Color[i][j][2] / samples) << "\n";
			}
		}
		Output_Image.close();

		ofstream Output_Image2("val.txt");
		if (Output_Image2.is_open())
		{
			Output_Image2 << duration.count() << "\n";
			Output_Image2.close();

		}
		progressBar.done();
	}
	//Output_Image.close();
	WinExec("cd ..", 1);
	WinExec("magick \"./Output.ppm\" \"./Output.png\"", 1);
	return 0;
}
