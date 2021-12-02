#include "NetworkManager.h"
#include "QMap"
#include "JsonManager.h"
#include "HttpClient.h"

QString NetworkManager::token_ = QString();
NetworkManager::NetworkManager(QObject *parent) : QObject(parent)
{

}

void NetworkManager::SetToken(const QString& token)
{
	token_ = token;
}

QString NetworkManager::GetToken()
{
	return token_;
}

int NetworkManager::CheckUserLogin(const QString& strUrl, const QString& userId, const QString& userPsd, QString& token)
{
	QMap<QString, QVariant> dataMap;
	dataMap[QString("id")] = userId;
	dataMap[QString("psd")] = userPsd;
	QString strJson = JsonManager::GenerateJsonString(dataMap);

	HttpClient httpClient;
	QString strReponse;
	httpClient.PostRequest(strUrl, QString(), strJson, strReponse, true);
	int nCode = 0;
	JsonManager::CheckUserLogin(strReponse, nCode, token);
	return nCode;
}

