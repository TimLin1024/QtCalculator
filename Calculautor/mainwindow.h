#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QDialog>
#include <QLabel>
#include <QComboBox>

namespace Ui {
class MainWindow;
}
enum ActionFlag{
    NoAction,
    AdditionFlag,
    SubtractionFlag,
    MultiplicationFlag,
    DivisionFlag,
    evaluationFlag,
    LogFlag,
    LnFlag,
    PiFlag,
    NFlag,
    ModFlag,
    X1Flag,
    X_YFlag,
    SinFlag,
    CosFlag,
    TanFlag,
    TransformFlag,
    AndFlag,
    OrFlag,
    NotFlag,
    XorFlag
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    ActionFlag currentAction;//现在的动作
    QString lastNumber;
    QString currentNumber;//现在显示的数字
    bool isEmpty;//显示器是否为空
    QDialog *selectDialog;
    QComboBox *selectCombo;

public slots:
    void clear();
    void clearLast();
    void actionChanged();
    void inputNumber();
    void displayNumber();
    void negationClicked();
    void percentageClicked();
    void decimalPointClicked();
    void evaluationClicked();
//    void onAppAboutClicked();
    void onQuitClicked();

    void help();
    void unitSelected();
    void unitCalc();
    QString fun_1(int n);
};

#endif // MAINWINDOW_H
