#pragma once

// internal
#include "ShapeCommon.h"
#include "Line.h"
#include "Face.h"
#include "Transform.h"
#include "File.h"

using LineTable = QMap<ShapeId, Line*>;
using FaceTable = QMap<ShapeId, Face*>;
using PointTable = QMap<ShapeId, Point*>;

class Scene
{
public:
	// singleton
	static Scene* GetInstance();

	// individual shapes
	const ShapeId GetNextShapeId() { return ++mCurrentId; }
	ShapeId GetCurrentFocus() { return mCurrentFocus; }
	void SetCurrentFocus(ShapeId id) { mCurrentFocus = id; }

	// points
	PointTable::const_iterator GetPointBegin() const { return mPoints.begin(); }
	PointTable::const_iterator GetPointEnd() const { return mPoints.end(); }
	Point* GetPoint(ShapeId id) const;

	Point* HitTestPoint(const QPointF& viewPoint, const Camera& camera) const;

	// lines
	LineTable::const_iterator GetLineBegin() const { return mLines.begin(); }
	LineTable::const_iterator GetLineEnd() const { return mLines.end(); }

	void AddLine(Line* line) { mLines.insert(line->GetId(), line); }
	Line* GetLine(ShapeId id) const;
	void DeleteLine(ShapeId id);
	Line* HitTestLine(const QPointF& viewPoint, const Camera& camera) const;

	// face
	FaceTable::const_iterator GetFaceBegin() const { return mFaces.begin(); }
	FaceTable::const_iterator GetFaceEnd() const { return mFaces.end(); }

	void AddFace(Face* face) { mFaces.insert(face->GetId(), face); }
	Face* GetFace(ShapeId id) const;
	void DeleteFace(ShapeId id);
	Face* HitTestFace(const QPointF& viewPoint, const Camera& camera) const;

	// file io
	void Save(QJsonObject& json);
	void Load(const QJsonObject& json);

private:
	// singleton
	Scene() = default;
	static Scene* gInstance;

	ShapeId mCurrentId = 0;
	ShapeId mCurrentFocus = -1;

	LineTable mLines;
	FaceTable mFaces;
	PointTable mPoints;
};
