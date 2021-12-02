#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>

/**
 * @brief �������ģ��
*/
class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);

    /**
     * @brief ����tokenֵ
     * @param token token ֵ����ֵ�Ǵӷ���˻�ȡ�ģ��ǿͻ������ӷ���˵�Ψһ��ʶ
    */
    static void SetToken(const QString& token);

    /**
     * @brief ��ȡ��ǰ�ͻ��˵�tokenֵ
     * @return ��ǰ�ͻ��˵�tokenֵ
    */
    static QString GetToken();

    /**
     * @brief ����¼�û��Ƿ��¼�ɹ�
     * @param strUrl �ͻ��������¼ʱ��url
     * @param userId �û���¼Ψһ��ʶ
     * @param userPsd �û���¼����
     * @param token ��¼�ɹ��󣬷��ص�tokenֵ
     * @return 0 �ɹ����������ص�¼������
    */
   static int CheckUserLogin(const QString& strUrl, const QString& userId, const QString& userPsd, QString& token);
signals:

private:
   static QString token_;
};

#endif // NETWORKMANAGER_H
