#include "mainwindow.h"
#include <QtNetwork>
#include <QRegularExpression>


Main_Window::Main_Window()
{
    setWindowTitle("DotA 3.0");
    QApplication::setQuitOnLastWindowClosed(false);

    m_btn_getcounters = new QPushButton("GET COUNTERS",this);
    m_btn_getcounters->setFixedSize(100,30);
    m_btn_getcounters->setEnabled(false);
    m_btn_clear = new QPushButton("CLEAR",this);
    m_btn_clear->setFixedSize(80,20);

    m_label_enemy = new QLabel("Enemy team :", this);
    m_label_enemy->setMaximumHeight(30);
    m_label_counters = new QLabel("Do not pick :", this);
    m_label_counters->setMaximumHeight(30);
    m_label_countered = new QLabel("You should pick :", this);
    m_label_countered->setMaximumHeight(30);
    m_counters = new QTextEdit(this);
    m_counters->setReadOnly(true);
    m_counters->resize(150,220);
    m_counters->setFont(QFont("Calibri",9));
    m_counters->setMinimumWidth(110);
    m_countered = new QTextEdit(this);
    m_countered->setReadOnly(true);
    m_countered->resize(150,220);
    m_countered->setFont(QFont("Calibri",9));
    m_countered->setMinimumWidth(110);

    m_test_status = new QTextEdit(this);
    m_test_status->setReadOnly(true);
    m_test_status->setFixedSize(66,45);
    m_test_status->setAlignment(Qt::AlignCenter);
    m_test_status->append("test");
    m_test_status->append("status");
    m_test_status->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_test_status->setStyleSheet("border: 1px solid; border-radius: 5px; background-color: white; ");

    m_hero1 = new QLineEdit(this);
    m_hero2 = new QLineEdit(this);
    m_hero3 = new QLineEdit(this);
    m_hero4 = new QLineEdit(this);
    m_hero5 = new QLineEdit(this);

    formlayout = new QFormLayout;
    formlayout->addRow("Hero 1", m_hero1);
    formlayout->addRow("Hero 2", m_hero2);
    formlayout->addRow("Hero 3", m_hero3);
    formlayout->addRow("Hero 4", m_hero4);
    formlayout->addRow("Hero 5", m_hero5);

    m_progressbar = new QProgressBar(this);
    m_progressbar->setFixedSize(150,20);

    layout = new QGridLayout(this);
    layout->addWidget(m_test_status,0,0,Qt::AlignCenter);
    layout->addWidget(m_progressbar,0,1,Qt::AlignCenter);
    layout->addWidget(m_btn_getcounters,0,2,Qt::AlignCenter);
    layout->addWidget(m_label_enemy,1,0,Qt::AlignCenter);
    layout->addWidget(m_label_counters,1,1);
    layout->addWidget(m_label_countered,1,2);
    layout->addLayout(formlayout,2,0,Qt::AlignCenter);
    layout->addWidget(m_counters,2,1,2,1);
    layout->addWidget(m_countered,2,2,2,1);
    layout->addWidget(m_btn_clear,3,0,Qt::AlignCenter);

    setLayout(layout);

    connect(m_btn_clear, &QPushButton::clicked, this, &Main_Window::clear_all);
    connect(m_hero1, &QLineEdit::editingFinished, this, &Main_Window::enable_start);
    connect(m_btn_getcounters, &QPushButton::clicked, this, &Main_Window::get_counters);

}

void Main_Window::clear_all()
{
    m_progressbar->setValue(0);
    m_btn_getcounters->setEnabled(false);
    m_counters->clear();
    m_countered->clear();
    m_hero1->clear();
    m_hero2->clear();
    m_hero3->clear();
    m_hero4->clear();
    m_hero5->clear();
    m_test_status->setStyleSheet("border: 1px solid; border-radius:5px; background-color: white; ");
}

void Main_Window::closeEvent(QCloseEvent *event)
{
    delete this;
    event->accept();
}

void Main_Window::enable_start()
{
    m_btn_getcounters->setEnabled(true);
}
void Main_Window::get_counters()
{
    m_progressbar->setValue(0);
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl("https://fr.dotabuff.com/heroes/" + m_hero1->text() + "/counters"));
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    QNetworkReply *response = manager.get(request);
    m_progressbar->setValue(25);
    response->ignoreSslErrors();
    QEventLoop event;
    connect(response,SIGNAL(finished()),&event,SLOT(quit()));
    event.exec();
    hero_html = new QString(response->readAll());
    if (hero_html->isEmpty()){
        m_test_status->setStyleSheet("border: 1px solid; border-radius:5px; background-color: red; ");
    }
    else {

        m_progressbar->setValue(50);
        QRegularExpression counter("section class(.*?)alt=\"(?<hero1>(.*?))\"(.*?)alt=\"(?<hero2>(.*?))\""
                                   "(.*?)alt=\"(?<hero3>(.*?))\"(.*?)alt=\"(?<hero4>(.*?))\""
                                   "(.*?)alt=\"(?<hero5>(.*?))\"(.*?)section",
                                   QRegularExpression::DotMatchesEverythingOption |
                                   QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatchIterator counter_it = counter.globalMatch(hero_html);
        QRegularExpressionMatch counter_match = counter_it.next();

        m_countered->append(m_hero1->text() + " is countered by :");
        for (int i = 1; i<6; i++) {
            m_countered->append(counter_match.captured("hero" + QString::number(i)));
        }

        counter_match = counter_it.next();
        m_progressbar->setValue(75);

        m_counters->append(m_hero1->text() + " counters :");
        for (int i = 1; i<6; i++) {
            m_counters->append(counter_match.captured("hero" + QString::number(i)));
        }
        m_progressbar->setValue(100);
        m_test_status->setStyleSheet("border: 1px solid; border-radius:5px; background-color: lime; ");
    }
}
