#pragma once

// system
#include <map>
#include <cassert>

// internal
#include "ShapeCommon.h"

enum class StateInfo { SELECT_LINE, SELECT_FACE, DRAW_LINE, DRAW_FACE, CAMERA_MOVE, EDIT_POINT, EDIT_LINE, EDIT_FACE };

class SceneState;

class SceneStateMachine
{
public:

	static SceneStateMachine* GetInstance();

	SceneState* GetCurrentState() const { return mCurrentState; }

	void SetCurrentState(StateInfo state, std::function<SceneState* ()> stateFactory);

private:
	// singleton
	static SceneStateMachine* gInstance;

	// instance
	SceneState* mCurrentState = nullptr;
	std::map<StateInfo, SceneState*> mStates;
};

