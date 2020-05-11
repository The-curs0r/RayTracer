#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "ray.hpp"
#include <glm/glm.hpp>
#include "texture.hpp"

#define epsilon pow(10,-12)
#define M_PI 3.14159

class object {
public:
	glm::dvec3 diffuse;
	glm::dvec3 specular;
	glm::dvec3 emission;
	glm::dmat4 tranform;
	int reflectivity;
	int transparency;
	int isTextured;
	texture* objTex;
	double refractiveIndex;
	double shininess;
	virtual double intersect( ray& ray, glm::dvec3& normal,glm::dvec2 &uv) = 0;
	virtual void getuv(glm::dvec3& int_point, double& u, double& v) = 0;
};

class sphere : public object {
public:
	glm::dvec3 center;
	double radius;

	double intersect( ray& ray, glm::dvec3& normal, glm::dvec2& uv)  {
		
		/*glm::dvec3 toCenter = center - ray.origin;
		double tca = glm::dot(toCenter, ray.direction);
		double dis2 = glm::dot(toCenter, toCenter) - tca * tca;
		if (dis2 > radius * radius) return -1;
		double thc = glm::sqrt(radius * radius - dis2);
		double t1 = tca - thc;
		double t2 = tca + thc;

		if (t1 > t2) std::std::swap(t1, t2);

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
		glm::dvec3 intersectionPoint = ray.origin + t * ray.direction;
		intersectionPoint -= center;
		intersectionPoint /= radius;
		getuv(intersectionPoint, uv[0], uv[1]);
		return t;
	}
	void getuv(glm::dvec3& int_point,double &u,double&v) {
		double phi = atan2(int_point[2],int_point[0]);
		double theta = asin(int_point[1]);
		u = 1 - (phi + M_PI) / (2 * M_PI);
		v = (theta + M_PI/2) / (M_PI);
		//std::cout << int_point[1] << " " << v << "\n";
	}
};

class triangle {
public:
	glm::dvec3 v1;
	glm::dvec3 v2;
	glm::dvec3 v3;
	glm::dvec3 normal;
	glm::dvec2 uv_v1;
	glm::dvec2 uv_v2;
	glm::dvec2 uv_v3;

	//Check 
	double intersect( ray& ray, glm::dvec3& normal_ret)  {

		glm::dvec3 E1 = v2 - v1;
		glm::dvec3 E2 = v3 - v1;
		glm::dvec3 norm = (glm::cross(E1, E2));
		normal_ret = glm::normalize(normal);
		double det = -glm::dot(ray.direction, norm);
		double inndet = 1.0 / det;

		glm::dvec3 AO = ray.origin - v1;

		glm::dvec3 DAO = glm::cross(AO, ray.direction);

		double u = glm::dot(E2, DAO) * inndet;
		double v = -glm::dot(E1, DAO) * inndet;
		double t = glm::dot(AO, norm) * inndet;

		return((det >= epsilon && t >= 0.0 && u >= 0.0 && v >= 0.0 && (u + v) <= 1.0) ? t : -1);
	}
};

//BOUNDING BOX CLASS FOR MESH
class boundBox {
public:
	glm::dvec3 boundMin;
	glm::dvec3 boundMax;
	bool intersect(ray& ray) {
		//return true;
		double tmin = (boundMin[0]-ray.origin[0])/ray.direction[0];
		double tmax = (boundMax[0] - ray.origin[0]) / ray.direction[0];

		if (tmin > tmax) std::swap(tmin, tmax);

		double tymin = (boundMin[1] - ray.origin[1]) / ray.direction[1];
		double tymax = (boundMax[1] - ray.origin[1]) / ray.direction[1];

		if (tymin > tymax) std::swap(tymin, tymax);

		if ((tmin > tymax) || (tymin > tmax))
			return false;

		if (tymin > tmin)
			tmin = tymin;
		if (tymax < tmax)
			tmax = tymax;

		double tzmin = (boundMin[2] - ray.origin[2]) / ray.direction[2];
		double tzmax = (boundMax[2] - ray.origin[2]) / ray.direction[2];

		if (tzmin > tzmax) std::swap(tzmin, tzmax);
		
		if ((tmin > tzmax) || (tzmin > tmax))
			return false;
		return true;
	}
};

//MESH MADE UP OF ONLY TRIANGLES
class mesh :public object {
public:
	std::vector<triangle*> meshTri;
	boundBox box;
	glm::dvec2 uv_cur;

	void add(triangle* object) {
		meshTri.push_back(object);
	}

	double intersect(ray& rayIn, glm::dvec3& normal_ret,  glm::dvec2& uv) {
		
		if (box.intersect(rayIn))
		{
			std::vector<triangle*>::iterator meshItr = meshTri.begin();

			glm::dvec3 int_normal;
			glm::dvec3 normal;
			glm::dvec3 uv_int;

			double minDistance = FLT_MAX;

			for (meshItr = meshTri.begin(); meshItr < meshTri.end(); meshItr++)
			{
				double intersection = (*meshItr)->intersect(rayIn, normal);
				if (intersection < minDistance && intersection > epsilon)
				{
					//std::cout << "here";
					int_normal = normal;
					minDistance = intersection;
					normal_ret = int_normal;
				}
			}
			return minDistance;
		}
		else
			return -1;
	}
	void getuv(glm::dvec3& int_point, double& u, double& v) {
		;
	}
};


#endif // !OBJECT_HPP
