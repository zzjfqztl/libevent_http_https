#include "DownloadThread.h"

DownloadThread::DownloadThread()
{
	m_downloadManager = NULL;
}

DownloadThread::~DownloadThread()
{

}

void DownloadThread::reciveTargetAddress(const QString& strTargetAddress)
{
	m_strTargetAddress = strTargetAddress;
}

void DownloadThread::reciveDownloadAddress(const QString& strSourceAddress)
{
	m_strSourceAddress = strSourceAddress;
}

void DownloadThread::run()
{
	//��ʼ����
	if (m_downloadManager == NULL)
	{
		m_downloadManager = new DownLoadManager;
		QObject::connect(m_downloadManager, SIGNAL(signalDownloadProcess(qint64, qint64)), this, SIGNAL(signalDownloadProcess(qint64, qint64)));
		QObject::connect(m_downloadManager, SIGNAL(signalReplyFinished(int)), this, SIGNAL(signalReplyFinished(int)));
	}

	this->removeFile(m_strTargetAddress);//ԭ��������ڣ���ԭ�����غõ��ļ�ɾ����
	m_downloadManager->downloadFile(m_strSourceAddress, m_strTargetAddress);//����Ŀ���ַ
}

void DownloadThread::recivePauseDownload()
{
	if (m_downloadManager != NULL)
	{
		m_downloadManager->stopDownload();
		m_downloadManager->setClickPause();
	}
}

void DownloadThread::reciveCloseDownload()
{
	if (m_downloadManager != NULL)
	{
		m_downloadManager->closeDownload();

	}
}

void DownloadThread::removeFile(QString fileName)
{
	//ԭ���ļ�ɾ����
	QFileInfo fileInfo(fileName);
	if (fileInfo.exists())
	{
		QFile::remove(fileName);
	}
}

