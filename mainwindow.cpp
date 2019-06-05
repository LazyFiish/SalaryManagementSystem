#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "helper.h"
#include <QDate>
#include <QCalendarWidget>
#include <QStandardItemModel>
#include <QTableView>
#include <QStandardItem>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStandardItemModel *model=new QStandardItemModel();
    ui->tableView_salaryOrAward->setModel(model);
    model=new QStandardItemModel();
    ui->tableView_statView->setModel(model);

    ui->dateEdit_stat_beginDate->setCalendarPopup(true);
    ui->dateEdit_stat_beginDate->setDate(QDate::currentDate());
    ui->dateEdit_stat_endDate->setCalendarPopup(true);
    ui->dateEdit_stat_endDate->setDate(QDate::currentDate());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_staff_selFile_clicked()
{
    ui->ldt_staff_filepath->setText(QFileDialog::getOpenFileName(this,
        tr("Open txt"), "/home/me", tr("txt Files (*.txt)")));
}

void MainWindow::on_btn_staff_import_clicked()
{
    QString filePath=ui->ldt_staff_filepath->text().simplified();
    QFile file(filePath);
    if(!file.exists()){
        QMessageBox::warning(nullptr,"导入失败",QString("文件“%1”不存在！").arg(filePath),QMessageBox::Ok);
        return;
    }
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(nullptr,"导入失败",QString("文件“%1”打开失败！").arg(filePath),QMessageBox::Ok);
        return;
    }

    QTextStream ataffInfo(file.readAll());
    file.close();
    int lineNumber=0,failCount=0;
    while (!ataffInfo.atEnd()) {
        lineNumber++;
        QStringList staffRecord=ataffInfo.readLine().simplified().split(' ');
        for(int i=staffRecord.size();i<10;i++)
            staffRecord.append("");
        staffRecord[2]=(staffRecord[2]=="男"? "1": "0");
        staffRecord[6]=(staffRecord[6]=="是"? "1": "0");
        if(!this->isLegal(staffRecord)||!Helper::insertStaffRecord(staffRecord)){
            failCount++;
            QMessageBox::warning(nullptr,"插入员工记录失败",QString("失败记录所在行数：%1").arg(lineNumber),QMessageBox::Ok);
        }
    }
    QMessageBox::information(nullptr,"导入完成",QString("成功记录数：%1   失败记录数：%2").arg(lineNumber-failCount).arg(failCount),QMessageBox::Ok);

}

void MainWindow::on_btn_staff_search_clicked()
{
    QString staffId=ui->ldt_staff_id->text();
    QStringList staffRecord;
    if(Helper::getStaffRecord(staffId,staffRecord)){
        ui->ldt_staff_name->setText(staffRecord[1]);
        ui->ldt_staff_sex->setText(staffRecord[2]=="1"? "男": "女");
        ui->ldt_staff_birth->setText(staffRecord[3]);
        ui->ldt_staff_idcard->setText(staffRecord[4]);
        ui->ldt_staff_workType->setText(staffRecord[5]);
        ui->ldt_staff_isLeave->setText(staffRecord[6]=="1"? "是": "否");
        ui->ldt_staff_entryDate->setText(staffRecord[7]);
        ui->ldt_staff_leaveDate->setText(staffRecord[8]);
        ui->ldt_staff_bankCardId->setText(staffRecord[9]);
    }
}

void MainWindow::on_btn_ataff_maintain_clicked()
{
    QStringList staffRecord{
        ui->ldt_staff_id->text().simplified(),
        ui->ldt_staff_name->text().simplified(),
        ui->ldt_staff_sex->text().simplified()=="男"? "1": "0",
        ui->ldt_staff_birth->text().simplified(),
        ui->ldt_staff_idcard->text().simplified(),
        ui->ldt_staff_workType->text().simplified(),
        ui->ldt_staff_isLeave->text().simplified()=="是"? "1": "0",
        ui->ldt_staff_entryDate->text().simplified(),
        ui->ldt_staff_leaveDate->text().simplified(),
        ui->ldt_staff_bankCardId->text().simplified()
    };

    if(!this->isLegal(staffRecord)){
        QMessageBox::warning(nullptr,"操作失败","员工信息不合法",QMessageBox::Ok);
        return;
    }

    if(ui->rdb_staff_addRecd->isChecked()){
        if(!Helper::insertStaffRecord(staffRecord)){
            QMessageBox::warning(nullptr,"操作失败","插入员工信息失败",QMessageBox::Ok);
        }
        else {
            QMessageBox::information(nullptr,"操作成功","插入员工信息成功",QMessageBox::Ok);
        }
    }
    else{
        if(!Helper::modiStaffRecord(staffRecord)){
            QMessageBox::warning(nullptr,"操作失败","修改员工信息失败",QMessageBox::Ok);
        }
        else{
            QMessageBox::information(nullptr,"操作成功","修改员工信息成功",QMessageBox::Ok);
        }
    }
}


void MainWindow::on_btn_attd_selpath_clicked()
{
    ui->ldt_attd_filepath->setText(QFileDialog::getOpenFileName(this,
        tr("Open txt"), "/home/me", tr("txt Files (*.txt)")));
}

void MainWindow::on_btn_attd_import_clicked()
{
    QString filePath=ui->ldt_attd_filepath->text().simplified();
    QFile file(filePath);
    if(!file.exists()){
        QMessageBox::warning(nullptr,"导入失败",QString("文件“%1”不存在！").arg(filePath),QMessageBox::Ok);
        return;
    }
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(nullptr,"导入失败",QString("文件“%1”打开失败！").arg(filePath),QMessageBox::Ok);
        return;
    }

    QTextStream attdInfo(file.readAll());
    file.close();
    int lineNumber=0,failCount=0;
    while (!attdInfo.atEnd()) {
        lineNumber++;
        QStringList attdRecord=attdInfo.readLine().simplified().split(' ');
        if(!Helper::insertAttdRecord(attdRecord)){
            failCount++;
            QMessageBox::warning(nullptr,"插入考勤记录失败",QString("失败记录所在行数：%1").arg(lineNumber),QMessageBox::Ok);
        }
    }
    QMessageBox::information(nullptr,"导入完成",QString("成功记录数：%1   失败记录数：%2").arg(lineNumber-failCount).arg(failCount),QMessageBox::Ok);
}

void MainWindow::on_btn_attd_search_clicked()
{
    QString staffId=ui->ldt_attd_staffId->text().simplified();
    QString attdDate=ui->ldt_attd_Date->text().simplified();
    QStringList attdRecord;
    if(Helper::getAttdRecord(staffId,attdDate,attdRecord)){
        ui->ldt_attd_deduction->setText(attdRecord[2]);
        ui->ldt_attd_Status->setText(attdRecord[3]);
    }
}

void MainWindow::on_btn_attd_insert_clicked()
{
    QStringList attdRecord{
        ui->ldt_attd_staffId->text().simplified(),
        ui->ldt_attd_Date->text().simplified(),
        ui->ldt_attd_deduction->text().simplified(),
        ui->ldt_attd_Status->text().simplified()
    };

    if(ui->rdb_attd_add->isChecked()){
        if(!Helper::insertAttdRecord(attdRecord))
            QMessageBox::warning(nullptr,"插入考勤记录失败","请点击确认继续",QMessageBox::Ok);
        else
            QMessageBox::information(nullptr,"插入考勤记录成功","请点击确认继续",QMessageBox::Ok);
    }
    else if(ui->rdb_attd_del->isChecked()){
        if(!Helper::delAttdRecord(attdRecord[0],attdRecord[1]))
            QMessageBox::warning(nullptr,"操作失败","删除考勤记录失败",QMessageBox::Ok);
        else
            QMessageBox::information(nullptr,"操作成功","删除考勤记录成功",QMessageBox::Ok);
    }
    else {
        if(!Helper::modiAttdRecord(attdRecord))
            QMessageBox::warning(nullptr,"操作失败","修改考勤记录失败",QMessageBox::Ok);
        else
            QMessageBox::information(nullptr,"操作成功","修改考勤记录成功",QMessageBox::Ok);

    }
}

void MainWindow::on_btn_allw_selpath_clicked()
{
    ui->ldt_allw_filepath->setText(QFileDialog::getOpenFileName(this,
        tr("Open txt"), "/home/me", tr("txt Files (*.txt)")));
}

void MainWindow::on_btn_allw_import_clicked()
{
    QString filePath=ui->ldt_allw_filepath->text().simplified();
    QFile file(filePath);
    if(!file.exists()){
        QMessageBox::warning(nullptr,"导入失败",QString("文件“%1”不存在！").arg(filePath),QMessageBox::Ok);
        return;
    }
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(nullptr,"导入失败",QString("文件“%1”打开失败！").arg(filePath),QMessageBox::Ok);
        return;
    }

    QTextStream allwInfo(file.readAll());
    file.close();
    int lineNumber=0,failCount=0;
    while (!allwInfo.atEnd()) {
        lineNumber++;
        QStringList allwRecord=allwInfo.readLine().simplified().split(' ');
        if(!Helper::insertAllwRecord(allwRecord)){
            failCount++;
            QMessageBox::warning(nullptr,"插入津贴记录失败",QString("失败记录所在行数：%1").arg(lineNumber),QMessageBox::Ok);
        }
    }
    QMessageBox::information(nullptr,"导入完成",QString("成功记录数：%1   失败记录数：%2").arg(lineNumber-failCount).arg(failCount),QMessageBox::Ok);
}

void MainWindow::on_btn_allw_search_clicked()
{
    QString staffId=ui->ldt_allw_staffId->text().simplified();
    QString allwDate=ui->ldt_allw_date->text().simplified();
    QStringList allwRecord;
    if(Helper::getAllwRecord(staffId,allwDate,allwRecord)){
        ui->ldt_allw_days->setText(allwRecord[2]);
        ui->ldt_allw_pay->setText(allwRecord[3]);
    }
}

void MainWindow::on_btn_allw_insert_clicked()
{
    QStringList allwRecord{
        ui->ldt_allw_staffId->text().simplified(),
        ui->ldt_allw_date->text().simplified(),
        ui->ldt_allw_days->text().simplified(),
        ui->ldt_allw_pay->text().simplified()
    };

    if(ui->rdb_allw_add->isChecked()){
        if(!Helper::insertAllwRecord(allwRecord))
            QMessageBox::warning(nullptr,"录入津贴信息失败","请点击确认按钮继续",QMessageBox::Ok);
        else
            QMessageBox::information(nullptr,"录入津贴信息成功","请点击确认按钮继续",QMessageBox::Ok);
    }
    else if (ui->rdb_allw_del->isChecked()) {
        if(!Helper::delAllwRecord(allwRecord[0],allwRecord[1]))
            QMessageBox::warning(nullptr,"操作失败","删除津贴记录失败",QMessageBox::Ok);
        else
            QMessageBox::information(nullptr,"操作成功","删除津贴记录成功",QMessageBox::Ok);

    }
    else {
        if(!Helper::modiAllwRecord(allwRecord))
            QMessageBox::warning(nullptr,"操作失败","修改津贴记录失败",QMessageBox::Ok);
        else
            QMessageBox::information(nullptr,"操作成功","修改津贴记录成功",QMessageBox::Ok);
    }
}

void MainWindow::on_btn_salary_prodTable_clicked()
{
    QString year=ui->ldt_salary_year->text().simplified();
    QString month=ui->ldt_salary_mon->text().simplified();
    QDate inDate(year.toInt(),month.toInt(),1);
    QDate curDate(QDate::currentDate());

    inDate.isValid();
    if(!inDate.isValid() || inDate>curDate){
        QMessageBox::warning(nullptr,"生成工资表失败","输入的日期不合法",QMessageBox::Ok);
        return;
    }
    else{
        qDebug()<<QString("正在生成%1年%2月的月工资表").arg(year).arg(month)<<endl;
        if(!Helper::prodMonthSalary(year,month))
            QMessageBox::warning(nullptr,"生成工资表失败","sql语句执行失败",QMessageBox::Ok);
        else
            QMessageBox::information(nullptr,"生成月工资表成功","点击确认按钮继续",QMessageBox::Ok);
    }
}

void MainWindow::on_btn_award_prodTable_clicked()
{
    QString year=ui->ldt_award_year->text().simplified();
    QDate curDate(QDate::currentDate());

    if(year.toInt()>curDate.year()){
        QMessageBox::warning(nullptr,"生成年终奖金表失败","输入的日期不合法",QMessageBox::Ok);
    }
    else {
        qDebug()<<QString("正在生成%1年终奖金表").arg(year)<<endl;
        if(!Helper::prodYearEndAward(year))
            QMessageBox::warning(nullptr,"生成年终奖金表失败","sql语句执行失败",QMessageBox::Ok);
        else
            QMessageBox::information(nullptr,"生成年终奖金表成功","请点击确认按钮继续",QMessageBox::Ok);
    }
}

void MainWindow::on_rdb_salary_view_clicked()
{
    QString year=ui->ldt_salary_year->text().simplified();
    QString month=ui->ldt_salary_mon->text().simplified();
    QVector<QStringList> salaryInfo;

    QStandardItemModel *model=static_cast<QStandardItemModel *>(ui->tableView_salaryOrAward->model());
    model->clear();
    QStringList labels{"员工编号","年","月","工资","津贴","是否发放","发放员工编号","发放日期"};

    model->setColumnCount(8);
    model->setHorizontalHeaderLabels(labels);

    Helper::getAllStaffMonSalary(year,month,salaryInfo);
    for(int i=0;i<salaryInfo.size();i++){
        QList<QStandardItem *> itemList;
        for(int j=0;j<8;j++)
            itemList.append(new QStandardItem(salaryInfo[i][j]));
        itemList[6]->setText(itemList[6]->text()=="0"? "": itemList[6]->text());
        itemList[5]->setText(salaryInfo[i][5]=="0"? "否": "是");
        itemList[0]->setCheckable(true);
        for(int j=0;j<5;j++)
            itemList[j]->setEditable(false);
        for(int j=5;j<8;j++)
            itemList[j]->setEditable(true);
        model->appendRow(itemList);
    }
}

void MainWindow::on_rdb_award_view_clicked()
{
    QString year=ui->ldt_award_year->text().simplified();
    QVector<QStringList> yearAwardInfo;

    QStandardItemModel *model=static_cast<QStandardItemModel *>(ui->tableView_salaryOrAward->model());
    model->clear();
    QStringList labels{"员工编号","年","奖金","是否发放","发放员工编号","发放日期"};

    model->setColumnCount(6);
    model->setHorizontalHeaderLabels(labels);

    Helper::getAllStaffYearEndAward(year,yearAwardInfo);
    for(int i=0;i<yearAwardInfo.size();i++){
        QList<QStandardItem *> itemList;
        for(int j=0;j<6;j++)
            itemList.append(new QStandardItem(yearAwardInfo[i][j]));
        itemList[3]->setText(yearAwardInfo[i][3]=="0"? "否": "是");
        itemList[0]->setCheckable(true);
        itemList[4]->setText(itemList[4]->text()=="0"? "": itemList[4]->text());
        for(int j=0;j<3;j++)
            itemList[j]->setEditable(false);
        for(int j=3;j<6;j++){
            itemList[j]->setEditable(true);
        }
        model->appendRow(itemList);
    }
}

void MainWindow::on_btn_modiRecord_clicked()
{
    QStandardItemModel *model=static_cast<QStandardItemModel *>(ui->tableView_salaryOrAward->model());
    for(int i=0;i<model->rowCount();i++){
        if(model->item(i,0)->checkState()==Qt::Checked){
            model->item(i,0)->setCheckState(Qt::Unchecked);
            QStringList record;
            if(ui->rdb_salary_view->isChecked()){
                for(int j=0;j<8;j++)
                    record.append(model->item(i,j)->text());
                record[5]=record[5]=="是"? "true": "false";
                if(!Helper::modiSalaryRecord(record))
                    QMessageBox::warning(nullptr,"操作失败",QString("第%1行记录修改失败").arg(i),QMessageBox::Ok);
            }
            else{
                for(int j=0;j<6;j++)
                    record.append(model->item(i,j)->text());
                record[3]=record[3]=="是"? "true": "false";
                if(!Helper::modiYearEndAwardRecord(record))
                    QMessageBox::warning(nullptr,"操作失败",QString("第%1行记录修改失败").arg(i),QMessageBox::Ok);

            }
        }
    }

}

void MainWindow::on_btn_staff_clrFilePath_clicked()
{
    ui->ldt_staff_filepath->clear();
}

void MainWindow::on_btn_staff_clrRecord_clicked()
{
    ui->ldt_staff_id->clear();
    ui->ldt_staff_name->clear();
    ui->ldt_staff_sex->clear();
    ui->ldt_staff_birth->clear();
    ui->ldt_staff_idcard->clear();
    ui->ldt_staff_workType->clear();
    ui->ldt_staff_isLeave->clear();
    ui->ldt_staff_entryDate->clear();
    ui->ldt_staff_leaveDate->clear();
    ui->ldt_staff_bankCardId->clear();
}

void MainWindow::on_btn_attd_clrFilePath_clicked()
{
    ui->ldt_attd_filepath->clear();
}

void MainWindow::on_btn_attd_clrRecord_clicked()
{
    ui->ldt_attd_Date->clear();
    ui->ldt_attd_Status->clear();
    ui->ldt_attd_staffId->clear();
    ui->ldt_attd_deduction->clear();
}

void MainWindow::on_btn_allw_clrFilePath_clicked()
{
    ui->ldt_allw_filepath->clear();
}

void MainWindow::on_btn_allw_clrRecord_clicked()
{
    ui->ldt_allw_pay->clear();
    ui->ldt_allw_date->clear();
    ui->ldt_allw_days->clear();
    ui->ldt_allw_pay->clear();
}

bool MainWindow::isLegal(const QStringList &staffRecord)
{
    bool isLeave=staffRecord[6].toInt();
    QDate enDate=QDate::fromString(staffRecord[7],QString("yyyy-M-d"));
    QDate leDate=QDate::fromString(staffRecord[8],QString("yyyy-M-d"));

    qDebug()<<isLeave<<' '<<enDate<<' '<<leDate<<endl;

    if(!enDate.isValid())
        return false;
    if(isLeave==true && !leDate.isValid())
        return false;
    if(isLeave==true && leDate<enDate)
        return false;
    return true;
}

void MainWindow::on_btn_stat_search_clicked()
{
    QStringList list1=ui->dateEdit_stat_beginDate->text().simplified().split('-');
    QStringList list2=ui->dateEdit_stat_endDate->text().simplified().split('-');
    QDate beginDate(list1[0].toInt(),list1[1].toInt(),1);
    QDate endDate(list2[0].toInt(),list2[1].toInt(),1);
    QStandardItemModel *model=static_cast<QStandardItemModel *>(ui->tableView_statView->model());
    model->clear();

    if(ui->rdb_stat_staff->isChecked()){
        QStringList labels{"员工编号","员工姓名","年","月","工资","津贴"};
        model->setHorizontalHeaderLabels(labels);
        QString staffId=ui->ldt_staffId->text();
        QStringList record;

        while(beginDate<=endDate){
            if(Helper::getStaffMonSalary(staffId,QString("%1").arg(beginDate.year()),QString("%1").arg(beginDate.month()),record)){
                QList<QStandardItem *> itemList{
                    new QStandardItem(staffId),
                    new QStandardItem(record[0]),
                    new QStandardItem(QString("%1").arg(beginDate.year())),
                    new QStandardItem(QString("%2").arg(beginDate.month())),
                    new QStandardItem(record[1]),
                    new QStandardItem(record[2])
                };
                model->appendRow(itemList);
            }
            beginDate=beginDate.addMonths(1);
        }
    }
    else if(ui->rdb_stat_dept->isChecked()){
        QStringList labels{"部门编号","部门名称"};
        model->setHorizontalHeaderLabels(labels);
        QString deptId=ui->ldt_deptId->text();
        QVector<QStringList> info;

        if(!Helper::getAllDeptInfo(info))
            return;
        for(int i=0;i<info.size();i++)
            model->appendRow(QList<QStandardItem *>{new QStandardItem(info[i][0]),new QStandardItem(info[i][1])});

        while(beginDate<=endDate){
            info.clear();
            if(Helper::getDeptMonSalary(QString("%1").arg(beginDate.year()),QString("%1").arg(beginDate.month()),info)){

                QList<QStandardItem *> itemList;
                for(int i=0;i<info.size();i++){
                    itemList.append(new QStandardItem(info[i][2]));
                }
                model->appendColumn(itemList);
                labels.append(QString("%1年%2月工资").arg(beginDate.year()).arg(beginDate.month()));
                model->setHorizontalHeaderLabels(labels);
            }
            beginDate=beginDate.addMonths(1);
        }
    }
    else{
        QStringList labels{"年月","工资"};
        model->setHorizontalHeaderLabels(labels);

        while(beginDate<=endDate){
            QString sum;
            if(Helper::getMonSalary(QString("%1").arg(beginDate.year()),QString("%1").arg(beginDate.month()),sum)){
                QList<QStandardItem *> itemList{new QStandardItem(QString("%1年%2月").arg(beginDate.year()).arg(beginDate.month())),new QStandardItem(sum)};
                model->appendRow(itemList);
            }
            beginDate=beginDate.addMonths(1);
        }
    }
}
