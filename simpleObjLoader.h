#ifndef SIMPLE_OBJ_LOADER_H_
#define SIMPLE_OBJ_LOADER_H_

#include <string>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/string_cast.hpp>
#include <string>       
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <maya/MGlobal.h>


struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoords;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

struct VertexCombineIndex
{
	GLuint posIndex;
	GLuint textCoordIndex;
	GLuint normIndex;
	GLuint tangentIndex;
	GLuint bitangentIndex;
};

class ObjLoader
{
public:
	const char*           path;
	std::vector<Vertex>   vertData;

	bool                  loadFromFile();
};

#endif
