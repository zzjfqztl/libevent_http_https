#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
class QNetworkRequest;
class HttpClient : public QObject
{
    Q_OBJECT
signals:
    
    /**
     * @brief �����ļ�����
	 * @param bytesReceived �������ļ����ֽ���
	 * @param bytesTotal �ļ����ֽ���
    */
    void downloadProgressSgn(qint64 bytesReceived, qint64 bytesTotal);

    /**
     * @brief �ϴ��ļ�����
     * @param bytesSent ���ϴ��ļ����ֽ���
     * @param bytesTotal �ļ����ֽ���
    */
    void uploadProgressSgn(qint64 bytesSent, qint64 bytesTotal);
public:
    explicit HttpClient(QObject *parent = nullptr);

    /**
     * @brief �������������ݣ�HTTP GET����
     * @param strUrl �������,�����Url��ַ,��:http://www.baidu.com
     * @param strParamers �������,ʹ�����¸�ʽname=val1&pwd=val2&��
     * @param strReponse �������,���ص����ݣ����󵽵�����
     * @param isAsync �Ƿ��첽����
     * @return int 0 �ɹ� ��0 QNetworkReply�Ĵ�������
    */
    int GetRequest(const QString& strUrl, const QString& strParamers, QString& strReponse, bool isAsync = true);

    /**
     * @brief HTTP POST ����
     * @param strUrl �������,�����Url��ַ,��:http://www.baidu.com
     * @param strParamers �������,ʹ�����¸�ʽname=val1&pwd=val2&��
     * @param strJson �������,body ���ⲿ�֣�json ���ݸ�ʽ
     * @param strReponse �������,���ص����ݣ����󵽵�����
     * @param bJson �Ƿ����json ��ʽ
     * @param isAsync �Ƿ��첽����
     * @return 0 �ɹ� -1 json ���ݸ�ʽ����ȷ ����0 QNetworkReply�Ĵ�������
    */
    int PostRequest(const QString& strUrl, const QString& strParamers, const QString& strJson, QString& strReponse, bool bJson = true, bool isAsync = true);

    /**
     * @brief HTTP PUT ����
     * @param strUrl �������,�����Url��ַ,��:http://www.baidu.com
     * @param strParamers �������,ʹ�����¸�ʽname=val1&pwd=val2&��
     * @param strJson �������,body ���ⲿ�֣�json ���ݸ�ʽ
     * @param strReponse �������,���ص����ݣ����󵽵�����
     * @param bJson �Ƿ����json ��ʽ
     * @param isAsync �Ƿ��첽����
     * @return 0 �ɹ� -1 json ���ݸ�ʽ����ȷ ����0 QNetworkReply�Ĵ�������
    */
    int PutRequest(const QString& strUrl, const QString& strParamers, const QString& strJson, QString& strReponse, bool bJson = true, bool isAsync = true);

    /**
        * @brief �������������ݣ�HTTP DELETE����
        * @param strUrl �������,�����Url��ַ,��:http://www.baidu.com
        * @param strParamers �������,ʹ�����¸�ʽname=val1&pwd=val2&��
        * @param strReponse �������,���ص����ݣ����󵽵�����
        * @param isAsync �Ƿ��첽����
        * @return int 0 �ɹ� ��0 QNetworkReply�Ĵ�������
       */
    int DeleteRequest(const QString& strUrl, const QString& strParamers, QString& strReponse, bool isAsync = true);

    /**
     * @brief �ӷ���������ļ�
     * @param strUrl �������,�����Url��ַ,��:http://www.baidu.com
     * @param strParamers �������,ʹ�����¸�ʽname=val1&pwd=val2&��
     * @param savePath ���������ļ���·��
     * @param isAsync �Ƿ��첽����,true �첽��false ͬ����Ĭ���첽
     * @return 0 �ɹ�����������QNetworkReply�Ĵ�������
    */
    int DownloadFileRequest(const QString& strUrl, const QString& strParamers, const QString& savePath, bool isAsync = true);

    /**
     * @brief �ϴ��ļ���������
     * @param strUrl strUrl �������,�����Url��ַ,��:http://www.baidu.com
     * @param strParamers strParamers �������,ʹ�����¸�ʽname=val1&pwd=val2&��
     * @param uploadfile ���ϴ����ļ�
     * @param strReponse �������,���ص����ݣ����󵽵�����
     * @param isAsync �Ƿ��첽����,true �첽��false ͬ����Ĭ���첽
     * @return 0 �ɹ�����������QNetworkReply�Ĵ�������
    */
    int uploadFileRequest(const QString& strUrl, const QString& strParamers, const QString& uploadfile, QString& strReponse, bool isAsync = true);
signals:

private:
    /**
     * @brief ssl���ã�������https
    */
    void setSSlConfig(QNetworkRequest& request);
};

#endif // HTTPCLIENT_H
