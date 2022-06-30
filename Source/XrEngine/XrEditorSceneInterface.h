#pragma once
#include "game_graph.h"
class CObjectSpace;
class ENGINE_API XrEditorSceneInterface
{
public:
	XrEditorSceneInterface();
	virtual ~XrEditorSceneInterface();
	virtual IReader* LoadSpawn() = 0;
	virtual IGameGraph* GetGameGraph() = 0;
	virtual ILevelGraph* GetLevelGraph() = 0;
	virtual bool RayPick(const Fvector& start, const Fvector& dir, float& dist, Fvector* pt = 0, Fvector* n = 0) = 0;
};