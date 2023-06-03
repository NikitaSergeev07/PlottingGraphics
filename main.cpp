#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QMessageBox>
#include <QColorDialog>
#include <QColor>
#include <QPen>
#include "qcustomplot.h"
#include <cmath>
#include "Solver.h"
#include "SquareFunc.h"
#include "LinearFunc.h"

int num_graph = 0;
QColor colorGraph(0, 0, 0);

class MyPlot : public QCustomPlot {
public:
    MyPlot(QWidget* parent);
};

MyPlot::MyPlot(QWidget *parent) {
    this->xAxis->setLabel("X");
    this->yAxis->setLabel("Y");
    this->xAxis->setRange(-25, 25);
    this->yAxis->setRange(-25, 25);
    this->setParent(parent);
    this->move(145, 5);
    this->resize(650, 650);

    this->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

bool containsDigits(const QString& str) {
    for (QChar ch : str) {
        if (ch.isDigit()) {
            return true;
        }
    }
    return false;
}

void resetScale(MyPlot* plot) {
    plot->xAxis->setRange(-25, 25);
    plot->yAxis->setRange(-25, 25);
    plot->replot();
}

void createGraphic(QLineEdit* func, QVector<Solver*>& funcs, QVector<QVector<QVector<double>>>& coordsX,
                   QVector<QVector<QVector<double>>>& coordsY, QCustomPlot* plot, double xmin, double ymin,
                        double xmax, double ymax) {
    QVector<double> tempX;
    QString toParse = func->text();

    if (toParse == "" || toParse == " ") {
        QMessageBox::warning(func, "Пустой ввод", "Вы ввели пустую строку.");
        return;
    }

    toParse.erase(std::remove(toParse.begin(), toParse.end(), ' '), toParse.end());
    toParse += "|";

    if (toParse.contains("x^2|") || toParse.contains("x^2+") || toParse.contains("x^2-")) {
        QString fCoeff = "", sCoeff = "", thCoeff = "";
        int pos = 0;
        double first = 0, second = 0, third = 0;

        pos = toParse.indexOf("x^2");
        pos--;
        while (pos >= 0) {
            fCoeff = toParse[pos] + fCoeff;
            pos--;
        }

        if (toParse.contains("x^2|")) {
            sCoeff = thCoeff = "0";
        }
        else if (toParse.contains("x+") || toParse.contains("x-") || toParse.contains("x|")) {
            pos = toParse.indexOf("x");
            pos++;
            pos = toParse.indexOf("x", pos);
            pos--;
            while (toParse[pos] != '-' && toParse[pos] != '+') {
                sCoeff = toParse[pos] + sCoeff;
                pos--;
            }
            sCoeff = toParse[pos] + sCoeff;

            if (toParse.contains("x|")) {
                thCoeff = "0";
            }
            else {
                pos = toParse.indexOf('|');
                pos--;
                while (toParse[pos] != '-' && toParse[pos] != '+') {
                    thCoeff = toParse[pos] + thCoeff;
                    pos--;
                }
                thCoeff = toParse[pos] + thCoeff;
            }
        }
        else if (toParse[toParse.size() - 2].isDigit()) {
            sCoeff = "0";
            pos = toParse.size() - 2;
            while (toParse[pos] != '-' && toParse[pos] != '+') {
                thCoeff = toParse[pos] + thCoeff;
                pos--;
            }
            thCoeff = toParse[pos] + thCoeff;
        }

        if (sCoeff.isEmpty()) {
            second = 1;
        }
        else if (sCoeff == "-") {
            second = -1;
        }
        else {
            second = sCoeff.toDouble();
        }

        if (fCoeff.isEmpty()) {
            first = 1;
        }
        else if (fCoeff == "-") {
            first = -1;
        }
        else {
            first = fCoeff.toDouble();
        }

        third = thCoeff.toDouble();


        Solver* temp = new SquareFunc(first, second, third);
        funcs.push_back(temp);

        double d = xmin;
        while (d <= xmax) {
            tempX.push_back(d);
            d += 0.05;
        }

        QVector<double> temptempX;

        coordsY.push_back(QVector<QVector<double>>());
        coordsY.back().push_back(QVector<double>());
        for (int i = 0; i < tempX.size(); i++) {
            if (funcs.back()->GetValue(tempX[i]) >= ymin &&
                funcs.back()->GetValue(tempX[i]) <= ymax) {
                coordsY.back().back().push_back(funcs.back()->GetValue(tempX[i]));
                temptempX.push_back(tempX[i]);
            }
        }
        QPen pen(colorGraph);
        plot->addGraph();
        coordsX.push_back(QVector<QVector<double>>());
        coordsX.back().push_back(QVector<double>(temptempX));
        plot->graph(num_graph)->setPen(pen);
        plot->graph(num_graph)->setData(coordsX.back().back(), coordsY.back().back());
        num_graph++;
        plot->replot();
    }
    else if (toParse.contains("x|") || toParse.contains("x+") || toParse.contains("x-")) {
        QString fCoeff = "", sCoeff = "";
        int pos = 0;
        double first = 0, second = 0;

        pos = toParse.indexOf('x');
        pos--;
        while (pos >= 0) {
            fCoeff = toParse[pos] + fCoeff;
            pos--;
        }

        if (toParse.contains("x|")) {
            sCoeff = "0";
        }
        else {
            pos = toParse.size() - 2;
            while (toParse[pos] != '-' && toParse[pos] != '+') {
                sCoeff = toParse[pos] + sCoeff;
                pos--;
            }
            sCoeff = toParse[pos] + sCoeff;
        }

        if (fCoeff.isEmpty()) {
            first = 1;
        }
        else if (fCoeff == "-") {
            first = -1;
        }
        else {
            first = fCoeff.toDouble();
        }
        second = sCoeff.toDouble();

        Solver* temp = new LinearFunc(first, second);
        funcs.push_back(temp);

        double d = xmin;
        while (d <= xmax) {
            tempX.push_back(d);
            d += 0.05;
        }

        QVector<double> temptempX;

        coordsY.push_back(QVector<QVector<double>>());
        coordsY.back().push_back(QVector<double>());
        for (int i = 0; i < tempX.size(); i++) {
            if (funcs.back()->GetValue(tempX[i]) >= ymin &&
                funcs.back()->GetValue(tempX[i]) <= ymax) {
                coordsY.back().back().push_back(funcs.back()->GetValue(tempX[i]));
                temptempX.push_back(tempX[i]);
            }
        }
        QPen pen(colorGraph);
        plot->addGraph();
        coordsX.push_back(QVector<QVector<double>>());
        coordsX.back().push_back(QVector<double>(temptempX));
        plot->graph(num_graph)->setPen(pen);
        plot->graph(num_graph)->setData(coordsX.back().back(), coordsY.back().back());
        num_graph++;
        plot->replot();
    }
    else {
        QMessageBox::warning(func, "Ошибка распознавания", "Нельзя построить график с такой конфигурацией.");
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow* w = new MainWindow;
    w->resize(800, 800);
    MyPlot* plot = new MyPlot(w);
    QLabel* yFunc = new QLabel("y = ", w);
    QLineEdit* edit1 = new QLineEdit(w);
    QLineEdit* edit2 = new QLineEdit(w);
    QLineEdit* edit3 = new QLineEdit(w);
    QLineEdit* edit4 = new QLineEdit(w);
    QPushButton* btn_save = new QPushButton("Применить", w);
    QPushButton* btn_reset = new QPushButton("Отцентровать", w);
    QPushButton* btn_color = new QPushButton("Цвет", w);
    QPushButton* btn_clear = new QPushButton("Сбросить", w);


    QLineEdit* fieldFunc = new QLineEdit(w);
    fieldFunc->setPlaceholderText("Введите функцию");

    QPushButton::connect(btn_reset, &QPushButton::clicked, [&plot]() {
        resetScale(plot);
    });

    QPushButton::connect(btn_color, &QPushButton::clicked, [](){
        QColorDialog colorDlg;
        if (colorDlg.exec() == QDialog::Accepted) {
            colorGraph = colorDlg.selectedColor();
        }
    });

    QVector<Solver*> graphics;
    QVector<QVector<QVector<double>>> coordsX;
    QVector<QVector<QVector<double>>> coordsY;

    QPushButton::connect(btn_save, &QPushButton::clicked, [&fieldFunc, &graphics, &coordsX, &coordsY, &plot,
                                                            &edit1, &edit2, &edit3, &edit4]() {
        double xmin = -50, xmax = 50, ymin = -50, ymax = 50;
        if (containsDigits(edit1->text())) {
            xmin = edit1->text().toDouble();
        }
        if (containsDigits(edit2->text())) {
            xmax = edit2->text().toDouble();
        }
        if (containsDigits(edit3->text())) {
            ymin = edit3->text().toDouble();
        }
        if (containsDigits(edit4->text())) {
            ymax = edit4->text().toDouble();
        }
        createGraphic(fieldFunc, graphics, coordsX, coordsY, plot, xmin, ymin, xmax, ymax);
    });

    QPushButton::connect(btn_clear, &QPushButton::clicked, [&plot] {
        plot->clearPlottables();
        plot->replot();
        num_graph = 0;
    });

    edit1->setGeometry(15, 30, 50, 30);
    edit2->setGeometry(15, 70, 50, 30);
    edit3->setGeometry(15, 110, 50, 30);
    edit4->setGeometry(15, 150, 50, 30);

    yFunc->setGeometry(325, 670, 40, 35);
    yFunc->setFont(QFont("Times new roman", 16));
    fieldFunc->setGeometry(350, 670, 300, 35);
    fieldFunc->setFont(QFont("Times new roman", 18));

    btn_save->setGeometry(8, 190, 130, 45);
    btn_reset->setGeometry(8, 250, 130, 45);
    btn_color->setGeometry(8, 310, 130, 45);
    btn_clear->setGeometry(8, 370, 130, 45);

    edit1->setPlaceholderText("x min");
    edit2->setPlaceholderText("x max");
    edit3->setPlaceholderText("y min");
    edit4->setPlaceholderText("y max");

    w->show();
    return a.exec();
}
