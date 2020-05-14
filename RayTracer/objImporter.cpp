#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <glm/glm.hpp>
#include "objimporter.hpp"
#include "scene.hpp"
#include "object.hpp"

using namespace std;

bool loadOBJ(const char* path,Scene* scene) {

	printf("Loading OBJ file %s...\n", path);
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::dvec3> temp_vertices;
	std::vector<glm::dvec2> temp_uvs;
	std::vector<glm::dvec3> temp_normals;

	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("File not found.\n");
		getchar();
		return false;
	}

	while (true) {
		char lineHeader[128];

		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "v") == 0) {
			glm::dvec3 vertex;
			fscanf(file, "%lf %lf %lf\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::dvec2 uv;
			fscanf(file, "%lf %lf\n", &uv.x, &uv.y);
			uv.y = uv.y;
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::dvec3 normal;
			fscanf(file, "%lf %lf %lf\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read.\n");
				fclose(file);
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else {
			char comment[1000];
			fgets(comment, 1000, file);
		}
	}
	mesh* meshtmp = new mesh();
	meshtmp->box.boundMax = glm::dvec3(FLT_MIN);
	meshtmp->box.boundMin = glm::dvec3(FLT_MAX);
	
	for (unsigned int i = 0; i < vertexIndices.size(); i+=3) {

		unsigned int vertexIndex1 = vertexIndices[i];
		unsigned int vertexIndex2 = vertexIndices[i+1];
		unsigned int vertexIndex3 = vertexIndices[i+2];
		unsigned int uvIndex1 = uvIndices[i];
		unsigned int uvIndex2 = uvIndices[i+1];
		unsigned int uvIndex3 = uvIndices[i+2];
		unsigned int normalIndex1 = normalIndices[i];
		unsigned int normalIndex2 = normalIndices[i+1];
		unsigned int normalIndex3 = normalIndices[i+2];


		glm::dvec3 vertex1 = temp_vertices[vertexIndex1 - 1];
		glm::dvec3 vertex2 = temp_vertices[vertexIndex2 - 1];
		glm::dvec3 vertex3 = temp_vertices[vertexIndex3 - 1];

		glm::dvec2 uv1 = temp_uvs[uvIndex1 - 1];
		glm::dvec2 uv2 = temp_uvs[uvIndex2 - 1];
		glm::dvec2 uv3 = temp_uvs[uvIndex3 - 1];

		glm::dvec3 normal1 = temp_normals[normalIndex1 - 1];
		glm::dvec3 normal2 = temp_normals[normalIndex2 - 1];
		glm::dvec3 normal3 = temp_normals[normalIndex3 - 1];

		triangle* temptri = new triangle();
		temptri->v1 = vertex1;
		temptri->v2 = vertex2;
		temptri->v3 = vertex3;
		
		glm::dvec3 edge1 = vertex2 - vertex1;
		glm::dvec3 edge2 = vertex3 - vertex1;

		temptri->normal = glm::normalize(glm::cross(edge1, edge2));
		
		glm::dvec3 avgVecNormal = (normal1 + normal2 + normal3) / 3.0;
		if (glm::dot(temptri->normal, avgVecNormal) < 0) temptri->normal = -temptri->normal;
		
		for (int i = 0;i < 3;i++)
		{
			if (vertex1[i] <= meshtmp->box.boundMin[i]) meshtmp->box.boundMin[i] = vertex1[i];
			if (vertex2[i] <= meshtmp->box.boundMin[i]) meshtmp->box.boundMin[i] = vertex2[i];
			if (vertex3[i] <= meshtmp->box.boundMin[i]) meshtmp->box.boundMin[i] = vertex3[i];

			if (vertex1[i] > meshtmp->box.boundMax[i]) meshtmp->box.boundMax[i] = vertex1[i];
			if (vertex2[i] > meshtmp->box.boundMax[i]) meshtmp->box.boundMax[i] = vertex2[i];
			if (vertex3[i] > meshtmp->box.boundMax[i]) meshtmp->box.boundMax[i] = vertex3[i];
		}
		temptri->uv_v1 = uv1;
		temptri->uv_v2 = uv2;
		temptri->uv_v3 = uv3;
		meshtmp->add(temptri);
	}

	meshtmp->transparency = 0;
	
	int inpRefl , inpShi, inpIsTex;
	glm::dvec3 inpDif, inpSpec;
	glm::dvec3 inpEmi = glm::dvec3(0, 0, 0);

	std::cout<< "Is mesh reflective ? 0 : 1 == ";
	std::cin >> inpRefl;

	std::cout << "Is mesh textured ? 0 : 1 == ";
	std::cin >> inpIsTex;

	meshtmp->isTextured = inpIsTex;

	if (!inpIsTex) {
		std::cout << "Enter mesh's emissive color  rgb : ";
		std::cin >> inpEmi[0] >> inpEmi[1] >> inpEmi[2];
	}

	std::cout << "Enter mesh's diffuse color  rgb : ";
	std::cin >> inpDif[0] >> inpDif[1] >> inpDif[2];

	std::cout << "Enter mesh's specular color  rgb : ";
	std::cin >> inpSpec[0] >> inpSpec[1] >> inpSpec[2];

	std::cout << "Enter mesh's shininess : ";
	std::cin >> inpShi;

	meshtmp->reflectivity = inpRefl;
	meshtmp->emission = inpEmi;
	
	meshtmp->diffuse = inpDif;
	meshtmp->shininess = .2;
	meshtmp->specular = glm::dvec3(.45, .31, .77);
	meshtmp->isTextured = 1;

	scene->add(meshtmp);

	fclose(file);
	return true;
}