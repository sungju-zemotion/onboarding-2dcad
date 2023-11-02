#pragma once

// internal
#include "SceneState.h"

class DrawFaceState : public SceneState
{
public:
	void HandleMousePressEvent(QMouseEvent* event, const Camera& camera) override;
	void HandleMouseMoveEvent(QMouseEvent* event, const Camera& camera) override;
	void HandleMouseReleaseEvent(QMouseEvent* event, const Camera& camera) override;

private:
	QVector<Line*> mTempLines;
};
