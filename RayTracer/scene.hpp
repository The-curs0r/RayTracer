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

	const glm::dvec3 intersectray(ray& ray_int) {
		
		double minDistance = FLT_MAX;

		std::vector<object*>::iterator objIterator = objects.begin();

		const object* int_object = nullptr;
		glm::dvec3 int_point;
		for (objIterator = objects.begin();objIterator != objects.end(); objIterator++) {
			//Add interesection with objects here
		}
	}

};

#endif // !SCENE_HPP
