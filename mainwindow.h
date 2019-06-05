#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QStandardItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    bool isLegal(const QStringList &staffRecord);

private slots:

    void on_btn_attd_selpath_clicked();

    void on_btn_attd_import_clicked();

    void on_btn_attd_insert_clicked();

    void on_btn_allw_selpath_clicked();

    void on_btn_allw_import_clicked();

    void on_btn_allw_insert_clicked();

    void on_btn_salary_prodTable_clicked();

    void on_btn_award_prodTable_clicked();

    void on_rdb_salary_view_clicked();

    void on_rdb_award_view_clicked();

    void on_btn_staff_selFile_clicked();

    void on_btn_staff_import_clicked();

    void on_btn_ataff_maintain_clicked();

    void on_btn_staff_clrFilePath_clicked();
    
    void on_btn_staff_clrRecord_clicked();

    void on_btn_staff_search_clicked();

    void on_btn_attd_search_clicked();

    void on_btn_allw_search_clicked();

    void on_btn_attd_clrFilePath_clicked();

    void on_btn_attd_clrRecord_clicked();

    void on_btn_allw_clrFilePath_clicked();

    void on_btn_allw_clrRecord_clicked();

    void on_btn_modiRecord_clicked();

    void on_btn_stat_search_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
