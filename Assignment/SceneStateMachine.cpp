#include "SceneStateMachine.h"

SceneStateMachine* SceneStateMachine::gInstance = nullptr;

SceneStateMachine* SceneStateMachine::GetInstance()
{
	if (gInstance == nullptr)
	{
		gInstance = new SceneStateMachine;
	}

	return gInstance;
}

void SceneStateMachine::SetCurrentState(StateInfo state, std::function<SceneState*()> stateFactory)
{
	if (mStates.find(state) == mStates.end())
	{
		mStates[state] = stateFactory();
	}

	mCurrentState = mStates[state];
	assert(mCurrentState);
}
