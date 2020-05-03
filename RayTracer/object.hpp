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
	double reflectivity;
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

class triangle : public object {
public:
	glm::dvec3 v1;
	glm::dvec3 v2;
	glm::dvec3 v3;
	glm::dvec3 normal;


	//Check 
	double intersect(const ray& ray, glm::dvec3& normal_ret) const {

		glm::dvec3 E1 = v2 - v1;
		glm::dvec3 E2 = v3 - v1;

		glm::dvec3 norm = (glm::cross(E1, E2));
		normal_ret = glm::normalize(norm);
		double det = -glm::dot(ray.direction, norm);
		double inndet = 1.0 / det;

		glm::dvec3 AO = ray.origin - v1;

		glm::dvec3 DAO = glm::cross(AO, ray.direction);

		double u = glm::dot(E2, DAO) * inndet;
		double v = -glm::dot(E1, DAO) * inndet;
		double t = glm::dot(AO, norm) * inndet;

		return((det >= epsilon && t >= 0.0 && u >= 0.0 && v >= 0.0 && (u + v) <= 1.0) ? t : -1);
	}

	bool check(glm::dvec3& point) const {
		return true;
	}
};

#endif // !OBJECT_HPP
