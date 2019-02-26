#ifndef CLASSOGLWIDGET_H
#define CLASSOGLWIDGET_H

#include "simpleObjLoader.h"

#include <QtCore/QString>
#include <QtOpenGL/QtOpenGL>
#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QSurfaceFormat>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLFrameBufferObject>
#include <QtGui/QOpenGLFramebufferObjectFormat>
#include <QtGui/QMatrix4x4>
#include <QtWidgets/QComboBox>
#include <Qt3dInput/QMouseEvent>
#include <QtGui/QOpenGLShaderProgram>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtGui/QDoubleValidator>

#include <maya/MCommonSystemUtils.h>


QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class ClassMainOglWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT;

public:
	ClassMainOglWidget(QWidget* parent = Q_NULLPTR);
	~ClassMainOglWidget();

	QWidget*                   mainWidget;
	std::string                inputPluginPath;

	void                       cleanup();
	void                       loadTextureMap(GLenum _texID,
		                                      GLuint _texMapName,
		                                      const char* uniformName,
		                                      QString texPath,
		                                      QOpenGLShaderProgram* curProgram,
		                                      GLint texIndex);

	float                      diffuseWeight,
		                       diffuseRoughness,
		                       metalness,
		                       specularWeight,
		                       specularRoughness,
		                       specularIOR,
		                       specularAnisotropy,
		                       specularRotation,
		                       transmissionWeight,
		                       transmissionDepth,
							   subsurfaceWeight,
							   subsurfaceScale,
		                       coatSpecularWeight,
		                       coatSpecularRoughness,
		                       emissionWeight,

							   light1_intensity,
							   light1_positionx,
							   light1_positiony,
							   light1_positionz,

		                       depthScale;

	QVector3D                  diffuseColor,
		                       specularColor,
		                       transmissionColor,
		                       transmissionScatterColor,
							   subsurfaceColor,
							   subsurfaceRadiusColor,
		                       coatSpecularColor,
		                       emissionColor;

public Q_SLOTS:
    void on_lineEdit_textChanged(const char* _inParam, float _value);

	void on_lineEdit_specularMapPath_textChanged(QString _inPath);
	void on_lineEdit_transmissionMapPath_textChanged(QString _inPath);
	void on_lineEdit_transmissionScatterMapPath_textChanged(QString _inPath);
	void on_lineEdit_subsurfaceColorMapPath_textChanged(QString _inPath);
	void on_lineEdit_subsurfaceRadiusMapPath_textChanged(QString _inPath);
	void on_lineEdit_albedoMapPath_textChanged(QString _inPath);
	void on_lineEdit_roughnessMapPath_textChanged(QString _inPath);
	void on_lineEdit_metalnessMapPath_textChanged(QString _inPath);
	void on_lineEdit_emissiveMapPath_textChanged(QString _inPath);
	void on_lineEdit_normalMapPath_textChanged(QString _inPath);
	void on_lineEdit_displacementMapPath_textChanged(QString _inPath);

	void on_diffuseColor_valueChanged(QColor _inColor);
	void on_specularColor_valueChanged(QColor _inColor);
	void on_transmissionColor_valueChanged(QColor _inColor);
	void on_transmissionScatterColor_valueChanged(QColor _inColor);
	void on_subsurfaceColor_valueChanged(QColor _inColor);
	void on_subsurfaceRadiusColor_valueChanged(QColor _inColor);
	void on_coatSpecularColor_valueChanged(QColor _inColor);
	void on_emissionColor_valueChanged(QColor _inColor);

	void on_pushButton_updateShader_sendData(QString _vs, QString _fs, QString _gs);

	void setupViewModel(QString modelPath);


protected:
	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;
	void resizeGL(int w, int h) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
	std::vector<std::string>        facesHdrPath;

	ObjLoader                       *myObjLoader;

	int	                            m_xRot, m_yRot, m_zRot,
		                            mcp_xRot, mcp_yRot, mcp_zRot;

	float                           fov, nearClipDis, farClipDis;

	QPoint                          m_lastPos;

	QOpenGLVertexArrayObject        m_vao, m_vao_cubeMap;
	QOpenGLBuffer                   m_vbo, m_vbo_cubeMap;
	QOpenGLFramebufferObject        *m_fboUpSample, *m_fboDownSample;
	QOpenGLFramebufferObjectFormat  glFormatUpSample, glFormatDownSample;

	QOpenGLShaderProgram            *m_program,
		                            *m_program_cube_map,
		                            *m_program_thickness,
		                            *m_program_stressMask;

	QMatrix3x3                 normalMatrix;
	QMatrix4x4                 m_proj;
	QMatrix4x4                 m_camera;
	QMatrix4x4                 m_world;

	std::string                vsStr, fsStr, gsStr;
	const char                 *vertexShaderPathString,
		                       *fragmentShaderPathString,
							   *geometryShaderPathString;

	void                       createFramebufferObject();
	void                       loadTextures();
	void                       setupShaderFiles(QOpenGLShaderProgram* shaderProgram,
		                                        const char* vertexShaderPath,
		                                        const char* fragmentShaderPath);

	void                       setupShaderFiles(QOpenGLShaderProgram* shaderProgram,
		                                        const char* vertexShaderPath,
		                                        const char* geometryShaderPath,
		                                        const char* fragmentShaderPath);

	GLuint                     textureID,
		                       textureSpecularMap,
		                       textureTransmissionMap,
		                       textureTransmissionScatterMap,
		                       textureSubSurfaceMap,
		                       textureSubSurfaceRadiusMap,
		                       textureCoatSpecularMap,
		                       textureAlbedoMap,
		                       textureRoughnessMap,
		                       textureMetalnessMap,
		                       textureEmissiveMap,
		                       textureNormalMap,
	                           textureDisplacementMap;

	bool                       useSpecularMap,
	                           useTransmissionMap,
	                           useTransmissionScatterMap,
	                           useSubsurfaceMap,
	                           useSubsurfaceRadiusMap,
		                       useCoatSpecularMap,
	                           useAlbedoMap,
		                       useRoughnessMap,
		                       useMetalnessMap,
		                       useEmissiveMap,
		                       useNormalMap,
		                       useDisplacementMap,

		                       textureChanged;

	QString                    specularMapPath,
		                       transmissionMapPath,
		                       transmissionScatterMapPath,
		                       subsurfaceMapPath,
		                       subsurfaceRadiusMapPath,
		                       coatSpecularMapPath,
		                       albedoMapPath,
		                       roughnessMapPath,
		                       metalnessMapPath,
		                       emissiveMapPath,
		                       normalMapPath,
		                       displacementMapPath;
		                       
	void                       setupCubeMapTexture();
};

#endif
