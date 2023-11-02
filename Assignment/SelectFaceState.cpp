#include "SceneState.h"

void SelectFaceState::Render(QPainter* painter, const Camera& camera)
{
	SceneState::Render(painter, camera);
}

void SelectFaceState::HandleMousePressEvent(QMouseEvent* event, const Camera& camera)
{
	Scene* scene = Scene::GetInstance();

	// point
	Point* point = scene->HitTestPoint(event->position(), camera);
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
	Line* line = scene->HitTestLine(event->position(), camera);
	if (line)
	{
		scene->SetCurrentFocus(line->GetId());
		SetDragEnterPoint(event->position());

		Enter(StateInfo::EDIT_LINE, []() {
			return new EditLineState;
			});
		EditLineState* state = dynamic_cast<EditLineState*>(
			SceneStateMachine::GetInstance()->GetCurrentState()
			);
		state->SetLine(line);

		return;
	}

	// face
	Face* face = scene->HitTestFace(event->position(), camera);
	if (face)
	{
		scene->SetCurrentFocus(face->GetId());
		SetDragEnterPoint(event->position());

		Enter(StateInfo::EDIT_FACE, []() {
			return new EditFaceState;
			});
		EditFaceState* state = dynamic_cast<EditFaceState*>(
			SceneStateMachine::GetInstance()->GetCurrentState()
			);
		state->SetFace(face);
	}

	return;
}
