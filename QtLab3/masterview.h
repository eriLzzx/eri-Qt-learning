#ifndef MASTERVIEW_H
#define MASTERVIEW_H

#include <QWidget>

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

private:
    Ui::masterView *ui;
};
#endif // MASTERVIEW_H
