#include "mainwindow.h"

#include <string>
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
#include <SolverTest.h>
#include <QComboBox>

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

void createGraphic(QVector<Solver*>& funcs, QVector<QVector<QVector<double>>>& coordsX,
                   QVector<QVector<QVector<double>>>& coordsY, QCustomPlot* plot, double xmin, double ymin,
                        double xmax, double ymax, int selectedGraphIndex) {
        QVector<double> tempX;

        double d = xmin;
        while (d <= xmax) {
            tempX.push_back(d);
            d += 0.05;
        }

        QVector<double> temptempX;

        coordsY.push_back(QVector<QVector<double>>());
        coordsY.back().push_back(QVector<double>());
        for (int i = 0; i < tempX.size(); i++) {
            if (funcs[selectedGraphIndex]->GetValue(tempX[i]) >= ymin &&
                funcs[selectedGraphIndex]->GetValue(tempX[i]) <= ymax) {
                coordsY.back().back().push_back(funcs[selectedGraphIndex]->GetValue(tempX[i]));
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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow* w = new MainWindow;
    w->resize(800, 800);
    MyPlot* plot = new MyPlot(w);
    QLineEdit* edit1 = new QLineEdit(w);
    QLineEdit* edit2 = new QLineEdit(w);
    QLineEdit* edit3 = new QLineEdit(w);
    QLineEdit* edit4 = new QLineEdit(w);
    QPushButton* btn_save = new QPushButton("Применить", w);
    QPushButton* btn_reset = new QPushButton("Отцентровать", w);
    QPushButton* btn_color = new QPushButton("Цвет", w);
    QPushButton* btn_clear = new QPushButton("Сбросить", w);
    QComboBox* combo_graphics = new QComboBox(w);


    QPushButton::connect(btn_reset, &QPushButton::clicked, [&plot]() {
        resetScale(plot);
    });

    QPushButton::connect(btn_color, &QPushButton::clicked, [](){
        QColorDialog colorDlg;
        if (colorDlg.exec() == QDialog::Accepted) {
            colorGraph = colorDlg.selectedColor();
        }
    });

    QVector<Solver*> graphics = getGraphics();

    int z = 1;
    for (const Solver* graph : graphics) {
        QString ts = "Solver №" + QString::number(z);
        combo_graphics->addItem(ts);
        z++;
    }

    QVector<QVector<QVector<double>>> coordsX;
    QVector<QVector<QVector<double>>> coordsY;

    QPushButton::connect(btn_save, &QPushButton::clicked, [&graphics, &coordsX, &coordsY, &plot,
                                                            &edit1, &edit2, &edit3, &edit4, &combo_graphics]() {
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
        int selectedGraphIndex = combo_graphics->currentIndex();
        if (selectedGraphIndex >= 0 && selectedGraphIndex < graphics.size()) {
            createGraphic(graphics, coordsX, coordsY, plot, xmin, ymin, xmax, ymax, selectedGraphIndex);
        }
    });

    QPushButton::connect(btn_clear, &QPushButton::clicked, [&plot, &combo_graphics, &coordsX, &coordsY]() {
        int selectedGraphIndex = combo_graphics->currentIndex();
        if (selectedGraphIndex >= 0 && selectedGraphIndex < coordsX.size()) {
            plot->removeGraph(selectedGraphIndex);
            coordsX.remove(selectedGraphIndex);
            coordsY.remove(selectedGraphIndex);
            plot->replot();
            num_graph--;

            combo_graphics->setCurrentIndex(-1);
        } else if (coordsX.size() > 0) {
            plot->removeGraph(coordsX.size() - 1);
            coordsX.remove(coordsX.size() - 1);
            coordsY.remove(coordsY.size() - 1);
            plot->replot();
            num_graph--;
        }
    });





    combo_graphics->setGeometry(15, 190, 50, 30);

    edit1->setGeometry(15, 30, 50, 30);
    edit2->setGeometry(15, 70, 50, 30);
    edit3->setGeometry(15, 110, 50, 30);
    edit4->setGeometry(15, 150, 50, 30);


    combo_graphics->setGeometry(8, 190, 130, 45);
    btn_save->setGeometry(8, 250, 130, 45);
    btn_reset->setGeometry(8, 310, 130, 45);
    btn_color->setGeometry(8, 370, 130, 45);
    btn_clear->setGeometry(8, 420, 130, 45);

    edit1->setPlaceholderText("x min");
    edit2->setPlaceholderText("x max");
    edit3->setPlaceholderText("y min");
    edit4->setPlaceholderText("y max");

    w->show();
    return a.exec();
}
