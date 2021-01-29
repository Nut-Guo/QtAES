#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qhexview.h"
#include "aes_ni.h"
#include "aes_step.h"
#include "aes_table.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QRandomGenerator>
#include <string>
#include <stdint.h>
#include <x86intrin.h>
#include "document/buffer/qmemorybuffer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpenPlainText_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, "Open File", ".");
    QHexDocument* document = QHexDocument::fromFile<QMemoryBuffer>(path);
    ui->plainHexView->setDocument(document);
}

void MainWindow::on_actionAES_NI_Encrypt_triggered()
{
    auto *plainText = ui->plainHexView->document();
    QByteArray plainBytes = plainText->read(0, plainText->length());
    auto plainString = plainBytes.toStdString();
    auto keyBytes = ui->keyHexView->document()->read(0, 16);
    auto keyString = keyBytes.toStdString();
    AES_NI aes_ni(keyString);
    auto result = aes_ni.encrypt(plainString);
    QHexDocument* document = QHexDocument::fromMemory<QMemoryBuffer>(QByteArray(result.c_str(), result.size()));
    ui->cipherHexView->setDocument(document);
}

void MainWindow::on_actionOpenKey_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, "Open Key", ".");
    QHexDocument* document = QHexDocument::fromFile<QMemoryBuffer>(path);
    ui->keyHexView->setDocument(document);
}

void MainWindow::on_actionAES_NI_Decrypt_triggered()
{
    auto *cipherText = ui->cipherHexView->document();
    QByteArray cipherBytes = cipherText->read(0, cipherText->length());
    auto cipherString = cipherBytes.toStdString();
    auto keyBytes = ui->keyHexView->document()->read(0, 16);
    auto keyString = keyBytes.toStdString();
    AES_NI aes_ni(keyString);
    auto result = aes_ni.decrypt(cipherString);
    QHexDocument* document = QHexDocument::fromMemory<QMemoryBuffer>(QByteArray(result.c_str(), result.size()));
    ui->plainHexView->setDocument(document);
}

void MainWindow::on_actionAES_Step_Encrypt_triggered()
{
    auto *plainText = ui->plainHexView->document();
    QByteArray plainBytes = plainText->read(0, plainText->length());
    auto plainString = plainBytes.toStdString();
    auto keyBytes = ui->keyHexView->document()->read(0, 16);
    auto keyString = keyBytes.toStdString();
    AES_Step aes_step(keyString);
    auto result = aes_step.encrypt(plainString);
    QHexDocument* document = QHexDocument::fromMemory<QMemoryBuffer>(QByteArray(result.c_str(), result.size()));
    ui->cipherHexView->setDocument(document);
}

void MainWindow::on_actionAES_Step_Decrypt_triggered()
{
    auto *cipherText = ui->cipherHexView->document();
    QByteArray cipherBytes = cipherText->read(0, cipherText->length());
    auto cipherString = cipherBytes.toStdString();
    auto keyBytes = ui->keyHexView->document()->read(0, 16);
    auto keyString = keyBytes.toStdString();
    AES_Step aes_step(keyString);
    auto result = aes_step.decrypt(cipherString);
    QHexDocument* document = QHexDocument::fromMemory<QMemoryBuffer>(QByteArray(result.c_str(), result.size()));
    ui->plainHexView->setDocument(document);
}

void MainWindow::on_actionAES_TableEncrypt_triggered()
{
    auto *plainText = ui->plainHexView->document();
    QByteArray plainBytes = plainText->read(0, plainText->length());
    auto plainString = plainBytes.toStdString();
    auto keyBytes = ui->keyHexView->document()->read(0, 16);
    auto keyString = keyBytes.toStdString();
    AES_Table aes_table(keyString);
    auto result = aes_table.encrypt(plainString);
    QHexDocument* document = QHexDocument::fromMemory<QMemoryBuffer>(QByteArray(result.c_str(), result.size()));
    ui->cipherHexView->setDocument(document);
}

void MainWindow::on_actionAES_TableDecrypt_triggered()
{
    auto *cipherText = ui->cipherHexView->document();
    QByteArray cipherBytes = cipherText->read(0, cipherText->length());
    auto cipherString = cipherBytes.toStdString();
    auto keyBytes = ui->keyHexView->document()->read(0, 16);
    auto keyString = keyBytes.toStdString();
    AES_Table aes_table(keyString);
    auto result = aes_table.decrypt(cipherString);
    QHexDocument* document = QHexDocument::fromMemory<QMemoryBuffer>(QByteArray(result.c_str(), result.size()));
    ui->plainHexView->setDocument(document);
}

void MainWindow::on_actionSavePlainText_triggered()
{
    QString path = QFileDialog::getSaveFileName(this);
    if (path.isEmpty())
        return;
    else {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }
        ui->plainHexView->document()->saveTo(&file);
    }
}

void MainWindow::on_actionSaveCipherText_triggered()
{
    QString path = QFileDialog::getSaveFileName(this);
    if (path.isEmpty())
        return;
    else {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }
        ui->cipherHexView->document()->saveTo(&file);
    }
}

void MainWindow::on_actionSaveKey_triggered()
{
    QString path = QFileDialog::getSaveFileName(this);
    if (path.isEmpty())
        return;
    else {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }
        ui->keyHexView->document()->saveTo(&file);
    }
}

void MainWindow::on_pushButton_clicked()
{
    auto numbytes = ui->lineEdit->text().toLongLong();
    QByteArray plainText(numbytes, Qt::Initialization());
    QRandomGenerator::global()->generate(plainText.begin(), plainText.end());
    auto plainString = plainText.toStdString();
    auto keyBytes = ui->keyHexView->document()->read(0, 16);
    auto keyString = keyBytes.toStdString();
    AES_NI aes_ni(keyString);
    AES_Step aes_step(keyString);
    AES_Table aes_table(keyString);
    uint64_t ni_start = _rdtsc();
    aes_ni.encrypt(plainString);
    uint64_t ni_stop = _rdtsc();
    uint64_t ni_cycles = ni_stop - ni_start;
    uint64_t step_start = _rdtsc();
    aes_step.encrypt(plainString);
    uint64_t step_stop = _rdtsc();
    uint64_t step_cycles = step_stop - step_start;
    uint64_t table_start = _rdtsc();
    aes_table.encrypt(plainString);
    uint64_t table_stop = _rdtsc();
    uint64_t table_cycles = table_stop - table_start;
    auto *ni_cycles_item = new QTableWidgetItem(tr("%1").arg(ni_cycles));
    auto *ni_cpb_item = new QTableWidgetItem(tr("%1").arg((double)ni_cycles / numbytes));
    auto *step_cycles_item = new QTableWidgetItem(tr("%1").arg(step_cycles));
    auto *step_cpb_item = new QTableWidgetItem(tr("%1").arg((double)step_cycles / numbytes));
    auto *table_cycles_item = new QTableWidgetItem(tr("%1").arg(table_cycles));
    auto *table_cpb_item = new QTableWidgetItem(tr("%1").arg((double)table_cycles / numbytes));
    ui->tableWidget->setItem(0, 0, ni_cycles_item);
    ui->tableWidget->setItem(0, 1, ni_cpb_item);
    ui->tableWidget->setItem(1, 0, step_cycles_item);
    ui->tableWidget->setItem(1, 1, step_cpb_item);
    ui->tableWidget->setItem(2, 0, table_cycles_item);
    ui->tableWidget->setItem(2, 1, table_cpb_item);
}
