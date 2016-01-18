#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(813,433); // 禁止改变窗口大小。
//    ui->scrollArea->setMinimumSize(this->size());
    //ui->scrollArea->setFixedSize(this->size().width(), this->size().height()-30);
    //ui->label->setFixedSize(this->size());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()//open bmp
{
        pictureSize = this->size();
        filename=QFileDialog::getOpenFileName(this,
                                              tr("选择图像"),
                                              "",
                                              tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
        if(filename.isEmpty())
        {
             return;
        }
        else
        {
            img = new QImage;
            newImg = new QImage;
            if(! ( img->load(filename) && newImg->load(filename)) ) //加载图像
            {
                QMessageBox::information(this,
                                         tr("打开图像失败"),
                                         tr("打开图像失败!"));
                delete img;
                delete newImg;
                return;
            }
            ui->label->setPixmap(QPixmap::fromImage(*img));
        }
}

void MainWindow::on_actionZoom_bigger_triggered()
{
    QImage* imgScaled = new QImage;
    pictureSize.setHeight(pictureSize.height()+50);
    pictureSize.setWidth(pictureSize.width()+50);
    *imgScaled=img->scaled(pictureSize.width(), pictureSize.height(), Qt::KeepAspectRatio);
    ui->label->setPixmap(QPixmap::fromImage(*imgScaled));
}

void MainWindow::on_actionZoom_smaller_triggered()
{
    QImage* imgScaled = new QImage;
    pictureSize.setHeight(pictureSize.height()-50);
    pictureSize.setWidth(pictureSize.width()-50);
    *imgScaled=img->scaled(pictureSize.width(), pictureSize.height(), Qt::KeepAspectRatio);
    ui->label->setPixmap(QPixmap::fromImage(*imgScaled));
}

void MainWindow::on_actionZoom_full_triggered()
{
    QImage* imgScaled = new QImage;
    pictureSize = this->size();
    *imgScaled=img->scaled(pictureSize.width(), pictureSize.height(), Qt::KeepAspectRatio);
    ui->label->setPixmap(QPixmap::fromImage(*imgScaled));
}

QImage * MainWindow::greyScale(QImage * origin){
    QImage * newImage = new QImage(origin->width(), origin->height(), QImage::Format_ARGB32);

    QColor oldColor;

    for(int x = 0; x<newImage->width(); x++){
        for(int y = 0; y<newImage->height(); y++){
            oldColor = QColor(origin->pixel(x,y));
            int average = (oldColor.red()+oldColor.green()+oldColor.blue())/3;
            newImage->setPixel(x,y,qRgb(average,average,average));
        }
    }

    return newImage;
}

QImage * MainWindow::changeLight(int delta, QImage * origin){
    QImage * newImage = new QImage(origin->width(), origin->height(), QImage::Format_ARGB32);

    QColor oldColor;

    for(int x = 0; x<newImage->width(); x++){
        for(int y = 0; y<newImage->height(); y++){
            oldColor = QColor(origin->pixel(x,y));
            int red = oldColor.red()+delta, blue = oldColor.blue()+delta, green = oldColor.green()+delta;
            if (red > 255)
                red = 255;
            else if (red < 0)
                red = 0;
            if (blue > 255)
                blue = 255;
            else if (blue < 0)
                blue = 0;
            if (green > 255)
                green = 255;
            else if (green < 0)
                green = 0;

            newImage->setPixel(x,y,qRgb(red, green, blue));
        }
    }

    return newImage;
}

QImage * MainWindow::warm(int delta, QImage * origin){
    QImage *newImage = new QImage(origin->width(), origin->height(), QImage::Format_ARGB32);

    QColor oldColor;
    int r,g,b;

    for(int x=0; x<newImage->width(); x++){
        for(int y=0; y<newImage->height(); y++){
            oldColor = QColor(origin->pixel(x,y));

            r = oldColor.red() + delta;
            g = oldColor.green() + delta;
            b = oldColor.blue();

            //we check if the new values are between 0 and 255
            r = qBound(0, r, 255);
            g = qBound(0, g, 255);

            newImage->setPixel(x,y, qRgb(r,g,b));
        }
    }

    return newImage;
}


QImage * MainWindow::cool(int delta, QImage * origin){
    QImage *newImage = new QImage(origin->width(), origin->height(), QImage::Format_ARGB32);

    QColor oldColor;
    int r,g,b;

    for(int x=0; x<newImage->width(); x++){
        for(int y=0; y<newImage->height(); y++){
            oldColor = QColor(origin->pixel(x,y));

            r = oldColor.red();
            g = oldColor.green();
            b = oldColor.blue()+delta;

            //we check if the new value is between 0 and 255
            b = qBound(0, b, 255);

            newImage->setPixel(x,y, qRgb(r,g,b));
        }
    }

    return newImage;
}


QImage * MainWindow::saturation(int delta, QImage * origin){
    QImage * newImage = new QImage(origin->width(), origin->height(), QImage::Format_ARGB32);

    QColor oldColor;
    QColor newColor;
    int h,s,l;

    for(int x=0; x<newImage->width(); x++){
        for(int y=0; y<newImage->height(); y++){
            oldColor = QColor(origin->pixel(x,y));

            newColor = oldColor.toHsl();
            h = newColor.hue();
            s = newColor.saturation()+delta;
            l = newColor.lightness();

            //we check if the new value is between 0 and 255
            s = qBound(0, s, 255);

            newColor.setHsl(h, s, l);

            newImage->setPixel(x, y, qRgb(newColor.red(), newColor.green(), newColor.blue()));
        }
    }

    return newImage;
}

QImage * MainWindow::contrast(double delta, QImage * origin) {
    QImage * newImage = new QImage(origin->width(), origin->height(), QImage::Format_ARGB32);

    QColor oldColor;
    QColor newColor;
    int h,s,l;
    int r,g,b;
    for(int x=0; x<newImage->width(); x++){
        for(int y=0; y<newImage->height(); y++){
            oldColor = QColor(origin->pixel(x,y));

            r = oldColor.red();
            g = oldColor.green();
            b = oldColor.blue();

            r = (r-127)*delta+127;
            g = (g-127)*delta+127;
            b = (b-127)*delta+127;


            //we check if the new value is between 0 and 255
            b = qBound(0, b, 255);
            r = qBound(0, r, 255);
            g = qBound(0, g, 255);

            newImage->setPixel(x,y, qRgb(r,g,b));
        }
    }

    return newImage;
}

void MainWindow::on_pushButton_clicked()
{
    if (!newImg)
        return;
    newImg = changeLight(20, newImg);
    ui->label->setPixmap(QPixmap::fromImage(*newImg));
}

void MainWindow::on_pushButton_2_clicked()
{
    if (!newImg)
        return;
    newImg = changeLight(-20,newImg);
    ui->label->setPixmap(QPixmap::fromImage(*newImg));
}

void MainWindow::on_pushButton_6_clicked()
{
    if (!newImg)
        return;
    ui->label->setPixmap(QPixmap::fromImage(*img));
    *newImg = *img;
}

void MainWindow::on_pushButton_3_clicked()
{
    if (!newImg)
        return;
    newImg = greyScale(img);
    ui->label->setPixmap(QPixmap::fromImage(*newImg));
}

void MainWindow::on_pushButton_4_clicked()
{
    if (!newImg)
        return;
    newImg = warm(20, newImg);
    ui->label->setPixmap(QPixmap::fromImage(*newImg));
}

void MainWindow::on_pushButton_5_clicked()
{
    if (!newImg)
        return;
    newImg = cool(20, newImg);
    ui->label->setPixmap(QPixmap::fromImage(*newImg));
}

void MainWindow::on_actionSave_triggered()
{
    if (!newImg)
        return;
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Save Image"),
        "",
        tr("*.bmp;; *.png;; *.jpg;; *.tif;; *.GIF")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        if(! ( newImg->save(filename) ) ) //保存图像
        {
            QMessageBox::information(this,
                tr("Failed to save the image"),
                tr("Failed to save the image!"));
            return;
        }
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    if (!newImg)
        return;
    QMatrix matrix;
    matrix.rotate(-90);
    *newImg = newImg->transformed(matrix);
    ui->label->setPixmap(QPixmap::fromImage(*newImg));
}

void MainWindow::on_pushButton_8_clicked()
{
    if (!newImg)
        return;
    QMatrix matrix;
    matrix.rotate(90);
    *newImg = newImg->transformed(matrix);
    ui->label->setPixmap(QPixmap::fromImage(*newImg));
}

void MainWindow::on_pushButton_9_clicked()
{
    if (!newImg)
        return;
    newImg = saturation(20, newImg);
    ui->label->setPixmap(QPixmap::fromImage(*newImg));
}

void MainWindow::on_pushButton_10_clicked()
{
    if (!newImg)
        return;
    newImg = saturation(-20, newImg);
    ui->label->setPixmap(QPixmap::fromImage(*newImg));
}

void MainWindow::on_pushButton_11_clicked()
{
    if (!newImg)
        return;
    newImg = contrast(1.1, newImg);
    ui->label->setPixmap(QPixmap::fromImage(*newImg));
}

void MainWindow::on_pushButton_12_clicked()
{
    if (!newImg)
        return;
    newImg = contrast(0.9, newImg);
    ui->label->setPixmap(QPixmap::fromImage(*newImg));
}
