#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QColorDialog>
#include <QFontDialog>
#include <QTimer>

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

    //
    QPlainTextEdit::LineWrapMode mode = ui->TextEdit->lineWrapMode();

    if(mode == QTextEdit::NoWrap){
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->actionLineWrap->setChecked(false);
    }else{
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->actionLineWrap->setChecked(true);
    }
    //
    ui->actionShowStatusBar->setCheckable(true);
    ui->actionToolBar->setChecked(true);
    //
    statusLabel.setMaximumWidth(180);
    statusLabel.setText("length：" + QString::number(0) +"     lines：" +QString::number(1));
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMaximumWidth(180);
    statusCursorLabel.setText("Ln：" + QString::number(0) +"     Col：" +QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);
    //此处必须使用new方法
    QLabel *author = new QLabel(ui->statusbar);
    author->setText(tr("梁梓轩"));
    ui->statusbar->addPermanentWidget(author);
    //当定时器超时（timeout）时，自动调用 MainWindow 类中的 autoSave 函数。实现自动保存
    autoSaveTimer = new QTimer(this);
    connect(autoSaveTimer, &QTimer::timeout, this, &MainWindow::autoSave);
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
    SearchDialog dig(this,ui->TextEdit);
    dig.exec();
}


void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog dig(this,ui->TextEdit);
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
    statusLabel.setText("length:"+ QString::number(ui->TextEdit->toPlainText().length())
                        +"      lines:"+
                        QString::number(ui->TextEdit->document()->lineCount()));
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


void MainWindow::on_actionFontColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black,this,"选择字体颜色");
    if(color.isValid()){
        ui->TextEdit->setStyleSheet(QString("QPlainTextEdit {color:%1}").arg(color.name()));
    }

}



void MainWindow::on_actionLineWrap_triggered()
{
    QPlainTextEdit::LineWrapMode mode = ui->TextEdit->lineWrapMode();

    if(mode == QTextEdit::NoWrap){
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->actionLineWrap->setChecked(true);
    }else{
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->actionLineWrap->setChecked(false);
    }
}


void MainWindow::on_actionBackGroundColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black,this,"选择背景颜色");
    if(color.isValid()){
        ui->TextEdit->setStyleSheet(QString("QPlainTextEdit {background-color:%1}").arg(color.name()));
    }

}


void MainWindow::on_actionFontBackGroundColor_triggered()
{

}


void MainWindow::on_actionFont_triggered()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok,this);
    if(ok){
        ui->TextEdit->setFont(font);
    }
}


void MainWindow::on_actionToolBar_triggered()
{
    bool visible = ui->toolBar->isVisible();
    ui->toolBar->setVisible(!visible);
    ui->actionToolBar->setCheckable(!visible);
}



void MainWindow::on_actionShowStatusBar_triggered()
{
    bool visible = ui->statusbar->isVisible();
    ui->statusbar->setVisible(!visible);
    ui->actionShowStatusBar->setCheckable(!visible);
}


void MainWindow::on_actionSelectAll_triggered()
{
    ui->TextEdit->selectAll();
}


void MainWindow::on_actionExit_triggered()
{
    if(userEditConfirmed()){
        exit(0);
    }
}


void MainWindow::on_TextEdit_cursorPositionChanged()
{
    int col =0;
    int ln = 0;
    int flg= -1;
    int pos = ui->TextEdit->textCursor().position();
    QString text = ui->TextEdit->toPlainText();
    for(int i = 0 ;i<pos;i++){
        if(text[i]=='\n'){
            ln ++;
            flg = i;
        }
    }
    flg++;
    col = pos - flg;
    statusCursorLabel.setText("Ln：" + QString::number(ln + 1)
                              +"    Col：" +QString::number(col + 1));
}

void MainWindow::autoSave()
{
    if (filePath.isEmpty()) {
        // 如果没有打开文件，提醒用户并弹出文件选择对话框
        QMessageBox::warning(this, "自动保存", "当前没有打开文件，请先打开一个文件。");

        // 弹出文件打开对话框
        QString filename = QFileDialog::getOpenFileName(this, "打开文件", ".", tr("Text files (*.txt) ;; ALL(*.*)"));
        if (filename.isEmpty()) {
            return; // 用户取消选择文件，直接返回
        }

        // 设置文件路径并打开文件

        QFile file(filename);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, "文件打开失败", QString("无法打开文件: %1").arg(filePath));
            return;
        }
        filePath = filename;
        // 文件读取并填充到文本编辑器
        QTextStream in(&file);
        QString text = in.readAll();
        ui->TextEdit->clear();
        ui->TextEdit->insertPlainText(text);
        file.close();
    }

    // 进行自动保存
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "自动保存失败", "无法打开文件进行保存");
        return;
    }

    QTextStream out(&file);
    QString text = ui->TextEdit->toPlainText();
    out << text;

    file.close();
}


void MainWindow::on_actionAutoSave_triggered()
{
    if (filePath.isEmpty()) {
        // 如果当前没有打开文件，提醒用户并请求打开文件
        QMessageBox::warning(this, "自动保存", "当前没有打开文件，请先打开一个文件来启用自动保存。");

        // 弹出文件打开对话框，用户选择文件
        QString filename = QFileDialog::getOpenFileName(this, "打开文件", ".", tr("Text files (*.txt)"));
        if (filename.isEmpty()) {
            return; // 用户取消文件选择，退出
        }

        // 设置文件路径并打开文件
        filePath = filename;
        QFile file(filePath);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, "文件打开失败", QString("无法打开文件: %1").arg(filePath));
            return;
        }

        // 文件读取并填充到文本编辑器
        QTextStream in(&file);
        QString text = in.readAll();
        ui->TextEdit->clear();
        ui->TextEdit->insertPlainText(text);
        file.close();
    }

    if (autoSaveEnabled) {
        // 如果自动保存已启用，停止定时器
        autoSaveTimer->stop();
        autoSaveEnabled = false;
        QMessageBox::information(this, "自动保存", "自动保存已停止");
    } else {
        // 启动定时器，定时保存
        autoSaveTimer->start(autoSaveInterval);
        autoSaveEnabled = true;
        QMessageBox::information(this, "自动保存", "自动保存已启动");
    }
}

