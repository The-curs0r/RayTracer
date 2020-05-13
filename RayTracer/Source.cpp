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

const int WIDTH = 1920;
const int HEIGHT = 1080;
int MAXDEPTH = 1;
double CAMERA[] = { -4,0 , 0, 0, 0, 0, 0, 1, 0, 45 };
glm::dvec3 Color[HEIGHT][WIDTH];

int main()
{
	Scene* scene = new Scene();

	int sceneId;
	std::cout << "Enter Scene Id : ";
	std::cin >> sceneId;
	std::cout << "Enter Maximum Number Of Ray Bounces : ";
	std::cin >> MAXDEPTH;

	init(scene,sceneId); 

	//SSAA Setup
	double jitterMatrix[5 * 2] = {
	0, 0,
	-1.0 / 4.0,  3.0 / 4.0,
	 3.0 / 4.0,  1.0 / 3.0,
	-3.0 / 4.0, -1.0 / 4.0,
	 1.0 / 4.0, -3.0 / 4.0,
	};
	const int samples = 16;

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
				/*ray* temp = new ray();
				temp->raythrough(CAMERA, i , j , WIDTH, HEIGHT);
				pixColor = scene->intersectray(*temp, MAXDEPTH);
				delete temp;*/

				//Implemeted (Fixed) SSAA (Super Sampling Anti Aliasing)       //Randomize Later
				for (int sample = 0; sample < 5; ++sample) {

					ray* temp = new ray();
					temp->raythrough(CAMERA, i + jitterMatrix[2 * sample], j + jitterMatrix[2 * sample + 1], WIDTH, HEIGHT);
					glm::dvec3 focalPt = temp->origin + (focallength)*temp->direction;

					//Depth Of Field
					for (int divs = 0;divs < samples;divs++) {

						ray* rayInt = new ray();
						glm::dvec3 shift = glm::dvec3(((double)rand() / (RAND_MAX)) - 0.5, ((double)rand() / (RAND_MAX)) - 0.5, ((double)rand() / (RAND_MAX)) - 0.5);
						rayInt->origin = temp->origin + aperture * shift;
						rayInt->direction = glm::normalize(focalPt - rayInt->origin);
						pixColor += scene->intersectray(*rayInt, MAXDEPTH);
						delete rayInt;
					}
					delete temp;
				}
				pixColor /= 5;
				Color[i][j] = pixColor;
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
		Output_Image << "#" << duration.count() << "\n";
		Output_Image.close();
		progressBar.done();
	}
	//Output_Image.close();
	WinExec("cd ..", 1);
	WinExec("magick \"./Output.ppm\" \"./Output.png\"", 1);
	return 0;
}
