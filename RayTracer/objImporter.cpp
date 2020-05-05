/**
	* File:    objImporter.cpp
	*
	* Author1:  Akshay Tiwari (2018A7PS0201H)
	* Author2:  Mushkan Surekha  (2018B5A70477H)
	* Author3:  Mahesh Swaminathan  (2018A4PS0982H)
	* Date:     09-04-2020
	* Course:   Computer Graphics (IS F311)
	*
	* Summary of File:
	*
	*   Program to import an obj file into required vectors
	*	by using a simple parser.
*/
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
/**
	*   bool loadOBJ(const char* path,std::vector<glm::dvec3>& out_vertices,std::vector<glm::dvec2>& out_uvs,std::vector<glm::dvec3>& out_normals)
	*
	*   Summary of loadOBJ function:
	*
	*       Reads a .obj file and pushes vertices, uvs and normals to respective vectors
	*
	*   Parameters  :
	*
	*		const char* path - Path of the obj file
	*		std::vector<glm::dvec3>& out_vertices - Vertex array (contains all the vertices)
	*		std::vector<glm::dvec2>& out_uvs - UV arrat (contains all the UVs)
	*		std::vector<glm::dvec3>& out_normals) - Normal array (contains all the normals)
	*
	*   Return Value :
	*
	*		Boolean value - True if file read successfully; else False
	*
	*   Description :
	*
	*       loadOBJ is simple file parser which reads .obj files and stores the vertices, uvs and normals
	*		in their respective vectors.
	*		This is a very simple file parser and can't read complex files.
*/
bool loadOBJ(const char* path,Scene* scene) {

	printf("Loading OBJ file %s...\n", path);
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::dvec3> temp_vertices;
	std::vector<glm::dvec2> temp_uvs;
	std::vector<glm::dvec3> temp_normals;

	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
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
			uv.y = -uv.y; // DDS texture are inverted.
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
				printf("File can't be read by our simple parser. Try exporting with other options\n");
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
	//double xmin=FLT_MIN, ymin = FLT_MIN, zmin = FLT_MIN;
	//double xmax = FLT_MAX, ymax = FLT_MAX, zmax= FLT_MAX;
	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i+=3) {

		// Get the indices of its attributes
		unsigned int vertexIndex1 = vertexIndices[i];
		unsigned int vertexIndex2 = vertexIndices[i+1];
		unsigned int vertexIndex3 = vertexIndices[i+2];
		unsigned int uvIndex1 = uvIndices[i];
		unsigned int uvIndex2 = uvIndices[i+1];
		unsigned int uvIndex3 = uvIndices[i+2];
		unsigned int normalIndex1 = normalIndices[i];
		unsigned int normalIndex2 = normalIndices[i+1];
		unsigned int normalIndex3 = normalIndices[i+2];

		// Get the attributes thanks to the index
		glm::dvec3 vertex1 = temp_vertices[vertexIndex1 - 1];
		glm::dvec3 vertex2 = temp_vertices[vertexIndex2 - 1];
		glm::dvec3 vertex3 = temp_vertices[vertexIndex3 - 1];

		glm::dvec2 uv1 = temp_uvs[uvIndex1 - 1];
		glm::dvec2 uv2 = temp_uvs[uvIndex2 - 1];
		glm::dvec2 uv3 = temp_uvs[uvIndex3 - 1];

		glm::dvec3 normal1 = temp_normals[normalIndex1 - 1];
		glm::dvec3 normal2 = temp_normals[normalIndex2 - 1];
		glm::dvec3 normal3 = temp_normals[normalIndex3 - 1];


		// Generate triangle and calculate normal
		triangle* temptri = new triangle();
		temptri->v1 = vertex1;
		temptri->v2 = vertex2;
		temptri->v3 = vertex3;
		//std::cout << vertex1[0] << " " << vertex1[1] << " " << vertex1[2] << "\n" << vertex2[0] << " " << vertex2[1] << " " << vertex2[2] << "\n" << vertex3[0] << " " << vertex3[1] << " " << vertex3[2] << "\n";
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

		meshtmp->add(temptri);
	}
	//Set Other parameters
	meshtmp->ambient = glm::dvec3(0.3, 0.6, 0.5);
	meshtmp->reflectivity = 0.5;
	meshtmp->emission = glm::dvec3(0, 0, 0);
	std::cout << meshtmp->box.boundMax[0] << " " << meshtmp->box.boundMax[1] << " " << meshtmp->box.boundMax[2] << " \n";
	std::cout << meshtmp->box.boundMin[0] << " " << meshtmp->box.boundMin[1] << " " << meshtmp->box.boundMin[2] << " \n";
	scene->add(meshtmp);
	std::cout << meshtmp->meshTri.size();
	fclose(file);
	return true;
}