#include "userwindow.h"
#include "ui_userwindow.h"
#include <QStandardItem>
#include <QStandardItemModel>
#include <QList>
#include "helper.h"
#include <QMessageBox>
#include <QDebug>

UserWindow::UserWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserWindow)
{
    ui->setupUi(this);  
    QStandardItemModel *model=new QStandardItemModel();
    ui->tableView->setModel(model);
    updateView();
}

UserWindow::~UserWindow()
{
    delete ui;
}

void UserWindow::updateView()
{
    QVector<QStringList> userInfo;
    QStandardItemModel *model=static_cast<QStandardItemModel*>(ui->tableView->model());
    QStringList labels{"id","特权级"};
    model->clear();
    model->setHorizontalHeaderLabels(labels);
    if(Helper::getAllUserInfo(userInfo)){
        for(int i=0;i<userInfo.size();i++){
            QList<QStandardItem *> itemList{new QStandardItem(userInfo[i][0]),new QStandardItem(userInfo[i][1])};
            itemList[0]->setEditable(false);
            itemList[1]->setEditable(false);
            model->appendRow(itemList);
        }
    }
}

void UserWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    QStandardItemModel *model=static_cast<QStandardItemModel*>(ui->tableView->model());
    QString userId=model->item(index.row(),0)->text().simplified();

    int ret=QMessageBox::information(nullptr,"删除用户",QString("请确认是否删除用户%1").arg(userId),QMessageBox::Ok | QMessageBox::Cancel);
    if(ret == QMessageBox::Ok){
        if(userId=="root" || !Helper::delUserRecord(userId))
                QMessageBox::warning(nullptr,"操作失败",QString("删除用户%1失败").arg(userId),QMessageBox::Ok);
        else{
            QMessageBox::information(nullptr,"操作成功",QString("删除用户%1成功").arg(userId),QMessageBox::Ok);
            updateView();
        }
    }
}

void UserWindow::on_btn_adduser_clicked()
{
    if(ui->rdb_add->isChecked()){
        QStringList record{ui->ldt_id->text().simplified(),ui->ldt_password->text().simplified(),"1"};
        if(record[0]=="" || record[1]=="" || !Helper::addUserRecord(record))
            QMessageBox::warning(nullptr,"操作失败","添加用户失败",QMessageBox::Ok);
        else{
            QMessageBox::information(nullptr,"操作成功","添加用户成功",QMessageBox::Ok);
            updateView();
        }
    }
    else{
        QString userId=ui->ldt_id->text().simplified();
        QString password=ui->ldt_password->text().simplified();
        if(password=="" || !Helper::modiUserRecord(userId,password))
            QMessageBox::warning(nullptr,"操作失败","修改用户失败",QMessageBox::Ok);
        else{
            QMessageBox::information(nullptr,"操作成功","修改用户成功",QMessageBox::Ok);
            updateView();
        }
    }
}
