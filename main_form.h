#ifndef MAIN_FORM_H
#define MAIN_FORM_H

#include <Windows.h>

#include "Ui/mainUi.h"
#include "SelectColorWidget.h"
#include "class_ogl_widget.h"

#include <QtCore/QPointer>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtCore/QDebug>
#include <QtGui/QPaintEvent>
#include <QtCore/QSize>

#include <QtGui/QDoubleValidator>
#include <QtCore/QFile>
#include <QtCore/QLocale>
#include <QtUiTools/QtUiTools>

#include <maya/MGlobal.h>
#include <maya/MObject.h>
#include <maya/MStringArray.h>


class UiWidget : public QWidget, private Ui::Form
{
	Q_OBJECT

public:
	UiWidget(QWidget* parent = Q_NULLPTR);
	virtual	~UiWidget();

	std::string envPluginPath;

Q_SIGNALS:
	void sendDataQString2(QString, QString, QString);
Q_SIGNALS:
	void sendDataConstCharPointerInt(const char*, float);

private Q_SLOTS:
    void on_lineEdit_diffuseWeight_textChanged(QString _value);
	void on_lineEdit_diffuseRoughness_textChanged(QString _value);
	void on_lineEdit_metalness_textChanged(QString _value);
	void on_lineEdit_specularWeight_textChanged(QString _value);
	void on_lineEdit_specularRoughness_textChanged(QString _value);
	void on_lineEdit_specularIOR_textChanged(QString _value);
	void on_lineEdit_specularAnisotropy_textChanged(QString _value);
	void on_lineEdit_specularRotation_textChanged(QString _value);
	void on_lineEdit_transmissionWeight_textChanged(QString _value);
	void on_lineEdit_transmissionDepth_textChanged(QString _value);
	void on_lineEdit_subsurfaceWeight_textChanged(QString _value);
	void on_lineEdit_subsurfaceScale_textChanged(QString _value);
	void on_lineEdit_coatSpecularWeight_textChanged(QString _value);
	void on_lineEdit_coatSpecularRoughness_textChanged(QString _value);
	void on_lineEdit_emissionWeight_textChanged(QString _value);
	void on_lineEdit_light1_intensity_textChanged(QString _value);
	void on_lineEdit_light1_positionx_textChanged(QString _value);
	void on_lineEdit_light1_positiony_textChanged(QString _value);
	void on_lineEdit_light1_positionz_textChanged(QString _value);

	void on_horizontalSlider_diffuseWeight_valueChanged(int _value);
	void on_horizontalSlider_diffuseRoughness_valueChanged(int _value);
	void on_horizontalSlider_metalness_valueChanged(int _value);
	void on_horizontalSlider_specularWeight_valueChanged(int _value);
	void on_horizontalSlider_specularRoughness_valueChanged(int _value);
	void on_horizontalSlider_specularIOR_valueChanged(int _value);
	void on_horizontalSlider_specularAnisotropy_valueChanged(int _value);
	void on_horizontalSlider_specularRotation_valueChanged(int _value);
	void on_horizontalSlider_transmissionWeight_valueChanged(int _value);
	void on_horizontalSlider_transmissionDepth_valueChanged(int _value);
	void on_horizontalSlider_subsurfaceWeight_valueChanged(int _value);
	void on_horizontalSlider_subsurfaceScale_valueChanged(int _value);
	void on_horizontalSlider_coatSpecularWeight_valueChanged(int _value);
	void on_horizontalSlider_coatSpecularRoughness_valueChanged(int _value);
	void on_horizontalSlider_emissionWeight_valueChanged(int _value);
	void on_horizontalSlider_light1_intensity_valueChanged(int _value);
	void on_horizontalSlider_light1_positionx_valueChanged(int _value);
	void on_horizontalSlider_light1_positiony_valueChanged(int _value);
	void on_horizontalSlider_light1_positionz_valueChanged(int _value);

	void on_pushButton_updateShader_clicked();

private:
	QPointer<QObject>        fCurrentDialog;
};

#endif
