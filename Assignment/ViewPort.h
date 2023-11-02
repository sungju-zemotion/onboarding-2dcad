#pragma once

// system
#include <cassert>

// qt
#include <QMap>
#include <QtWidgets>

// internal
#include "ShapeCommon.h"
#include "Line.h"
#include "Face.h"
#include "SceneStateMachine.h"
#include "Camera.h"

const int WIDTH = 600;
const int HEIGHT = 400;

class ViewPort : public QWidget
{
	Q_OBJECT;

public:
	ViewPort(QWidget* parent = nullptr);

	void paintEvent(QPaintEvent* event) override;

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;

private:
	SceneStateMachine* mSceneStateMachine = nullptr;
	Camera mCamera;
	QPainter* mPainter = nullptr;

	void Render(const Camera& camera);		// currently running a loop; render queue could be used instead
	void DrawGrid();

public slots:
	void ShowContextMenu(const QPoint& point);
};

