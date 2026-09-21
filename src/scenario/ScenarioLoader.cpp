#include "ScenarioLoader.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>

/*
filePath
   ↓
文件存在吗？
   ↓
能打开吗？
   ↓
readAll()
   ↓
JSON语法正确吗？
   ↓
最外层是 {} 吗？
   ↓
name 是 string 吗？
   ↓
obstacles 是 [] 吗？
   ↓
每个 obstacle 是 {} 吗？
   ↓
x 是 number 吗？
y 是 number 吗？
   ↓
QPointF(x, y)
   ↓
Scenario
*/

bool ScenarioLoader::load(
    const QString &filePath,
    Scenario &scenario,
    QString &errorMessage)
{
    // ==========================================
    // 1. 检查文件是否存在
    // ==========================================

    QFileInfo fileInfo(filePath);

    if (!fileInfo.exists())
    {
        errorMessage =
            QString(
                "Scenario file not found: %1")
                .arg(filePath);

        return false;
    }

    // ==========================================
    // 2. 打开文件
    // ==========================================

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly))
    {
        errorMessage =
            QString(
                "Failed to open scenario file: %1")
                .arg(filePath);

        return false;
    }

    // ==========================================
    // 3. 一次读取整个 JSON 文件
    // ==========================================

    QByteArray jsonData =
        file.readAll();

    file.close();

    // ==========================================
    // 4. 解析 JSON
    // ==========================================

    QJsonParseError parseError;

    QJsonDocument document =
        QJsonDocument::fromJson(
            jsonData,
            &parseError);

    if (parseError.error !=
        QJsonParseError::NoError)
    {
        errorMessage =
            QString(
                "Scenario JSON parse failed: %1")
                .arg(
                    parseError.errorString());

        return false;
    }

    // ==========================================
    // 5. 最外层必须是 {}
    // ==========================================

    if (!document.isObject())
    {
        errorMessage =
            "Scenario root must be a JSON object";

        return false;
    }
    /*
    把 JSON 根对象取出来。
    现在：  root 代表：
    {
    "name":"test",
    "obstacles":[]
    }
    */
    QJsonObject root =
        document.object();

    // ==========================================
    // 6. 检查 name
    // ==========================================

    QJsonValue nameValue =
        root.value("name");

    if (!nameValue.isString())
    {
        errorMessage =
            "Scenario missing valid string field: name";

        return false;
    }

    QString scenarioName =
        nameValue.toString();

    if (scenarioName.isEmpty())
    {
        errorMessage =
            "Scenario name cannot be empty";

        return false;
    }

    // ==========================================
    // 7. 检查 obstacles
    // ==========================================

    QJsonValue obstaclesValue =
        root.value("obstacles");

    if (!obstaclesValue.isArray())
    {
        errorMessage =
            "Scenario missing valid array field: obstacles";

        return false;
    }

    QJsonArray obstacleArray =
        obstaclesValue.toArray();

    // 先解析到临时容器。
    // 整个 JSON 全部合法以后，
    // 再真正写进 scenario。
    QVector<QPointF> obstacles;

    // ==========================================
    // 8. 逐个检查障碍物
    // ==========================================

    for (int i = 0;
         i < obstacleArray.size();
         ++i)
    {
        QJsonValue obstacleValue =
            obstacleArray.at(i);

        if (!obstacleValue.isObject())
        {
            errorMessage =
                QString(
                    "Obstacle at index %1 "
                    "must be a JSON object")
                    .arg(i);

            return false;
        }

        QJsonObject obstacleObject =
            obstacleValue.toObject();

        QJsonValue xValue =
            obstacleObject.value("x");

        QJsonValue yValue =
            obstacleObject.value("y");

        if (!xValue.isDouble())
        {
            errorMessage =
                QString(
                    "Obstacle at index %1 "
                    "missing numeric field: x")
                    .arg(i);

            return false;
        }

        if (!yValue.isDouble())
        {
            errorMessage =
                QString(
                    "Obstacle at index %1 "
                    "missing numeric field: y")
                    .arg(i);

            return false;
        }

        double x =
            xValue.toDouble();

        double y =
            yValue.toDouble();

        obstacles.append(
            QPointF(x, y));
    }

    // ==========================================
    // 9. 所有检查通过以后再更新输出
    // ==========================================

    scenario.name =
        scenarioName;

    scenario.obstacles =
        obstacles;

    errorMessage.clear();

    return true;
}