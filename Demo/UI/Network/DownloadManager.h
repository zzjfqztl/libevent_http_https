#pragma once
#ifndef DOWNLOAD_MANAGER_H
#define DOWNLOAD_MANAGER_H


#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkConfigurationManager>
#include <QEventLoop>
#include <QTimer>
#include <QtCore/QtCore>

/**
 * @brief �����ļ�������
*/
class DownLoadManager : public QObject
{
	Q_OBJECT

public:
	/**
	 * @brief �����ļ����������캯��
	*/
	DownLoadManager(QObject *parent = Q_NULLPTR);

	/**
	 * ��������
	 */
	~DownLoadManager();

public:
	/**
	 * @brief ��ʼ�����ļ�
	 * @param url ָ����url��ַ
	 * @param fileName ������ļ�ȫ·��
	*/
	void downloadFile(QString url, QString fileName);

	/**
	 * @brief ��ͣ����
	*/
	void stopDownload();
	
	/**
	 * @brief �ر�����
	*/
	void closeDownload();
	
	/**
	 * @brief ��ȡ���ص�Դ��ַ
	 * @return 
	*/
	QString getDownloadUrl();
	
	/**
	 * @brief ·������������
	*/
	void reset();

	/**
	 * @brief ��ͣ����
	*/
	void setClickPause() { m_bClickPause = true; }
private:
	/**
	 * @brief ֹͣ����
	*/
	void stopWork();

	/**
	 * @brief ɾ����ʱ�ļ�
	 * @param fileName ��ʱ�ļ�
	*/
	void removeFile(QString fileName);

	/**
	 * @brief ����ɾ������Ҫ���صĵ�ǰ�汾�����ļ�֮���Tmp�ļ�
	 * @param fileName ��ǰ�汾�����ļ�
	*/
	void removeFileterFiles(QString fileName);

	/**
	 * @brief ��ȡ�Ƿ�֧�ֶϵ�����
	 * @param url ָ��url
	 * @return 
	*/
	bool bSupportBreak(const QUrl &url);

signals:
	/**
	 * @brief �����ļ�����
	 * @param bytesReceived �������ļ����ֽ���
	 * @param bytesTotal �ļ����ֽ���
	*/
	void signalDownloadProcess(qint64 bytesReceived, qint64 bytesTotal);//�������ع����ź�
	
	/**
	 * @brief ������������ź�
	 * @param  ������ɱ��
	*/
	void signalReplyFinished(int);
	
	/**
	 * @brief �������ع����г��ִ����ź�
	*/
	void signalDownloadError();

private slots:
	/**
	 * @brief �����ļ�����
	 * @param bytesReceived �������ļ����ֽ���
	 * @param bytesTotal �ļ����ֽ���
	*/
	void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
	
	/**
	 * @brief ���ع��������ݷ����ı��¼�
	*/
	void onReadyRead();
	
	/**
	 * @brief ��������¼�
	*/
	void onFinished();
	
	/**
	 * @brief ���ش����¼�
	 * @param code ������
	*/
	void onError(QNetworkReply::NetworkError code);

	/**
	 * @brief ��������¼�
	 * @param  
	*/
	void replyFinished(QNetworkReply*);

	/**
	 * @brief ��ʱ�������¼�
	*/
	void handleTimeOut();

private:
	QNetworkAccessManager* m_networkManager;//���ع������

	QNetworkReply* m_reply;//���󷵻ض���

	QUrl m_url;//�������ص�url��ַ

	QString m_fileName;//Ŀ�����ص�ַ

	qint64 m_bytesReceived;//���ν��յ������ֽڴ�С

	qint64 m_bytesTotal;//���ݵ����ֽڴ�С

	qint64 m_bytesCurrentReceived;//�ϵ�֮ǰ�Ѿ����ص��ֽڴ�С

	bool m_isStop;//��ʶ�Ƿ�ֹͣ����

	QEventLoop*   qEvent;//�������ص��¼�ѭ��

	QTimer        *_timeOut;//���ڿ��Ƴ�ʱ�Ķ�ʱ��

	qint64 m_bytesDown;//�ϵ�֮ǰ�Ѿ����ص��ֽڴ�С�������ж�һ��ʱ���������Ƿ��б仯��

	QString		m_qStrUrl;//Դ���ص�ַ

	QString		m_qStrFilePath;//Ŀ�����ص�ַ

	QTime m_timeElapsed;//���ڳ�ʱ30���ӵĶ�ʱ������

	bool m_bClickPause;
};

#endif // 
