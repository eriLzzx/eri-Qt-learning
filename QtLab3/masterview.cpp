#include "masterview.h"
#include <QDebug>
#include "ui_masterview.h"



masterView::masterView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::masterView)
{
    ui->setupUi(this);

    goLoginView();
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
    welcomeView = new WelcomeView(this);
    pushWidgetToStackView(welcomeView);
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

