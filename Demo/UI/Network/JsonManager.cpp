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
	//json ����У��
	if (json_error.error != QJsonParseError::NoError)
	{
		nCode -= (int)json_error.error;
		return false;
	}
	QJsonObject rootObj = jsonDoc.object();
	QString strResult = rootObj.value("result").toString();

	if (strResult.compare("success") == 0)
	{
		//���ش���Ϊ200��ʱ��֤������ɹ��԰����Ľṹ���ݽ��д���
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
		//����ʧ�ܶԶ�Ӧ�Ĵ���
		if (rootObj.contains("code"))
		{
			QString codeStr = rootObj.value("code").toString();
			nCode = codeStr.toInt();
		}
		
		return false;
	}
}

