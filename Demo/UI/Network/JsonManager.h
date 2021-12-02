#ifndef JSONMANAGER_H
#define JSONMANAGER_H

#include <QObject>
#include "QMap"
#include "QVariant"
/**
 * @brief Json ���ݽ�������װ
*/
class JsonManager : public QObject
{
    Q_OBJECT
public:
    explicit JsonManager(QObject *parent = nullptr);

    /**
     * @brief ����map ����json�ַ���
     * @param dataMap ���ɽ�����Ҫ������
     * @return QString json �ַ���
    */
    static QString GenerateJsonString(const QMap<QString, QVariant>& dataMap);

    /**
     * @brief ���ݷ���˷��ص���ϢУ���Ƿ��¼�ɹ�
     * @param strReponse ����˷��ص���Ϣ
     * @param nCode ����˷��صĴ�����; ����0 ���������صĴ����룻С��0json �������Ӧ�ĸ�ֵ
     * @param token У��ɹ������˷��ص�token
     * @return true �ɹ���false ʧ��
    */
    static bool CheckUserLogin(const QString& strReponse, int& nCode, QString& token);
signals:

};

#endif // JSONMANAGER_H
