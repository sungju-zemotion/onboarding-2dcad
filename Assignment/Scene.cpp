#include "Scene.h"

Scene* Scene::gInstance = nullptr;

Scene* Scene::GetInstance()
{
	if (gInstance == nullptr)
	{
		gInstance = new Scene;
	}

	return gInstance;
}

Point* Scene::HitTestPoint(const QPointF& viewPoint, const Camera& camera) const
{
	for (LineTable::const_iterator it = mLines.begin(); it != mLines.end(); it++)
	{
		Point* start = (*it)->GetStartPoint().get();
		if (start->HitTest(viewPoint.x(), viewPoint.y(), camera))
		{
			return start;
		}
		Point* end = (*it)->GetEndPoint().get();
		if (end->HitTest(viewPoint.x(), viewPoint.y(), camera))
		{
			return end;
		}
	}

	return nullptr;
}

Point* Scene::GetPoint(ShapeId id) const
{
	if (mPoints.contains(id))
	{
		return mPoints.value(id);
	}

	return nullptr;
}

Line* Scene::GetLine(ShapeId id) const
{
	if (mLines.contains(id))
	{
		return mLines.value(id);
	}

	return nullptr;
}

void Scene::DeleteLine(ShapeId id)
{
	if (mLines.contains(id))
	{
		mLines.remove(id);
	}
}

Line* Scene::HitTestLine(const QPointF& viewPoint, const Camera& camera) const
{
	for (LineTable::const_iterator it = mLines.begin(); it != mLines.end(); it++)
	{
		if ((*it)->HitTest(viewPoint.x(), viewPoint.y(), camera))
		{
			return (*it);
		}
	}

	return nullptr;
}

Face* Scene::GetFace(ShapeId id) const
{
	if (mFaces.contains(id))
	{
		return mFaces.value(id);
	}

	return nullptr;
}

void Scene::DeleteFace(ShapeId id)
{
	if (mFaces.contains(id))
	{
		mFaces.remove(id);
	}
}

Face* Scene::HitTestFace(const QPointF& viewPoint, const Camera& camera) const
{
	for (FaceTable::const_iterator it = mFaces.begin(); it != mFaces.end(); it++)
	{
		if ((*it)->HitTest(viewPoint.x(), viewPoint.y(), camera))
		{
			return (*it);
		}
	}

	return nullptr;
}

void Scene::Save(QJsonObject& json)
{
	// step 0: prepare for the json object
	QJsonArray pointArray, lineArray, faceArray;
	QSet<ShapeId> pointIdSet;

	// step 1: save the lines & points
	for (LineTable::const_iterator it = GetLineBegin(); it != GetLineEnd(); it++)
	{
		SaveLine(*it, lineArray);
		const Point* startPoint = (*it)->GetStartPoint().get();
		ShapeId startPointId = startPoint->GetId();
		if (!pointIdSet.contains(startPointId))
		{
			pointIdSet.insert(startPointId);
			SavePoint(startPoint, pointArray);
		}

		const Point* endPoint = (*it)->GetEndPoint().get();
		ShapeId endPointId = endPoint->GetId();
		if (!pointIdSet.contains(endPointId))
		{
			pointIdSet.insert(endPointId);
			SavePoint(endPoint, pointArray);
		}
	}

	// step 2: save the faces 
	using PointIterator = QVector<std::shared_ptr<Point>>::const_iterator;
	for (FaceTable::const_iterator it = GetFaceBegin(); it != GetFaceEnd(); it++)
	{
		SaveFace(*it, faceArray);
	}

	json["points"] = pointArray;
	json["lines"] = lineArray;
	json["faces"] = faceArray;
}

void Scene::Load(const QJsonObject& json)
{
	Scene* scene = Scene::GetInstance();
	PointTable tempPoints;

	// load points
	const QJsonArray pointArray = json["points"].toArray();
	for (const QJsonValue& value : pointArray)
	{
		Point* point = LoadPoint(value.toObject());
		tempPoints[point->GetId()] = point;
	}

	// load lines
	const QJsonArray lineArray = json["lines"].toArray();
	for (const QJsonValue& value : lineArray)
	{
		scene->AddLine(LoadLine(value.toObject(), tempPoints));
	}

	// load faces
	const QJsonArray faceArray = json["faces"].toArray();
	for (const QJsonValue& value : faceArray)
	{
		scene->AddFace(LoadFace(value.toObject(), tempPoints));
	}
}
