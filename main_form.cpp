#include "main_form.h"


QWidget* mayaMainWindow()
{
	QWidgetList mayaWidgetList = qApp->topLevelWidgets();
	for each (QWidget* obj in mayaWidgetList)
	{
		if (obj->objectName() == "MayaWindow")
		{
			return obj;
		}
	}
	return NULL;
}

UiWidget::UiWidget(QWidget* parent)
	: QWidget(mayaMainWindow())
{
	setupUi(this);

	setWindowTitle("Shader Lib Beta");
	setObjectName("VHQ_Shader_Lib");
	setWindowFlags(Qt::Window);

	envPluginPath = MCommonSystemUtils::getEnv("PYRAMID_LOCATION").asChar();

	// 遍历提供的预览模型
	comboBox_modelList->clear();
	QDir dir(QString(envPluginPath.c_str()) + "/viewObj");
	if (dir.exists())
	{
		dir.setSorting(QDir::Size | QDir::Reversed);
		QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);

		for (int i = 0; i != file_list.size(); i++)
		{
			QString name = file_list.at(i).baseName();
			comboBox_modelList->addItem(name);
		}
	}

	// 添加OpenGL窗体
	ClassMainOglWidget* openGLWidget = new ClassMainOglWidget(widgetGL);
	openGLWidget->mainWidget = this;
	openGLWidget->inputPluginPath = envPluginPath;
	openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
	verticalLayout_3->addWidget(openGLWidget);

	toolBox_shaderPath->setCurrentIndex(0);

	connect(comboBox_modelList, SIGNAL(currentIndexChanged(QString)),
		openGLWidget, SLOT(setupViewModel(QString)));

	// 添加带颜色的属性
	QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Maximum);
	sizePolicy2.setHorizontalStretch(0);
	sizePolicy2.setVerticalStretch(0);

	QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Maximum);
	sizePolicy3.setHorizontalStretch(0);
	sizePolicy3.setVerticalStretch(0);

	// Page - Base
	SelectColorWidget* colorViewWidget_diffuseColor = new SelectColorWidget(page);
	colorViewWidget_diffuseColor->setObjectName(QStringLiteral("colorViewWidget_diffuseColor"));
	sizePolicy2.setHeightForWidth(colorViewWidget_diffuseColor->sizePolicy().hasHeightForWidth());
	colorViewWidget_diffuseColor->setSizePolicy(sizePolicy2);
	colorViewWidget_diffuseColor->setMinimumSize(QSize(0, 20));
	colorViewWidget_diffuseColor->setMaximumSize(QSize(57, 20));
	horizontalLayout_2->addWidget(colorViewWidget_diffuseColor);

	QLineEdit* lineEdit_diffuseColorMapPath = new QLineEdit(page);
	lineEdit_diffuseColorMapPath->setObjectName(QStringLiteral("lineEdit_diffuseColorMap"));
	sizePolicy3.setHeightForWidth(lineEdit_diffuseColorMapPath->sizePolicy().hasHeightForWidth());
	lineEdit_diffuseColorMapPath->setSizePolicy(sizePolicy3);
	lineEdit_diffuseColorMapPath->setMinimumSize(QSize(0, 20));
	lineEdit_diffuseColorMapPath->setMaximumSize(QSize(16777215, 20));
	horizontalLayout_2->addWidget(lineEdit_diffuseColorMapPath);

	// Page - Specular
	SelectColorWidget* colorViewWidget_specularColor = new SelectColorWidget(page_3);
	colorViewWidget_specularColor->setObjectName(QStringLiteral("colorViewWidget_specularColor"));
	sizePolicy2.setHeightForWidth(colorViewWidget_specularColor->sizePolicy().hasHeightForWidth());
	colorViewWidget_specularColor->setSizePolicy(sizePolicy2);
	colorViewWidget_specularColor->setMinimumSize(QSize(0, 20));
	colorViewWidget_specularColor->setMaximumSize(QSize(57, 20));
	horizontalLayout_13->addWidget(colorViewWidget_specularColor);

	QLineEdit* lineEdit_specularMapPath = new QLineEdit(page_3);
	lineEdit_specularMapPath->setObjectName(QStringLiteral("lineEdit_specularColorMap"));
	sizePolicy3.setHeightForWidth(lineEdit_specularMapPath->sizePolicy().hasHeightForWidth());
	lineEdit_specularMapPath->setSizePolicy(sizePolicy3);
	lineEdit_specularMapPath->setMinimumSize(QSize(0, 20));
	lineEdit_specularMapPath->setMaximumSize(QSize(16777215, 20));
	horizontalLayout_13->addWidget(lineEdit_specularMapPath);

	// Page - Transmission
	SelectColorWidget* colorViewWidget_transmissionColor = new SelectColorWidget(page_4);
	colorViewWidget_transmissionColor->setObjectName(QStringLiteral("colorViewWidget_transmissionColor"));
	sizePolicy2.setHeightForWidth(colorViewWidget_transmissionColor->sizePolicy().hasHeightForWidth());
	colorViewWidget_transmissionColor->setSizePolicy(sizePolicy2);
	colorViewWidget_transmissionColor->setMinimumSize(QSize(0, 20));
	colorViewWidget_transmissionColor->setMaximumSize(QSize(57, 20));
	horizontalLayout_24->addWidget(colorViewWidget_transmissionColor);

	QLineEdit* lineEdit_transmissionMapPath = new QLineEdit(page_4);
	lineEdit_transmissionMapPath->setObjectName(QStringLiteral("lineEdit_transmissionColorMap"));
	sizePolicy3.setHeightForWidth(lineEdit_transmissionMapPath->sizePolicy().hasHeightForWidth());
	lineEdit_transmissionMapPath->setSizePolicy(sizePolicy3);
	lineEdit_transmissionMapPath->setMinimumSize(QSize(0, 20));
	lineEdit_transmissionMapPath->setMaximumSize(QSize(16777215, 20));
	horizontalLayout_24->addWidget(lineEdit_transmissionMapPath);

	SelectColorWidget* colorViewWidget_transmissionScatterColor = new SelectColorWidget(page_4);
	colorViewWidget_transmissionScatterColor->setObjectName(QStringLiteral("colorViewWidget_transmissionScatterColor"));
	sizePolicy2.setHeightForWidth(colorViewWidget_transmissionScatterColor->sizePolicy().hasHeightForWidth());
	colorViewWidget_transmissionScatterColor->setSizePolicy(sizePolicy2);
	colorViewWidget_transmissionScatterColor->setMinimumSize(QSize(0, 20));
	colorViewWidget_transmissionScatterColor->setMaximumSize(QSize(57, 20));
	horizontalLayout_27->addWidget(colorViewWidget_transmissionScatterColor);

	QLineEdit* lineEdit_transmissionScatterMapPath = new QLineEdit(page_4);
	lineEdit_transmissionScatterMapPath->setObjectName(QStringLiteral("lineEdit_transmissionScatterColorMap"));
	sizePolicy3.setHeightForWidth(lineEdit_transmissionScatterMapPath->sizePolicy().hasHeightForWidth());
	lineEdit_transmissionScatterMapPath->setSizePolicy(sizePolicy3);
	lineEdit_transmissionScatterMapPath->setMinimumSize(QSize(0, 20));
	lineEdit_transmissionScatterMapPath->setMaximumSize(QSize(16777215, 20));
	horizontalLayout_27->addWidget(lineEdit_transmissionScatterMapPath);

	// Page - Subsurface
	SelectColorWidget* colorViewWidget_subsurfaceColor = new SelectColorWidget(page_5);
	colorViewWidget_subsurfaceColor->setObjectName(QStringLiteral("colorViewWidget_subsurfaceColor"));
	sizePolicy2.setHeightForWidth(colorViewWidget_subsurfaceColor->sizePolicy().hasHeightForWidth());
	colorViewWidget_subsurfaceColor->setSizePolicy(sizePolicy2);
	colorViewWidget_subsurfaceColor->setMinimumSize(QSize(0, 20));
	colorViewWidget_subsurfaceColor->setMaximumSize(QSize(57, 20));
	horizontalLayout_34->addWidget(colorViewWidget_subsurfaceColor);

	QLineEdit* lineEdit_subsurfaceMapPath = new QLineEdit(page_5);
	lineEdit_subsurfaceMapPath->setObjectName(QStringLiteral("lineEdit_subfurfaceColorMap"));
	sizePolicy3.setHeightForWidth(lineEdit_subsurfaceMapPath->sizePolicy().hasHeightForWidth());
	lineEdit_subsurfaceMapPath->setSizePolicy(sizePolicy3);
	lineEdit_subsurfaceMapPath->setMinimumSize(QSize(0, 20));
	lineEdit_subsurfaceMapPath->setMaximumSize(QSize(16777215, 20));
	horizontalLayout_34->addWidget(lineEdit_subsurfaceMapPath);

	SelectColorWidget* colorViewWidget_subsurfaceRadiusColor = new SelectColorWidget(page_5);
	colorViewWidget_subsurfaceRadiusColor->setObjectName(QStringLiteral("colorViewWidget_subsurfaceRadiusColor"));
	sizePolicy2.setHeightForWidth(colorViewWidget_subsurfaceRadiusColor->sizePolicy().hasHeightForWidth());
	colorViewWidget_subsurfaceRadiusColor->setSizePolicy(sizePolicy2);
	colorViewWidget_subsurfaceRadiusColor->setMinimumSize(QSize(0, 20));
	colorViewWidget_subsurfaceRadiusColor->setMaximumSize(QSize(57, 20));
	horizontalLayout_35->addWidget(colorViewWidget_subsurfaceRadiusColor);

	QLineEdit* lineEdit_subsurfaceRadiusMapPath = new QLineEdit(page_5);
	lineEdit_subsurfaceRadiusMapPath->setObjectName(QStringLiteral("lineEdit_subsurfaceRadiusColorMap"));
	sizePolicy3.setHeightForWidth(lineEdit_subsurfaceRadiusMapPath->sizePolicy().hasHeightForWidth());
	lineEdit_subsurfaceRadiusMapPath->setSizePolicy(sizePolicy3);
	lineEdit_subsurfaceRadiusMapPath->setMinimumSize(QSize(0, 20));
	lineEdit_subsurfaceRadiusMapPath->setMaximumSize(QSize(16777215, 20));
	horizontalLayout_35->addWidget(lineEdit_subsurfaceRadiusMapPath);

	// Page - Coat
	SelectColorWidget* colorViewWidget_coatSpecularColor = new SelectColorWidget(page_6);
	colorViewWidget_coatSpecularColor->setObjectName(QStringLiteral("colorViewWidget_emissionColor"));
	sizePolicy2.setHeightForWidth(colorViewWidget_coatSpecularColor->sizePolicy().hasHeightForWidth());
	colorViewWidget_coatSpecularColor->setSizePolicy(sizePolicy2);
	colorViewWidget_coatSpecularColor->setMinimumSize(QSize(0, 20));
	colorViewWidget_coatSpecularColor->setMaximumSize(QSize(57, 20));
	horizontalLayout_28->addWidget(colorViewWidget_coatSpecularColor);

	QLineEdit* lineEdit_coatSpecularMapPath = new QLineEdit(page_6);
	lineEdit_coatSpecularMapPath->setObjectName(QStringLiteral("lineEdit_coatSpecularColorMap"));
	sizePolicy3.setHeightForWidth(lineEdit_coatSpecularMapPath->sizePolicy().hasHeightForWidth());
	lineEdit_coatSpecularMapPath->setSizePolicy(sizePolicy3);
	lineEdit_coatSpecularMapPath->setMinimumSize(QSize(0, 20));
	lineEdit_coatSpecularMapPath->setMaximumSize(QSize(16777215, 20));
	horizontalLayout_28->addWidget(lineEdit_coatSpecularMapPath);

	// Page - Emission
	SelectColorWidget* colorViewWidget_emissionColor = new SelectColorWidget(page_7);
	colorViewWidget_emissionColor->setObjectName(QStringLiteral("colorViewWidget_emissionColor"));
	sizePolicy2.setHeightForWidth(colorViewWidget_emissionColor->sizePolicy().hasHeightForWidth());
	colorViewWidget_emissionColor->setSizePolicy(sizePolicy2);
	colorViewWidget_emissionColor->setMinimumSize(QSize(0, 20));
	colorViewWidget_emissionColor->setMaximumSize(QSize(57, 20));
	horizontalLayout_25->addWidget(colorViewWidget_emissionColor);

	QLineEdit* lineEdit_emissionMapPath = new QLineEdit(page_7);
	lineEdit_emissionMapPath->setObjectName(QStringLiteral("lineEdit_emissionColorMap"));
	sizePolicy3.setHeightForWidth(lineEdit_emissionMapPath->sizePolicy().hasHeightForWidth());
	lineEdit_emissionMapPath->setSizePolicy(sizePolicy3);
	lineEdit_emissionMapPath->setMinimumSize(QSize(0, 20));
	lineEdit_emissionMapPath->setMaximumSize(QSize(16777215, 20));
	horizontalLayout_25->addWidget(lineEdit_emissionMapPath);


	colorViewWidget_diffuseColor->color = QColor(160.0, 160.0, 160.0);
	colorViewWidget_diffuseColor->setColor();
	colorViewWidget_specularColor->color = QColor(0.0, 0.0, 0.0);
	colorViewWidget_specularColor->setColor();
	colorViewWidget_transmissionColor->color = QColor(255.0, 255.0, 255.0);
	colorViewWidget_transmissionColor->setColor();
	colorViewWidget_transmissionScatterColor->color = QColor(0.0, 0.0, 0.0);
	colorViewWidget_transmissionScatterColor->setColor();
	colorViewWidget_subsurfaceColor->color = QColor(255.0, 255.0, 255.0);
	colorViewWidget_subsurfaceColor->setColor();
	colorViewWidget_subsurfaceRadiusColor->color = QColor(255.0, 255.0, 255.0);
	colorViewWidget_subsurfaceRadiusColor->setColor();
	colorViewWidget_coatSpecularColor->color = QColor(0.0, 0.0, 0.0);
	colorViewWidget_coatSpecularColor->setColor();
	colorViewWidget_emissionColor->color = QColor(0.0, 0.0, 0.0);
	colorViewWidget_emissionColor->setColor();

	// 滑块数值改变绑定

	/* Diffuse Weight */
	lineEdit_diffuseWeight->setValidator(new QDoubleValidator(0.0, 1.0, 2, this));
	horizontalSlider_diffuseWeight->setRange(0, 100);
	connect(lineEdit_diffuseWeight, SIGNAL(textChanged(QString)),
		this, SLOT(on_lineEdit_diffuseWeight_textChanged(QString)));
	connect(this, SIGNAL(sendDataConstCharPointerInt(const char*, float)),
		openGLWidget, SLOT(on_lineEdit_textChanged(const char*, float)));
	connect(horizontalSlider_diffuseWeight, SIGNAL(valueChanged(int)),
		this, SLOT(on_horizontalSlider_diffuseWeight_valueChanged(int)));

	/* Diffuse Roughness */
	lineEdit_diffuseRoughness->setValidator(new QDoubleValidator(0.0, 1.0, 2, this));
	horizontalSlider_diffuseRoughness->setRange(0, 100);
	connect(lineEdit_diffuseRoughness, SIGNAL(textChanged(QString)),
		this, SLOT(on_lineEdit_diffuseRoughness_textChanged(QString)));
	connect(this, SIGNAL(sendDataConstCharPointerInt(const char*, float)),
		openGLWidget, SLOT(on_lineEdit_textChanged(const char*, float)));
	connect(horizontalSlider_diffuseRoughness, SIGNAL(valueChanged(int)),
		this, SLOT(on_horizontalSlider_diffuseRoughness_valueChanged(int)));

	/* Metalness */
	lineEdit_metalness->setValidator(new QDoubleValidator(0.0, 1.0, 2, this));
	horizontalSlider_metalness->setRange(0, 100);
	connect(lineEdit_metalness, SIGNAL(textChanged(QString)),
		this, SLOT(on_lineEdit_metalness_textChanged(QString)));
	connect(this, SIGNAL(sendDataConstCharPointerInt(const char*, float)),
		openGLWidget, SLOT(on_lineEdit_textChanged(const char*, float)));
	connect(horizontalSlider_metalness, SIGNAL(valueChanged(int)),
		this, SLOT(on_horizontalSlider_metalness_valueChanged(int)));

	/* Specular Weight */
	lineEdit_specularWeight->setValidator(new QDoubleValidator(0.0, 1.0, 2, this));
	horizontalSlider_specularWeight->setRange(0, 100);
	connect(lineEdit_specularWeight, SIGNAL(textChanged(QString)),
		this, SLOT(on_lineEdit_specularWeight_textChanged(QString)));
	connect(this, SIGNAL(sendDataConstCharPointerInt(const char*, float)),
		openGLWidget, SLOT(on_lineEdit_textChanged(const char*, float)));
	connect(horizontalSlider_specularWeight, SIGNAL(valueChanged(int)),
		this, SLOT(on_horizontalSlider_specularWeight_valueChanged(int)));

	/* Specular Roughness */
	lineEdit_specularRoughness->setValidator(new QDoubleValidator(0.0, 1.0, 2, this));
	horizontalSlider_specularRoughness->setRange(0, 100);
	connect(lineEdit_specularRoughness, SIGNAL(textChanged(QString)),
		this, SLOT(on_lineEdit_specularRoughness_textChanged(QString)));
	connect(this, SIGNAL(sendDataConstCharPointerInt(const char*, float)),
		openGLWidget, SLOT(on_lineEdit_textChanged(const char*, float)));
	connect(horizontalSlider_specularRoughness, SIGNAL(valueChanged(int)),
		this, SLOT(on_horizontalSlider_specularRoughness_valueChanged(int)));

	/* Specular IOR */
	lineEdit_specularIOR->setValidator(new QDoubleValidator(1.0, 20.0, 2, this));
	horizontalSlider_specularIOR->setRange(100, 2000);
	connect(lineEdit_specularIOR, SIGNAL(textChanged(QString)),
		this, SLOT(on_lineEdit_specularIOR_textChanged(QString)));
	connect(this, SIGNAL(sendDataConstCharPointerInt(const char*, float)),
		openGLWidget, SLOT(on_lineEdit_textChanged(const char*, float)));
	connect(horizontalSlider_specularIOR, SIGNAL(valueChanged(int)),
		this, SLOT(on_horizontalSlider_specularIOR_valueChanged(int)));

	/* Specular Anisotropy */
	lineEdit_specularAnisotropy->setValidator(new QDoubleValidator(0.0, 1.0, 2, this));
	horizontalSlider_specularAnisotropy->setRange(0, 100);
	connect(lineEdit_specularAnisotropy, SIGNAL(textChanged(QString)),
		this, SLOT(on_lineEdit_specularAnisotropy_textChanged(QString)));
	connect(this, SIGNAL(sendDataConstCharPointerInt(const char*, float)),
		openGLWidget, SLOT(on_lineEdit_textChanged(const char*, float)));
	connect(horizontalSlider_specularAnisotropy, SIGNAL(valueChanged(int)),
		this, SLOT(on_horizontalSlider_specularAnisotropy_valueChanged(int)));

	/* Specular Rotation */
	lineEdit_specularRotation->setValidator(new QDoubleValidator(0.0, 1.0, 2, this));
	horizontalSlider_specularRotation->setRange(0, 100);
	connect(lineEdit_specularRotation, SIGNAL(textChanged(QString)),
		this, SLOT(on_lineEdit_specularRotation_textChanged(QString)));
	connect(this, SIGNAL(sendDataConstCharPointerInt(const char*, float)),
		openGLWidget, SLOT(on_lineEdit_textChanged(const char*, float)));
	connect(horizontalSlider_specularRotation, SIGNAL(valueChanged(int)),
		this, SLOT(on_horizontalSlider_specularRotation_valueChanged(int)));

	/* Transmission Weight */
	lineEdit_transmissionWeight->setValidator(new QDoubleValidator(0.0, 1.0, 2, this));
	horizontalSlider_transmissionWeight->setRange(0, 100);
	connect(lineEdit_transmissionWeight, SIGNAL(textChanged(QString)),
		this, SLOT(on_lineEdit_transmissionWeight_textChanged(QString)));
	connect(this, SIGNAL(sendDataConstCharPointerInt(const char*, float)),
		openGLWidget, SLOT(on_lineEdit_textChanged(const char*, float)));
	connect(horizontalSlider_transmissionWeight, SIGNAL(valueChanged(int)),
		this, SLOT(on_horizontalSlider_transmissionWeight_valueChanged(int)));

	/* Transmission Depth */
	lineEdit_transmissionDepth->setValidator(new QDoubleValidator(0.0, 1.0, 2, this));
	horizontalSlider_transmissionDepth->setRange(0, 100);
	connect(lineEdit_transmissionDepth, SIGNAL(textChanged(QString)),
		this, SLOT(on_lineEdit_transmissionDepth_textChanged(QString)));
	connect(this, SIGNAL(sendDataConstCharPointerInt(const char*, float)),
		openGLWidget, SLOT(on_lineEdit_textChanged(const char*, float)));
	connect(horizontalSlider_transmissionDepth, SIGNAL(valueChanged(int)),
		this, SLOT(on_horizontalSlider_transmissionDepth_valueChanged(int)));

	/* Subsurface Weight */
	lineEdit_subsurfaceWeight->setValidator(new QDoubleValidator(0.0, 1.0, 2, this));
	horizontalSlider_subsurfaceWeight->setRange(0, 100);
	connect(lineEdit_subsurfaceWeight, SIGNAL(textChanged(QString)),
		this, SLOT(on_lineEdit_subsurfaceWeight_textChanged(QString)));
	connect(this, SIGNAL(sendDataConstCharPointerInt(const char*, float)),
		openGLWidget, SLOT(on_lineEdit_textChanged(const char*, float)));
	connect(horizontalSlider_subsurfaceWeight, SIGNAL(valueChanged(int)),
		this, SLOT(on_horizontalSlider_subsurfaceWeight_valueChanged(int)));

	/* Subsurface Scale */
	lineEdit_subsurfaceScale->setValidator(new QDoubleValidator(0.0, 10.0, 2, this));
	horizontalSlider_subsurfaceScale->setRange(0, 1000);
	connect(lineEdit_subsurfaceScale, SIGNAL(textChanged(QString)),
		this, SLOT(on_lineEdit_subsurfaceScale_textChanged(QString)));
	connect(this, SIGNAL(sendDataConstCharPointerInt(const char*, float)),
		openGLWidget, SLOT(on_lineEdit_textChanged(const char*, float)));
	connect(horizontalSlider_subsurfaceScale, SIGNAL(valueChanged(int)),
		this, SLOT(on_horizontalSlider_subsurfaceScale_valueChanged(int)));

	/* Coat Specular Weight */
	lineEdit_coatSpecularWeight->setValidator(new QDoubleValidator(0.0, 1.0, 2, this));
	horizontalSlider_coatSpecularWeight->setRange(0, 100);
	connect(lineEdit_coatSpecularWeight, SIGNAL(textChanged(QString)),
		this, SLOT(on_lineEdit_coatSpecularWeight_textChanged(QString)));
	connect(this, SIGNAL(sendDataConstCharPointerInt(const char*, float)),
		openGLWidget, SLOT(on_lineEdit_textChanged(const char*, float)));
	connect(horizontalSlider_coatSpecularWeight, SIGNAL(valueChanged(int)),
		this, SLOT(on_horizontalSlider_coatSpecularWeight_valueChanged(int)));

	/* Coat Specular Roughness */
	lineEdit_coatSpecularRoughness->setValidator(new QDoubleValidator(0.0, 1.0, 2, this));
	horizontalSlider_coatSpecularRoughness->setRange(0, 100);
	connect(lineEdit_coatSpecularRoughness, SIGNAL(textChanged(QString)),
		this, SLOT(on_lineEdit_coatSpecularRoughness_textChanged(QString)));
	connect(this, SIGNAL(sendDataConstCharPointerInt(const char*, float)),
		openGLWidget, SLOT(on_lineEdit_textChanged(const char*, float)));
	connect(horizontalSlider_coatSpecularRoughness, SIGNAL(valueChanged(int)),
		this, SLOT(on_horizontalSlider_coatSpecularRoughness_valueChanged(int)));

	/* Emission Weight */
	lineEdit_emissionWeight->setValidator(new QDoubleValidator(0.0, 10.0, 2, this));
	horizontalSlider_emissionWeight->setRange(0, 1000);
	connect(lineEdit_emissionWeight, SIGNAL(textChanged(QString)),
		this, SLOT(on_lineEdit_emissionWeight_textChanged(QString)));
	connect(this, SIGNAL(sendDataConstCharPointerInt(const char*, float)),
		openGLWidget, SLOT(on_lineEdit_textChanged(const char*, float)));
	connect(horizontalSlider_emissionWeight, SIGNAL(valueChanged(int)),
		this, SLOT(on_horizontalSlider_emissionWeight_valueChanged(int)));

	/* Light1 Intensity */
	lineEdit_light1_intensity->setValidator(new QDoubleValidator(0.0, 1.0, 2, this));
	horizontalSlider_light1_intensity->setRange(0, 100);
	connect(lineEdit_light1_intensity, SIGNAL(textChanged(QString)),
		this, SLOT(on_lineEdit_light1_intensity_textChanged(QString)));
	connect(this, SIGNAL(sendDataConstCharPointerInt(const char*, float)),
		openGLWidget, SLOT(on_lineEdit_textChanged(const char*, float)));
	connect(horizontalSlider_light1_intensity, SIGNAL(valueChanged(int)),
		this, SLOT(on_horizontalSlider_light1_intensity_valueChanged(int)));

	/* Light1 Positionx */
	lineEdit_light1_positionx->setValidator(new QDoubleValidator(0, 1.0, 5, this));
	horizontalSlider_light1_positionx->setRange(0, 1000);
	connect(lineEdit_light1_positionx, SIGNAL(textChanged(QString)),
		this, SLOT(on_lineEdit_light1_positionx_textChanged(QString)));
	connect(this, SIGNAL(sendDataConstCharPointerInt(const char*, float)),
		openGLWidget, SLOT(on_lineEdit_textChanged(const char*, float)));
	connect(horizontalSlider_light1_positionx, SIGNAL(valueChanged(int)),
		this, SLOT(on_horizontalSlider_light1_positionx_valueChanged(int)));

	/* Light1 Positiony */
	lineEdit_light1_positiony->setValidator(new QDoubleValidator(0, 1.0, 5, this));
	horizontalSlider_light1_positiony->setRange(0, 1000);
	connect(lineEdit_light1_positiony, SIGNAL(textChanged(QString)),
		this, SLOT(on_lineEdit_light1_positiony_textChanged(QString)));
	connect(this, SIGNAL(sendDataConstCharPointerInt(const char*, float)),
		openGLWidget, SLOT(on_lineEdit_textChanged(const char*, float)));
	connect(horizontalSlider_light1_positiony, SIGNAL(valueChanged(int)),
		this, SLOT(on_horizontalSlider_light1_positiony_valueChanged(int)));

	/* Light1 Positionz */
	lineEdit_light1_positionz->setValidator(new QDoubleValidator(0, 1.0, 5, this));
	horizontalSlider_light1_positionz->setRange(0, 1000);
	connect(lineEdit_light1_positionz, SIGNAL(textChanged(QString)),
		this, SLOT(on_lineEdit_light1_positionz_textChanged(QString)));
	connect(this, SIGNAL(sendDataConstCharPointerInt(const char*, float)),
		openGLWidget, SLOT(on_lineEdit_textChanged(const char*, float)));
	connect(horizontalSlider_light1_positionz, SIGNAL(valueChanged(int)),
		this, SLOT(on_horizontalSlider_light1_positionz_valueChanged(int)));

	// 颜色块儿改变绑定

	/* Diffuse Color */
	connect(colorViewWidget_diffuseColor, SIGNAL(colorHasChanged(QColor)),
		openGLWidget, SLOT(on_diffuseColor_valueChanged(QColor)));

	/* Specular Color */
	connect(colorViewWidget_specularColor, SIGNAL(colorHasChanged(QColor)),
		openGLWidget, SLOT(on_specularColor_valueChanged(QColor)));

	/* Transmission Color */
	connect(colorViewWidget_transmissionColor, SIGNAL(colorHasChanged(QColor)),
		openGLWidget, SLOT(on_transmissionColor_valueChanged(QColor)));

	/* Transmission Scatter Color */
	connect(colorViewWidget_transmissionScatterColor, SIGNAL(colorHasChanged(QColor)),
		openGLWidget, SLOT(on_transmissionScatterColor_valueChanged(QColor)));

	/* Subsurface Color */
	connect(colorViewWidget_subsurfaceColor, SIGNAL(colorHasChanged(QColor)),
		openGLWidget, SLOT(on_subsurfaceColor_valueChanged(QColor)));

	/* Subsurface Radius Color */
	connect(colorViewWidget_subsurfaceRadiusColor, SIGNAL(colorHasChanged(QColor)),
		openGLWidget, SLOT(on_subsurfaceRadiusColor_valueChanged(QColor)));

	/* Coat Specular Color */
	connect(colorViewWidget_coatSpecularColor, SIGNAL(colorHasChanged(QColor)),
		openGLWidget, SLOT(on_coatSpecularColor_valueChanged(QColor)));

	/* Emission Color */
	connect(colorViewWidget_emissionColor, SIGNAL(colorHasChanged(QColor)),
		openGLWidget, SLOT(on_emissionColor_valueChanged(QColor)));

	// 贴图路径改变绑定
	connect(lineEdit_specularMapPath, SIGNAL(textChanged(QString)),
		openGLWidget, SLOT(on_lineEdit_specularMapPath_textChanged(QString)));
	connect(lineEdit_transmissionMapPath, SIGNAL(textChanged(QString)),
		openGLWidget, SLOT(on_lineEdit_transmissionMapPath_textChanged(QString)));
	connect(lineEdit_transmissionScatterMapPath, SIGNAL(textChanged(QString)),
		openGLWidget, SLOT(on_lineEdit_transmissionScatterMapPath_textChanged(QString)));
	connect(lineEdit_subsurfaceMapPath, SIGNAL(textChanged(QString)),
		openGLWidget, SLOT(on_lineEdit_subsurfaceMapPath_textChanged(QString)));
	connect(lineEdit_subsurfaceRadiusMapPath, SIGNAL(textChanged(QString)),
		openGLWidget, SLOT(on_lineEdit_subsurfaceRadiusMapPath_textChanged(QString)));
	connect(lineEdit_albedoMapPath, SIGNAL(textChanged(QString)),
		openGLWidget, SLOT(on_lineEdit_albedoMapPath_textChanged(QString)));
	connect(lineEdit_roughnessMapPath, SIGNAL(textChanged(QString)),
		openGLWidget, SLOT(on_lineEdit_roughnessMapPath_textChanged(QString)));
	connect(lineEdit_metalnessMapPath, SIGNAL(textChanged(QString)),
		openGLWidget, SLOT(on_lineEdit_metalnessMapPath_textChanged(QString)));
	connect(lineEdit_emissiveMapPath, SIGNAL(textChanged(QString)),
		openGLWidget, SLOT(on_lineEdit_emissiveMapPath_textChanged(QString)));
	connect(lineEdit_normalMapPath, SIGNAL(textChanged(QString)),
		openGLWidget, SLOT(on_lineEdit_normalMapPath_textChanged(QString)));
	connect(lineEdit_displacementMapPath, SIGNAL(textChanged(QString)),
		openGLWidget, SLOT(on_lineEdit_displacementMapPath_textChanged(QString)));
	connect(lineEdit_coatSpecularMapPath, SIGNAL(textChanged(QString)),
		openGLWidget, SLOT(on_lineEdit_coatSpecularMapPath_textChanged(QString)));

	connect(pushButton_updateShader, SIGNAL(clicked()),
		this, SLOT(on_pushButton_updateShader_clicked()));
	connect(this, SIGNAL(sendDataQString2(QString, QString, QString)),
		openGLWidget, SLOT(on_pushButton_updateShader_sendData(QString, QString, QString)));

	// 设置UI材质球属性默认值
	lineEdit_diffuseWeight->setText("1.0");
	horizontalSlider_diffuseWeight->setValue(100);
	lineEdit_diffuseRoughness->setText("0.0");
	horizontalSlider_diffuseRoughness->setValue(0);
	lineEdit_metalness->setText("0.0");
	horizontalSlider_metalness->setValue(0);
	lineEdit_specularWeight->setText("1.0");
	horizontalSlider_specularWeight->setValue(100);
	lineEdit_specularRoughness->setText("0.2");
	horizontalSlider_specularRoughness->setValue(20);
	lineEdit_specularIOR->setText("1.5");
	horizontalSlider_specularIOR->setValue(150); //1.0 - 20
	lineEdit_specularAnisotropy->setText("0.5");
	horizontalSlider_specularAnisotropy->setValue(50);
	lineEdit_specularRotation->setText("0.0");
	horizontalSlider_specularRotation->setValue(0);
	lineEdit_transmissionWeight->setText("0.0");
	horizontalSlider_transmissionWeight->setValue(0);
	lineEdit_transmissionDepth->setText("0.0");
	horizontalSlider_transmissionDepth->setValue(0);
	lineEdit_subsurfaceWeight->setText("0.0");
	horizontalSlider_subsurfaceWeight->setValue(0);
	lineEdit_subsurfaceScale->setText("1.0");
	horizontalSlider_subsurfaceScale->setValue(100);
	lineEdit_coatSpecularWeight->setText("0.0");
	horizontalSlider_coatSpecularWeight->setValue(0);
	lineEdit_coatSpecularRoughness->setText("0.0");
	horizontalSlider_coatSpecularRoughness->setValue(0);
	lineEdit_emissionWeight->setText("0.0");
	horizontalSlider_emissionWeight->setValue(0);

	lineEdit_light1_intensity->setText("1.0");
	horizontalSlider_light1_intensity->setValue(100);
	lineEdit_light1_positionx->setText("0.8");
	horizontalSlider_light1_positionx->setValue(800);
	lineEdit_light1_positiony->setText("0.8");
	horizontalSlider_light1_positiony->setValue(800);
	lineEdit_light1_positionz->setText("0.8");
	horizontalSlider_light1_positionz->setValue(800);

	lineEdit_vertexShaderPath->setText(QString(envPluginPath.c_str()) + "/GLSL/dhsSurface.vert");
	lineEdit_fragmentShaderPath->setText(QString(envPluginPath.c_str()) + "/GLSL/dhsSurface.frag");
	lineEdit_geometryShaderPath->setText(QString(envPluginPath.c_str()) + "/GLSL/dhsSurface.geom");
	show();
}

UiWidget::~UiWidget()
{
	if (!fCurrentDialog.isNull()) delete fCurrentDialog;
}

void UiWidget::on_lineEdit_diffuseWeight_textChanged(QString _value)
{
	emit sendDataConstCharPointerInt("diffuseWeight", _value.toFloat());
	horizontalSlider_diffuseWeight->setValue((int)(_value.toFloat() * 100));
}

void UiWidget::on_lineEdit_diffuseRoughness_textChanged(QString _value)
{
	emit sendDataConstCharPointerInt("diffuseRoughness", _value.toFloat());
	horizontalSlider_diffuseRoughness->setValue((int)(_value.toFloat() * 100));
}

void UiWidget::on_lineEdit_metalness_textChanged(QString _value)
{
	emit sendDataConstCharPointerInt("metalness", _value.toFloat());
	horizontalSlider_metalness->setValue((int)(_value.toFloat() * 100));
}

void UiWidget::on_lineEdit_specularWeight_textChanged(QString _value)
{
	emit sendDataConstCharPointerInt("specularWeight", _value.toFloat());
	horizontalSlider_specularWeight->setValue((int)(_value.toFloat() * 100));
}

void UiWidget::on_lineEdit_specularRoughness_textChanged(QString _value)
{
	emit sendDataConstCharPointerInt("specularRoughness", _value.toFloat());
	horizontalSlider_specularRoughness->setValue((int)(_value.toFloat() * 100));
}

void UiWidget::on_lineEdit_specularIOR_textChanged(QString _value)
{
	emit sendDataConstCharPointerInt("specularIOR", _value.toFloat());
	horizontalSlider_specularIOR->setValue((int)(_value.toFloat() * 100));
}

void UiWidget::on_lineEdit_specularAnisotropy_textChanged(QString _value)
{
	emit sendDataConstCharPointerInt("specularAnisotropy", _value.toFloat());
	horizontalSlider_specularAnisotropy->setValue((int)(_value.toFloat() * 100));
}

void UiWidget::on_lineEdit_specularRotation_textChanged(QString _value)
{
	emit sendDataConstCharPointerInt("specularRotation", _value.toFloat());
	horizontalSlider_specularRotation->setValue((int)(_value.toFloat() * 100));
}

void UiWidget::on_lineEdit_transmissionWeight_textChanged(QString _value)
{
	emit sendDataConstCharPointerInt("transmissionWeight", _value.toFloat());
	horizontalSlider_transmissionWeight->setValue((int)(_value.toFloat() * 100));
}

void UiWidget::on_lineEdit_transmissionDepth_textChanged(QString _value)
{
	emit sendDataConstCharPointerInt("transmissionDepth", _value.toFloat());
	horizontalSlider_transmissionDepth->setValue((int)(_value.toFloat() * 100));
}

void UiWidget::on_lineEdit_subsurfaceWeight_textChanged(QString _value)
{
	emit sendDataConstCharPointerInt("subsurfaceWeight", _value.toFloat());
	horizontalSlider_subsurfaceWeight->setValue((int)(_value.toFloat() * 100));
}

void UiWidget::on_lineEdit_subsurfaceScale_textChanged(QString _value)
{
	emit sendDataConstCharPointerInt("subsurfaceScale", _value.toFloat());
	horizontalSlider_subsurfaceScale->setValue((int)(_value.toFloat() * 100));
}

void UiWidget::on_lineEdit_coatSpecularWeight_textChanged(QString _value)
{
	emit sendDataConstCharPointerInt("coatSpecularWeight", _value.toFloat());
	horizontalSlider_coatSpecularWeight->setValue((int)(_value.toFloat() * 100));
}

void UiWidget::on_lineEdit_coatSpecularRoughness_textChanged(QString _value)
{
	emit sendDataConstCharPointerInt("coatSpecularRoughness", _value.toFloat());
	horizontalSlider_coatSpecularRoughness->setValue((int)(_value.toFloat() * 100));
}

void UiWidget::on_lineEdit_emissionWeight_textChanged(QString _value)
{
	emit sendDataConstCharPointerInt("emissionWeight", _value.toFloat());
	horizontalSlider_emissionWeight->setValue((int)(_value.toFloat() * 100));
}

void UiWidget::on_lineEdit_light1_intensity_textChanged(QString _value)
{
	emit sendDataConstCharPointerInt("light1_intensity", _value.toFloat());
	horizontalSlider_light1_intensity->setValue((int)(_value.toFloat() * 100));
}

void UiWidget::on_lineEdit_light1_positionx_textChanged(QString _value)
{
	emit sendDataConstCharPointerInt("light1_positionx", _value.toFloat());
	horizontalSlider_light1_positionx->setValue((int)(_value.toFloat() * 1000));
}

void UiWidget::on_lineEdit_light1_positiony_textChanged(QString _value)
{
	emit sendDataConstCharPointerInt("light1_positiony", _value.toFloat());
	horizontalSlider_light1_positiony->setValue((int)(_value.toFloat() * 1000));
}

void UiWidget::on_lineEdit_light1_positionz_textChanged(QString _value)
{
	emit sendDataConstCharPointerInt("light1_positionz", _value.toFloat());
	horizontalSlider_light1_positionz->setValue((int)(_value.toFloat() * 1000));
}

void UiWidget::on_horizontalSlider_diffuseWeight_valueChanged(int _value)
{
	lineEdit_diffuseWeight->setText(QString::number(_value / 100.0));
}

void UiWidget::on_horizontalSlider_diffuseRoughness_valueChanged(int _value)
{
	lineEdit_diffuseRoughness->setText(QString::number(_value / 100.0));
}

void UiWidget::on_horizontalSlider_metalness_valueChanged(int _value)
{
	lineEdit_metalness->setText(QString::number(_value / 100.0));
}

void UiWidget::on_horizontalSlider_specularWeight_valueChanged(int _value)
{
	lineEdit_specularWeight->setText(QString::number(_value / 100.0));
}

void UiWidget::on_horizontalSlider_specularRoughness_valueChanged(int _value)
{
	lineEdit_specularRoughness->setText(QString::number(_value / 100.0));
}

void UiWidget::on_horizontalSlider_specularIOR_valueChanged(int _value)
{
	lineEdit_specularIOR->setText(QString::number(_value / 100.0));
}

void UiWidget::on_horizontalSlider_specularAnisotropy_valueChanged(int _value)
{
	lineEdit_specularAnisotropy->setText(QString::number(_value / 100.0));
}

void UiWidget::on_horizontalSlider_specularRotation_valueChanged(int _value)
{
	lineEdit_specularRotation->setText(QString::number(_value / 100.0));
}

void UiWidget::on_horizontalSlider_transmissionWeight_valueChanged(int _value)
{
	lineEdit_transmissionWeight->setText(QString::number(_value / 100.0));
}

void UiWidget::on_horizontalSlider_transmissionDepth_valueChanged(int _value)
{
	lineEdit_transmissionDepth->setText(QString::number(_value / 100.0));
}

void UiWidget::on_horizontalSlider_subsurfaceWeight_valueChanged(int _value)
{
	lineEdit_subsurfaceWeight->setText(QString::number(_value / 100.0));
}

void UiWidget::on_horizontalSlider_subsurfaceScale_valueChanged(int _value)
{
	lineEdit_subsurfaceScale->setText(QString::number(_value / 100.0));
}

void UiWidget::on_horizontalSlider_coatSpecularWeight_valueChanged(int _value)
{
	lineEdit_coatSpecularWeight->setText(QString::number(_value / 100.0));
}

void UiWidget::on_horizontalSlider_coatSpecularRoughness_valueChanged(int _value)
{
	lineEdit_coatSpecularRoughness->setText(QString::number(_value / 100.0));
}

void UiWidget::on_horizontalSlider_emissionWeight_valueChanged(int _value)
{
	lineEdit_emissionWeight->setText(QString::number(_value / 100.0));
}

void UiWidget::on_horizontalSlider_light1_intensity_valueChanged(int _value)
{
	lineEdit_light1_intensity->setText(QString::number(_value / 100.0));
}

void UiWidget::on_horizontalSlider_light1_positionx_valueChanged(int _value)
{
	lineEdit_light1_positionx->setText(QString::number(_value / 1000.0));
}

void UiWidget::on_horizontalSlider_light1_positiony_valueChanged(int _value)
{
	lineEdit_light1_positiony->setText(QString::number(_value / 1000.0));
}

void UiWidget::on_horizontalSlider_light1_positionz_valueChanged(int _value)
{
	lineEdit_light1_positionz->setText(QString::number(_value / 1000.0));
}

void UiWidget::on_pushButton_updateShader_clicked()
{
	emit sendDataQString2(lineEdit_vertexShaderPath->text(),
		                  lineEdit_geometryShaderPath->text(),
		                  lineEdit_fragmentShaderPath->text());
}
