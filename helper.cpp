#include "helper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDate>

const QString Helper::configFile="config.json";
QSqlDatabase Helper::data_base;

Helper::Helper()
{
}

bool Helper::connectDatabase()
{
    QString hostName="127.0.0.1";
    int port=3306;
    QString userName="root";
    QString password="password";
    QString databaseName="database";

    QFile file(Helper::configFile);
    if(!file.exists()){
        qDebug()<<"配置文件不存在"<<endl;
        return false;
    }
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"配置文件打开失败"<<endl;
        return false;
    }

    QJsonDocument jsonDoc(QJsonDocument::fromJson(file.readAll()));
    file.close();
    QJsonObject jsonObj(jsonDoc.object());
    if(jsonObj.contains("host"))
        hostName=jsonObj.take("host").toString();
    if(jsonObj.contains("port"))
        port=jsonObj.take("port").toInt();
    if(jsonObj.contains("id"))
        userName=jsonObj.take("id").toString();
    if(jsonObj.contains("password"))
        password=jsonObj.take("password").toString();
    if(jsonObj.contains("database"))
        databaseName=jsonObj.take("database").toString();

    qDebug()<<"配置信息："<<hostName<<' '<<port<<' '<<userName<<' '<<password<<' '<<databaseName<<endl;
    data_base=QSqlDatabase::addDatabase("QMYSQL");
    data_base.setHostName(hostName);
    data_base.setPort(port);
    data_base.setUserName(userName);
    data_base.setPassword(password);
    data_base.setDatabaseName(databaseName);

    if(!data_base.open()){
        qDebug()<<"failed to connect to database"<<endl;
        return false;
    }

    return true;
}


bool Helper::getUserInfo(const QString &userId, User &user)
{
    QSqlQuery query;
    QString sql="select * from User where userId="+addQuotes(userId);
    if(!query.exec(sql) || query.size()!=1){
        qDebug()<<"getUserInfo：sql语句执行失败或不存在用户记录："<<sql<<endl;
        return false;
    }

    query.next();
    user.id=query.value("userId").toString();
    user.password=query.value("password").toString();
    user.level=query.value("level").toInt();
    return true;
}

bool Helper::insertStaffRecord(const QStringList &staffRecord)
{
    if(staffRecord.size()<10)
        return false;

    QSqlQuery qury;
    QString sql=QString("insert Staff(staffId,staffName,sex,birthDate,idcard,workId,isLeave,entryDate,leaveDate,bankCardId) values(%1,%2,%3,%4,%5,%6,%7,%8,%9,%10)")
            .arg(staffRecord[0]).arg(addQuotes(staffRecord[1])).arg(staffRecord[2]).arg(addQuotes(staffRecord[3])).arg(addQuotes(staffRecord[4])).arg(staffRecord[5])
            .arg(staffRecord[6]).arg(addQuotes(staffRecord[7])).arg(staffRecord[8]==""? "null":addQuotes(staffRecord[8])).arg(staffRecord[9]==""? "null":addQuotes(staffRecord[9]));
    if(!qury.exec(sql)){
        qDebug()<<"insertStaffRecord：sql语句执行失败："<<sql<<endl;
        return false;
    }
    return true;
}

bool Helper::getStaffRecord(const QString staffId,QStringList &staffRecord)
{
    QSqlQuery qury;
    QString sql=QString("select * from Staff where staffId=%1").arg(staffId);
    if(!qury.exec(sql) || qury.size()==0){
        qDebug()<<"getStaffRecord：查询员工记录失败:"<<sql<<endl;
        return false;
    }

    qury.next();
    staffRecord.append(qury.value("staffId").toString());
    staffRecord.append(qury.value("staffName").toString());
    staffRecord.append(qury.value("sex").toString());
    staffRecord.append(qury.value("birthDate").toString());
    staffRecord.append(qury.value("idcard").toString());
    staffRecord.append(qury.value("workId").toString());
    staffRecord.append(qury.value("isLeave").toString());
    staffRecord.append(qury.value("entryDate").toString());
    staffRecord.append(qury.value("leaveDate").toString());
    staffRecord.append(qury.value("bankCardId").toString());
    return true;
}

bool Helper::modiStaffRecord(const QStringList &staffRecord)
{
    QSqlQuery qury;
    QString sql=QString("update Staff set staffName=%1,sex=%2,birthDate=%3,idcard=%4,workId=%5,isLeave=%6,entryDate=%7,leaveDate=%8,bankCardId=%9 where staffId=%10")
            .arg(addQuotes(staffRecord[1])).arg(staffRecord[2]).arg(addQuotes(staffRecord[3])).arg(addQuotes(staffRecord[4])).arg(staffRecord[5]).arg(staffRecord[6])
            .arg(addQuotes(staffRecord[7])).arg(staffRecord[8]==""? "null":addQuotes(staffRecord[8])).arg(staffRecord[9]==""? "null":addQuotes(staffRecord[9])).arg(staffRecord[0]);
    if(!qury.exec(sql)){
        qDebug()<<"modiStaffRecord:sql语句执行失败:"<<sql<<endl;
        return false;
    }
    return true;
}

bool Helper::insertAttdRecord(const QStringList &attdRecord)
{
    if(attdRecord.size()<4)
        return false;

    QSqlQuery query;
    QString sql=QString("insert Attendance(staffId,attdDate,deduction,descrp) values(%1,%2,%3,%4)")
            .arg(attdRecord[0]).arg(addQuotes(attdRecord[1])).arg(attdRecord[2]).arg(addQuotes(attdRecord[3]));
    if(!query.exec(sql)){
        qDebug()<<"insertAttdRecord：sql语句执行失败："<<sql<<endl;
        return false;
    }
    return true;
}

bool Helper::getAttdRecord(const QString &staffId, const QString &attdDate, QStringList &attdRecord)
{
    QSqlQuery qury;
    QString sql=QString("select * from Attendance where staffId=%1 and attdDate=%2").arg(staffId).arg(addQuotes(attdDate));
    if(!qury.exec(sql) || qury.size()==0){
        qDebug()<<"getAttdRecord:sql语句执行失败或者不存在考勤记录:"<<sql<<endl;
        return false;
    }

    qury.next();
    attdRecord.append(qury.value("staffId").toString());
    attdRecord.append(qury.value("attdDate").toString());
    attdRecord.append(qury.value("deduction").toString());
    attdRecord.append(qury.value("descrp").toString());
    return true;
}

bool Helper::delAttdRecord(const QString &staffId,const QString &date)
{
    QSqlQuery qury;
    QString sql=QString("delete from Attendance where staffId=%1 and attdDate=%2").arg(staffId).arg(addQuotes(date));
    if(!qury.exec(sql)){
        qDebug()<<"delAttdRecord:sql语句执行失败："<<sql<<endl;
        return false;
    }
    return true;
}

bool Helper::modiAttdRecord(const QStringList &attdRecord)
{
    QSqlQuery qury;
    QString sql=QString("update Attendance set deduction=%1,descrp=%2 where staffId=%3 and attdDate=%4")
            .arg(attdRecord[2]).arg(addQuotes(attdRecord[3])).arg(attdRecord[0]).arg(addQuotes(attdRecord[1]));
    if(!qury.exec(sql)){
        qDebug()<<"modiAttdRecord:sql语句执行失败:"<<sql<<endl;
        return false;
    }
    return true;
}

bool Helper::insertAllwRecord(const QStringList &allwRecord)
{
    if(allwRecord.size()<4)
        return false;

    QSqlQuery query;
    QString sql=QString("insert Allowance(staffId,allwDate,allwDays,allwPay) values(%1,%2,%3,%4)")
            .arg(allwRecord[0]).arg(addQuotes(allwRecord[1])).arg(allwRecord[2]).arg(allwRecord[3]);
    if(!query.exec(sql)){
        qDebug()<<"insertAllwRecord：sql语句执行失败"<<endl;
        return false;
    }
    return true;
}

bool Helper::getAllwRecord(const QString &staffId, const QString allwDate, QStringList &allwRecord)
{
    QSqlQuery qury;
    QString sql=QString("select * from Allowance where staffId=%1 and allwDate=%2").arg(staffId).arg(addQuotes(allwDate));
    if(!qury.exec(sql) || qury.size()==0){
        qDebug()<<"getAllwRecord:sql语句执行失败或者不存在加班记录:"<<sql<<endl;
        return false;
    }

    qury.next();
    allwRecord.append(qury.value("staffId").toString());
    allwRecord.append(qury.value("allwDate").toString());
    allwRecord.append(qury.value("allwDays").toString());
    allwRecord.append(qury.value("allwPay").toString());
    return true;
}

bool Helper::delAllwRecord(const QString &staffId, const QString allwDate)
{
    QSqlQuery qury;
    QString sql=QString("delete from Allowance whree staffId=%1 and allwDate=%2").arg(staffId).arg(addQuotes(allwDate));
    if(!qury.exec(sql)){
        qDebug()<<"delAllwRecord:sql语句执行失败:"<<sql<<endl;
        return false;
    }
    return true;
}

bool Helper::modiAllwRecord(const QStringList &allwRecord)
{
    QSqlQuery qury;
    QString sql=QString("update Allowance set allwDays=%1,allwPay=%2 where staffId=%3 and allwDate=%4")
            .arg(allwRecord[2]).arg(allwRecord[3]).arg(allwRecord[0]).arg(addQuotes(allwRecord[1]));
    if(!qury.exec(sql)){
        qDebug()<<"modiAllwRecord:sql语句执行失败:"<<sql<<endl;
        return false;
    }
    return true;
}

bool Helper::prodMonthSalary(const QString &year,const QString &month)
{
    QSqlQuery selStaffQuery;
    QString selStaffSql=QString("select staffId,workId,isLeave,entryDate,leaveDate from Staff");
    if(!selStaffQuery.exec(selStaffSql)){
        qDebug()<<"prodMonthSalary：sql语句执行失败"<<endl;
        return false;
    }


    QDate date(year.toInt(),month.toInt(),1);

    //计算每个在职员工的月工资并生成记录
    while(selStaffQuery.next()){
        //判断指定月份该员工是否已离职或者未入职
        bool isLeave=selStaffQuery.value("isLeave").toInt();
        QDate enDate=QDate::fromString(selStaffQuery.value("entryDate").toString(),QString("yyyy-M-d"));
        QDate leDate=(isLeave? QDate::fromString(selStaffQuery.value("leaveDate").toString(),QString("yyyy-M-d")): QDate::currentDate());
        enDate.setDate(enDate.year(),enDate.month(),1);
        leDate.setDate(leDate.year(),leDate.month(),1);
        if(date<enDate || date>leDate)
            continue;

        double basePay=0,attdDeu=0,allwPay=0;//basePay：基本工资 attdDeu：考勤扣款 allowPay：加班费
        //计算基本工资
        QSqlQuery basePayQuery;
        QString basePaySql=QString("select initSalary from WorkType where workId=%1").arg(selStaffQuery.value("workId").toString());
        if(!basePayQuery.exec(basePaySql) || basePayQuery.size()==0){
            qDebug()<<"查询基本工资失败"<<endl;
            continue;
        }
        else{
            basePayQuery.next();
            basePay=basePayQuery.value("initSalary").toDouble();
        }
        //计算考勤扣款金额
        QSqlQuery attdDeuQury;
        QString attdDeuSql=QString("select sum(deduction) from Attendance where staffId=%1 and Year(attdDate)=%2 and Month(attdDate)=%3")
                .arg(selStaffQuery.value("staffId").toString()).arg(year).arg(month);
        if(!attdDeuQury.exec(attdDeuSql)){
            qDebug()<<"查询考勤扣款金额失败"<<attdDeuSql<<endl;
            continue;
        }
        else{
            attdDeuQury.next();
            attdDeu=attdDeuQury.value(0).toDouble();
        }

        //计算津贴金额
        QSqlQuery allwPayQury;
        QString allwPaySql=QString("select sum(allwPay) from Allowance where staffId=%1 and Year(allwDate)=%2 and Month(allwDate)=%3")
                .arg(selStaffQuery.value("staffId").toString()).arg(year).arg(month);
        if(!allwPayQury.exec(allwPaySql)){
            qDebug()<<"查询加班津贴金额失败"<<endl;
            continue;
        }
        else{
            allwPayQury.next();
            allwPay=allwPayQury.value("sum(allwPay)").toDouble();
        }

        //更新表
        QSqlQuery chkRecdQury;
        QString chkRecdSql=QString("select * from MonthSalary where staffId=%1 and year=%2 and month=%3;")
                .arg(selStaffQuery.value("staffId").toString()).arg(year).arg(month);
        if(!chkRecdQury.exec(chkRecdSql)){
            qDebug()<<"检查记录失败"<<endl;
            continue;
        }
        else {
            if(chkRecdQury.size()==0){
                //执行插入记录操作
                QSqlQuery insertQury;
                QString insertSql=QString("insert into MonthSalary(staffId,year,month,salary,allwance,isPay) values(%1,%2,%3,%4,%5,false)")
                        .arg(selStaffQuery.value("staffId").toString()).arg(year).arg(month).arg(basePay-attdDeu).arg(allwPay);
                if(!insertQury.exec(insertSql))
                    qDebug()<<"插入失败："<<insertSql<<endl;
            }
            else {
                //执行更新记录操作
                QSqlQuery updateQury;
                QString updateSql=QString("update MonthSalary set salary=%1,allwance=%2 where staffId=%3 and year=%4 and month=%5")
                        .arg(basePay-attdDeu).arg(allwPay).arg(selStaffQuery.value("staffId").toString()).arg(year).arg(month);
                if(!updateQury.exec(updateSql))
                    qDebug()<<"更新失败："<<updateSql<<endl;
            }
        }
    }

    return true;
}

bool Helper::prodYearEndAward(const QString &year)
{
    QSqlQuery selStaffQuery;
    QString selStaffSql=QString("select staffId,workId,isLeave,entryDate,leaveDate from Staff");

    if(!selStaffQuery.exec(selStaffSql)){
        qDebug()<<"prodYearEndAward：sql语句执行失败"<<endl;
        return false;
    }

    int curYear=year.toInt();
    //计算每个员工的年终奖并生成记录
    while (selStaffQuery.next()) {
        //判断指定年份，员工是否未入职或者已离职
        bool isLeave=selStaffQuery.value("isLeave").toInt();
        int enYear=selStaffQuery.value("entryDate").toInt();
        int leYear=(isLeave==true? selStaffQuery.value("leDate").toInt(): QDate::currentDate().year());
        if(curYear<enYear || curYear>leYear)
            continue;

        QSqlQuery awardQury;
        QString awardSql=QString("select sum(salary),sum(allwance) from MonthSalary where staffId=%1 and year=%2")
                .arg(selStaffQuery.value("staffId").toString()).arg(year);
        if(!awardQury.exec(awardSql)){
            qDebug()<<"prodYearEndAward：sql语句执行失败："<<awardSql<<endl;
            continue;
        }
        awardQury.next();
        double award=(awardQury.value("sum(salary)").toDouble()+awardQury.value("sum(allwance)").toDouble())/12;

        QSqlQuery chkQury;
        QString chkSql=QString("select * from YearEndAward where staffId=%1 and year=%2")
                .arg(selStaffQuery.value("staffId").toString()).arg(year);
        if(!chkQury.exec(chkSql)){
            qDebug()<<"prodYearEndAward：sql语句执行失败："<<chkSql<<endl;
            continue;
        }
        if(chkQury.size()==0){
            //执行插入操作
            QSqlQuery insertQury;
            QString insertSql=QString("insert into YearEndAward(staffId,year,award,isPay) values(%1,%2,%3,%4)")
                    .arg(selStaffQuery.value("staffId").toString()).arg(year).arg(award).arg(false);
            if(!insertQury.exec(insertSql)){
                qDebug()<<"prodYearEndAward：sql语句执行失败："<<insertSql<<endl;
                continue;
            }
        }
        else{
            //执行更新操作
            QSqlQuery updateQury;
            QString updateSql=QString("update YearEndAward set award=%1 where staffId=%2 and year=%3")
                    .arg(award).arg(selStaffQuery.value("staffId").toString()).arg(year);
            if(!updateQury.exec(updateSql)){
                qDebug()<<"prodYearEndAward：sql语句执行失败："<<updateSql<<endl;
                continue;
            }
        }
    }

    return true;
}

bool Helper::getAllStaffMonSalary(const QString &year,const QString &month,QVector<QStringList> &salaryInfo)
{
    QSqlQuery qury;
    QString sql=QString("select * from MonthSalary where year=%1 and month=%2;").arg(year).arg(month);
    if(!qury.exec(sql)){
        qDebug()<<"getAllStaffMonSalary：sql语句执行失败："<<sql<<endl;
        return false;
    }

    while(qury.next()){
        QStringList record{qury.value(0).toString(),qury.value(1).toString(),qury.value(2).toString(),qury.value(3).toString(),
                    qury.value(4).toString(),qury.value(5).toString(),qury.value(6).toString(),qury.value(7).toString()};
        salaryInfo.append(record);
    }
    return true;
}

bool Helper::getAllStaffYearEndAward(const QString &year,QVector<QStringList> &yearAwardInfo)
{
    QSqlQuery qury;
    QString sql=QString("select * from YearEndAward where year=%1").arg(year);
    if(!qury.exec(sql)){
        qDebug()<<"getAllStaffMonSalary：sql语句执行失败："<<sql<<endl;
        return false;
    }

    while(qury.next()){
        QStringList record{qury.value(0).toString(),qury.value(1).toString(),qury.value(2).toString(),
                    qury.value(3).toString(),qury.value(4).toString(),qury.value(5).toString()};
        yearAwardInfo.append(record);
    }
    return true;
}

bool Helper::modiSalaryRecord(const QStringList &salaryRecord)
{
    QSqlQuery qury;
    QString sql=QString("update MonthSalary set isPay=%1,payStaffId=%2,payDate=%3 where staffId=%4 and year=%5 and month=%6")
            .arg(salaryRecord[5]==""? "null": salaryRecord[5]).arg(salaryRecord[6]==""? "null": salaryRecord[6]).arg(salaryRecord[7]==""? "null": addQuotes(salaryRecord[7]))
            .arg(salaryRecord[0]).arg(salaryRecord[1]).arg(salaryRecord[2]);
    if(!qury.exec(sql)){
        qDebug()<<"modiSalaryRecord:sql语句执行失败:"<<sql<<endl;
        return false;
    }
    return true;
}

bool Helper::modiYearEndAwardRecord(const QStringList &awardRecord)
{
    QSqlQuery qury;
    QString sql=QString("update YearEndAward set isPay=%1,payStaffId=%2,payDate=%3 where staffId=%4 and year=%5")
            .arg(awardRecord[3]==""? "null": awardRecord[3]).arg(awardRecord[4]==""? "null": awardRecord[4]).arg(awardRecord[5]==""? "null": addQuotes(awardRecord[5]))
            .arg(awardRecord[0]).arg(awardRecord[1]);
    if(!qury.exec(sql)){
        qDebug()<<"modiYearEndAwardRecord:sql语句执行失败:"<<sql<<endl;
        return false;
    }
    return true;
}

bool Helper::getStaffMonSalary(const QString &staffId,const QString &year,const QString &month,QStringList &record)
{
    QSqlQuery qury;
    QString sql=QString("select staffName,salary,allwance from Staff,MonthSalary where MonthSalary.staffId=%1 and year=%2 and month=%3 and Staff.staffId=MonthSalary.staffId")
            .arg(staffId).arg(year).arg(month);
    if(!qury.exec(sql) || qury.size()==0){
        qDebug()<<"getStaffMonSalary:sql语句执行失败或者查找结果记录为零:"<<sql<<endl;
        return false;
    }
    qury.next();
    record.append(qury.value("staffName").toString());
    record.append(qury.value("salary").toString());
    record.append(qury.value("allwance").toString());
    return true;
}

bool Helper::getDeptMonSalary(const QString &year,const QString &month,QVector<QStringList> &deptSalaryInfo)
{
    QSqlQuery qury;
    QString sql=QString("select t1.deptId,t1.deptName,sum(salary)+sum(allwance) from Dept as t1,WorkType as t2,Staff as t3,MonthSalary as t4 "
                        "where t4.year=%1 and t4.month=%2 and t1.deptId=t2.deptId and t2.workId=t3.workId and t3.staffId=t4.staffId group by deptId order by t1.deptId asc")
                        .arg(year).arg(month);
    if(!qury.exec(sql)){
        qDebug()<<"getDeptMonSalary:sql语句执行失败:"<<sql<<endl;
        return false;
    }
    while(qury.next())
        deptSalaryInfo.append(QStringList{qury.value("t1.deptId").toString(),qury.value("t1.deptName").toString(),qury.value("sum(salary)+sum(allwance)").toString()});
    return true;
}

bool Helper::getMonSalary(const QString &year,const QString &month,QString &monSalary)
{
    QSqlQuery qury;
    QString sql=QString("select sum(salary)+sum(allwance) from MonthSalary where year=%1 and month=%2").arg(year).arg(month);
    if(!qury.exec(sql)){
        qDebug()<<"getDeptMonSalary:sql语句执行失败:"<<sql<<endl;
        return false;
    }
    while(qury.next())
        monSalary.append(qury.value("sum(salary)+sum(allwance)").toString());
    return true;
}

bool Helper::getAllDeptInfo(QVector<QStringList> &info)
{
    QSqlQuery qury;
    QString sql=QString("select deptId,deptName from Dept");
    if(!qury.exec(sql)){
        qDebug()<<"getAllDeptInfo:sql语句执行失败:"<<sql<<endl;
        return false;
    }
    while(qury.next())
        info.append(QStringList{qury.value("deptId").toString(),qury.value("deptName").toString()});
    return true;
}

bool Helper::getAllUserInfo(QVector<QStringList> &info)
{
    QSqlQuery qury;
    QString sql=QString("select userId,level from User");
    if(!qury.exec(sql)){
        qDebug()<<"getAllUserInfo:sql语句执行失败:"<<sql<<endl;
        return false;
    }
    while(qury.next())
        info.append(QStringList{qury.value("userId").toString(),qury.value("level").toString()});
    return true;
}

bool Helper::addUserRecord(const QStringList &record)
{
    if(record.size()<3)
        return false;

    QSqlQuery qury;
    QString sql=QString("insert User(userId,password,level) values(%1,%2,%3)").arg(addQuotes(record[0])).arg(addQuotes(record[1])).arg(record[2]);
    if(!qury.exec(sql)){
        qDebug()<<"addUserRecord:sql语句执行失败:"<<sql<<endl;
        return false;
    }
    return true;
}

bool Helper::delUserRecord(const QString &userId)
{
    QSqlQuery qury;
    QString sql=QString("delete from User where userId=%1").arg(addQuotes(userId));
    if(!qury.exec(sql)){
        qDebug()<<"delUserRecord:sql语句执行失败:"<<sql<<endl;
        return false;
    }
    return true;
}

bool Helper::modiUserRecord(const QString &userId,const QString &userPassword)
{
    QSqlQuery qury;
    QString sql=QString("update User set password=%1 where userId=%2").arg(addQuotes(userPassword)).arg(addQuotes(userId));
    if(!qury.exec(sql)){
        qDebug()<<"modiUserRecord:sql语句执行失败"<<sql<<endl;
        return false;
    }
    return true;
}

QString Helper::addQuotes(const QString &str)
{
    return '\''+str+'\'';
}
