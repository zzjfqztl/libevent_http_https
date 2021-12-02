#include "DownloadManager.h"
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QDir>

#define DOWNLOAD_FILE_SUFFIX	"_tmp"

#pragma warning(disable:4099)

DownLoadManager::DownLoadManager(QObject *parent)
	: QObject(parent)
	, m_networkManager(NULL)
	, m_url(QUrl(""))
	, m_fileName("")
	, m_bytesReceived(0)
	, m_bytesTotal(0)
	, m_bytesCurrentReceived(0)
	, m_isStop(true)
	, m_bytesDown(0)
	, m_bClickPause(false)
{
	_timeOut = new QTimer(this);

	connect(_timeOut, SIGNAL(timeout()), this, SLOT(handleTimeOut()));

	qEvent = new QEventLoop();

	m_reply = Q_NULLPTR;
}

DownLoadManager::~DownLoadManager()
{}

QString DownLoadManager::getDownloadUrl()
{
	return m_url.toString();
}

void DownLoadManager::downloadFile(QString url, QString fileName)
{
	// ��ֹ������أ�ֻ����ֹͣ��־����Ϊtrueʱ�Ž�������;
	if (m_isStop)
	{
		m_qStrUrl			= url;	
		m_qStrFilePath	= fileName;
		m_isStop				= false;
		m_bClickPause		= false;

		m_url					= QUrl(url);
		m_fileName			= fileName + DOWNLOAD_FILE_SUFFIX;// ����ǰ�ļ�������Ϊ��ʱ�ļ������������ʱ�޸Ļ���;

		this->removeFileterFiles(m_fileName);
		//��������ļ��Ƿ���ڻ��汾���ļ�����������������
		QFileInfo fileInfo(m_fileName);
		m_bytesCurrentReceived = fileInfo.size();

		m_networkManager = new QNetworkAccessManager(this);
		connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

		QNetworkRequest request;
		request.setUrl(m_url);

		if (this->bSupportBreak(m_url))
		{
			//�ϵ�������Ҫ���Range����ͷ
			QString strRange = QString("bytes=%1-").arg(m_bytesCurrentReceived);
			request.setRawHeader("Range", strRange.toLatin1());
		}
		else
			m_bytesCurrentReceived = 0;

		if (m_bytesCurrentReceived <= 0)
		{
			removeFile(m_fileName);
		}

		m_reply =  m_networkManager->get(QNetworkRequest(request));
		connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(onDownloadProgress(qint64, qint64)));
		connect(m_reply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
		connect(m_reply, SIGNAL(finished()), this, SLOT(onFinished()));
		connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));

		//����ÿ5s�������ʱ�������쳣
 		_timeOut->start(5000);

		if (!qEvent->isRunning())
		{
			qEvent->exec();
		}
	}
}

void DownLoadManager::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	if (!m_isStop)
	{
		m_bytesReceived		= bytesReceived;
		m_bytesTotal			= bytesTotal;

		// �������ؽ���;(���� m_bytesCurrentReceived ��Ϊ�˶ϵ�����ʱ֮ǰ���ص��ֽ�)
		emit signalDownloadProcess(bytesReceived + m_bytesCurrentReceived, bytesTotal + m_bytesCurrentReceived);
	}
}

void DownLoadManager::onReadyRead()
{
	if (m_reply == NULL) return;

	if (!m_isStop)
	{
		QFile file(m_fileName);
		if (file.open(QIODevice::WriteOnly | QIODevice::Append))
		{
			file.write(m_reply->readAll());
		}
		file.close();
	}
}

void DownLoadManager::onFinished()
{
	m_isStop = true;
	if (m_reply == NULL) return;
	
	QVariant statusCode = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

	if (m_reply->error() == QNetworkReply::NoError)
	{
		// ��������ʱ�ļ�;
		QFileInfo fileInfo(m_fileName);
		if (fileInfo.exists()) 
		{
			int index = m_fileName.lastIndexOf(DOWNLOAD_FILE_SUFFIX);
			QString realName = m_fileName.left(index);
			QFile::rename(m_fileName, realName);
		}
		qDebug() << "Finished" ;
	}
	else
	{
		// �д����������;
		QString strError = m_reply->errorString();
		qDebug() << "__________" + strError;
	}

	qEvent->exit();
	emit signalReplyFinished(statusCode.toInt());
}

void DownLoadManager::onError(QNetworkReply::NetworkError code)
{
	QString strError = m_reply->errorString();
	qDebug() << "__________" + strError << code;

	closeDownload();
	emit signalDownloadError();
}

void DownLoadManager::handleTimeOut()
{
	if (m_bytesDown != m_bytesReceived) {
		m_bytesDown = m_bytesReceived;
	}
	else if( !m_bClickPause){

		//����Ͽ�ʱֹͣ����
		if (m_reply != NULL) 
		{
			stopDownload();
			m_timeElapsed.start();
		}

		//30����ѭ����ʱ(30�����ٴ�����������Լ�������)
		int nElapsed = m_timeElapsed.elapsed();
		if (nElapsed >= 1 * 30 * 60 * 1000) 
		{
			_timeOut->stop();
			return;
		}

		//�������
		bool bConnect =	this->bSupportBreak(m_qStrUrl);
		if (bConnect)
		{
			_timeOut->stop();
			this->downloadFile(m_qStrUrl, m_qStrFilePath);
		}

	}
}

void DownLoadManager::stopWork()
{
	m_isStop = true;
	if (m_reply != NULL)
	{
		disconnect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(onDownloadProgress(qint64, qint64)));
		disconnect(m_reply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
		disconnect(m_reply, SIGNAL(finished()), this, SLOT(onFinished()));
		disconnect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
		m_reply->abort();
		m_reply->deleteLater();
		m_reply = NULL;
	}
}

void DownLoadManager::stopDownload()
{
	if (!m_isStop)
	{
		//��¼��ǰ�Ѿ������ֽ�����
		QFileInfo info(m_fileName);
		m_bytesCurrentReceived = info.size();

		stopWork();
	}
}

void DownLoadManager::reset()
{
	m_bytesCurrentReceived = 0;
	m_bytesReceived = 0;
	m_bytesTotal = 0;
}

void DownLoadManager::removeFile(QString fileName)
{
	// ɾ�������ص���ʱ�ļ�;
	QFileInfo fileInfo(fileName);
	if (fileInfo.exists())
	{
		QFile::remove(fileName);
	}
}

void DownLoadManager::removeFileterFiles(QString fileName)
{
	QString  qStrPath =  QCoreApplication::applicationDirPath() + "/Download/";
	QDir dir(qStrPath);

	//����ɸѡ����
	QStringList filterLst;
	filterLst << "*.exe_tmp";
	dir.setNameFilters(filterLst);

	QFileInfoList fileInfoLst = dir.entryInfoList(filterLst);
	for (int i = 0; i < fileInfoLst.count(); i++)
	{
		QString fileComplete =fileInfoLst[i].filePath();
		if (fileComplete.compare(fileName) !=0){
			removeFile(fileComplete);
		}
	}
}

void DownLoadManager::closeDownload()
{
	stopWork();
	reset();
	removeFile(m_fileName);
}

void DownLoadManager::replyFinished(QNetworkReply*) /* download finished */
{
	
}

bool DownLoadManager::bSupportBreak(const QUrl &url)
{
	QNetworkAccessManager manager;
	QEventLoop loop;
	QTimer timer;

	QNetworkRequest request;
	request.setUrl(url);
	request.setRawHeader("Range","bytes=0-9");

	//�������󣬻�ȡ�ļ���ַ��ͷ����Ϣ;
	QNetworkReply *reply = manager.get(request);
	if (!reply)		return false;

	connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));

	timer.start(2000);
	loop.exec(QEventLoop::ExcludeUserInputEvents);

	if (reply->error() != QNetworkReply::NoError)
	{
		// ����������;
		qDebug() << reply->errorString();
		return false;
	}
	else if (!timer.isActive())
	{
		// ����ʱ��ʱ,δ��ȡ���ļ���Ϣ;
		qDebug() << "Request Timeout";
		return false;
	}
	timer.stop();

	QByteArray range = reply->rawHeader("Content-Range");
	if (!QString::fromLocal8Bit(range).isEmpty())
		return true;

	return false;
}