/**
 * 主窗体部分
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ComboBoxFrame;
class QTextEdit;
class QCheckBox;
class QPushButton;
class FileSelectFrame;
class QLineEdit;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

public slots:
    void onGetClick();
    void onPostClick();
    void onPutClick();
    void onDeleteClick();
    void onUploadClick();
    void onDownloadClick();
    void onClearClick();
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onUploadProgress(qint64 bytesSent, qint64 bytesTotal);
private:
    /**
     * @brief 主界面初始化
    */
    void InitUI();

private:
    QLineEdit* idLineEdit_ = nullptr;
    QLineEdit* psdLineEdit_ = nullptr;
    FileSelectFrame* fileSelectFrame_ = nullptr;
    QPushButton* btnDownload_ = nullptr;
    FileSelectFrame* fileDirFrame_ = nullptr;
    QPushButton* btnUpload_ = nullptr;
    QPushButton* btnGet_ = nullptr;
    QPushButton* btnPost_ = nullptr;
    QPushButton* btnPut_ = nullptr;
    QPushButton* btnDelete_ = nullptr;
    QPushButton* btnClear_ = nullptr;
    QTextEdit* textEdit_ = nullptr;
    QString token_;
    
};

#endif // MAINWINDOW_H
