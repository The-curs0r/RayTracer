#ifndef SCENE_HPP
#define SCENE_HPP

#include "object.hpp"
#include "ray.hpp"
#include <vector>

class Scene {
public:
	std::vector<object*> objects;
	
	void add(object* object) {
		objects.push_back(object);
	}

	const glm::dvec3 intersectray(ray& rayIn) {
		
		double minDistance = FLT_MAX;

		glm::dvec3 outColor = glm::dvec3(0, 0, 0);

		std::vector<object*>::iterator objIterator = objects.begin();

		const object* int_object = nullptr;
		glm::dvec3 int_point;
		glm::dvec3 normal;
		for (objIterator = objects.begin();objIterator != objects.end(); objIterator++) {
			//Add interesection with objects here
			double intersection = (*objIterator)->intersect(rayIn,normal);

			if (intersection < minDistance && intersection > 0)
			{
				minDistance = intersection;
				int_object = *objIterator;
				int_point = rayIn.origin + intersection * rayIn.direction;
			}
		}
		if (int_object != nullptr)
		{
			outColor = int_object->ambient;
		}
		return outColor;
	}

};

#endif // !SCENE_HPP
