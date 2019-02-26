#ifndef SELECTCOLORWIDGET_H
#define SELECTCOLORWIDGET_H

#include <QtGui/QColor>
#include <QtWidgets/QWidget>
#include <Qt3dInput/QMouseEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QStyle>
#include <QtWidgets/QStyleOption>

#include <maya/MGlobal.h>
#include <maya/MQtUtil.h>


class SelectColorWidget : public QWidget
{
	Q_OBJECT

public:
	SelectColorWidget(QWidget* parent = Q_NULLPTR);
	~SelectColorWidget();

	QColor  color;
	void    setColor();

protected:
	void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;

Q_SIGNALS:
	void colorHasChanged(QColor);
};

#endif
