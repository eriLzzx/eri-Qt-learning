#include "searchdialog.h"
#include "ui_searchdialog.h"
#include <QMessageBox>

SearchDialog::SearchDialog(QWidget *parent,QPlainTextEdit * textEdit )
    : QDialog(parent)
    , ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
    pTextEdit = textEdit;
    ui->rbDown->setChecked(true);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::on_btnFindNext_clicked()
{
    QString target = ui->searchText->text();
    if(target =="" || pTextEdit == nullptr){
        return;
    }
    //将文本框所有的内容获取到text
    QString text = pTextEdit->toPlainText();
    QTextCursor c = pTextEdit->textCursor();//获取光标位置
    int index = -1;

    if(ui->rbDown->isChecked()){
        index = text.indexOf(target,c.position(),
                             ui->cbCaseSensetive->isChecked() ? Qt::CaseSensitive: Qt::CaseInsensitive);
        if(index >=0){
            c.setPosition(index);
            c.setPosition(index + target.length(),QTextCursor::KeepAnchor);
            pTextEdit->setTextCursor(c);
        }
    }else if(ui->rbUp->isChecked()){
        index = text.lastIndexOf(target,c.position() - text.length() - 1,
                                 ui->cbCaseSensetive->isChecked()?Qt::CaseSensitive: Qt::CaseInsensitive);
        if(index >=0){
            c.setPosition(index + target.length());
            c.setPosition(index,QTextCursor::KeepAnchor);
            pTextEdit->setTextCursor(c);
        }
    }
    if(index<0){
        QMessageBox msg(this);
        msg.setWindowTitle("warning");
        msg.setText(QString("找不到输入的字符串")+target);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }

}


void SearchDialog::on_btnCancel_clicked()
{
    accept();
}

