#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "ray.hpp"
#include <glm/glm.hpp>

class object {
public:
	glm::dvec3 diffuse;
	glm::dvec3 specular;
	glm::dvec3 emission;
	glm::dvec3 ambient;
	glm::dmat4 tranform;
	double shininess;
	virtual double intersect(const ray& ray, glm::dvec3& normal) const = 0;
};

class sphere : public object {
public:
	glm::dvec3 center;
	double radius;

	double intersect(const ray& ray, glm::dvec3& normal) const {
		
		glm::dvec3 toCenter = center - ray.origin;
		double tca = glm::dot(toCenter, ray.direction);
		double dis2 = glm::dot(toCenter, toCenter) - tca * tca;
		if (dis2 > radius * radius) return -1;
		double thc = glm::sqrt(radius * radius - dis2);
		double t1 = tca - thc;
		double t2 = tca + thc;

		if (t1 > t2) std::swap(t1, t2);

		if (t1 < 0) {
			t1 = t2;
			if (t1 < 0)
				return -1;
		}
		normal = glm::normalize(ray.origin + t1 * ray.direction - center);
		return t1;
	}
};

#endif // !OBJECT_HPP
