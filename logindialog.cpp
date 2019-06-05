#include "logindialog.h"
#include "ui_logindialog.h"
#include "helper.h"
#include <QMessageBox>
#include <QDebug>
#include <mainwindow.h>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_pushButton_clicked()
{
    QString userId=ui->lineEdit_userId->text();
    QString password=ui->lineEdit_password->text();
    User user;


    if(!Helper::getUserInfo(userId,user)){
        QMessageBox::warning(nullptr,"登录失败","用户名不存在",QMessageBox::Ok);
    }
    else if(password!=user.password){
        QMessageBox::warning(nullptr,"登录失败","密码错误",QMessageBox::Ok);
    }
    else {
        curUser=user;
        this->close();
        if(curUser.level==0){

        }
        else if(curUser.level==1){
            MainWindow *mainwindow=new MainWindow();
            mainwindow->setWindowTitle("您好，"+curUser.id+"用户！");
            mainwindow->show();
            qDebug()<<"login success"<<endl;
        }

    }
}

void LoginDialog::on_pushButton_2_clicked()
{
    this->close();
}
