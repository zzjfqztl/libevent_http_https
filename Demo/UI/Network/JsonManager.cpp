#include "JsonManager.h"
#include "QJsonDocument"
#include "QJsonObject"

JsonManager::JsonManager(QObject *parent) : QObject(parent)
{

}

QString JsonManager::GenerateJsonString(const QMap<QString, QVariant>& dataMap)
{
	if (dataMap.isEmpty())
	{
		return QString();
	}
	QJsonDocument document;
	QJsonObject json;
	for (auto itr = dataMap.begin(); itr != dataMap.end(); itr++)
	{
		if (itr.key().isEmpty())
		{
			continue;
		}
		json.insert(itr.key(), itr.value().toJsonValue());
		
	}
	document.setObject(json);

	return document.toJson(QJsonDocument::Compact);
}

bool JsonManager::CheckUserLogin(const QString& strReponse, int& nCode, QString& token)
{
	nCode = 0;
	QJsonParseError json_error;
	QJsonDocument jsonDoc(QJsonDocument::fromJson(strReponse.toUtf8(), &json_error));
	//json 数据校验
	if (json_error.error != QJsonParseError::NoError)
	{
		nCode -= (int)json_error.error;
		return false;
	}
	QJsonObject rootObj = jsonDoc.object();
	QString strResult = rootObj.value("result").toString();

	if (strResult.compare("success") == 0)
	{
		//返回代码为200的时候证明请求成功对包含的结构数据进行处理
		if (rootObj.contains("token"))
		{
			token = rootObj.value("token").toString();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		//请求失败对对应的处理
		if (rootObj.contains("code"))
		{
			QString codeStr = rootObj.value("code").toString();
			nCode = codeStr.toInt();
		}
		
		return false;
	}
}

