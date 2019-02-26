#include "SelectColorWidget.h"

#include <QtWidgets/QColorDialog>


SelectColorWidget::SelectColorWidget(QWidget* parent)
{
}

SelectColorWidget::~SelectColorWidget()
{
}

void SelectColorWidget::mouseReleaseEvent(QMouseEvent *event)
{
	QColor newColor = QColorDialog::getColor(color, parentWidget());
	if (newColor != color)
	{
		this->color = newColor;
		setColor();
		emit colorHasChanged(newColor);
	}
}

void SelectColorWidget::setColor()
{
	setStyleSheet("background-color: " + color.name());
}

void SelectColorWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QStyleOption StyleOpt;

	StyleOpt.initFrom(this);
	style()->drawPrimitive(QStyle::PE_Widget, &StyleOpt, &painter, this);
}
