#include "masterview.h"
#include <QDebug>
#include "ui_masterview.h"
#include "idatabase.h"


masterView::masterView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::masterView)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::FramelessWindowHint);

    goLoginView();

    IDatabase::getInstance();
}

masterView::~masterView()
{
    delete ui;
}

void masterView::goLoginView()
{
    qDebug () << "goLoginView";
    loginView = new LoginView(this);
    pushWidgetToStackView(loginView);

    connect(loginView,SIGNAL(loginSuccess()),this,SLOT(goWelcomeView()));
}



void masterView::goWelcomeView() {
    qDebug () << "goWelcomeView";
    welcomeView = new WelcomeView(this);
    pushWidgetToStackView(welcomeView);

    connect(welcomeView,SIGNAL(goDoctorView()),this,SLOT(goDoctorView()));
    connect(welcomeView,SIGNAL(goPatientView()),this,SLOT(goPatientView()));
    connect(welcomeView,SIGNAL(goDepartmentView()),this,SLOT(goDepartmentView()));
}

void masterView::goDoctorView() {
    qDebug () << "goDoctorView";
    doctorView = new DoctorView(this);
    pushWidgetToStackView(doctorView);
}

void masterView::goDepartmentView() {
    qDebug () << "goDepartmentView";
    departmentView = new DepartmentView(this);
    pushWidgetToStackView(departmentView);
}

void masterView::goPatientView() {
    qDebug () << "goWelcomeView";
    patientView = new PatientView(this);
    pushWidgetToStackView(patientView);

    connect(patientView,SIGNAL(goPatientEditView()),this,SLOT(goPatientEditView()));
}

void masterView::goPreviousView() {

    int count = ui->stackedWidget->count();
    if(count >1){
        ui->stackedWidget->setCurrentIndex(count - 2);
        ui->labelTitle->setText(ui->stackedWidget->currentWidget()->windowTitle());

        QWidget *widget = ui->stackedWidget->widget(count - 1);
        ui->stackedWidget->removeWidget(widget);
        delete widget;
    }
}

void masterView::goPatientEditView() {
    qDebug () << "goPatientEditView";
    patientEditView = new PatientEditView(this);
    pushWidgetToStackView(patientEditView);
}

void masterView::pushWidgetToStackView(QWidget *widget)
{
    ui->stackedWidget->addWidget(widget);
    int count = ui->stackedWidget->count();
    ui->stackedWidget->setCurrentIndex(count - 1);
    ui->labelTitle->setText(widget->windowTitle());
}

void masterView::on_btBack_clicked()
{
    goPreviousView();
}


void masterView::on_stackedWidget_currentChanged(int arg1)
{
    int count = ui->stackedWidget->count();
    if(count > 1)
        ui->btBack->setEnabled(true);
    else
        ui->btBack->setEnabled(false);
    QString title = ui->stackedWidget->currentWidget()->windowTitle();
    if(title =="欢迎"){
        ui->btLogout->setEnabled(true);
    //只有在欢迎界面才可以注销
    }else
        ui->btLogout->setEnabled(false);
    //只有在登录页面不可返回
    if(title =="登录"){
        ui->btLogout->setEnabled(true);
        ui->btBack->setEnabled(false);
    }else
        ui->btLogout->setEnabled(false);
}

