#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QWidget>

namespace Ui {
class UserWindow;
}

class UserWindow : public QWidget
{
    Q_OBJECT

private:
    void updateView();

public:
    explicit UserWindow(QWidget *parent = nullptr);
    ~UserWindow();

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_btn_adduser_clicked();

private:
    Ui::UserWindow *ui;
};

#endif // USERWINDOW_H
