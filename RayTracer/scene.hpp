#ifndef SCENE_HPP
#define SCENE_HPP

#include "object.hpp"
#include "ray.hpp"
#include "light.hpp"
#include <algorithm>
#include <vector>
#include "texture.hpp"

#define epsilon_ pow(10,-12)

class Scene {
public:
	std::vector<object*> objects;
	std::vector<light*> lights;
	double ambientIntensity;
	int cur_i;
	int cur_j;

	void add(object* object) {
		objects.push_back(object);
	}

	void add(light* light) {
		lights.push_back(light);
	}

	bool checkvis(glm::dvec3 target, glm::dvec3 source) {

		glm::dvec3 toSource = source - target;

		double t_light = glm::sqrt(glm::dot(toSource, toSource));

		ray* nray = new ray();
		nray->origin = target;
		nray->direction = toSource * (1 / t_light);
		double min_t = t_light;
		object* nearest_obj = nullptr;
		double t = FLT_MAX;
		glm::dvec2 uvData;
		glm::dvec3 normal = glm::dvec3(NULL, NULL, NULL);
		std::vector<object*>::iterator it = objects.begin();

		for (;it != objects.end();it++) {
		
			double t = (*it)->intersect(*nray, normal, uvData);
			if (t < min_t && t>0 && t > epsilon_)
			{
				min_t = t;
				(nearest_obj) = *it;
				delete nray;
				return false;  //OBJECT IS NOT VISIBLE
			}
		}
		delete nray;
		return true;
	}

	glm::dvec3 intersectray(ray& rayIn, int depth) {

		if (depth == 0)
			return glm::dvec3(0, 0, 0);

		double minDistance = FLT_MAX;

		glm::dvec3 outColor = glm::dvec3(1 * cur_i / 1080.0, 1 * cur_i / 1080.0, 1 * cur_i / 1080.0);
		outColor = glm::dvec3(0, 0, 0);

		std::vector<object*>::iterator objIterator = objects.begin();

		object* int_object = nullptr;
		glm::dvec3 int_point;
		glm::dvec3 int_normal;
		glm::dvec3 normal;
		glm::dvec2 uv_hit;

		for (objIterator = objects.begin();objIterator != objects.end(); objIterator++) {
			//Interesection with objects here
			double intersection = (*objIterator)->intersect(rayIn, normal, uv_hit);

			if (intersection < minDistance && intersection > epsilon_)
			{
				//std::cout << "here";

				int_normal = normal;
				minDistance = intersection;
				int_object = *objIterator;
				if (glm::dot(rayIn.direction, int_normal) < 0)
					int_point = rayIn.origin + intersection * rayIn.direction + (epsilon_ * int_normal);
				else
					int_point = rayIn.origin + intersection * rayIn.direction - (epsilon_ * int_normal);
			}
		}
		if (int_object != nullptr)
		{
			outColor = (ambientIntensity * int_object->diffuse + int_object->emission) * 0.5;
			if (int_object->isTextured) {
				//std::cout << uv_hit[0] << " " << uv_hit[1] << "\n";
				outColor = (int_object->objTex)->value(uv_hit[0], uv_hit[1], int_point);
				//outColor = glm::dvec3(0,0,0);
				//outColor /= 255.0;
				//std::cout << outColor[0]<<" " << outColor[1] <<" "<< outColor[2] << "\n";
			}

			std::vector<light*>::iterator lightIterator = lights.begin();

			for ( lightIterator = lights.begin(); lightIterator != lights.end(); lightIterator++)
			{
				glm::dvec3 source = (*lightIterator)->source;
				if ((*lightIterator)->type == 0) source = int_point - source;        //Directional light

				if (checkvis(int_point, source)) {
					//COLOR OBJECT AS IT IS VISIBLE
					glm::dvec3 L = source - int_point;
					double sourceDis = glm::dot(L, L);
					if ((*lightIterator)->type == 0) L = (*lightIterator)->source;        //Directional light
					L = glm::normalize(L);
					glm::dvec3 H = glm::normalize(-rayIn.direction + L);

					glm::dvec3 lambert = glm::dvec3(0, 0, 0);
					glm::dvec3 phong = glm::dvec3(0, 0, 0);
					if (glm::dot(int_normal, L) > 0)
					{
						
						if (!int_object->isTextured)
						{
							
							lambert = (*lightIterator)->color * int_object->diffuse * (glm::dot(int_normal, L));
						}
						else
							lambert = (*lightIterator)->color * (int_object->objTex)->value(uv_hit[0], uv_hit[1], int_point) * (glm::dot(int_normal, L));
					}
					if (glm::dot(int_normal, H) > 0)
					{
						if (!int_object->isTextured)
							phong = (*lightIterator)->color * int_object->specular * pow(glm::dot(int_normal, H), int_object->shininess);
					}
					outColor += ((*lightIterator)->intensity *(phong+lambert) / ((*lightIterator)->attenuation[0] + sourceDis * (*lightIterator)->attenuation[1] + sourceDis * sourceDis * (*lightIterator)->attenuation[2])) ;
				}
			}

			rayIn.direction = glm::normalize(rayIn.direction);
			int_normal = glm::normalize(int_normal);

			if (int_object->reflectivity)
			{
				ray* reflected = new ray();
				reflected->direction = rayIn.direction - (2 * glm::dot(rayIn.direction, int_normal)) * int_normal;
				reflected->origin = int_point;
				outColor += (intersectray(*reflected, depth - 1))*0.5;
				delete reflected;
			}
			if (int_object->transparency)
			{
				ray* refracted = new ray();
				if(glm::dot(rayIn.direction,int_normal)<0)
					refracted->origin = int_point - 3 * epsilon_ * int_normal;
				else
					refracted->origin = int_point + 3 * epsilon_ * int_normal;

				double cosi = glm::dot(rayIn.direction, int_normal);
				double etai = 1;
				double etat = int_object->refractiveIndex;
				glm::dvec3 normCpy = int_normal;
				if (cosi < 0)
					cosi = -cosi;
				else
				{
					std::swap(etai, etat);
					normCpy = -normCpy;
				}
				double eta = etai / etat;
				double k = 1 - eta * eta * (1 - cosi * cosi);
				if (k > 0)
				{
					refracted->direction = glm::normalize(eta*rayIn.direction+(eta*cosi-sqrt(k))*normCpy);
					outColor = (intersectray(*refracted, depth - 1));
				}
				delete refracted;
			}
		}
			
		
		return outColor;
	}

};

#endif // !SCENE_HPP
