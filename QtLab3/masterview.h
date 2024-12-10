#ifndef MASTERVIEW_H
#define MASTERVIEW_H

#include <QWidget>
#include "departmentview.h"
#include "doctorview.h"
#include "loginview.h"
#include "patienteditview.h"
#include "patientview.h"
#include "welcomeview.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class masterView;
}
QT_END_NAMESPACE

class masterView : public QWidget
{
    Q_OBJECT

public:
    masterView(QWidget *parent = nullptr);
    ~masterView();

public slots:
    void goLoginView();
    void goWelcomeView();
    void goDoctorView();
    void goDepartmentView();
    void goPatientView();
    void goPreviousView();
    void goPatientEditView(int rowNo);

private slots:
    void on_btBack_clicked();

    void on_stackedWidget_currentChanged(int arg1);

    void on_btLogout_clicked();

private:
    void pushWidgetToStackView(QWidget *widget);

    Ui::masterView *ui;

    WelcomeView *welcomeView;
    DoctorView *doctorView;
    PatientView *patientView;
    DepartmentView *departmentView;
    LoginView *loginView;
    PatientEditView *patientEditView;



};
#endif // MASTERVIEW_H
