#include <iostream>
#include <stdio.h>
#include <vector>

#include "light.hpp"
#include "scene.hpp"
#include "object.hpp"
#include "objImporter.hpp"

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
	if (sceneId == 2) {
		
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
}