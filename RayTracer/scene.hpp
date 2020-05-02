#ifndef SCENE_HPP
#define SCENE_HPP

#include "object.hpp"
#include "ray.hpp"
#include "light.hpp"
#include <algorithm>
#include <vector>

#define epsilon pow(10,-6)

class Scene {
public:
	std::vector<object*> objects;
	std::vector<light*> lights;

	void add(object* object) {
		objects.push_back(object);
	}

	void add(light* light) {
		lights.push_back(light);
	}

	const bool checkvis(glm::dvec3 target, glm::dvec3 source) {

		glm::dvec3 toSource = source - target;

		double t_light = glm::sqrt(glm::dot(toSource, toSource));

		ray* nray = new ray();
		nray->origin = target;
		nray->direction = toSource * (1 / t_light);
		double min_t = t_light;
		const object* nearest_obj = nullptr;
		double t = FLT_MAX;
		glm::dvec3 normal = glm::dvec3(NULL, NULL, NULL);
		std::vector<object*>::iterator it = objects.begin();

		for (it = objects.begin();it != objects.end();it++) {
		
			double t = (*it)->intersect(*nray, normal);
			if (t < min_t && t>0 && t > epsilon)
			{
				min_t = t;
				(nearest_obj) = *it;
				return false;  //OBJECT IS NOT VISIBLE
			}

		}
		return true;
	}

	const glm::dvec3 intersectray(ray& rayIn,int depth) {
		
		if (!depth)
			return glm::dvec3(0, 0, 0);

		double minDistance = FLT_MAX;

		glm::dvec3 outColor = glm::dvec3(0, 0, 0);

		std::vector<object*>::iterator objIterator = objects.begin();

		const object* int_object = nullptr;
		glm::dvec3 int_point;
		glm::dvec3 normal;
		for (objIterator = objects.begin();objIterator != objects.end(); objIterator++) {
			//Add interesection with objects here
			double intersection = (*objIterator)->intersect(rayIn,normal);

			if (intersection < minDistance && intersection > 0  )
			{
				minDistance = intersection;
				int_object = *objIterator;
				int_point = rayIn.origin + intersection * rayIn.direction;
				
				int_point += (epsilon*normal);
				
			}
		}
		if (int_object != nullptr)
		{
			outColor = (int_object->ambient +int_object->emission) * 0.5;

			std::vector<light*>::iterator lightIterator = lights.begin();
			
			for ( lightIterator = lights.begin(); lightIterator != lights.end(); lightIterator++)
			{
				glm::dvec3 source = (*lightIterator)->source;
				if ((*lightIterator)->type == 0) source = int_point - source;        //Directional light

				if (checkvis(int_point, source)) {
					//COLOR OBJECT AS IT IS VISIBLE
					glm::dvec3 L = source - int_point;
					if ((*lightIterator)->type == 0) L = (*lightIterator)->source;        //Directional light
					L = glm::normalize(L);
					glm::dvec3 H = glm::normalize(-rayIn.direction + L);

					glm::dvec3 lambert = glm::dvec3(0, 0, 0);
					glm::dvec3 phong = glm::dvec3(0, 0, 0);
					if(glm::dot(normal, L)>0)
						lambert = (*lightIterator)->color * int_object->diffuse * (glm::dot(normal, L));
					if (glm::dot(normal, H) > 0)
						phong = (*lightIterator)->color * int_object->specular * pow(glm::dot(normal, H), int_object->shininess);	

					outColor =( outColor + ( (*lightIterator)->intensity *(phong+lambert) / ((*lightIterator)->attenuation[0] + minDistance * (*lightIterator)->attenuation[1] + minDistance * minDistance * (*lightIterator)->attenuation[2]))) * 0.5;
				}
			}

			ray* reflected = new ray();
			reflected->direction = rayIn.direction - 2 * glm::dot(rayIn.direction, normal) * normal;
			reflected->origin = int_point;
			outColor = (outColor + (int_object->reflectivity)*intersectray(*reflected, depth - 1))/ (int_object->reflectivity+1);
		}
		return outColor;
	}

};

#endif // !SCENE_HPP
