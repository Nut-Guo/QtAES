#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "aes_ni.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_actionOpenPlainText_triggered();

    void on_actionAES_NI_Encrypt_triggered();

    void on_actionOpenKey_triggered();

    void on_actionAES_NI_Decrypt_triggered();

    void on_actionAES_Step_Encrypt_triggered();

    void on_actionAES_Step_Decrypt_triggered();

    void on_actionAES_TableEncrypt_triggered();

    void on_actionAES_TableDecrypt_triggered();

    void on_actionSavePlainText_triggered();

    void on_actionSaveCipherText_triggered();

    void on_actionSaveKey_triggered();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    AES* aes;
};
#endif // MAINWINDOW_H
