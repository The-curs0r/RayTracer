#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "ray.hpp"
#include <glm/glm.hpp>

#define epsilon pow(10,-6)

class object {
public:
	glm::dvec3 diffuse;
	glm::dvec3 specular;
	glm::dvec3 emission;
	glm::dvec3 ambient;
	glm::dmat4 tranform;
	double shininess;
	virtual double intersect(const ray& ray, glm::dvec3& normal) const = 0;
	virtual bool check(glm::dvec3& point) const = 0;
};

class sphere : public object {
public:
	glm::dvec3 center;
	double radius;

	double intersect(const ray& ray, glm::dvec3& normal) const {
		
		/*glm::dvec3 toCenter = center - ray.origin;
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
		return t1;*/
		double c = glm::dot(ray.origin - center, ray.origin - center) - radius * radius;
		double b = 2 * glm::dot(ray.direction, ray.origin - center);
		double a = glm::dot(ray.direction, ray.direction);

		if (b * b - 4 * a * c < 0)
			return -1;

		double s1 = (-b + glm::sqrt(b * b - 4 * a * c)) / 2 * a;
		double s2 = (-b - glm::sqrt(b * b - 4 * a * c)) / 2 * a;

		double t;

		if (s1 < 0 && s2 < 0)
			return -1;
		else if (s1 < 0)
			t = s2;
		else if (s2 < 0)
			t = s1;
		else
			t = s1>s2?s2:s1;
		normal = glm::normalize(ray.origin + t * ray.direction - center);
		return t;
	}

	bool check(glm::dvec3& point) const {
		double val = pow((point[0] - center[0]), 2) + pow((point[1] - center[1]), 2) + pow((point[2] - center[2]), 2);
		if (val < pow(radius,2))
			return false;
		return true;
	}
};

#endif // !OBJECT_HPP
