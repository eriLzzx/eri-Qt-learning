#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAbout_triggered();

    void on_actionSearch_triggered();

    void on_actionReplace_triggered();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_TextEdit_textChanged();

    void on_actionUndo_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionRedo_triggered();

    void on_actionPaste_triggered();

    void on_TextEdit_copyAvailable(bool b);

    void on_TextEdit_undoAvailable(bool b);

    void on_TextEdit_redoAvailable(bool b);

    void on_actionFontColor_triggered();

    void on_actionLineWrap_triggered();

    void on_actionBackGroundColor_triggered();

    void on_actionFontBackGroundColor_triggered();

    void on_actionFont_triggered();

    void on_actionToolBar_triggered();

    void on_actionShowStatusBar_triggered();

    void on_actionSelectAll_triggered();

    void on_actionExit_triggered();

    void on_TextEdit_cursorPositionChanged();

    void on_actionAutoSave_triggered();
    void autoSave(); //  autoSave()

    void on_actionShowLineNumber_triggered();

private:
    Ui::MainWindow *ui;
    //必须在库文件里手动添加来实现
    QLabel statusCursorLabel;
    QLabel statusLabel;

    QString filePath;
    bool textChanged;

    bool userEditConfirmed();

    QTimer *autoSaveTimer; // 定时器
    bool autoSaveEnabled = false; // 初始不启用
    int autoSaveInterval = 6000; // 自动保存间隔，单位是毫秒（6秒 = 6000ms）

};
#endif // MAINWINDOW_H
