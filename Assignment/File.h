#pragma once

#include <QtCore>

// reference: https://doc.qt.io/qt-6/qtcore-serialization-savegame-example.html

QJsonObject LoadJson(const QString& filepath);

void SaveJson(const QJsonObject& json, const QString& filepath);
