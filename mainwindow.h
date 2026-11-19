#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Qvector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // Vectores para almacenar los datos del dron

    QVector<double> coor_x;
    QVector<double> coor_y;
    QVector<double> time;
    QVector<double> batery;

private slots:

    void graf();
    void consultarValores();
    void calcularArea();
};
#endif // MAINWINDOW_H
