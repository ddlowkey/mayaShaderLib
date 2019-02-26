#include <stdio.h>
#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <gtc/type_ptr.hpp>

#include "class_ogl_widget.h"

#include <maya/MGlobal.h>
#include <maya/MQtUtil.h>
#include <maya/MString.h>
#include <maya/MRenderingInfo.h>
#include <maya/MFnCamera.h>
#include <maya/MAngle.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MItDag.h>
#include <maya/MMatrix.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnMesh.h>
#include <maya/MBoundingBox.h>
#include <maya/MHardwareRenderer.h>
#include <maya/MIOStream.h>
#include <maya/MDrawTraversal.h>
#include <maya/MGeometryManager.h>
#include <maya/MGeometry.h>
#include <maya/MGeometryData.h>
#include <maya/MGeometryPrimitive.h>
#include <maya/MHwTextureManager.h>
#include <maya/MImageFileInfo.h>
#include <maya/MFnSingleIndexedComponent.h>


float points[] = {
	//                     vertex1                 vertex2                 vertex3                 vertex4
	/* xpos */		+10.0, -10.0, +10.0,    +10.0, +10.0, +10.0,	+10.0, +10.0, -10.0,	+10.0, -10.0, -10.0,
	/* xneg */		-10.0, -10.0, -10.0,	-10.0, +10.0, -10.0,	-10.0, +10.0, +10.0,	-10.0, -10.0, +10.0,
	/* ypos */		-10.0, +10.0, -10.0,	+10.0, +10.0, -10.0,	+10.0, +10.0, +10.0,	-10.0, +10.0, +10.0,
	/* yneg */		-10.0, -10.0, +10.0,    +10.0, -10.0, +10.0,	+10.0, -10.0, -10.0,	-10.0, -10.0, -10.0,
	/* zpos */		+10.0, -10.0, +10.0,	-10.0, -10.0, +10.0,	-10.0, +10.0, +10.0,	+10.0, +10.0, +10.0,
	/* zneg */		-10.0, -10.0, -10.0,	+10.0, -10.0, -10.0,	+10.0, +10.0, -10.0,	-10.0, +10.0, -10.0,
};

ClassMainOglWidget::ClassMainOglWidget(QWidget* parent)
	: QOpenGLWidget(parent)
{
	setFocusPolicy(Qt::TabFocus);

	QSurfaceFormat format = QSurfaceFormat();
	format.setSamples(16);
	setFormat(format);

	m_xRot = 0;
	m_yRot = 0;
	m_zRot = 0;
	mcp_xRot = 0;
	mcp_yRot = 0;
	mcp_zRot = 0;

	fov = 45.0f;
	nearClipDis = 0.1f;
	farClipDis = 30.0f;

	diffuseWeight = 0.0f;
	diffuseRoughness = 0.0f;
	metalness = 0.0f;
	specularWeight = 0.0f;
	specularRoughness = 0.0f;
	specularIOR = 2.6315f / 100.0f * 19.0f + 1.0f;
	specularAnisotropy = 0.5f;
	specularRotation = 0.0f;
	transmissionWeight = 0.0f;
	transmissionDepth = 0.0f;
	subsurfaceWeight = 0.0f;
	subsurfaceScale = 1.0f;
	coatSpecularWeight = 0.0f;
	coatSpecularRoughness = 0.0f;
	emissionWeight = 0.0f;

	light1_intensity = 1.0f;
	light1_positionx = 0.8f;
	light1_positiony = 0.8f;
	light1_positionz = 0.8f;

	depthScale = 1.0f;

	textureID = 0;

	textureSpecularMap = 0;
	textureTransmissionMap = 0;
	textureTransmissionScatterMap = 0;
	textureSubSurfaceMap = 0;
	textureSubSurfaceRadiusMap = 0;
	textureCoatSpecularMap = 0;
	textureAlbedoMap = 0;
	textureRoughnessMap = 0;
	textureMetalnessMap = 0;
	textureEmissiveMap = 0;  
	textureNormalMap = 0;

	useSpecularMap = false;
	useTransmissionMap = false;
	useTransmissionScatterMap = false;
	useSubsurfaceMap = false;
	useSubsurfaceRadiusMap = false;
	useCoatSpecularMap = false;
	useAlbedoMap = false;
	useRoughnessMap = false;
	useMetalnessMap = false;
	useEmissiveMap = false;
	useNormalMap = false;

	textureChanged = false;

	specularMapPath = "";
	transmissionMapPath = "";
	transmissionScatterMapPath = "";
	subsurfaceMapPath = "";
	subsurfaceRadiusMapPath = "";
	coatSpecularMapPath = "";
	albedoMapPath = "";
	roughnessMapPath = "";
	metalnessMapPath = "";
	emissiveMapPath = "";
	normalMapPath = "";
	displacementMapPath = "";

	diffuseColor = QVector3D(0.62f, 0.62f, 0.62f);
	specularColor = QVector3D(0.0f, 0.0f, 0.0f);
	transmissionColor = QVector3D(1.0f, 1.0f, 1.0f);
	transmissionScatterColor = QVector3D(0.0f, 0.0f, 0.0f);
	subsurfaceColor = QVector3D(1.0f, 1.0f, 1.0f);
	subsurfaceRadiusColor = QVector3D(1.0f, 1.0f, 1.0f);
	coatSpecularColor = QVector3D(0.0f, 0.0f, 0.0f);
	emissionColor = QVector3D(0.0f, 0.0f, 0.0f);
}

ClassMainOglWidget::~ClassMainOglWidget()
{
}

void ClassMainOglWidget::initializeGL()
{
	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &ClassMainOglWidget::cleanup);

	initializeOpenGLFunctions();
	glEnable(GL_MULTISAMPLE);
	glClearColor(0, 0, 0, 1);

	myObjLoader = new ObjLoader();

	m_program = new QOpenGLShaderProgram;
	m_program_cube_map = new QOpenGLShaderProgram;
	m_program_thickness = new QOpenGLShaderProgram;
	m_program_stressMask = new QOpenGLShaderProgram;

	setupShaderFiles(m_program,
		(inputPluginPath + "/GLSL/dhsSurface.vert").c_str(),
		(inputPluginPath + "/GLSL/dhsSurface.geom").c_str(),
		(inputPluginPath + "/GLSL/dhsSurface.frag").c_str());

	setupShaderFiles(m_program_cube_map,
		(inputPluginPath + "/GLSL/defaultCubeMap.vert").c_str(),
		(inputPluginPath + "/GLSL/defaultCubeMap.frag").c_str());

	setupShaderFiles(m_program_thickness,
		(inputPluginPath + "/GLSL/dhsThickness.vert").c_str(),
		(inputPluginPath + "/GLSL/dhsThickness.frag").c_str());

	setupCubeMapTexture();

	m_vao.create();
	m_vbo.create();
	m_vao_cubeMap.create();
	m_vbo_cubeMap.create();

	m_camera.setToIdentity();
	m_camera.translate(0, 0, -1);

	setupViewModel("Shader Ball");

	glFormatUpSample.setAttachment(QOpenGLFramebufferObject::NoAttachment);
	glFormatUpSample.setMipmap(true);
	glFormatUpSample.setSamples(16);
	glFormatUpSample.setTextureTarget(GL_TEXTURE_2D);
	glFormatUpSample.setInternalTextureFormat(GL_RGBA32F_ARB);

	glFormatDownSample.setAttachment(QOpenGLFramebufferObject::NoAttachment);
	glFormatDownSample.setMipmap(true);
	glFormatDownSample.setTextureTarget(GL_TEXTURE_2D);
	glFormatDownSample.setInternalTextureFormat(GL_RGBA32F_ARB);
}

void ClassMainOglWidget::resizeGL(int w, int h)
{
	createFramebufferObject();

	m_proj.setToIdentity();
	m_proj.perspective(fov, (GLdouble)w / (GLdouble)h, nearClipDis, farClipDis);
}

void ClassMainOglWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 画厚度
	m_world.setToIdentity();
	m_world.rotate(m_xRot / 2.0f, 1, 0, 0);
	m_world.rotate(m_yRot / 2.0f, 0, 1, 0);
	m_world.rotate(m_zRot / 2.0f, 0, 0, 1);
	normalMatrix = m_world.normalMatrix();

	m_vao.bind();
	m_vbo.bind();

	m_program_thickness->bind();
	m_program_thickness->setUniformValue(m_program_thickness->uniformLocation("projMatrix"), m_proj);
	m_program_thickness->setUniformValue(m_program_thickness->uniformLocation("mvMatrix"), m_camera * m_world);
	m_program_thickness->setUniformValue(m_program_thickness->uniformLocation("normalMatrix"), normalMatrix);

	createFramebufferObject();

	m_fboUpSample->bind();
	glViewport(0, 0, width(), height());
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glCullFace(GL_BACK);
	m_program_thickness->setUniformValue(m_program_thickness->uniformLocation("depthScale"), depthScale);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)myObjLoader->vertData.size());
	glCullFace(GL_FRONT);
	m_program_thickness->setUniformValue(m_program_thickness->uniformLocation("depthScale"), -depthScale);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)myObjLoader->vertData.size());

	m_vbo.release();
	m_vao.release();
	m_fboUpSample->release();
	m_program_thickness->release();

	glActiveTexture(GL_TEXTURE31);
	glBindTexture(GL_TEXTURE_2D, m_fboDownSample->texture());
	QOpenGLFramebufferObject::blitFramebuffer(m_fboDownSample,
		m_fboUpSample,
		GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT |
		GL_STENCIL_BUFFER_BIT,
		GL_NEAREST);

	glClear(GL_COLOR_BUFFER_BIT);

	// 画天空盒
	//
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	m_world.setToIdentity();
	m_world.rotate(mcp_xRot / 2.0f, 1, 0, 0);
	m_world.rotate(mcp_yRot / 2.0f, 0, 1, 0);
	m_world.rotate(mcp_zRot / 2.0f, 0, 0, 1);
	glDepthFunc(GL_LEQUAL);

	m_vao_cubeMap.bind();
	m_vbo_cubeMap.bind();
	m_vbo_cubeMap.setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_vbo_cubeMap.allocate(&points, sizeof(points));

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)(sizeof(float) * 0));
	glEnableVertexAttribArray(0);

	m_program_cube_map->bind();
	m_program_cube_map->setUniformValue(m_program_cube_map->uniformLocation("projMatrix"), m_proj);
	m_program_cube_map->setUniformValue(m_program_cube_map->uniformLocation("mvMatrix"), m_camera * m_world);
	normalMatrix = m_world.normalMatrix();
	m_program_cube_map->setUniformValue(m_program_cube_map->uniformLocation("normalMatrix"), normalMatrix);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glCullFace(GL_FRONT);
	glDrawArrays(GL_QUADS, 0, 24);

	m_vbo_cubeMap.release();
	m_vao_cubeMap.release();
	m_program_cube_map->release();

	// 画模型
	//
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	m_world.setToIdentity();
	m_world.rotate(m_xRot / 2.0f, 1, 0, 0);
	m_world.rotate(m_yRot / 2.0f, 0, 1, 0);
	m_world.rotate(m_zRot / 2.0f, 0, 0, 1);
	normalMatrix = m_world.normalMatrix();

	m_vao.bind();
	m_vbo.bind();

	if (textureChanged)
	{
		loadTextures();
		textureChanged = false;
	}

	m_program->bind();
	m_program->setUniformValue(m_program->uniformLocation("projMatrix"), m_proj);
	m_program->setUniformValue(m_program->uniformLocation("mvMatrix"), m_camera * m_world);
	m_program->setUniformValue(m_program->uniformLocation("normalMatrix"), normalMatrix);

	m_program->setUniformValue(m_program->uniformLocation("cubeMapWorldRotY_angle"), (float)mcp_yRot);

	m_program->setUniformValue(m_program->uniformLocation("diffuseWeight"), diffuseWeight);
	m_program->setUniformValue(m_program->uniformLocation("diffuseRoughness"), diffuseRoughness);
	m_program->setUniformValue(m_program->uniformLocation("metalness"), metalness);
	m_program->setUniformValue(m_program->uniformLocation("specularWeight"), specularWeight);
	m_program->setUniformValue(m_program->uniformLocation("specularRoughness"), specularRoughness);
	m_program->setUniformValue(m_program->uniformLocation("specularIOR"), specularIOR);
	m_program->setUniformValue(m_program->uniformLocation("specularAnisotropy"), specularAnisotropy);
	m_program->setUniformValue(m_program->uniformLocation("specularRotation"), specularRotation);
	m_program->setUniformValue(m_program->uniformLocation("transmissionWeight"), transmissionWeight);
	m_program->setUniformValue(m_program->uniformLocation("transmissionDepth"), transmissionDepth);
	m_program->setUniformValue(m_program->uniformLocation("subsurfaceWeight"), subsurfaceWeight);
	m_program->setUniformValue(m_program->uniformLocation("subsurfaceScale"), subsurfaceScale);
	m_program->setUniformValue(m_program->uniformLocation("coatSpecularWeight"), coatSpecularWeight);
	m_program->setUniformValue(m_program->uniformLocation("coatSpecularRoughness"), coatSpecularRoughness);
	m_program->setUniformValue(m_program->uniformLocation("emissionWeight"), emissionWeight);

	m_program->setUniformValue(m_program->uniformLocation("light1_intensity"), light1_intensity);
	m_program->setUniformValue(m_program->uniformLocation("light1_positionx"), light1_positionx);
	m_program->setUniformValue(m_program->uniformLocation("light1_positiony"), light1_positiony);
	m_program->setUniformValue(m_program->uniformLocation("light1_positionz"), light1_positionz);

	m_program->setUniformValue(m_program->uniformLocation("diffuseColor"), diffuseColor);
	m_program->setUniformValue(m_program->uniformLocation("specularColor"), specularColor);
	m_program->setUniformValue(m_program->uniformLocation("transmissionColor"), transmissionColor);
	m_program->setUniformValue(m_program->uniformLocation("transmissionScatterColor"), transmissionScatterColor);
	m_program->setUniformValue(m_program->uniformLocation("subsurfaceColor"), subsurfaceColor);
	m_program->setUniformValue(m_program->uniformLocation("subsurfaceRadiusColor"), subsurfaceRadiusColor);
	m_program->setUniformValue(m_program->uniformLocation("coatSpecularColor"), coatSpecularColor);
	m_program->setUniformValue(m_program->uniformLocation("emissionColor"), emissionColor);

	glUniform1i(glGetUniformLocation(m_program->programId(), "thicknessMap"), 31);

	m_program->setUniformValue(m_program->uniformLocation("Size"), width(), height());

	m_program->setUniformValue(m_program->uniformLocation("useSpecularMap"), (int)useSpecularMap);
	m_program->setUniformValue(m_program->uniformLocation("useTransmissionMap"), (int)useTransmissionMap);
	m_program->setUniformValue(m_program->uniformLocation("useTransmissionScatterMap"), (int)useTransmissionScatterMap);
	m_program->setUniformValue(m_program->uniformLocation("useSubsurfaceMap"), (int)useSubsurfaceMap);
	m_program->setUniformValue(m_program->uniformLocation("useSubsurfaceRadiusMap"), (int)useSubsurfaceRadiusMap);
	m_program->setUniformValue(m_program->uniformLocation("useCoatSpecularMap"), (int)useCoatSpecularMap);
	m_program->setUniformValue(m_program->uniformLocation("useAlbedoMap"), (int)useAlbedoMap);
	m_program->setUniformValue(m_program->uniformLocation("useRoughnessMap"), (int)useRoughnessMap);
	m_program->setUniformValue(m_program->uniformLocation("useMetalnessMap"), (int)useMetalnessMap);
	m_program->setUniformValue(m_program->uniformLocation("useEmissiveMap"), (int)useEmissiveMap);
	m_program->setUniformValue(m_program->uniformLocation("useNormalMap"), (int)useNormalMap);
	m_program->setUniformValue(m_program->uniformLocation("useDisplacementMap"), (int)useDisplacementMap);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_FRONT);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)myObjLoader->vertData.size());
	glCullFace(GL_BACK);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)myObjLoader->vertData.size());

	m_vbo.release();
	m_vao.release();
	m_program->release();
}

void ClassMainOglWidget::createFramebufferObject()
{
	if (m_fboUpSample)
	{
		delete m_fboUpSample;
	}

	if (m_fboDownSample)
	{
		delete m_fboDownSample;
	}

	m_fboUpSample = new QOpenGLFramebufferObject(width(), height(), glFormatUpSample);
	m_fboDownSample = new QOpenGLFramebufferObject(width(), height(), glFormatDownSample);
}

void ClassMainOglWidget::loadTextures()
{
	m_program->bind();

	loadTextureMap(GL_TEXTURE1, textureAlbedoMap, "textureAlbedoMap", albedoMapPath, m_program, 1);
	loadTextureMap(GL_TEXTURE2, textureRoughnessMap, "textureRoughnessMap", roughnessMapPath, m_program, 2);
	loadTextureMap(GL_TEXTURE3, textureMetalnessMap, "textureMetalnessMap", metalnessMapPath, m_program, 3);
	loadTextureMap(GL_TEXTURE4, textureEmissiveMap, "textureEmissiveMap", emissiveMapPath, m_program, 4);
	loadTextureMap(GL_TEXTURE5, textureNormalMap, "textureNormalMap", normalMapPath, m_program, 5);
	loadTextureMap(GL_TEXTURE6, textureDisplacementMap, "textureDisplacementMap", displacementMapPath, m_program, 6);
	loadTextureMap(GL_TEXTURE7, textureSpecularMap, "textureSpecularMap", specularMapPath, m_program, 7);
	loadTextureMap(GL_TEXTURE8, textureTransmissionMap, "textureTransmissionMap", transmissionMapPath, m_program, 8);
	loadTextureMap(GL_TEXTURE9, textureTransmissionScatterMap, "textureTransmissionScatterMap", transmissionScatterMapPath, m_program, 9);
	loadTextureMap(GL_TEXTURE10, textureSubSurfaceMap, "textureSubSurfaceMap", subsurfaceMapPath, m_program, 10);
	loadTextureMap(GL_TEXTURE11, textureSubSurfaceRadiusMap, "textureSubSurfaceRadiusMap", subsurfaceRadiusMapPath, m_program, 11);
	loadTextureMap(GL_TEXTURE12, textureCoatSpecularMap, "textureCoatSpecularMap", coatSpecularMapPath, m_program, 12);

	m_program->release();
}

void ClassMainOglWidget::loadTextureMap(GLenum _texID, GLuint _texMapName,
	const char* uniformName, QString texPath,
	QOpenGLShaderProgram* curProgram, GLint texIndex)
{
	glDeleteTextures(1, &_texMapName);
	glGenTextures(1, &_texMapName);
	glActiveTexture(_texID);
	glBindTexture(GL_TEXTURE_2D, _texMapName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	std::string str;
	str = texPath.toStdString();
	const char* _charPath = str.c_str();

	unsigned char *data = stbi_load(_charPath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);
	glUniform1i(glGetUniformLocation(curProgram->programId(), uniformName), texIndex);
}

void ClassMainOglWidget::setupShaderFiles(QOpenGLShaderProgram* shaderProgram,
	const char* vertexShaderPath,
	const char* fragmentShaderPath)
{
	shaderProgram->removeAllShaders();
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderPath);
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderPath);
	MGlobal::displayInfo("::Read error: " + MQtUtil::toMString(shaderProgram->log()));
	shaderProgram->link();
	MGlobal::displayInfo("::Link error: " + MQtUtil::toMString(shaderProgram->log()));
}

void ClassMainOglWidget::setupShaderFiles(QOpenGLShaderProgram* shaderProgram,
	const char* vertexShaderPath,
	const char* geometryShaderPath,
	const char* fragmentShaderPath)
{
	shaderProgram->removeAllShaders();
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderPath);
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Geometry, geometryShaderPath);
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderPath);
	MGlobal::displayInfo("::Read error: " + MQtUtil::toMString(shaderProgram->log()));
	shaderProgram->link();
	MGlobal::displayInfo("::Link error: " + MQtUtil::toMString(shaderProgram->log()));
}

void ClassMainOglWidget::setupCubeMapTexture()
{
	facesHdrPath =
	{
		inputPluginPath + "/hdr/posx.jpg", // right
		inputPluginPath + "/hdr/negx.jpg", // left
		inputPluginPath + "/hdr/posy.jpg", // top
		inputPluginPath + "/hdr/negy.jpg", // bottom
		inputPluginPath + "/hdr/posz.jpg", // back
		inputPluginPath + "/hdr/negz.jpg"  // front
	};

	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	int x, y, n;
	int force_channels = 4;
	for (unsigned int i = 0; i < facesHdrPath.size(); i++)
	{
		unsigned char* image_data = stbi_load(facesHdrPath[i].c_str(), &x, &y, &n, force_channels);
		if (image_data)
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGBA,
				x, y,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				image_data);
		}
		stbi_image_free(image_data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void ClassMainOglWidget::setupViewModel(QString modelPath)
{
	QString inputString = QString(inputPluginPath.c_str()) + "/viewObj/" + modelPath + ".obj";
	std::string s = inputString.toStdString();
	myObjLoader->path = s.c_str();
	myObjLoader->loadFromFile();

	m_vao.bind();
	m_vbo.bind();
	m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_vbo.allocate(&myObjLoader->vertData[0], (int)(myObjLoader->vertData.size() * sizeof(Vertex)));

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(5 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(8 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(11 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(4);

	m_vbo.release();
	m_vao.release();

	update();
}

void ClassMainOglWidget::mousePressEvent(QMouseEvent *event)
{
	m_lastPos = event->pos();
}

void ClassMainOglWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - m_lastPos.x();

	if ((event->buttons() & Qt::LeftButton) == Qt::LeftButton)
		m_yRot += (float)((dx * 180.0) / 500.0);

	if ((event->buttons() & Qt::RightButton) == Qt::RightButton)
		mcp_yRot += (float)((dx * 180.0) / 500.0);

	m_lastPos = event->pos();
	update();
}

void ClassMainOglWidget::cleanup()
{
	makeCurrent();

	m_vbo.destroy();
	m_vao.destroy();
	m_vbo_cubeMap.destroy();
	m_vao_cubeMap.destroy();

	delete m_program; m_program = 0;
	delete m_program_cube_map; m_program_cube_map = 0;
	delete m_program_thickness; m_program_thickness = 0;

	doneCurrent();
}

void ClassMainOglWidget::on_lineEdit_textChanged(const char* _inParam, float _value)
{
	if (_inParam == "diffuseWeight")
	{
		diffuseWeight = _value;
	}
	if (_inParam == "diffuseRoughness")
	{
		diffuseRoughness = _value;
	}
	if (_inParam == "metalness")
	{
		metalness = _value;
	}
	if (_inParam == "specularWeight")
	{
		specularWeight = _value;
	}
	if (_inParam == "specularRoughness")
	{
		specularRoughness = _value;
	}
	if (_inParam == "specularIOR")
	{
		specularIOR = _value;
	}
	if (_inParam == "specularAnisotropy")
	{
		specularAnisotropy = _value;
	}
	if (_inParam == "specularRotation")
	{
		specularRotation = _value;
	}
	if (_inParam == "transmissionWeight")
	{
		transmissionWeight = _value;
	}
	if (_inParam == "transmissionDepth")
	{
		transmissionDepth = _value;
	}
	if (_inParam == "subsurfaceWeight")
	{
		subsurfaceWeight = _value;
	}
	if (_inParam == "subsurfaceScale")
	{
		subsurfaceScale = _value;
	}
	if (_inParam == "coatSpecularWeight")
	{
		coatSpecularWeight = _value;
	}
	if (_inParam == "coatSpecularRoughness")
	{
		coatSpecularRoughness = _value;
	}
	if (_inParam == "emissionWeight")
	{
		subsurfaceScale = _value;
	}
	if (_inParam == "light1_intensity")
	{
		light1_intensity = _value;
	}
	if (_inParam == "light1_positionx")
	{
		light1_positionx = _value;
	}
	if (_inParam == "light1_positiony")
	{
		light1_positiony = _value;
	}
	if (_inParam == "light1_positionz")
	{
		light1_positionz = _value;
	}

	update();
}

void ClassMainOglWidget::on_lineEdit_specularMapPath_textChanged(QString _inPath)
{
	std::string str;
	str = _inPath.toStdString();
	const char* _charPath = str.c_str();

	std::fstream _file;
	_file.open(_charPath, ios::in);
	if (_file)
	{
		specularMapPath = _inPath;
		useAlbedoMap = true;
		textureChanged = true;
		update();
	}
	else
	{
		specularMapPath = QString("");
		useAlbedoMap = false;
	}
}

void ClassMainOglWidget::on_lineEdit_transmissionMapPath_textChanged(QString _inPath)
{
	std::string str;
	str = _inPath.toStdString();
	const char* _charPath = str.c_str();

	std::fstream _file;
	_file.open(_charPath, ios::in);
	if (_file)
	{
		transmissionMapPath = _inPath;
		useTransmissionMap = true;
		textureChanged = true;
		update();
	}
	else
	{
		transmissionMapPath = QString("");
		useTransmissionMap = false;
	}
}

void ClassMainOglWidget::on_lineEdit_transmissionScatterMapPath_textChanged(QString _inPath)
{
	std::string str;
	str = _inPath.toStdString();
	const char* _charPath = str.c_str();

	std::fstream _file;
	_file.open(_charPath, ios::in);
	if (_file)
	{
		transmissionScatterMapPath = _inPath;
		useTransmissionScatterMap = true;
		textureChanged = true;
		update();
	}
	else
	{
		transmissionScatterMapPath = QString("");
		useTransmissionScatterMap = false;
	}
}

void ClassMainOglWidget::on_lineEdit_subsurfaceColorMapPath_textChanged(QString _inPath)
{
	std::string str;
	str = _inPath.toStdString();
	const char* _charPath = str.c_str();

	std::fstream _file;
	_file.open(_charPath, ios::in);
	if (_file)
	{
		subsurfaceMapPath = _inPath;
		useSubsurfaceMap = true;
		textureChanged = true;
		update();
	}
	else
	{
		subsurfaceMapPath = QString("");
		useSubsurfaceMap = false;
	}
}

void ClassMainOglWidget::on_lineEdit_subsurfaceRadiusMapPath_textChanged(QString _inPath)
{
	std::string str;
	str = _inPath.toStdString();
	const char* _charPath = str.c_str();

	std::fstream _file;
	_file.open(_charPath, ios::in);
	if (_file)
	{
		subsurfaceRadiusMapPath = _inPath;
		useSubsurfaceRadiusMap = true;
		textureChanged = true;
		update();
	}
	else
	{
		subsurfaceRadiusMapPath = QString("");
		useSubsurfaceRadiusMap = false;
	}
}

void ClassMainOglWidget::on_lineEdit_albedoMapPath_textChanged(QString _inPath)
{
	std::string str;
	str = _inPath.toStdString();
	const char* _charPath = str.c_str();

	std::fstream _file;
	_file.open(_charPath, ios::in);
	if (_file)
	{
		albedoMapPath = _inPath;
		useAlbedoMap = true;
		textureChanged = true;
		update();
	}
	else
	{
		albedoMapPath = QString("");
		useAlbedoMap = false;
	}
}

void ClassMainOglWidget::on_lineEdit_roughnessMapPath_textChanged(QString _inPath)
{
	std::string str;
	str = _inPath.toStdString();
	const char* _charPath = str.c_str();

	std::fstream _file;
	_file.open(_charPath, ios::in);
	if (_file)
	{
		roughnessMapPath = _inPath;
		useRoughnessMap = true;
		textureChanged = true;
		update();
	}
	else
	{
		roughnessMapPath = QString("");
		useRoughnessMap = false;
	}
}

void ClassMainOglWidget::on_lineEdit_metalnessMapPath_textChanged(QString _inPath)
{
	std::string str;
	str = _inPath.toStdString();
	const char* _charPath = str.c_str();

	std::fstream _file;
	_file.open(_charPath, ios::in);
	if (_file)
	{
		metalnessMapPath = _inPath;
		useMetalnessMap = true;
		textureChanged = true;
		update();
	}
	else
	{
		metalnessMapPath = QString("");
		useMetalnessMap = false;
	}
}

void ClassMainOglWidget::on_lineEdit_emissiveMapPath_textChanged(QString _inPath)
{
	std::string str;
	str = _inPath.toStdString();
	const char* _charPath = str.c_str();

	std::fstream _file;
	_file.open(_charPath, ios::in);
	if (_file)
	{
		emissiveMapPath = _inPath;
		useEmissiveMap = true;
		textureChanged = true;
		update();
	}
	else
	{
		emissiveMapPath = QString("");
		useEmissiveMap = false;
	}
}

void ClassMainOglWidget::on_lineEdit_normalMapPath_textChanged(QString _inPath)
{
	std::string str;
	str = _inPath.toStdString();
	const char* _charPath = str.c_str();

	std::fstream _file;
	_file.open(_charPath, ios::in);
	if (_file)
	{
		normalMapPath = _inPath;
		useNormalMap = true;
		textureChanged = true;
		update();
	}
	else
	{
		normalMapPath = QString("");
		useNormalMap = false;
	}
}

void ClassMainOglWidget::on_lineEdit_displacementMapPath_textChanged(QString _inPath)
{
	std::string str;
	str = _inPath.toStdString();
	const char* _charPath = str.c_str();

	std::fstream _file;
	_file.open(_charPath, ios::in);
	if (_file)
	{
		displacementMapPath = _inPath;
		useDisplacementMap = true;
		textureChanged = true;
		update();
	}
	else
	{
		displacementMapPath = QString("");
		useDisplacementMap = false;
	}
}

void ClassMainOglWidget::on_diffuseColor_valueChanged(QColor _inColor)
{
	diffuseColor = QVector3D(_inColor.red(), _inColor.green(), _inColor.blue()) / 255.0;
	update();
}

void ClassMainOglWidget::on_specularColor_valueChanged(QColor _inColor)
{
	specularColor = QVector3D(_inColor.red(), _inColor.green(), _inColor.blue()) / 255.0;
	update();
}

void ClassMainOglWidget::on_transmissionColor_valueChanged(QColor _inColor)
{
	transmissionColor = QVector3D(_inColor.red(), _inColor.green(), _inColor.blue()) / 255.0;
	update();
}

void ClassMainOglWidget::on_transmissionScatterColor_valueChanged(QColor _inColor)
{
	transmissionScatterColor = QVector3D(_inColor.red(), _inColor.green(), _inColor.blue()) / 255.0;
	update();
}

void ClassMainOglWidget::on_subsurfaceColor_valueChanged(QColor _inColor)
{
	subsurfaceColor = QVector3D(_inColor.red(), _inColor.green(), _inColor.blue()) / 255.0;
	update();
}

void ClassMainOglWidget::on_subsurfaceRadiusColor_valueChanged(QColor _inColor)
{
	subsurfaceRadiusColor = QVector3D(_inColor.red(), _inColor.green(), _inColor.blue()) / 255.0;
	update();
}

void ClassMainOglWidget::on_coatSpecularColor_valueChanged(QColor _inColor)
{
	coatSpecularColor = QVector3D(_inColor.red(), _inColor.green(), _inColor.blue()) / 255.0;
	update();
}

void ClassMainOglWidget::on_emissionColor_valueChanged(QColor _inColor)
{
	emissionColor = QVector3D(_inColor.red(), _inColor.green(), _inColor.blue()) / 255.0;
	update();
}

void ClassMainOglWidget::on_pushButton_updateShader_sendData(QString _vs, QString _fs, QString _gs)
{
	vsStr = _vs.toStdString();
	vertexShaderPathString = vsStr.c_str();
	gsStr = _gs.toStdString();
	geometryShaderPathString = gsStr.c_str();
	fsStr = _fs.toStdString();
	fragmentShaderPathString = fsStr.c_str();
	setupShaderFiles(m_program, vertexShaderPathString, fragmentShaderPathString, geometryShaderPathString);
	textureChanged = true;
	update();
}
