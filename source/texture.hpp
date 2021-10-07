#ifndef TEXTURE_H
#define TEXTURE_H

// a big refactoring of colors
class texture {
public:
	virtual glm::dvec3 value(double u, double v, const glm::dvec3& p) const = 0;
};
#endif //TEXTURE_H