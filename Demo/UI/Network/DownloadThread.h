#pragma once
#ifndef DOWNLOAD_THREAD_H
#define  DOWNLOAD_THREAD_H

#include <QThread>
#include <QtCore>
#include "Downloadmanager.h"

/**
 * @brief �����ļ��߳�
*/
class DownloadThread : public QThread
{
	Q_OBJECT

public:
	/**
	 * @brief ���캯��
	*/
	DownloadThread();

	/**
	 * ��������
	 */
	~DownloadThread();

private:
	/**
	 * @brief �Ƴ��ļ�
	 * @param fileName ɾ��ָ���ļ�
	*/
	void removeFile(QString fileName);

private slots :
	/**
	 * @brief ִ�к���
	*/
	void run();

	/**
	 * @brief ����Դ���ص�ַ�¼�
	 * @param strSourceAddress ���ص�ַ
	*/
	void reciveDownloadAddress(const QString& strSourceAddress);

	/**
	 * @brief ����Ŀ�����ص�ַ�¼�
	 * @param strTargetAddress Ŀ���ַ
	*/
	void reciveTargetAddress(const QString& strTargetAddress);

	/**
	 * @brief ��ͣ����
	*/
	void recivePauseDownload();

	/**
	 * @brief ֹͣ����
	*/
	void reciveCloseDownload();

signals:
	/**
	 * @brief �����ļ�����
	 * @param bytesReceived �������ļ����ֽ���
	 * @param bytesTotal �ļ����ֽ���
	*/
	void signalDownloadProcess(qint64, qint64);

	/**
	 * @brief ���󷵻��ź�
	 * @param  
	*/
	void signalReplyFinished(int);

private:
	QString m_strTargetAddress;//����Ŀ���ַ

	DownLoadManager* m_downloadManager;//���ع���

	QString m_strSourceAddress;//������Դ���ص�ַ
};


#endif // !downloadThread_H
