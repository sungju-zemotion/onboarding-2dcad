#pragma once

#include "SceneState.h"

class DrawLineState : public SceneState
{
public:
	void HandleMousePressEvent(QMouseEvent* event, const Camera& camera) override;
	void HandleMouseMoveEvent(QMouseEvent* event, const Camera& camera) override;
	void HandleMouseReleaseEvent(QMouseEvent* event, const Camera&) override;

private:
	Line* mTempLine = nullptr;
};
