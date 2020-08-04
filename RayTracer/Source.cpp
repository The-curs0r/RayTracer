#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <glm.hpp>

#include <omp.h>
#include <cmath>

#include <chrono>

#ifdef _WIN32
#include <Windows.h>
#elif defined(unix) || defined(__unix__) || defined(__unix)
#include <stdlib.h>
#endif

#include "ray.hpp"
#include "object.hpp"
#include "scene.hpp"
#include "light.hpp"
#include "progressbar.hpp"
#include "objImporter.hpp"
#include "initScene.hpp"

using namespace std;

int WIDTH = 3840;
int HEIGHT = 2160;
const int MAX_THREADS = 4;

int MAXDEPTH = 1;
double CAMERA[] = { -4 ,0 , 0, 0, 0, 0, 0, 1, 0, 45 };

int main()
{
	Scene* scene = new Scene();

	cout << "Enter height in pixels : (4000 px >) ";
	cin >> HEIGHT;
	cout << "Enter width in pixels : (4000 px >) ";
	cin >> WIDTH;
	if (HEIGHT > 4000) HEIGHT = 4000;
	if (WIDTH > 4000) WIDTH = 4000;
	if (HEIGHT < 0)	HEIGHT = 1000;
	if (WIDTH < 0)	WIDTH = 1000;

	//glm::dvec3 Color[HEIGHT][WIDTH] is dynamically sized array
	glm::dvec3** Color = new glm::dvec3 * [HEIGHT];
	for (int i = 0; i < HEIGHT; i++)
		Color[i] = new glm::dvec3[WIDTH];

	int sceneId;
	std::cout << "Enter Scene Id : ";
	std::cin >> sceneId;
	std::cout << "Enter Maximum Number Of Ray Bounces : ";
	std::cin >> MAXDEPTH;
	if (MAXDEPTH > 35)	MAXDEPTH = 35;
	if (MAXDEPTH <= 0)	MAXDEPTH = 1;

	if (!init(scene, sceneId)) {
		cout << "Unable to load the scene\n. Please try again.\n\n";
		for (int i = 0; i < HEIGHT; i++)
			delete[] Color[i];
		delete[] Color;
		/*cout << "Press any button to exit\n";
		getchar();*/
		return 0;
	}

	//SSAA Setup
	//From this answer https://computergraphics.stackexchange.com/a/4253
	double jitterMatrix[5 * 2] = {
	0, 0,
	-1.0 / 4.0,  3.0 / 4.0,
	 3.0 / 4.0,  1.0 / 3.0,
	-3.0 / 4.0, -1.0 / 4.0,
	 1.0 / 4.0, -3.0 / 4.0,
	};
	const int samples = 32;

	double focallength = 5.0;
	int simple = 0;

	std::cout << "Enable Anti-aliasing and Depth Of Field (1 or 0) : ";
	std::cin >> simple;
	if (simple) {
		std::cout << "Enter Focal Length : ";
		std::cin >> focallength;
		if (focallength < 0)
			focallength = 10;
	}

	double aperture = 0.05;
	double planedis = glm::length(glm::dvec3(CAMERA[0], CAMERA[1], CAMERA[2]) - glm::dvec3(CAMERA[3], CAMERA[4], CAMERA[5]));

	//ProgressBar from https://github.com/prakhar1989/progress-cpp
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
			omp_set_dynamic(0);
			omp_set_num_threads(MAX_THREADS);
#pragma omp parallel for
			for (int j = 0; j < WIDTH; j++)
			{
				glm::dvec3 pixColor = glm::dvec3(0, 0, 0);
				//cout << "threads=" << omp_get_num_threads() << endl;

				scene->cur_i = i;
				scene->cur_j = j;

				//Simple
				if (!simple) {
					ray* temp = new ray();
					temp->raythrough(CAMERA, i, j, WIDTH, HEIGHT);
					pixColor = scene->intersectray(*temp, MAXDEPTH);
					Color[i][j] = pixColor;
					delete temp;
				}

				else {
					//Implemeted (Fixed) SSAA (Super Sampling Anti Aliasing) 
					for (int sample = 0; sample < 5; ++sample) {

						ray* temp = new ray();
						temp->raythrough(CAMERA, i + jitterMatrix[2 * sample], j + jitterMatrix[2 * sample + 1], WIDTH, HEIGHT);
						glm::dvec3 focalPt = temp->origin + (focallength)*temp->direction;

						//Depth Of Field
						//http://cg.skeelogy.com/depth-of-field-using-raytracing/
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
		}

		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

		for (int i =0;i <HEIGHT;i++)
		{
			for (int j = 0;j < WIDTH;j++)
			{
				if (!simple) {
					Output_Image << (int)(255 * Color[i][j][0] / 1) << ' ' << (int)(255 * Color[i][j][1] / 1) << ' ' << (int)(255 * Color[i][j][2] / 1) << "\n";
					continue;
				}
				Output_Image << (int)(255 * Color[i][j][0] / samples) << ' ' << (int)(255 * Color[i][j][1] / samples) << ' ' << (int)(255 * Color[i][j][2] / samples) << "\n";
			}
		}
		Output_Image << "#" << duration.count() << "\n";
		Output_Image.close();
		progressBar.done();
	}
	for (int i = 0; i < HEIGHT; i++)
		delete[] Color[i];
	delete[] Color;
#ifdef _WIN32
	WinExec("cd ..", 1);
	WinExec("magick \"./Output.ppm\" \"./Output.png\"", 1);
#elif defined(unix) || defined(__unix__) || defined(__unix)
	system("cd ..");
	system("magick \"./Output.ppm\" \"./Output.png\"");
#endif

	return 0;
}
