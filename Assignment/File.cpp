#include "File.h"

QJsonObject LoadJson(const QString& filepath)
{
	QFile loadFile(filepath);
	if (!loadFile.open(QIODevice::ReadOnly))
	{
		qDebug() << "cannot open the file";

		return QJsonObject();
	}

	QByteArray saveData = loadFile.readAll();
	QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

	return loadDoc.object();
}

void SaveJson(const QJsonObject& json, const QString& filepath)
{
	QFile saveFile(filepath);
	if (!saveFile.open(QIODevice::WriteOnly))
	{
		qDebug() << "cannot open the save file";

		return;
	}

	saveFile.write(QJsonDocument(json).toJson());
}
