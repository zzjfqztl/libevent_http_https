#ifndef MESSAGEBOXDIALOG_H
#define MESSAGEBOXDIALOG_H

#include <QWidget>
#include <QDialog>

class QLabel;
class QPushButton;
class QFrame;
class QVBoxLayout;
class QTextEdit;
class MessageBoxDialog : public QDialog
{
    Q_OBJECT
public:
	enum MessageButtonType 
	{
		MB_NONE,
		MB_OK,
		MB_OKCANCEL,
	};
	explicit MessageBoxDialog(const QString& title, const QString& text,MessageButtonType buttons = MB_OK, QWidget* parent = nullptr);
	void initUI();
	void initTitleFrame();
	void initCenterFrame();
	void initBottomFrame();

public slots:
	void onOK();
	void onClose();

protected:
	bool eventFilter(QObject* watched, QEvent* event);
private:

	QVBoxLayout* mainLayout_ = nullptr;

    QLabel* titleLabel_ = nullptr;
	QPushButton* btnClose_ = nullptr;
	QFrame* titleFrame_ = nullptr;

	QVBoxLayout* centerLayout_ = nullptr;
	QFrame* centerFrame_ = nullptr;

	QFrame* messageInfoFrame_ = nullptr;
	QTextEdit* messageTextEdit_ = nullptr;

	QFrame* bottomFrame_ = nullptr;
	QPushButton* btnOK_ = nullptr;
	QPushButton* btnCancel_ = nullptr;

	QString titleContent;
	QString msgContent;
	MessageButtonType messageButtonType_ = MB_NONE;
};

#endif // MESSAGEBOXDIALOG_H
