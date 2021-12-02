#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QWidget>
#include <QDialog>

class QLabel;
class QPushButton;
class QFrame;
class QLineEdit;
class QCheckBox;
class QVBoxLayout;
class QTextEdit;
class QComboBox;
class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    void initUI();
    void initTitleFrame();
    void initConnect();
    void initCenterFrame();

    void initLoginInfo();
    void initLoginBottom();

    void initProxyFrame();
    void initOfflineFrame();
    void initExtensionFrame();



public slots:
    void onClose();
    void onUpDown();
    void onLogin();

    void onCheckFirewall(bool bcheck);
    void onCheckProxy(bool bcheck);
    void onCheckOffline(bool bcheck);

    void onbtnCreateHWClick();
    void onbtnRegisterLCN();
protected:
    bool eventFilter(QObject* watched, QEvent* event);
private:
    QLabel* titleLabel_ = nullptr;
    QPushButton* btnClose_ = nullptr;
    QFrame* titleFrame_ = nullptr;

    QVBoxLayout* centerLayout_ = nullptr;
    QFrame* centerFrame_ = nullptr;
    QLineEdit* idLineEdit_ = nullptr;
    QLineEdit* pwdLineEdit_ = nullptr;
    QCheckBox* ckbSaveId_ = nullptr;
    QCheckBox* ckbSavePwd_ = nullptr;
    QCheckBox* ckbAutoDeactivation_ = nullptr;
    QPushButton* btnUpDown_ = nullptr;

    QPushButton* btnLogin_ = nullptr;
    QPushButton* btnCancel_ = nullptr;
    QVBoxLayout* mainLayout_ = nullptr;

    QFrame* extensionFrame_ = nullptr;
	QCheckBox* ckbFirewall_ = nullptr;
	QCheckBox* ckbProxy_ = nullptr;
	QCheckBox* ckbOffline_ = nullptr;

    QFrame* proxyFrame_ = nullptr;
    QLineEdit* hostNameLineEdit_ = nullptr;
    QComboBox* proxyTypeComBobox_ = nullptr;
    QLineEdit* portLineEdit_ = nullptr;
    QLineEdit* proxyIdLineEdit_ = nullptr;
    QLineEdit* proxyPwdLineEdit_ = nullptr;

    QFrame* offlineFrame_ = nullptr;
    QPushButton* btnCreateHW_ = nullptr;
    QPushButton* btnRegisterLCN_ = nullptr;

    QTextEdit* textEdit_ = nullptr;
	QTextEdit* textEdit1_ = nullptr;
	QTextEdit* textEdit2_ = nullptr;
	QTextEdit* textEdit3_ = nullptr;
    bool bShowExtension_ = false;
    QString token_;
};

#endif // LOGINDIALOG_H
