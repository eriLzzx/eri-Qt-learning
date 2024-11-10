#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    textChanged = false;
    on_actionNew_triggered();
    //初始化复制，粘贴，撤销，剪切，恢复按钮为不可用
    ui->actionCopy->setEnabled(false);
    ui->actionPaste->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionCut->setEnabled(false);


    statusLabel.setMaximumHeight(150);
    statusLabel.setText("length：" + QString::number(0) +"lines：" +QString::number(1));
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMaximumHeight(150);
    statusCursorLabel.setText("length：" + QString::number(0) +"lines：" +QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);
    //此处必须使用new方法
    QLabel *author = new QLabel(ui->statusbar);
    author->setText(tr("梁梓轩"));
    ui->statusbar->addPermanentWidget(author);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dig;
    dig.exec();
}


void MainWindow::on_actionSearch_triggered()
{
    SearchDialog dig;
    dig.exec();
}


void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog dig;
    dig.exec();
}


void MainWindow::on_actionNew_triggered()
{
    if(!userEditConfirmed())
        return;

    filePath = "";
    ui->TextEdit->clear();
    this->setWindowTitle(tr("新建文本文件 - 编辑器"));

    textChanged = false;
}


void MainWindow::on_actionOpen_triggered()
{
    if(!userEditConfirmed())
        return;
    //弹出文件对话框,让用户选择文件并获取文件名
    QString filename = QFileDialog::getOpenFileName(this,"打开文件",".",tr("Text files (*.txt) ;; ALL(*.*)"));
    if (filename.isEmpty()) {
        return; // 用户取消文件选择
    }
    //过滤器之间必须要有两个分号相隔
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "文件打开失败", QString("无法打开文件: %1").arg(filename));
        return;
    }
    //利用全局变量记录文件路径
    filePath = filename;
    // 清空当前文本框中的内容避免重复显示
    ui->TextEdit->clear();
    QTextStream in(&file);
    //一次性全部读取进来
    QString text = in.readAll();
    ui->TextEdit->insertPlainText(text);
    file.close();
    //absFilePath包含路径及其名称
    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());
    textChanged = false;
}

void MainWindow::on_actionSave_triggered()
{
    if (filePath.isEmpty()) {
        QString filename = QFileDialog::getSaveFileName(this, "保存文件", ".", tr("Text files (*.txt)"));
        if (filename.isEmpty()) {
            return; // 用户取消保存，直接退出
        }
        filePath = filename;
    }

    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "..", "打开保存文件失败");
        return;
    }

    QTextStream out(&file);
    QString text = ui->TextEdit->toPlainText();
    out << text;

    file.close();  // 只需关闭文件，无需显式调用 flush()

    // 更新窗口标题为当前文件的路径
    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());

    textChanged = false;  // 确保在文本保存后正确更新状态
}



void MainWindow::on_actionSaveAs_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,"保存文件",".",
                                                    tr("Text files (*.txt)"));
    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this,"..","打开保存文件失败");
        return;
    }

    filePath = filename;
    QTextStream out(&file);
    QString text = ui->TextEdit->toPlainText();
    out << text;
    //刷新缓冲区
    file.flush();
    file.close();
    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());
}


void MainWindow::on_TextEdit_textChanged()
{
    if(!textChanged){
        this->setWindowTitle("*" + this->windowTitle());
        textChanged = true ;
    }
}

bool MainWindow::userEditConfirmed(){
    if(textChanged){
        QString path = filePath != ""? filePath :"无标题.txt";

        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Question);
        msg.setWindowTitle("...");
        msg.setWindowFlag(Qt::Drawer);
        msg.setText(QString("是否将更改保存到\n")+"\""+ path + "\" ? ");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        int r = msg.exec();
        switch(r){
        case QMessageBox::Yes:
            on_actionSave_triggered();
            break;
        case QMessageBox::No:
            textChanged = false;
            break;
        case QMessageBox::Cancel:
            return false;
        }
    }
    return true;
}

void MainWindow::on_actionUndo_triggered()
{
    ui->TextEdit->undo();
}


void MainWindow::on_actionCut_triggered()
{
    ui->TextEdit->cut();
}


void MainWindow::on_actionCopy_triggered()
{
    ui->TextEdit->copy();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionRedo_triggered()
{
    ui->TextEdit->redo();
}


void MainWindow::on_actionPaste_triggered()
{
    ui->TextEdit->paste();
}


void MainWindow::on_TextEdit_copyAvailable(bool b)
{
    ui->actionCopy->setEnabled(b);
    ui->actionCut->setEnabled(b);
}


void MainWindow::on_TextEdit_undoAvailable(bool b)
{
    ui->actionUndo->setEnabled(b);
}


void MainWindow::on_TextEdit_redoAvailable(bool b)
{
    ui->actionRedo->setEnabled(b);
}

