#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void Mouse_Pressed();
    void showMousePosition(QPoint& pos);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QImage sourceImage;

private slots:

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();





    void on_pushButton_3_clicked();
    void PrintVectors();
    void insertionSort();
    void mergeSort(int,int);
    void merge(int,int,int);
    void quickSort(int,int);
    int partition(int,int);
    void heapSort(int);
    void heapify(int,int);
    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_6_clicked();

protected:


private:
    Ui::MainWindow *ui;
    QPoint p1,p2;
    void point(QImage&,int,int,int, int, int, int);
    void ClearScreen(void);

};

#endif // MAINWINDOW_H
