#ifndef IMAGETEXTUREHPP
#define IMAGETEXTUREHPP

#include "texture.hpp"
#include <glm/glm.hpp>

class imageTexture :public texture {
public:
	imageTexture(){}
	imageTexture(unsigned char* pixels,int A,int B):data(pixels),nx(A),ny(B){}
	virtual glm::dvec3 value(double u, double v, const glm::dvec3& p) const;
	unsigned char* data;
	int nx, ny;
};

glm::dvec3 imageTexture::value(double u, double v, const glm::dvec3& p) const {
	int i = (u)*nx;
	int j = (1 - v) * ny - 0.001;

	if (i < 0) i = 0;
	if (j < 0) j = 0;

	if (i > nx - 1) i = nx - 1;
	if (j > ny - 1) j = ny - 1;
	
	double r = int(data[3 * i + 3 * nx * j]) / 255.0;
	double g = int(data[3 * i + 3 * nx * j + 1]) / 255.0;
	double b = int(data[3 * i + 3 * nx * j + 2]) / 255.0;

	//double r = (i%255) / 255.0;
	//double g = 0;
	//double b = 0;

	return glm::dvec3(r, g, b);
}

#endif // !IMAGETEXTUREHPP
