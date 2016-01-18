#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSize>
#include <QImage>
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QImage * greyScale(QImage * origin);
    QImage * changeLight(int delta, QImage * origin);
    QImage * warm(int delta, QImage * origin);
    QImage * cool(int delta, QImage * origin);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionZoom_bigger_triggered();

    void on_actionZoom_smaller_triggered();

    void on_actionZoom_full_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_actionSave_triggered();

private:
    QString filename;
    QSize pictureSize;
    QImage* img;
    QImage* newImg;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
