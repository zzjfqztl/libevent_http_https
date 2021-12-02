/**
 * �Ի�����࣬�������������֣������������ɱ��⼰�رհ�ť���
 * ����ʱ��Ҫ��������ı�����
 */
#ifndef BASEDIALOG_H
#define BASEDIALOG_H

#include <QWidget>
#include "QDialog"

class DialogTitleFrame;
class BaseDialog : public QDialog
{
    Q_OBJECT
public:
	/// <summary>
    /// ���캯��
    /// </summary>
    /// <param name="title">�����ı�</param>
    /// <param name="parent">������ָ��</param>
    explicit BaseDialog(const QString& title, QWidget *parent = nullptr);

public slots:
    void onClose();
protected:
	/// <summary>
    /// �¼�����������Ҫ�����϶��������ƶ�����
    /// </summary>
    /// <param name="watched"></param>
    /// <param name="event"></param>
    /// <returns></returns>
	bool eventFilter(QObject* watched, QEvent* event);
private:
    void InitUI();
private:
    DialogTitleFrame* titleFrame_ = nullptr;
    QString titleContent_;
};

#endif // BASEDIALOG_H
