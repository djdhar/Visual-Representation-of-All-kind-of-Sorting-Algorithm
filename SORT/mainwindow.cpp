#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QImage>
#include <iostream>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintDevice>
#include <QPoint>
#include<stdlib.h>

class MyPoint{
    public:
    QPair<int,int> YX;
    QColor color;
    MyPoint(){
    }

    MyPoint(QPair<int,int> &YXi, QColor colori){
        YX=YXi;
        color=colori;
    }
    bool operator>(MyPoint YXi){
        return (this->YX>YXi.YX);
    }
    bool operator<(MyPoint YXi){
        return (this->YX<YXi.YX);
    }
    bool operator<=(MyPoint YXi){
        return (this->YX<=YXi.YX);
    }
};
bool ComparePoints(MyPoint a, MyPoint b){
    return a.YX<b.YX;
}
int pixelsize = 36;
QVector<QPair<int,int> >PolygonPoints;
QVector<QPair<int,int> >Base;
QVector<MyPoint > MyPoints;
QVector<QPair<int,int> >Window;
QColor edgeColor(255,255,255);
#define size_of_the_frame 900
#define PI 3.14159
#define DELAYTIME 20
QImage GlobalImg;
QString msg("You Win!!!");
void plotLineLow( QVector<QPair<int,int> > &v,int x0,int y0, int x1,int y1);
void plotLineHigh( QVector<QPair<int,int> > &v,int x0,int y0, int x1,int y1);
void Bresenham( QVector<QPair<int,int> > &v,int x0,int y0, int x1,int y1);


using namespace std;
double rad(int deg){
    return (PI/180)*deg;
}

inline void delay(int millisecondsWait)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsWait);
    loop.exec();
}

void plotLineLow( QVector<QPair<int,int> > &v,int x0,int y0, int x1,int y1){
    int dx=x1-x0;
    int dy=y1-y0;
    int yi=1;
    if(dy<0){
        yi=-1;
        dy=-dy;
    }
    int D=2*dy-dx;
    int y=y0;
    for(int x=x0;x<=x1;x=x+1){
        v.push_back(qMakePair(x,y));
        if(D>0){
            y=y+yi;
            D=D-2*dx;
        }
        D=D+2*dy;
    }

}
 void plotLineHigh( QVector<QPair<int,int> > &v,int x0,int y0, int x1,int y1){
     int dx=x1-x0;
     int dy=y1-y0;
     int xi=1;
     if(dx<0){
         xi=-1;
         dx=-dx;
     }
     int D=2*dx-dy;
     int x=x0;
     for(int y=y0;y<=y1;y=y+1){
         v.push_back(qMakePair(x,y));
         if(D>0){
             x=x+xi;
             D=D-2*dy;
         }
         D=D+2*dx;
     }
 }

 void Bresenham( QVector<QPair<int,int> > &v,int x0,int y0, int x1,int y1){
     if(abs(y1-y0)<abs(x1-x0)){
         if(x0>x1)
             plotLineLow(v,x1,y1,x0,y0);
         else {
             plotLineLow(v,x0,y0,x1,y1);
         }
     }
     else{
         if(y0>y1)
             plotLineHigh(v,x1,y1,x0,y0);
         else {
             plotLineHigh(v,x0,y0,x1,y1);
         }
     }
 }

 int RotX(int x, int y,int x1, int y1, int deg=45){
     return x1+(x-x1)*cos(rad(deg))-(y-y1)*sin(rad(deg));
 }
 int RotY(int y, int x,int x1, int y1, int deg=45){
     return y1+(x-x1)*sin(rad(deg))+(y-y1)*cos((rad(deg)));
 }

 void MainWindow::PrintVectors(){
     ClearScreen();
     int x=2;
     for(int i=0;i<MyPoints.size();i++){
         int y =MyPoints[i].YX.first;
         cout<<x<<" "<<y<<endl;
         auto r = MyPoints[i].color.red();
         auto g = MyPoints[i].color.green();
         auto b = MyPoints[i].color.blue();
         point(sourceImage,x,y,3,r,g,b);
         for(int j= size_of_the_frame-1;j>=y;j=j-pixelsize)
             point(sourceImage,x,j,3,r,g,b);
         x+=pixelsize;
     }
 }

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->frame,SIGNAL(Mouse_Pos()),this,SLOT(Mouse_Pressed()));
    connect(ui->frame,SIGNAL(sendMousePosition(QPoint&)),this,SLOT(showMousePosition(QPoint&)));

    ClearScreen();
}
void MainWindow::ClearScreen(void){
    QImage img=QImage(size_of_the_frame,size_of_the_frame,QImage::Format_RGB888);
        img.fill(QColor(Qt::black).rgb());

        QPainter painter( &img );
        QPen pen(Qt::red, 1);
        painter.setPen(pen);
        for(int i=0;i<size_of_the_frame;i=i+pixelsize)
            painter.drawLine(0,i,size_of_the_frame-1,i);
        for(int i=0;i<size_of_the_frame;i=i+pixelsize)
            painter.drawLine(i,0,i,size_of_the_frame-1);

        painter.end();
         ui->frame->setPixmap(QPixmap::fromImage(img));
         sourceImage=img;
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::point(QImage&img,int x,int y,int r, int red=255, int green=255, int blue=255)
{

    int left=x/pixelsize;
    int right=y/pixelsize;
    for(int i=left*pixelsize+1;i<(left+1)*pixelsize;i++){
        for(int j=right*pixelsize+1;j<(right+1)*pixelsize;j++){
            img.setPixel(i,j,qRgb(red,green,blue));
        }
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));
    sourceImage=img;
}

void MainWindow::showMousePosition(QPoint &pos)
{
    //ui->mouse_movement->setText(" X : "+QString::number(pos.x())+", Y : "+QString::number(pos.y()));
}
void MainWindow::Mouse_Pressed()
{
   // ui->mouse_pressed->setText(" X : "+QString::number(ui->frame->x)+", Y : "+QString::number(ui->frame->y));
    point(sourceImage,ui->frame->x,ui->frame->y,3,0,255,0);
    p1.setX((((int)ui->frame->x)/pixelsize)*pixelsize+pixelsize/2);
    p1.setY((((int)ui->frame->y)/pixelsize)*pixelsize+pixelsize/2);

}



void MainWindow::on_pushButton_clicked()
{
    ClearScreen();
    MyPoints.clear();
}

void MainWindow::on_pushButton_2_clicked()
{
    ClearScreen();
    MyPoints.clear();
    for(int i=2;i<size_of_the_frame;i=i+pixelsize){
        int x = i;
        int y = size_of_the_frame-rand()%size_of_the_frame;
        x=(x/pixelsize)*pixelsize+(pixelsize/2);
        y=(y/pixelsize)*pixelsize+(pixelsize/2);
        cout<<x<<" "<<y<<endl;

        auto r = rand()%119;
        auto g = 128+rand()%128;
        auto b = 128+rand()%128;
        QPair<int,int> mypair(y,x);
        MyPoints.push_back( MyPoint(mypair,QColor(r,g,b)));
        point(sourceImage,x,y,3,r,g,b);
        for(int j= size_of_the_frame-1;j>=y;j=j-pixelsize)
            point(sourceImage,x,j,3,r,g,b);
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    insertionSort();
    PrintVectors();
}
void MainWindow::insertionSort()
{
    int i, j;
    MyPoint key;
    for (i = 1; i < MyPoints.size(); i++)
    {
        key = MyPoints[i];
        j = i - 1;
        while (j >= 0 && MyPoints[j] > key)
        {
            MyPoints[j + 1] = MyPoints[j];
            PrintVectors();
            delay(DELAYTIME);
            j = j - 1;
        }
        MyPoints[j + 1] = key;
        PrintVectors();
        delay(DELAYTIME);
    }
}


void MainWindow::on_pushButton_4_clicked()
{
    int i, j, min_idx;
    for (i = 0; i < MyPoints.size(); i++)
    {
        min_idx = i;
        for (j = i+1; j < MyPoints.size(); j++)
        if (MyPoints[j] < MyPoints[min_idx])
            min_idx = j;
        PrintVectors();
        delay(DELAYTIME);
        swap(MyPoints[min_idx], MyPoints[i]);
        PrintVectors();
        delay(DELAYTIME);
    }
}

void MainWindow::merge(int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    MyPoint L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = MyPoints[l + i];
    for (j = 0; j < n2; j++)
        R[j] = MyPoints[m + 1+ j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            MyPoints[k] = L[i];
            i++;
            PrintVectors();
            delay(DELAYTIME);
        }
        else
        {
            MyPoints[k] = R[j];
            j++;
            PrintVectors();
            delay(DELAYTIME);
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        MyPoints[k] = L[i];
        PrintVectors();
        delay(DELAYTIME);
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        MyPoints[k] = R[j];
        PrintVectors();
        delay(DELAYTIME);
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void MainWindow::mergeSort(int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;

        // Sort first and second halves
        mergeSort(l, m);
        mergeSort(m+1, r);

        merge(l, m, r);
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    mergeSort(0,MyPoints.size()-1);
}

int MainWindow::partition (int low, int high)
{
    MyPoint pivot = MyPoints[high]; // pivot
    int i = (low - 1); // Index of smaller element

    for (int j = low; j <= high - 1; j++)
    {
        // If current element is smaller than the pivot
        if (MyPoints[j] < pivot)
        {
            i++; // increment index of smaller element
            swap(MyPoints[i], MyPoints[j]);
            PrintVectors();
            delay(DELAYTIME);

        }
    }
    swap(MyPoints[i + 1], MyPoints[high]);
    PrintVectors();
    delay(DELAYTIME);
    return (i + 1);
}

void MainWindow::quickSort(int low, int high)
{
    if (low < high)
    {
        int pi = partition(low, high);
        quickSort(low, pi - 1);
        quickSort(pi + 1, high);
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    quickSort(0,MyPoints.size()-1);
}

void MainWindow::heapify(int n, int i)
{
    int largest = i;
    int l = 2*i + 1;
    int r = 2*i + 2;
    if (l < n && MyPoints[l] > MyPoints[largest])
        largest = l;
    if (r < n && MyPoints[r] > MyPoints[largest])
        largest = r;
    if (largest != i)
    {
        swap(MyPoints[i], MyPoints[largest]);
        PrintVectors();
        delay(DELAYTIME);
        heapify(n, largest);
    }
}

void MainWindow::heapSort(int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(n, i);
    for (int i=n-1; i>0; i--)
    {
        swap(MyPoints[0], MyPoints[i]);
        PrintVectors();
        delay(DELAYTIME);
        heapify(i, 0);
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    heapSort(MyPoints.size());
}
