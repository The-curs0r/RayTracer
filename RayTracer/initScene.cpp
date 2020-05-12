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
		light1->source = glm::dvec3(-1, 2.4, 0);
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
		unsigned char* tex_dataload = stbi_load("jupiter.jpg", &nx1, &ny1, &nn, 0);
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
		unsigned char* tex_dataload = stbi_load("jupiter.jpg", &nx1, &ny1, &nn, 0);
		imageTexture* imgTex = new imageTexture(tex_dataload, nx1, ny1);
		sphere1->objTex = imgTex;

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
		unsigned char* tex_dataload = stbi_load("jupiter.jpg", &nx1, &ny1, &nn, 0);
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
	unsigned char* tex_dataload = stbi_load("jupiter.jpg", &nx1, &ny1, &nn, 0);
	imageTexture* imgTex = new imageTexture(tex_dataload, nx1, ny1);
	/*sphere1->objTex = imgTex;*/
	scene->objects[0]->objTex = imgTex;

	/*light* light1 = new light();
	light1->source = glm::dvec3(-4, 0, 0);
	light1->attenuation = glm::dvec3(1, 0.1, 0.05);
	light1->type = 1;
	light1->color = glm::dvec3(1, 1, 1);
	light1->intensity = 15.0;
	scene->add(light1);*/

	scene->ambientIntensity = 0.1 * 2.0;
	}
	else if (sceneId == 7) {

	loadOBJ("./Models/Plane1.obj", scene);

	int nx1 = 3840, ny1 = 2160;
	int nn = 3;
	unsigned char* tex_dataload = stbi_load("stars.jpg", &nx1, &ny1, &nn, 0);
	imageTexture* imgTex = new imageTexture(tex_dataload, nx1, ny1);
	/*sphere1->objTex = imgTex;*/
	scene->objects[0]->objTex = imgTex;

	loadOBJ("./Models/Sphere1Map.obj", scene);

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
	//scene->add(sphere1);

	int nx3 = 2048, ny4 = 1024;
	int nn1 = 3;
	unsigned char* tex_dataload1 = stbi_load("jupiter.jpg", &nx3, &ny4, &nn1, 0);
	imageTexture* imgTex1 = new imageTexture(tex_dataload1, nx3, ny4);
	scene->objects[1]->objTex = imgTex1;

	/*light* light1 = new light();
	light1->source = glm::dvec3(-4, 0, 0);
	light1->attenuation = glm::dvec3(1, 0.1, 0.05);
	light1->type = 1;
	light1->color = glm::dvec3(1, 1, 1);
	light1->intensity = 15.0;
	scene->add(light1);*/

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
		light1->source = glm::dvec3(-4, 0, 0);
		light1->attenuation = glm::dvec3(1, 0.1, 0.01);
		light1->type = 1;
		light1->color = glm::dvec3(1, 1, 1);
		light1->intensity = 5;
		scene->add(light1);

		scene->ambientIntensity = 0.1 * 2.0;
	}
}