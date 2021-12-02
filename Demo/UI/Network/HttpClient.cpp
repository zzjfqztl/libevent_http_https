#include "HttpClient.h"
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include "glog/logging.h"
#include <QJsonDocument>
#include "QFile"
#include "QDir"
#include <QHttpMultiPart>
#include <QDebug>
#include <QAuthenticator>
#include "QTextCodec"

QString GBK2UTF8(const QString& str)
{
	QTextCodec* utf8 = QTextCodec::codecForName("UTF-8");
	return utf8->toUnicode(str.toUtf8());
}

QString UTF82GBK(const QString& str)
{
	QTextCodec* gbk = QTextCodec::codecForName("GB18030");
	return gbk->toUnicode(str.toLocal8Bit());
}

std::string GBK2UTF8(std::string& str)
{
	QString temp = QString::fromLocal8Bit(str.c_str());
	std::string ret = temp.toUtf8().data();
	return ret;
}

std::string UTF82GBK(std::string& str)
{
	QString temp = QString::fromUtf8(str.c_str());
	std::string ret = temp.toLocal8Bit().data();
	return ret;
}

HttpClient::HttpClient(QObject *parent) : QObject(parent)
{

}

int HttpClient::GetRequest(const QString& strUrl, const QString& strParamers, QString& strReponse, bool isAsync)
{
	qDebug() << "QSslSocket=" << QSslSocket::sslLibraryBuildVersionString();
	qDebug() << "OpenSSL支持情况:" << QSslSocket::supportsSsl();
	//生成对应的网络请求
	QNetworkRequest request;
	QString requestHeader = strUrl;
	QString fullRequest = requestHeader + (strParamers.isEmpty() ? "" : QString("?%1").arg(strParamers.toUtf8().constData()));
	QUrl qurl(fullRequest);
	request.setUrl(qurl);
	DLOG(INFO) << "Get URL: " << qurl.toString().toLocal8Bit().constData();
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=utf-8");
	//发送Get请求
	QNetworkAccessManager manager;
	QNetworkReply* reply = manager.get(request);
	QEventLoop eventLoop;
	QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
	if (!isAsync)
		eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
	else
		eventLoop.exec();

	strReponse.clear();
	if (reply->error() == QNetworkReply::NoError) {
		strReponse = reply->readAll();
		DLOG(INFO) << QString("Get Response: %1").arg(strReponse).toLocal8Bit().constData();
	}
	else {
		//failure
		strReponse = reply->readAll();
		DLOG(ERROR) << QString("Network Get Ex: %1").arg(reply->errorString()).toLocal8Bit().constData();
	}
	int nerror = reply->error();
	reply->deleteLater();
	return nerror;

	////解析返回的Json结果
	//QByteArray replyData = reply->readAll();
	//QJsonParseError json_error;
	//QJsonDocument jsonDoc(QJsonDocument::fromJson(replyData, &json_error));
	//if (json_error.error != QJsonParseError::NoError)
	//{
	//	return -1;
	//}
	//QJsonObject rootObj = jsonDoc.object();
	//QString codeStr = rootObj.value("code").toString();
	//if (codeStr.compare("200") == 0)
	//{
	//	//返回代码为200的时候证明请求成功对包含的结构数据进行处理
	//	if (rootObj.contains("result"))
	//	{

	//	}
	//	return 0;
	//}
	//else
	//{
	//	//请求失败对对应的处理
	//	return codeStr.toInt();
	//}
}

int HttpClient::PostRequest(const QString& strUrl, const QString& strParamers, const QString& strJson, QString& strReponse, bool bJson /*= true*/, bool isAsync /*= true*/)
{
	//生成对应的网络请求
	QNetworkRequest request;
	QString requestHeader = strUrl;
	QString fullRequest = requestHeader + (strParamers.isEmpty() ? "" : QString("?%1").arg(strParamers.toUtf8().constData()));
	QUrl qurl(fullRequest);
	request.setUrl(qurl);
	DLOG(INFO) << "Post URL: " << qurl.toString().toLocal8Bit().constData();
	DLOG(INFO) << "Post Body: " << strJson.toLocal8Bit().constData();
	if (bJson)
	{
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
	}
	else
	{
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=utf-8");
	}
    if (bJson)
    {
		QJsonParseError json_error;
		QJsonDocument jsonDoc(QJsonDocument::fromJson(strJson.toUtf8(), &json_error));
		if (json_error.error != QJsonParseError::NoError)
		{
			DLOG(ERROR) << QString("Json data: ").toLocal8Bit().constData()<< QString(" %1 isn't right").arg(strJson).toLocal8Bit().constData();
			DLOG(ERROR) << QString("Json Ex: %1").arg(json_error.errorString()).toLocal8Bit().constData();
			return -1;
		}
    }
	if (strUrl.startsWith("https://"))
	{
		setSSlConfig(request);
	}

	QByteArray data = strJson.toUtf8();
	//发送POST请求
	QNetworkAccessManager manager;
	qDebug() << manager.supportedSchemes();
	QNetworkReply* reply = manager.post(request,data);
	QEventLoop eventLoop;
	QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
	if (!isAsync)
		eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
	else
		eventLoop.exec();

	strReponse.clear();
	if (reply->error() == QNetworkReply::NoError) {
		strReponse = reply->readAll();
		DLOG(INFO) << QString("Post Response: %1").arg(strReponse).toLocal8Bit().constData();
	}
	else {
		//failure
		DLOG(ERROR) << QString("Post Network Ex: %1").arg(reply->errorString()).toLocal8Bit().constData();
	}
	int nerror = reply->error();
	reply->deleteLater();
	return nerror;
}

int HttpClient::PutRequest(const QString& strUrl, const QString& strParamers, const QString& strJson, QString& strReponse, bool bJson /*= true*/, bool isAsync /*= true*/)
{
	//生成对应的网络请求
	QNetworkRequest request;
	QString requestHeader = strUrl;
	QString fullRequest = requestHeader + (strParamers.isEmpty() ? "" : QString("?%1").arg(strParamers.toUtf8().constData()));
	QUrl qurl(fullRequest);
	request.setUrl(qurl);
	DLOG(INFO) << "PUT URL: " << qurl.toString().toLocal8Bit().constData();
	DLOG(INFO) << "PUT Body: " << strJson.toLocal8Bit().constData();
	if (bJson)
	{
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
	}
	else
	{
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=utf-8");
	}
	if (bJson)
	{
		QJsonParseError json_error;
		QJsonDocument jsonDoc(QJsonDocument::fromJson(strJson.toUtf8(), &json_error));
		if (json_error.error != QJsonParseError::NoError)
		{
			DLOG(ERROR) << QString("PUT Json data: ").toLocal8Bit().constData() << QString(" %1 isn't right").arg(strJson).toLocal8Bit().constData();
			DLOG(ERROR) << QString("Put Json Ex: %1").arg(json_error.errorString()).toLocal8Bit().constData();
			return -1;
		}
	}

	QByteArray data = strJson.toUtf8();
	//发送PUT请求
	QNetworkAccessManager manager;
	QNetworkReply* reply = manager.put(request, data);
	QEventLoop eventLoop;
	QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
	if (!isAsync)
		eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
	else
		eventLoop.exec();

	strReponse.clear();
	if (reply->error() == QNetworkReply::NoError) {
		strReponse = reply->readAll();
		DLOG(INFO) << QString("PUT Response: %1").arg(strReponse).toLocal8Bit().constData();
	}
	else {
		//failure
		DLOG(ERROR) << QString("PUT Network Ex: %1").arg(reply->errorString()).toLocal8Bit().constData();
	}
	int nerror = reply->error();
	reply->deleteLater();
	return nerror;
}

int HttpClient::DeleteRequest(const QString& strUrl, const QString& strParamers, QString& strReponse, bool isAsync /*= true*/)
{
	//生成对应的网络请求
	QNetworkRequest request;
	QString requestHeader = strUrl;
	QString fullRequest = requestHeader + (strParamers.isEmpty() ? "" : QString("?%1").arg(strParamers.toUtf8().constData()));
	QUrl qurl(fullRequest);
	request.setUrl(qurl);
	DLOG(INFO) << "Delete URL: " << qurl.toString().toLocal8Bit().constData();
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=utf-8");
	//发送Delete请求
	QNetworkAccessManager manager;
	QNetworkReply* reply = manager.deleteResource(request);
	QEventLoop eventLoop;
	QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
	if (!isAsync)
		eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
	else
		eventLoop.exec();

	strReponse.clear();
	if (reply->error() == QNetworkReply::NoError) {
		strReponse = reply->readAll();
		DLOG(INFO) << QString("Delete Response: %1").arg(strReponse).toLocal8Bit().constData();
	}
	else {
		//failure
		DLOG(ERROR) << QString("Delete Network Get Ex: %1").arg(reply->errorString()).toLocal8Bit().constData();
	}
	int nerror = reply->error();
	reply->deleteLater();
	return nerror;

}

int HttpClient::DownloadFileRequest(const QString& strUrl, const QString& strParamers, const QString& savePath, bool isAsync)
{
	QNetworkRequest request;
	QString requestHeader = strUrl;
	QString fullRequest = requestHeader + (strParamers.isEmpty() ? "" : QString("?%1").arg(strParamers.toUtf8().constData()));
	QUrl qurl(fullRequest);
	request.setUrl(qurl);
	DLOG(INFO) << "DownloadFile URL: " << qurl.toString().toLocal8Bit().constData();

	QString strSaveFile = savePath;
	strSaveFile = QDir::fromNativeSeparators(strSaveFile);
	if (strSaveFile.endsWith("/"))
	{
		QDir().mkpath(strSaveFile);
		strSaveFile = strSaveFile + QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz");
	}
	else
	{
		QDir().mkpath(strSaveFile.mid(0, strSaveFile.lastIndexOf("/")));
	}
	QFile::remove(strSaveFile);
	QNetworkAccessManager manager;
	/*connect(&manager, &QNetworkAccessManager::authenticationRequired, this, [&](QNetworkReply* networkreply, QAuthenticator* auth){
		auth->setUser(QString("VP4YSTVNPUNU7VCLONM6"));
		auth->setPassword(QString("2kNJGOWBb0ygkZmX49uyTPzfyezbTcCxJ1Wf45kn"));
});*/
//#ifndef QT_NO_SSL
//	connect(&manager, &QNetworkAccessManager::sslErrors,
//		this, [&](QNetworkReply* networkReply, const QList<QSslError>& error) {
//			networkReply->ignoreSslErrors();
//		});
//#endif
	QEventLoop eventLoop;
	QNetworkReply* reply = manager.get(request);

	connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
	connect(reply, &QNetworkReply::readyRead, this, [this, reply, strSaveFile]() {
		QFile file(strSaveFile);
		if (file.open(QIODevice::WriteOnly | QIODevice::Append))
		{
			file.write(reply->readAll());
		}
		file.close();
		});
	//如果需要下载进度需要关联对应的信号
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SIGNAL(downloadProgressSgn(qint64, qint64)));
	if (!isAsync)
		eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
	else
		eventLoop.exec();

	int nRet = 0;
	if (reply->error() == QNetworkReply::NoError) {
		nRet = 0;
	}
	else {
		//failure
		nRet = reply->error();
		DLOG(ERROR) << QString("Download File Network Get Ex: %1").arg(reply->errorString()).toLocal8Bit().constData();
	}
	reply->deleteLater();
	return nRet;
}

int HttpClient::uploadFileRequest(const QString& strUrl, const QString& strParamers, const QString& uploadfile, QString& strReponse, bool isAsync /*= true*/)
{
	QFileInfo file_info(uploadfile);
	if (!QFile::exists(uploadfile) || file_info.isDir())
	{
		return -1;
	}
	QNetworkAccessManager manager;
	QEventLoop eventLoop;
	QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
	QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
	//QHttpPart showPart;
	///*showPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"action\""));
	//showPart.setBody("show");*/
	//showPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"filecount\""));
	//showPart.setBody("1");
	//multiPart->append(showPart);

	/*QHttpPart uPart;
	uPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"uploadway\""));
	uPart.setBody("qt");
	multiPart->append(uPart);*/
	
	
	QHttpPart filePart;

	QString suffix = file_info.suffix();
	QString fileName = file_info.fileName();
	if (suffix.compare("png", Qt::CaseInsensitive) == 0)
	{
		filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/png; charset=utf-8"));
	}
	else if (suffix.compare("jpg", Qt::CaseInsensitive) == 0 || suffix.compare("jpeg", Qt::CaseInsensitive) == 0)
	{
		filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg; charset=utf-8"));
	}
	else if (suffix.compare("gif", Qt::CaseInsensitive) == 0)
	{
		filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/gif; charset=utf-8"));
	}
	else if (suffix.compare("apk", Qt::CaseInsensitive) == 0)
	{
		filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/vnd.android.package-archive; charset=utf-8"));
	}
	else if (suffix.compare("mp3", Qt::CaseInsensitive) == 0)
	{
		filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("audio/mp3; charset=utf-8"));
	}
	else if (suffix.compare("mp4", Qt::CaseInsensitive) == 0)
	{
		filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("video/mpeg4; charset=utf-8"));
	}
	else if (suffix.compare("avi", Qt::CaseInsensitive) == 0)
	{
		filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("video/avi; charset=utf-8"));
	}
	else
	{
		filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/octet-stream"));
	}
	QString strContent = QString("form-data; name=\"upload\"; filename=\"%1\"").arg(fileName.toUtf8().data());
	filePart.setHeader(QNetworkRequest::ContentDispositionHeader
		, QVariant(strContent));
	QFile* file = new QFile(uploadfile);
	file->open(QIODevice::ReadOnly);
	/*QByteArray bytes = file->readAll();
	filePart.setBody(bytes);*/

	filePart.setBodyDevice(file);
	file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart
	multiPart->append(filePart);

	//QHttpPart suffixPart;
	//suffixPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file1\""));
	//suffixPart.setBody(fileName.toUtf8());
	//multiPart->append(suffixPart);

	/*QHttpPart sumitPart;
	sumitPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"fileToUpload\""));
	sumitPart.setBody("fileToUpload");
	multiPart->append(sumitPart);*/

	QString requestHeader = strUrl;
	QString fullRequest = requestHeader + (strParamers.isEmpty() ? "" : QString("?%1").arg(strParamers.toUtf8().constData()));
	QUrl qurl(fullRequest);
	QNetworkRequest request;
	request.setUrl(qurl);
	DLOG(INFO) << "UploadFile URL: " << qurl.toString().toLocal8Bit().constData();


	QNetworkReply* reply = manager.post(request, multiPart);
	multiPart->setParent(reply); // delete the multiPart with the reply
	connect(reply, SIGNAL(uploadProgress(qint64, qint64)), this, SIGNAL(uploadProgressSgn(qint64, qint64)));

	if (!isAsync)
		eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
	else
		eventLoop.exec();

	int nRet = 0;
	if (reply->error() == QNetworkReply::NoError) {
		nRet = 0;
		strReponse = reply->readAll();
		DLOG(INFO) << QString("Upload Response: %1").arg(strReponse).toLocal8Bit().constData();
	}
	else {
		//failure
		nRet = reply->error();
		DLOG(ERROR) << QString("Upload File Network Get Ex: %1").arg(reply->errorString()).toLocal8Bit().constData();
	}
	reply->deleteLater();
	return nRet;
}

void HttpClient::setSSlConfig(QNetworkRequest& request)
{
	QSslConfiguration config;
	QSslConfiguration conf = request.sslConfiguration();
	conf.setPeerVerifyMode(QSslSocket::VerifyNone);
	conf.setProtocol(QSsl::AnyProtocol);
	request.setSslConfiguration(conf);

}

