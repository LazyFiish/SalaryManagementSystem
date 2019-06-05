#ifndef HELPER_H
#define HELPER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>

struct User{
    QString id;
    QString password;
    int level;
};

static User curUser;

class Helper
{
private:
    static const QString configFile;
    static QSqlDatabase data_base;

private:
    Helper();

public:



    /*
     * 功能：连接数据库
     * 入口参数：
     * 出口参数：
     * 返回值：连接成功返回true;失败返回false
     */
    static bool connectDatabase();

    /*
     * 功能：通过用户名获取用户密码
     * 入口参数：userID——用户名
     * 出口参数：user-用户详细信息
     * 返回值：用户存在则返回true，否则返回false
     */
    static bool getUserInfo(const QString &userId, User &user);

    /*
     * 功能：插入职工记录
     * 入口参数：staffRecord——员工详细信息
     * 出口参数：
     * 返回值：成功返回true;失败返回false
     */
    static bool insertStaffRecord(const QStringList &staffRecord);

    /*
     * 功能：获取指定员工的信息
     * 入口参数：staffId——员工编号
     * 出口参数：staffRecord——员工详细信息
     * 返回值：成功返回true;失败返回false
     */
    static bool getStaffRecord(const QString staffId,QStringList &staffRecord);

    /*
     * 功能：修改指定员工信息
     * 入口参数：staffRecord——员工新信息
     * 出口参数：
     * 返回值：成功返回true;失败返回false
     */
    static bool modiStaffRecord(const QStringList &staffRecord);

    /*
     * 功能：插入考勤记录
     * 入口参数：attdRecord——考勤记录
     * 出口参数：
     * 返回值：插入成功返回true;失败返回false
     */
    static bool insertAttdRecord(const QStringList &attdRecord);

    /*
     * 功能：获取指定员工在指定日期的考勤记录信息
     * 入口参数：staffId——员工编号、date——考勤日期
     * 出口参数：attdRecord——考勤记录
     * 返回值：成功返回true;失败返回false
     */
    static bool getAttdRecord(const QString &staffId,const QString &attdDate,QStringList &attdRecord);

    /*
     * 功能：删除考勤记录
     * 入口参数：staffId——员工编号、date——考勤日期
     * 出口参数：
     * 返回值：成功返回true;失败返回false
     */
    static bool delAttdRecord(const QString &staffId,const QString &date);

    /*
     * 功能：修改考勤记录
     * 入口参数：attdRecord——新的考勤记录信息
     * 出口参数：
     * 返回值：成功返回true;失败返回false
     */
    static bool modiAttdRecord(const QStringList &attdRecord);

    /*
     * 功能：插入津贴记录
     * 入口参数：allwRecord——津贴记录
     * 出口参数：
     * 返回值：插入成功返回true;失败返回false
     */

    static bool insertAllwRecord(const QStringList &allwRecord);

    /*
     * 功能：获取指定员工在指定日期的津贴记录
     * 入口参数：staffId——员工编号、allwDate——加班日期
     * 出口参数：allwRecord——津贴记录
     * 返回值：成功返回true;失败返回false
     */
    static bool getAllwRecord(const QString &staffId,const QString allwDate,QStringList &allwRecord);

    /*
     * 功能：删除指定员工在指定日期的津贴记录
     * 入口参数：staffId——员工编号、allwDate——加班日期
     * 出口参数：
     * 返回值：成功返回true;失败返回false
     */
    static bool delAllwRecord(const QString &staffId,const QString allwDate);

    /*
     * 功能：修改指定员工在指定日期的加班记录
     * 入口参数：staffId——员工编号、allwRecord——新的加班记录信息
     * 出口参数：
     * 返回值：成功返回true;失败返回false
     */
    static bool modiAllwRecord(const QStringList &allwRecord);

    /*
     * 功能：生成指定年月的月工资表
     * 入口参数：year——年份、month——月份
     * 出口参数：
     * 返回值：成功返回true;失败返回false
     */
    static bool prodMonthSalary(const QString &year,const QString &month);

    /*
     * 功能：生成指定年份的年终奖金表
     * 入口参数：year——年份
     * 出口参数：
     * 返回值：成功返回true;失败返回false
     */
    static bool prodYearEndAward(const QString &year);

    /*
     * 功能：获取指定月所有员工的工资信息
     * 入口参数：year——年份、month——月份
     * 出口参数：salaryInfo——所有员工的工资信息
     * 返回值：成功返回true;失败返回false
     */
    static bool getAllStaffMonSalary(const QString &year,const QString &month,QVector<QStringList> &salaryInfo);

    /*
     * 功能：获取指定年份所有员工的年终奖信息
     * 入口参数：year——年份
     * 出口参数：yearAwardInfo——所有员工的年终奖信息
     * 返回值：成功返回true;失败返回false
     */
    static bool getAllStaffYearEndAward(const QString &year,QVector<QStringList> &yearAwardInfo);

    /*
     * 功能：修改月工资表记录
     * 入口参数：salaryRecord——新的月工资记录
     * 出口参数：
     * 返回值：成功返回true;失败返回false
     */
    static bool modiSalaryRecord(const QStringList &salaryRecord);

    /*
     * 功能：修改年终奖记录
     * 入口参数：awardRecord——新的年终奖记录
     * 出口参数
     */
    static bool modiYearEndAwardRecord(const QStringList &awardRecord);

    /*
     * 功能：或取指定年月的职工工资信息
     * 入口参数：staffId——职工id、year——年份、month——月份
     * 出口参数：salary——薪水、allowance——津贴
     * 返回值：成功返回true;失败返回false
     */
    static bool getStaffMonSalary(const QString &staffId,const QString &year,const QString &month,QStringList &record);

    /*
     * 功能：获取指定年月每个部门工资
     * 入口参数:year——年份、month——月份
     * 出口参数：deptSalaryInfo——部门工资情况
     * 返回值：成功返回true;失败返回false
     */
    static bool getDeptMonSalary(const QString &year,const QString &month,QVector<QStringList> &deptSalaryInfo);

    /*
     * 功能：获取指定年月的总工资
     * 入口参数：year——年份、month——月份
     * 出口参数：monthSalary——月总工资
     * 返回值：成功返回true;失败返回false
     */
    static bool getMonSalary(const QString &year,const QString &month,QString &monSalary);

    /*
     * 功能：获取所有部门信息
     * 入口参数：
     * 出口参数：info——所有部门的信息，包括部门编号与部门名称
     * 返回值：成功返回true;失败返回false
     */
    static bool getAllDeptInfo(QVector<QStringList> &info);

    /*
     * 功能：获取所有登录用户的信息
     * 入口参数：
     * 出口参数：info——所有登录用户的信息
     * 返回值：成功返回true;失败返回false
     */
    static bool getAllUserInfo(QVector<QStringList> &info);

    /*
     * 功能：添加登录用户记录
     * 入口参数：record——登录用户信息
     * 出口参数：
     * 返回值：成功返回true;失败返回false
     */
    static bool addUserRecord(const QStringList &record);

    /*
     * 功能：删除登录用户记录
     * 入口参数：userId——登录用户Id
     * 出口参数：
     * 返回值：成功返回true，失败返回false
     */
    static bool delUserRecord(const QString &userId);

    static bool modiUserRecord(const QString &userId,const QString &userPassword);

    static QString addQuotes(const QString &str);
};

#endif // HELPER_H
