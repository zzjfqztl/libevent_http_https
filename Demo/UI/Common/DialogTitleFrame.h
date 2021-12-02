/**
 * �������Զ���ؼ��������������ݺ͹رհ�ť���Լ������Ƿ����
 */

#ifndef DIALOGTITLEFRAME_H
#define DIALOGTITLEFRAME_H

#include <QWidget>
#include "QFrame"

class QLabel;
class QPushButton;
class DialogTitleFrame : public QFrame
{
    Q_OBJECT
public:
	/// <summary>
	/// ���캯��
	/// </summary>
	/// <param name="title">��������</param>
	/// <param name="parent">������ָ��</param>
	/// <param name="align">�����Ƿ���б�ʾ,Ĭ�Ͼ���</param>
    explicit DialogTitleFrame(const QString& title,QWidget *parent = nullptr, Qt::Alignment align = Qt::AlignCenter);
	
	/// <summary>
	/// ���ñ�������
	/// </summary>
	/// <param name="title"></param>
	void setTitle(const QString& title);
signals:
	void closeSgn();
private:
	void initUI();
private:
	QLabel* titleLabel_ = nullptr;
	QPushButton* btnClose_ = nullptr;
	QString titleContent_;
	Qt::Alignment titleAlign_ = Qt::AlignCenter;
};

#endif // DIALOGTITLEFRAME_H
