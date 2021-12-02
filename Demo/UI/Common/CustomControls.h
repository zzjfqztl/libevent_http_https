/**
 * �Զ���ؼ����ϣ����ļ������Զ�������Կؼ�
 */

#ifndef CUSTOMCONTROLS_H
#define CUSTOMCONTROLS_H
#include "QFrame"
#include "QMap"
#include "QList"
#include <vector>
#include "UI/Define/EnumType.h"

class QComboBox;
class QLabel;
class QPushButton;
class QLineEdit;

/**
 * @brief QLabel + QComboBox ��ϵ��Զ���ؼ�
 * QLabel ����QComboBox �Ϸ���Ҳ����QComboBox�����
*/
class ComboBoxFrame :public QFrame
{
	Q_OBJECT
public:

	/**
	 * @brief 
	*/
	/**
	 * @brief QLabel + QComboBox ��ϵ��Զ���ؼ� ���캯��
	 * @param title QLabel����
	 * @param parent ������ָ��
	 * @param org QLabel��QComboBox λ�ñ�ʶ
	*/
	explicit ComboBoxFrame(const QString& title, QWidget* parent = nullptr, Qt::Orientation org = Qt::Orientation::Horizontal);

	/**
	 * @brief ��ʼ��QComboBox��ʾ�����б���Ϣ
	 * @param itemList ��ʾ�����б�(�ɹ��ʻ�����)
	 * @param itemDataList ���б��Ӧ�����ݣ�������ʻ�������仯
	*/
	void InitData(const QStringList& itemList, const QStringList& itemDataList);
	
	/**
	 * @brief ��ȡQComboBox��ǰ���Ӧ��������Ϣ
	 * @return ����QComboBox��ǰ���ӦΨһ��ʶ������Ϣ
	*/
	QString GetCurrentData() const;
	
	/**
	 * @brief ����QComboBox��ǰ���Ӧ��Ψһ������Ϣ���Ӷ����õ�ǰ��
	 * @param text QComboBox��ǰ���ӦΨһ��ʶ������Ϣ
	*/
	void SetCurrentData(const QString& text);
	
	/**
	 * @brief ����QLabel�ı����뷽ʽ
	 * @param align ���뷽ʽ
	*/
	void SetTitleLabelAlign(Qt::Alignment align);
signals:

	/**
	 * @brief QComboBox��ǰ��仯�󷢳����ź�
	 * @param QString ������Ϣ
	*/
	void currentTextChangedSgn(const QString&);
public slots:

	/**
	 * @brief QComboBox��ǰ��仯��Ĳ���
	 * @param nIndex ��ǰ�������
	*/
	void onCurrentIndexChanged(int nIndex);
private:

	/**
	 * @brief ComboBoxFrame ������Ϣ
	*/
	void InitUI();
private:
	QLabel* titleLabel_ = nullptr;
	QComboBox* comboBox_ = nullptr;
	Qt::Orientation orientation_ = Qt::Orientation::Horizontal;
	QString titleContent_ = nullptr;
};

/**
 * @brief �ļ������ļ���ѡ�� �Զ���ؼ�
*/
class FileSelectFrame :public QFrame
{
	Q_OBJECT

public:

	/**
	 * @brief �ļ������ļ���ѡ���Զ���ؼ� ���캯��
	 * @param title	����
	 * @param bDir	false ѡ���ļ� true ѡ���ļ���
	 * @param bMultSelect �Ƿ�֧�ֶ�ѡ 
	 * @param parent ������ָ�� 
	*/
	explicit FileSelectFrame(const QString& title, QWidget* parent = nullptr, bool bDir = false, bool bMultSelect =false);

	/**
	 * @brief ��ȡѡ����ļ��б�
	 * @return ����QStringList����
	*/
	QStringList GetFileList() const;

	/**
	 * @brief �����Ƿ��ļ���
	 * @param bDir true �ļ��� false �ļ�
	*/
	void SetDirFlag(bool bDir);

	/**
	 * @brief �����Ƿ��ѡ�ļ�
	 * @param bMultSelect true ��ѡ false ��ѡ
	*/
	void SetMultSelect(bool bMultSelect);
	void clearText();
signals:
	/**
	 * @brief �ļ�ѡ����ͳ����ź�
	 * @param filelist ѡ����ļ��б�
	*/
	void selectFilesSgn(const QStringList& filelist);

public slots:
	/**
	 * @brief Select ����
	*/
	void onSelectClick();
private:
	/**
	 * @brief FileSelectFrame �����ʼ��
	*/
	void InitUI();
private:
	QLabel* titleLabel_ = nullptr;
	QLineEdit* lineEdit_ = nullptr;
	QPushButton* btnSelect_ = nullptr;
	QString titleContent_;
	bool bDir_ = false;
	bool bMultSelect_ = false;
};
#endif // CUSTOMCONTROLS_H