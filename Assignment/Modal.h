#pragma once

// qt
#include <QtWidgets>
#include <QtCore>

// internal
#include "ShapeCommon.h"

class PropertiesModal : public QDialog
{
public:
	PropertiesModal(Shape* shape, QWidget* parent = nullptr);

private:
	Shape* mShape = nullptr;
	QColorDialog* mColorModal = nullptr;
	QPushButton* mColorButton = nullptr;
	QColor mCurrentColor;

	void ApplyChanges();
};
