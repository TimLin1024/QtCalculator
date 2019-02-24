#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"
#include "dialog.h"
#include <QAction>
#include <QMenuBar>
#include <QTimer>
#include <QDialog>
#include <math.h>
#include <QLabel>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QtMath>

double pi = M_PI;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    isEmpty(true)
{
    ui->setupUi(this);

    //屏蔽放大窗口按钮
    setWindowFlags(Qt::CustomizeWindowHint| Qt::WindowCloseButtonHint| Qt::WindowMinimizeButtonHint);

    QAction *helpAction = new QAction("使用说明",this);
    QMenu *newMenu = menuBar()->addMenu("帮助");
    newMenu->addAction(helpAction);

    QAction *openAction = new QAction(tr("&转换"), this);
    openAction->setStatusTip(tr("单位、进制转换"));

    QToolBar *toolBar = addToolBar(tr("&File"));
    toolBar->addAction(openAction);
    /*信号槽的绑定
     **/
    connect(openAction,         SIGNAL(triggered(bool)),this,SLOT(transfrom()));
    connect(helpAction,          SIGNAL(triggered(bool)),this,SLOT(help()));
    connect(ui->actionAbout_Qt, SIGNAL(triggered(bool)),qApp,SLOT(aboutQt()));
    connect(ui->actionAbout,    SIGNAL(triggered(bool)),this,SLOT(aboutCalculator()));

    connect(ui->btnClear,       SIGNAL(clicked(bool)),this, SLOT(clear()));
    connect(ui->btnClear_1,     SIGNAL(clicked(bool)),this, SLOT(clearLast()));
    connect(ui->btnNegativity,  SIGNAL(clicked(bool)),this, SLOT(negationClicked()));
    connect(ui->btnPercentage,  SIGNAL(clicked(bool)),this, SLOT(percentageClicked()));
    connect(ui->btnDecimalPoint,SIGNAL(clicked(bool)),this, SLOT(decimalPointClicked()));
    connect(ui->btnEvaluation,  SIGNAL(clicked(bool)),this, SLOT(evaluationClicked()));
    //运算符
    connect(ui->btnAddition,        SIGNAL(clicked(bool)),this,SLOT(actionChanged()));
    connect(ui->btnSubtraction,     SIGNAL(clicked(bool)),this,SLOT(actionChanged()));
    connect(ui->btnMultiplication,  SIGNAL(clicked(bool)),this,SLOT(actionChanged()));
    connect(ui->btnDivision,        SIGNAL(clicked(bool)),this,SLOT(actionChanged()));
    connect(ui->btnLn,              SIGNAL(clicked(bool)),this,SLOT(actionChanged()));
    connect(ui->btnLog,             SIGNAL(clicked(bool)),this,SLOT(actionChanged()));
    connect(ui->btnSin,             SIGNAL(clicked(bool)),this,SLOT(actionChanged()));
    connect(ui->btnCos,             SIGNAL(clicked(bool)),this,SLOT(actionChanged()));
    connect(ui->btnTan,             SIGNAL(clicked(bool)),this,SLOT(actionChanged()));
    connect(ui->btn1_X,             SIGNAL(clicked(bool)),this,SLOT(actionChanged()));
    connect(ui->btnNN,              SIGNAL(clicked(bool)),this,SLOT(actionChanged()));
    connect(ui->btnPi,              SIGNAL(clicked(bool)),this,SLOT(actionChanged()));
    connect(ui->btnMod,             SIGNAL(clicked(bool)),this,SLOT(actionChanged()));
    connect(ui->btnX_Y,             SIGNAL(clicked(bool)),this,SLOT(actionChanged()));
    connect(ui->btnXor,             SIGNAL(clicked(bool)),this,SLOT(actionChanged()));
    connect(ui->btnAnd,             SIGNAL(clicked(bool)),this,SLOT(actionChanged()));
    connect(ui->btnNot,             SIGNAL(clicked(bool)),this,SLOT(actionChanged()));
    connect(ui->btnOr,              SIGNAL(clicked(bool)),this,SLOT(actionChanged()));


     //数字
    connect(ui->btnNum0,SIGNAL(clicked(bool)),this,SLOT(inputNumber()));
    connect(ui->btnNum00,SIGNAL(clicked(bool)),this,SLOT(inputNumber()));
    connect(ui->btnNum1,SIGNAL(clicked(bool)),this,SLOT(inputNumber()));
    connect(ui->btnNum2,SIGNAL(clicked(bool)),this,SLOT(inputNumber()));
    connect(ui->btnNum3,SIGNAL(clicked(bool)),this,SLOT(inputNumber()));
    connect(ui->btnNum4,SIGNAL(clicked(bool)),this,SLOT(inputNumber()));
    connect(ui->btnNum5,SIGNAL(clicked(bool)),this,SLOT(inputNumber()));
    connect(ui->btnNum6,SIGNAL(clicked(bool)),this,SLOT(inputNumber()));
    connect(ui->btnNum7,SIGNAL(clicked(bool)),this,SLOT(inputNumber()));
    connect(ui->btnNum8,SIGNAL(clicked(bool)),this,SLOT(inputNumber()));
    connect(ui->btnNum9,SIGNAL(clicked(bool)),this,SLOT(inputNumber()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
//帮助信息
void MainWindow::help()
{
    QMessageBox::about(this, "帮助", "\n"
                                    "1.“Backspace”按钮，每按一次删除编辑框最后一个字符;\n"
                                    "2.使用求“倒数”、“sin”、“cos”和“tan”功能时请先输入数值再点击相应的按钮;\n"
                                    "3.“％”和“00”按钮，这两个按钮并不是运算符，仅仅是修改编辑框；\n"
                                    "4.“pi”按钮: 当点击这个按钮后，无论刚才输入何数字，均被删除，用pi(取3.14159)取代; \n"
                                    "5.“n!”按钮，阶乘运算只支持不大于170的正整数；\n"
                                    "6.“x^y”按钮，此按钮用于乘方运算，使用时能要先选择底数后选择指数，注意数值范围；\n"
                                    "7.“ln”和“log”按钮，“ln”按钮是求底数为e的对数，“log”按钮是底数为10的对数；\n"
                                    "8.“Mod”求模运算符，只支持整型数；\n"
                                    "9.“And”、“Or”、“Not”和“Xor”按钮（一般针对二进制数）；\n"
                                    "10.“转换功能”需要先输入要转换的数值，再进行转换，转换后的数值将显示在原屏幕上");
}

//转换功能
void MainWindow::transfrom()
{
    selectDialog = new QDialog(this);
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox;
    QGroupBox *groupBox_2;
    QLabel *displayLabel;

    selectDialog->resize(400,150);

    buttonBox = new QDialogButtonBox(selectDialog);
    buttonBox->setGeometry(30,100,341,32);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    groupBox = new QGroupBox(selectDialog);
    groupBox->setGeometry(10,10,180,80);
    groupBox->setTitle("要转换的数值");
    groupBox_2 = new QGroupBox(selectDialog);
    groupBox_2->setGeometry(210,10,180,80);
    groupBox_2->setTitle("转换的单位");

    displayLabel = new QLabel(ui->label->text(),groupBox);
    displayLabel->setGeometry(10,20,120,26);
    selectCombo = new QComboBox(groupBox_2);
    selectCombo->setGeometry(10,40,120,26);
    selectCombo->clear();

    selectCombo->insertItem(0,"二进制转换");
    selectCombo->insertItem(1,"八进制转换");
    selectCombo->insertItem(2,"十六进制转换");
    selectCombo->insertItem(3,"米->厘米");
    selectCombo->insertItem(4,"米->平方米");

    connect(buttonBox,SIGNAL(rejected()),selectDialog,SLOT(deleteLater()));
    connect(buttonBox,SIGNAL(accepted()),this,SLOT(unitCalc()));
    selectDialog->exec();
}

void MainWindow::unitCalc()
{
    switch (selectCombo->currentIndex()) {
    case 0:
        currentNumber = QString::number(currentNumber.toInt(), 2);
        break;
    case 1:
         currentNumber = "0"+QString::number(currentNumber.toInt(), 8);
        break;
    case 2:
        currentNumber = "0x"+QString::number(currentNumber.toInt(), 16);
        break;
    case 3:
         currentNumber = QString::number(pow(currentNumber.toDouble(),2));
        break;
    case 4:
         currentNumber = QString::number(pow(currentNumber.toDouble(),2));
        break;

    default:
        break;
    }
    selectDialog->deleteLater();
    displayNumber();
}


void MainWindow::aboutCalculator()
{
    QMessageBox::about(this,"关于 计算器","此计算器为C++课程设计而编写，旨在:\n"
                                     "（1）使自身对C++基本语法熟练掌握；\n"
                                     "（2）训练自身对一个实际应用系统进行综合分析、设计、编程及调试等的能力；"
                                     "（3）培养自身自主学习和独立创新的精神。");

}

//清除数据
void MainWindow::clearLast()
{
    QString str = currentNumber;
    currentNumber = str.left(str.length()-1);
    displayNumber();
}
//清空数据
void MainWindow::clear(){
    currentNumber = "0";
    displayNumber();
}

//判断点击的运算符按钮来选择不同运算
void MainWindow::actionChanged()
{
    QPushButton *button = qobject_cast<QPushButton *>(this->sender());
    if(button)
    {
        isEmpty = false;

        if(button == ui->btnAddition)
        {
            currentAction = AdditionFlag;
            statusBar()->showMessage("选择了加法",3000);
        }
        else if (button == ui->btnSubtraction) {
            currentAction = SubtractionFlag;
            statusBar()->showMessage("选择了减法",3000);
        }
        else if (button == ui->btnMultiplication) {
            currentAction = MultiplicationFlag;
            statusBar()->showMessage("选择了乘法",3000);
        }
        else if (button == ui->btnDivision) {
            currentAction = DivisionFlag;
            statusBar()->showMessage("选择了除法",3000);
        }
        else if(button == ui->btnMod){
            currentAction = ModFlag;
            statusBar()->showMessage("选择了模运算",3000);
        }
        else if(button == ui->btnAnd){
            currentAction = AndFlag;
            statusBar()->showMessage("选择了And运算",3000);
        }
        else if(button == ui->btnOr){
            currentAction = OrFlag;
            statusBar()->showMessage("选择了Or运算",3000);
        }
        else if(button == ui->btnNot){
            currentNumber = QString::number(~currentNumber.toInt());
            displayNumber();
        }
        else if(button == ui->btnXor){
            currentAction = XorFlag;
            statusBar()->showMessage("选择了Xor运算",3000);
        }
        else if(button == ui->btnX_Y){
            currentAction = X_YFlag;
            statusBar()->showMessage("选择了乘方运算",3000);
        }//ln函数
        else if(button == ui->btnLn){
            currentNumber = QString::number(log(currentNumber.toDouble()));
            displayNumber();
        }//log10函数
        else if(button == ui->btnLog){
            currentNumber = QString::number(log10(currentNumber.toDouble()));
            displayNumber();
        }//sin函数
        else if(button == ui->btnSin){
            currentNumber = QString::number(qSin(qDegreesToRadians(currentNumber.toDouble())));
            displayNumber();
        }//cos函数
        else if(button == ui->btnCos){
            if(currentNumber.toInt()%90 == 0 && currentNumber.toInt() % 180 != 0){
                currentNumber = "0";
            }else{
                currentNumber = QString::number(qCos(qDegreesToRadians(currentNumber.toDouble())));
            }
            displayNumber();
        }//tan函数
        else if(button == ui->btnTan){
            if(currentNumber.toInt()%90 == 0 && currentNumber.toInt() % 180 != 0){
                currentNumber = "输入有误";
            }else{
                currentNumber = QString::number(qTan(qDegreesToRadians(currentNumber.toDouble())));

            }
            displayNumber();
        }//倒数
        else if(button == ui->btn1_X){
            currentNumber = QString::number(1.0/(currentNumber.toDouble()));
            displayNumber();
        }//阶乘
        else if(button == ui->btnNN){
            int n = currentNumber.toInt();
            if(currentNumber.contains(".")){
                statusBar()->showMessage("请输入整数");
                return ;
            }else if(n > 170){
                statusBar()->showMessage("请输入0-170之间的数字");
                return;
            }
            double sum = 1;
            while(n > 1){
                sum *= n;
                n--;
            }
            currentNumber = QString::number(sum);
            displayNumber();
        }//pi
        else if(button == ui->btnPi){
            currentNumber = QString::number(pi);
            displayNumber();
        }
        else {
            currentAction = NoAction;
        }
    }
}



//输入0-9
void MainWindow::inputNumber()
{
    QPushButton *button = qobject_cast<QPushButton *>(this->sender());
    if(button)
    {
        if(AdditionFlag == evaluationFlag){
            currentNumber.clear();
            isEmpty = true;
        }

        if(!isEmpty )
        {
            lastNumber = currentNumber;
            currentNumber.clear();
            isEmpty = true;
        }
        if(currentNumber == "0")  //另外,如果当前显示屏中已经显示了0了,那么我们显然不应该在输入一个数字后让它变成0102这种,所以我们这时候要给显示屏的内容先清空,一样使用clear.
        {
            currentNumber.clear();
        }
        if(button == ui->btnNum0)
        {
            currentNumber.append("0");
            statusBar()->showMessage("输入了0",3000);
        }
        if(button == ui->btnNum00)
        {
            currentNumber.append("00");
            statusBar()->showMessage("输入了00",3000);
        }
        if(button == ui->btnNum1)
        {
            currentNumber.append("1");
            statusBar()->showMessage("输入了1",3000);
        }
        if(button == ui->btnNum2)
        {
            currentNumber.append("2");
            statusBar()->showMessage("输入了2",3000);
        }
        if(button == ui->btnNum3)
        {
            currentNumber.append("3");
            statusBar()->showMessage("输入了3",3000);
        }
        if(button == ui->btnNum4)
        {
            currentNumber.append("4");
            statusBar()->showMessage("输入了4",3000);
        }
        if(button == ui->btnNum5)
        {
            currentNumber.append("5");
            statusBar()->showMessage("输入了5",3000);
        }if(button == ui->btnNum6)
        {
            currentNumber.append("6");
            statusBar()->showMessage("输入了6",3000);
        }if(button == ui->btnNum7)
        {
            currentNumber.append("7");
            statusBar()->showMessage("输入了7",3000);
        }if(button == ui->btnNum8)
        {
            currentNumber.append("8");
            statusBar()->showMessage("输入了8",3000);
        }if(button == ui->btnNum9)
        {
            currentNumber.append("9");
            statusBar()->showMessage("输入了9",3000);
        }

        displayNumber();
    }
}
//label上面显示的数
void MainWindow::displayNumber()
{
    ui->label->setText(currentNumber);
}
//符号变更
void MainWindow::negationClicked()
{
    if(!isEmpty || (currentNumber.size() == 0))
    {
        isEmpty = true;
        currentNumber = "0";
    }
    else {
        if(currentNumber.at(0) == '-')
        {
            currentNumber.remove(0,1);
        }
        else {
            currentNumber.push_front("-");
        }
    }
    statusBar()->showMessage("符号变更",3000);
    displayNumber();
}
//百分号
void MainWindow::percentageClicked()
{
    if(!isEmpty)
    {
        isEmpty = true;
        currentNumber = "0";
    }
    else
    {
        double number = currentNumber.toDouble() / 100.0;
        currentNumber = QString::number(number);
    }
    statusBar()->showMessage("百分比",3000);
    displayNumber();
}
//小数点
void MainWindow::decimalPointClicked()
{
    if(!isEmpty)
    {
        currentNumber = "0.";
        isEmpty = true;
    }
    else if(currentNumber.indexOf(".") == -1)
    {
        currentNumber.push_back(".");
    }

    statusBar()->showMessage("输入了小数点",3000);
    displayNumber();
}
//等于号
void MainWindow::evaluationClicked()
{


    if(currentAction == NoAction)
    {
        statusBar()->showMessage("请选择一个运算",3000);
        return;
    }
    switch (currentAction) {
    case AdditionFlag:
    {
        currentNumber = QString::number(lastNumber.toDouble() + currentNumber.toDouble());
        break;
    }
    case SubtractionFlag:
    {
        currentNumber = QString::number(lastNumber.toDouble() - currentNumber.toDouble());
        break;
    }
    case MultiplicationFlag:
    {
        currentNumber = QString::number(lastNumber.toDouble() * currentNumber.toDouble());
        break;
    }
    case DivisionFlag:
    {
        currentNumber = QString::number(lastNumber.toDouble() / currentNumber.toDouble());
        break;
    }
    case ModFlag:
    {
        if(currentNumber.contains(".")||lastNumber.contains("."))
        {
            statusBar()->showMessage("请输入整数");
            return;
        }

        currentNumber = QString::number(lastNumber.toInt() % currentNumber.toInt());
        break;
    }
    case X_YFlag:
    {
        currentNumber = QString::number(pow(lastNumber.toInt() , currentNumber.toInt()));
        break;
    }
    case XorFlag:
    {
        currentNumber = QString::number(lastNumber.toInt() ^ currentNumber.toInt());
        break;
    }
    case AndFlag:
    {

        currentNumber = QString::number(lastNumber.toInt() & currentNumber.toInt());
        break;
    }
    case OrFlag:
    {
        currentNumber = QString::number(lastNumber.toInt() | currentNumber.toInt());
        break;
    }
    default:
        break;
    }
    statusBar()->showMessage("计算完成",3000);
    currentAction = NoAction;
    displayNumber();
}


