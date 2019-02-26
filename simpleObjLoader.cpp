#include "simpleObjLoader.h"


bool ObjLoader::loadFromFile()
{
	vertData.clear();

	std::vector<VertexCombineIndex> vertComIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_textCoords;
	std::vector<glm::vec3> temp_normals;
	std::vector<glm::vec3> temp_tangents;
	std::vector<glm::vec3> temp_bitangents;

	std::ifstream file(path);
	if (!file)
		return false;

	std::string line;
	int faceIndex = 0;

	while (getline(file, line))
	{
		if (line.substr(0, 2) == "vt")
		{
			std::istringstream s(line.substr(2));
			glm::vec2 v;
			s >> v.x;
			s >> v.y;
			temp_textCoords.push_back(v);
		}
		else if (line.substr(0, 2) == "vn")
		{
			std::istringstream s(line.substr(2));
			glm::vec3 v;
			s >> v.x; s >> v.y; s >> v.z;
			temp_normals.push_back(v);
		}
		else if (line.substr(0, 1) == "v")
		{
			std::istringstream s(line.substr(2));
			glm::vec3 v;
			s >> v.x; s >> v.y; s >> v.z;
			temp_vertices.push_back(v);
		}
		else if (line.substr(0, 1) == "f")
		{
			std::vector<Vertex> current3VertexInfo(3);
			std::vector<VertexCombineIndex> current3VertexVertComIndex(3);
			int i = 0;

			std::istringstream vtns(line.substr(2));
			std::string vtn;
			while (vtns >> vtn)
			{
				VertexCombineIndex vertComIndex;
				std::replace(vtn.begin(), vtn.end(), '/', ' ');
				std::istringstream ivtn(vtn);

				if (vtn.find("  ") != std::string::npos)
					return false;

				ivtn >> vertComIndex.posIndex
					 >> vertComIndex.textCoordIndex
					 >> vertComIndex.normIndex;

				vertComIndex.posIndex--;
				vertComIndex.textCoordIndex--;
				vertComIndex.normIndex--;

				current3VertexVertComIndex[i].posIndex = vertComIndex.posIndex;
				current3VertexVertComIndex[i].textCoordIndex = vertComIndex.textCoordIndex;
				current3VertexVertComIndex[i].normIndex = vertComIndex.normIndex;

				current3VertexInfo[i].position = temp_vertices[current3VertexVertComIndex[i].posIndex];
				current3VertexInfo[i].texCoords = temp_textCoords[current3VertexVertComIndex[i].textCoordIndex];
				current3VertexInfo[i].normal = temp_normals[current3VertexVertComIndex[i].normIndex];

				i++;
			}

			glm::vec3 edge1 = current3VertexInfo[1].position - current3VertexInfo[0].position;
			glm::vec3 edge2 = current3VertexInfo[2].position - current3VertexInfo[0].position;
			glm::vec2 deltaUV1 = current3VertexInfo[1].texCoords - current3VertexInfo[0].texCoords;
			glm::vec2 deltaUV2 = current3VertexInfo[2].texCoords - current3VertexInfo[0].texCoords;

			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			glm::vec3 tangent, bitangent;

			tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
			tangent = glm::normalize(tangent);
			temp_tangents.push_back(tangent);

			bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
			bitangent = glm::normalize(bitangent);
			temp_bitangents.push_back(bitangent);

			current3VertexVertComIndex[0].tangentIndex = faceIndex;
			current3VertexVertComIndex[0].bitangentIndex = faceIndex;
			current3VertexVertComIndex[1].tangentIndex = faceIndex;
			current3VertexVertComIndex[1].bitangentIndex = faceIndex;
			current3VertexVertComIndex[2].tangentIndex = faceIndex;
			current3VertexVertComIndex[2].bitangentIndex = faceIndex;

			vertComIndices.push_back(current3VertexVertComIndex[0]);
			vertComIndices.push_back(current3VertexVertComIndex[1]);
			vertComIndices.push_back(current3VertexVertComIndex[2]);

			faceIndex++;
		}
		else if (line[0] == '#')
		{
		}
		else
		{
		}
	}

	for (std::vector<GLuint>::size_type i = 0; i < vertComIndices.size(); ++i)
	{
		Vertex vert;
		VertexCombineIndex comIndex = vertComIndices[i];

		vert.position = temp_vertices[comIndex.posIndex];
		vert.texCoords = temp_textCoords[comIndex.textCoordIndex];
		vert.normal = temp_normals[comIndex.normIndex];
		vert.tangent = temp_tangents[comIndex.tangentIndex];
		vert.bitangent = temp_bitangents[comIndex.bitangentIndex];

		vertData.push_back(vert);
	}

	return true;
}
