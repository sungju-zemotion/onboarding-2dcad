#include "SceneState.h"

void SelectLineState::Render(QPainter* painter, const Camera& camera)
{
	SceneState::Render(painter, camera);
}

void SelectLineState::HandleMousePressEvent(QMouseEvent* event, const Camera& camera)
{
	Scene* scene = Scene::GetInstance();
	const QPointF viewPoint = event->position();

	// point 
	Point* point = scene->HitTestPoint(viewPoint, camera);
	if (point)
	{
		scene->SetCurrentFocus(point->GetId());
		SetDragEnterPoint(event->position());
		// state change to edit point
		Enter(StateInfo::EDIT_POINT, []() {
			return new EditPointState;
			});
		EditPointState* state = dynamic_cast<EditPointState*>(
			SceneStateMachine::GetInstance()->GetCurrentState()
			);
		state->SetPoint(point);

		return;
	}

	// line
	Line* line = scene->HitTestLine(viewPoint, camera);

	if (line)
	{
		scene->SetCurrentFocus(line->GetId());
		SetDragEnterPoint(event->position());
		// state change to edit line
		Enter(StateInfo::EDIT_LINE, []() {
			return new EditLineState;
			});
		EditLineState* state = dynamic_cast<EditLineState*>(
			SceneStateMachine::GetInstance()->GetCurrentState()
			);
		state->SetLine(line);
	}

	return;
}
