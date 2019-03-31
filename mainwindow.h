#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QApplication>

class Main_Window : public QWidget
{

public:
    Main_Window();
    void closeEvent(QCloseEvent *event);

private slots:
    void get_counters();
    void clear_all();
    void enable_start();

private:

    QString *hero_html;

    QTextEdit *m_test_status;

    QPushButton *m_btn_getcounters;
    QPushButton *m_btn_clear;

    QProgressBar *m_progressbar;

    QLabel *m_label_enemy;
    QLabel *m_label_counters;
    QLabel *m_label_countered;

    QTextEdit *m_counters;
    QTextEdit *m_countered;

    QLineEdit *m_hero1;
    QLineEdit *m_hero2;
    QLineEdit *m_hero3;
    QLineEdit *m_hero4;
    QLineEdit *m_hero5;

    QGridLayout *layout;
    QFormLayout *formlayout;

};

#endif // MAINWINDOW_H
