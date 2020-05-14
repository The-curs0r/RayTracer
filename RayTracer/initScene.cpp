#include <iostream>
#include <stdio.h>
#include <vector>

#include "light.hpp"
#include "scene.hpp"
#include "object.hpp"
#include "objImporter.hpp"
#include "texture.hpp"
#include "imageTexture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void init(Scene* scene, int sceneId) {
	
	if (sceneId == 1) {

		loadOBJ("./Models/Floor3.obj", scene);
		int nx1 = 900, ny1 = 900;
		int nn = 3;
		unsigned char* tex_dataload = stbi_load("./Textures/Floor_2.jpg", &nx1, &ny1, &nn, 0);
		imageTexture* imgTex = new imageTexture(tex_dataload, nx1, ny1);
		scene->objects[0]->objTex = imgTex;

		sphere* sphere1 = new sphere();
		sphere1->center = glm::dvec3(1, 0.7, 0);
		sphere1->radius = 1;
		sphere1->diffuse = glm::dvec3(.6, .3, .9);
		sphere1->emission = glm::dvec3(0, 0, 0);
		sphere1->shininess = .2;
		sphere1->specular = glm::dvec3(0, 0, 0);
		sphere1->transparency = 0;
		sphere1->reflectivity = 1;
		scene->add(sphere1);

		light* light1 = new light();
		light1->source = glm::dvec3(-1, 2.4, 0);
		light1->attenuation = glm::dvec3(1, 0.1, 0.05);
		light1->type = 1;
		light1->color = glm::dvec3(1, 1, 1);
		light1->intensity = 2.0;
		scene->add(light1);

		scene->ambientIntensity = 0.1 * 2.0;
	}
	else if (sceneId == 2) {
		
		loadOBJ("./Models/Monkey.obj", scene);

		light* light1 = new light();
		light1->source = glm::dvec3(-1, 0, 0);
		light1->attenuation = glm::dvec3(1, 0.1, 0.05);
		light1->type = 1;
		light1->color = glm::dvec3(1, 1, 1);
		light1->intensity = 2.0;
		scene->add(light1);

		scene->ambientIntensity = 0.1 * 2.0;
	}
	else if (sceneId == 3) {

		sphere* sphere1 = new sphere();
		sphere1->center = glm::dvec3(3.5, 0, 0);
		sphere1->radius = 2;
		sphere1->diffuse = glm::dvec3(.6, .3, .9);
		sphere1->emission = glm::dvec3(0, 0, 0);
		sphere1->shininess = 10;
		sphere1->specular = glm::dvec3(0.2, 0.3, 0.4);
		sphere1->transparency = 0;
		sphere1->reflectivity = 0;
		sphere1->isTextured = 1;
		scene->add(sphere1);

		int nx1 = 2048, ny1 = 1024;
		int nn = 3;
		unsigned char* tex_dataload = stbi_load("./Textures/Jupiter.jpg", &nx1, &ny1, &nn, 0);
		imageTexture* imgTex = new imageTexture(tex_dataload,nx1,ny1);
		sphere1->objTex = imgTex;

		light* light1 = new light();
		light1->source = glm::dvec3(-3, 1, -3);
		light1->attenuation = glm::dvec3(1, 0.1, 0.05);
		light1->type = 1;
		light1->color = glm::dvec3(1, 1, 1);
		light1->intensity = 15.0;
		scene->add(light1);

		scene->ambientIntensity = 0.1 * 2.0;
	}
	else if (sceneId == 4) {

		sphere* sphere1 = new sphere();
		sphere1->center = glm::dvec3(3.5, 0, 1);
		sphere1->radius = 1;
		sphere1->diffuse = glm::dvec3(.6, .3, .9);
		sphere1->emission = glm::dvec3(0, 0, 0);
		sphere1->shininess = 10;
		sphere1->specular = glm::dvec3(0.2, 0.3, 0.4);
		sphere1->transparency = 0;
		sphere1->reflectivity = 1;
		sphere1->isTextured = 0;
		scene->add(sphere1);

		sphere* sphere2 = new sphere();
		sphere2->center = glm::dvec3(14, 2, 0);
		sphere2->radius = 4;
		sphere2->diffuse = glm::dvec3(0.7, 0.5, 0.7);
		sphere2->emission = glm::dvec3(0, 0, 0);
		sphere2->shininess = 10;
		sphere2->specular = glm::dvec3(0.5, 0.3, 0.8);
		sphere2->transparency = 0;
		sphere2->reflectivity = 1;
		sphere2->isTextured = 0;
		scene->add(sphere2);

		sphere* sphere3 = new sphere();
		sphere3->center = glm::dvec3(0, -2, -2);
		sphere3->radius = 1;
		sphere3->diffuse = glm::dvec3(.43, .11, .78);
		sphere3->emission = glm::dvec3(0, 0, 0);
		sphere3->shininess = 10;
		sphere3->specular = glm::dvec3(0.2, 0.3, 0.4);
		sphere3->transparency = 0;
		sphere3->reflectivity = 1;
		sphere3->isTextured = 0;
		scene->add(sphere3);

		sphere* sphere4 = new sphere();
		sphere4->center = glm::dvec3(0, -.6, -0.2);
		sphere4->radius = 0.5;
		sphere4->diffuse = glm::dvec3(.5, .7, .8);
		sphere4->emission = glm::dvec3(0, 0, 0);
		sphere4->shininess = 10;
		sphere4->specular = glm::dvec3(0.2, 0.3, 0.4);
		sphere4->transparency = 0;
		sphere4->reflectivity = 1;
		sphere4->isTextured = 0;
		scene->add(sphere4);

		light* light1 = new light();
		light1->source = glm::dvec3(-4, 2, -2);
		light1->attenuation = glm::dvec3(1, 0.1, 0.01);
		light1->type = 1;
		light1->color = glm::dvec3(1, 1, 1);
		light1->intensity = 5;
		scene->add(light1);

		light* light2 = new light();
		light2->source = glm::dvec3(0, 3, 0);
		light2->attenuation = glm::dvec3(1, 0.1, 0.01);
		light2->type = 1;
		light2->color = glm::dvec3(0.6, 0.2, 0.4);
		light2->intensity = 3;
		scene->add(light2);

		scene->ambientIntensity = 0.1 * 2.0;
	}
	else if (sceneId == 5) {

		sphere* sphere1 = new sphere();
		sphere1->center = glm::dvec3(3.5, 0, 0);
		sphere1->radius = 2;
		sphere1->diffuse = glm::dvec3(.6, .3, .9);
		sphere1->emission = glm::dvec3(0, 0, 0);
		sphere1->shininess = 10;
		sphere1->specular = glm::dvec3(0.2, 0.3, 0.4);
		sphere1->transparency = 0;
		sphere1->reflectivity = 0;
		sphere1->isTextured = 1;
		scene->add(sphere1);

		int nx1 = 2048, ny1 = 1024;
		int nn = 3;
		unsigned char* tex_dataload = stbi_load("./Textures/Jupiter.jpg", &nx1, &ny1, &nn, 0);
		imageTexture* imgTex = new imageTexture(tex_dataload, nx1, ny1);
		scene->objects[0]->objTex = imgTex;

		sphere* sphere2 = new sphere();
		sphere2->center = glm::dvec3(0, 0, 1);
		sphere2->radius = 1;
		sphere2->diffuse = glm::dvec3(1, 1, 1);
		sphere2->emission = glm::dvec3(0, 0, 0);
		sphere2->shininess = 10;
		sphere2->specular = glm::dvec3(0.2, 0.3, 0.4);
		sphere2->transparency = 0;
		sphere2->reflectivity = 1;
		sphere2->isTextured = 0;
		scene->add(sphere2);

		scene->ambientIntensity = 0.1 * 2.0;
	}
	else if (sceneId == 6) {

	loadOBJ("./Models/Plane1.obj", scene);

	int nx1 = 2048, ny1 = 1024;
	int nn = 3;
	unsigned char* tex_dataload = stbi_load("./Textures/Stars.jpg", &nx1, &ny1, &nn, 0);
	imageTexture* imgTex = new imageTexture(tex_dataload, nx1, ny1);
	scene->objects[0]->objTex = imgTex;

	sphere* sphere1 = new sphere();
	sphere1->center = glm::dvec3(3.5, 0, 0);
	sphere1->radius = 2;
	sphere1->diffuse = glm::dvec3(.6, .3, .9);
	sphere1->emission = glm::dvec3(0, 0, 0);
	sphere1->shininess = 10;
	sphere1->specular = glm::dvec3(0.2, 0.3, 0.4);
	sphere1->transparency = 0;
	sphere1->reflectivity = 0;
	sphere1->isTextured = 1;
	scene->add(sphere1);

	int nx3 = 2000, ny4 = 1000;
	int nn1 = 3;
	unsigned char* tex_dataload1 = stbi_load("./Textures/Jupiter.jpg", &nx3, &ny4, &nn1, 0);
	imageTexture* imgTex1 = new imageTexture(tex_dataload1, nx3, ny4);
	scene->objects[1]->objTex = imgTex1;

	scene->ambientIntensity = 0.1 * 2.0;
	}
	else if (sceneId == 7) {

	loadOBJ("./Models/Plane1.obj", scene);

	int nx1 = 3840, ny1 = 2160;
	int nn = 3;
	unsigned char* tex_dataload = stbi_load("./Textures/Stars.jpg", &nx1, &ny1, &nn, 0);
	imageTexture* imgTex = new imageTexture(tex_dataload, nx1, ny1);
	/*sphere1->objTex = imgTex;*/
	scene->objects[0]->objTex = imgTex;

	loadOBJ("./Models/Sphere1Map.obj", scene);

	int nx3 = 2000, ny4 = 1000;
	int nn1 = 3;
	unsigned char* tex_dataload1 = stbi_load("./Textures/Jupiter.jpg", &nx3, &ny4, &nn1, 0);
	imageTexture* imgTex1 = new imageTexture(tex_dataload1, nx3, ny4);
	scene->objects[1]->objTex = imgTex1;

	scene->ambientIntensity = 0.1 * 2.0;
	}
	else if (sceneId == 8) {

		loadOBJ("./Models/Plane1.obj", scene);

		int nx1 = 1920, ny1 = 1080;
		int nn = 3;
		unsigned char* tex_dataload = stbi_load("./Textures/Sky.jpg", &nx1, &ny1, &nn, 0);
		imageTexture* imgTex = new imageTexture(tex_dataload, nx1, ny1);
		scene->objects[0]->objTex = imgTex;

		sphere* sphere1 = new sphere();
		sphere1->center = glm::dvec3(0, 0, 0);
		sphere1->radius = 1;
		sphere1->diffuse = glm::dvec3(.6, .3, .9);
		sphere1->emission = glm::dvec3(0, 0, 0);
		sphere1->shininess = 10;
		sphere1->specular = glm::dvec3(0.2, 0.3, 0.4);
		sphere1->transparency = 1;
		sphere1->refractiveIndex = 2.0;
		sphere1->reflectivity = 0;
		sphere1->isTextured = 0;
		scene->add(sphere1);

		scene->ambientIntensity = 0.1 * 2.0;
	}
	else if (sceneId == 9) {

		sphere* sphere1 = new sphere();
		sphere1->center = glm::dvec3(-1, 0, -1.9);
		sphere1->radius = 0.383;
		sphere1->diffuse = glm::dvec3(.6, .3, .9);
		sphere1->emission = glm::dvec3(0, 0, 0);
		sphere1->shininess = 10;
		sphere1->specular = glm::dvec3(0.2, 0.3, 0.4);
		sphere1->transparency = 0;
		sphere1->reflectivity = 0;
		sphere1->isTextured = 1;
		int nx1 = 2048, ny1 = 1024;
		int nn1 = 3;
		unsigned char* tex_dataload1 = stbi_load("./Textures/Mercury.jpg", &nx1, &ny1, &nn1, 0);
		imageTexture* imgTex1 = new imageTexture(tex_dataload1, nx1, ny1);
		sphere1->objTex = imgTex1;
		scene->add(sphere1);

		sphere* sphere2 = new sphere();
		sphere2->center = glm::dvec3(3.5, 0, -1.4);
		sphere2->radius = 0.949;
		sphere2->diffuse = glm::dvec3(.6, .3, .9);
		sphere2->emission = glm::dvec3(0, 0, 0);
		sphere2->shininess = 10;
		sphere2->specular = glm::dvec3(0.2, 0.3, 0.4);
		sphere2->transparency = 0;
		sphere2->reflectivity = 0;
		sphere2->isTextured = 1;
		int nx2 = 2048, ny2 = 1024;
		int nn2 = 3;
		unsigned char* tex_dataload2 = stbi_load("./Textures/Venus.jpg", &nx2, &ny2, &nn2, 0);
		imageTexture* imgTex2 = new imageTexture(tex_dataload2, nx2, ny2);
		sphere2->objTex = imgTex2;
		scene->add(sphere2);

		sphere* sphere3 = new sphere();
		sphere3->center = glm::dvec3(7.5, 0, -0.2);
		sphere3->radius = 1;
		sphere3->diffuse = glm::dvec3(.6, .3, .9);
		sphere3->emission = glm::dvec3(0, 0, 0);
		sphere3->shininess = 10;
		sphere3->specular = glm::dvec3(0.2, 0.3, 0.4);
		sphere3->transparency = 0;
		sphere3->reflectivity = 0;
		sphere3->isTextured = 1;
		int nx3 = 2048, ny3 = 1024;
		int nn3 = 3;
		unsigned char* tex_dataload3 = stbi_load("./Textures/Earth.jpg", &nx3, &ny3, &nn3, 0);
		imageTexture* imgTex3 = new imageTexture(tex_dataload3, nx3, ny3);
		sphere3->objTex = imgTex3;
		scene->add(sphere3);

		sphere* sphere4 = new sphere();
		sphere4->center = glm::dvec3(10.5, 0, 2.5);
		sphere4->radius = 0.532;
		sphere4->diffuse = glm::dvec3(.6, .3, .9);
		sphere4->emission = glm::dvec3(0, 0, 0);
		sphere4->shininess = 10;
		sphere4->specular = glm::dvec3(0.2, 0.3, 0.4);
		sphere4->transparency = 0;
		sphere4->reflectivity = 0;
		sphere4->isTextured = 1;
		int nx4 = 2048, ny4 = 1024;
		int nn4 = 3;
		unsigned char* tex_dataload4 = stbi_load("./Textures/Mars.jpg", &nx4, &ny4, &nn4, 0);
		imageTexture* imgTex4 = new imageTexture(tex_dataload4, nx4, ny4);
		sphere4->objTex = imgTex4;
		scene->add(sphere4);

		sphere* sphere5 = new sphere();
		sphere5->center = glm::dvec3(34, 0, 0);
		sphere5->radius = 11.21;
		sphere5->diffuse = glm::dvec3(.6, .3, .9);
		sphere5->emission = glm::dvec3(0, 0, 0);
		sphere5->shininess = 10;
		sphere5->specular = glm::dvec3(0.2, 0.3, 0.4);
		sphere5->transparency = 0;
		sphere5->reflectivity = 0;
		sphere5->isTextured = 1;
		int nx5 = 2048, ny5 = 1024;
		int nn5 = 3;
		unsigned char* tex_dataload5 = stbi_load("./Textures/jupiter.jpg", &nx5, &ny5, &nn5, 0);
		imageTexture* imgTex5 = new imageTexture(tex_dataload5, nx5, ny5);
		sphere5->objTex = imgTex5;
		scene->add(sphere5);

		scene->ambientIntensity = 0.1 * 2.0;
	}
	else {

		sphere* sphere1 = new sphere();
		sphere1->center = glm::dvec3(3.5,0,0);
		sphere1->radius = 2;
		sphere1->diffuse = glm::dvec3(.6, .3, .9);
		sphere1->emission = glm::dvec3(0, 0, 0);
		sphere1->shininess = 10;
		sphere1->specular = glm::dvec3(0.2,0.3,0.4);
		sphere1->transparency = 0;
		sphere1->reflectivity = 0;
		scene->add(sphere1);

		light* light1 = new light();
		light1->source = glm::dvec3(-4, 2, -2);
		light1->attenuation = glm::dvec3(1, 0.1, 0.01);
		light1->type = 1;
		light1->color = glm::dvec3(1, 1, 1);
		light1->intensity = 5;
		scene->add(light1);

		scene->ambientIntensity = 0.1 * 2.0;
	}
}