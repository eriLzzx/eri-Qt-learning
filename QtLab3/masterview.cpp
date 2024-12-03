#include "masterview.h"
#include "ui_masterview.h"

masterView::masterView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::masterView)
{
    ui->setupUi(this);
}

masterView::~masterView()
{
    delete ui;
}
