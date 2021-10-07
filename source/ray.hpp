#ifndef RAY_HPP
#define RAY_HPP

#include <glm/glm.hpp>

class ray {
public:
	glm::dvec3 origin;
	glm::dvec3 direction;

	void raythrough(double cam[], double i, double j, double width, double height) {

		//Camera is vec3:looking_from vec3:looking_at vec3:up_vector double:fovy

		origin = glm::dvec3(cam[0], cam[1], cam[2]); //Ray origin

		glm::dvec3 a = glm::dvec3(cam[0], cam[1], cam[2]) - glm::dvec3(cam[3], cam[4], cam[5]);   //Where to look
		glm::dvec3 b = glm::dvec3(cam[6], cam[7], cam[8]);  //Up vector

		//Creating coordiante frame
		glm::dvec3 w = glm::normalize(a);
		glm::dvec3 u = glm::normalize(glm::cross(b, w));
		glm::dvec3 v = glm::cross(w, u);

		double fovx = 2 * glm::atan(tan(glm::radians(cam[9]) * 0.5) * (width / height));
		double fovy = glm::radians(cam[9]);


		double ang_alpha = glm::tan(fovx / 2) * (j - width / 2) / (width / 2);
		double ang_beta = glm::tan(fovy / 2) * (height / 2 - i) / (height / 2);

		direction = glm::normalize(glm::dvec3(ang_alpha * u + ang_beta * v - w)); //Ray direction
	}
};

#endif // !RAY_HPP
