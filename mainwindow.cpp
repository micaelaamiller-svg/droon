#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdio.h>
#include "muParser.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Conectar el boton Plot con la funcion graficar

    connect(ui->Plot, &QPushButton::clicked,
            this, &MainWindow::graf);

    // BOTON PARA CONSULTAR VALORES

    connect(ui->C_val, &QPushButton::clicked,
            this, &MainWindow::consultarValores);

    connect(ui->Area, &QPushButton::clicked,
            this, &MainWindow::calcularArea);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::graf()
{
    // ABRIMOS EL ARCHIVO BINARIO
    // REEMPLAZAR POR LA RUTA REAL DEL ARCHIVO

    FILE *a = fopen("C:/Users/micae/OneDrive/Documentos/INFO_DOS/QT_2026/2doParcial_Reyes_Roman/2doParcial_Reyes_Roman/registro_dron.dat", "rb");

    // VERIFICAMOS SI SE PUDO ABRIR

    if(a == NULL)
    {
        printf("Error al abrir el archivo.\n");
        return;
    }

    // VARIABLES TEMPORALES

    float x, y, t, b;

    // LIMPIAMOS LOS VECTORES

    coor_x.clear();
    coor_y.clear();
    time.clear();
    batery.clear();

    // LEEMOS EL ARCHIVO

    while(fread(&x, sizeof(float), 1, a) == 1)
    {
        // COORDENADA Y

        if(fread(&y, sizeof(float), 1, a) != 1)
            break;

        // TIEMPO

        if(fread(&t, sizeof(float), 1, a) != 1)
            break;

        // BATERIA

        if(fread(&b, sizeof(float), 1, a) != 1)
            break;

        // GUARDAMOS LOS DATOS EN LOS VECTORES

        coor_x.append((double)x);

        coor_y.append((double)y);

        time.append((double)t);

        batery.append((double)b);
    }

    // CERRAMOS EL ARCHIVO

    fclose(a);

    // GRAFICO DE COORDENADA X

    if(ui->plot_X->graphCount() == 0)
    {
        ui->plot_X->addGraph();
    }

    ui->plot_X->graph(0)->setData(time, coor_x);

    ui->plot_X->graph(0)->setPen(QPen(Qt::blue));

    ui->plot_X->xAxis->setLabel("Tiempo (s)");

    ui->plot_X->yAxis->setLabel("Coordenada X (m)");

    ui->plot_X->rescaleAxes();

    ui->plot_X->replot();

    // GRAFICO DE COORDENADA Y

    if(ui->plot_y->graphCount() == 0)
    {
        ui->plot_y->addGraph();
    }

    ui->plot_y->graph(0)->setData(time, coor_y);

    ui->plot_y->graph(0)->setPen(QPen(Qt::red));

    ui->plot_y->xAxis->setLabel("Tiempo (s)");

    ui->plot_y->yAxis->setLabel("Coordenada Y (m)");

    ui->plot_y->rescaleAxes();

    ui->plot_y->replot();

    // GRAFICO DE BATERIA

    if(ui->plot_bat->graphCount() == 0)
    {
        ui->plot_bat->addGraph();
    }

    ui->plot_bat->graph(0)->setData(time, batery);

    ui->plot_bat->graph(0)->setPen(QPen(Qt::magenta));

    ui->plot_bat->xAxis->setLabel("Tiempo (s)");

    ui->plot_bat->yAxis->setLabel("Bateria (%)");

    ui->plot_bat->rescaleAxes();

    ui->plot_bat->replot();

    printf("Archivo leido y graficos generados correctamente.\n");
}



// TERCER ITEM: CONSULTAR VALORES

void MainWindow::consultarValores()
{
    // VERIFICAMOS QUE EXISTAN DATOS

    if(time.isEmpty())
    {
        printf("Primero debe presionar Plot.\n");
        return;
    }

    // LEEMOS EL TIEMPO INGRESADO

    if(ui->Time->text().isEmpty())
    {
        printf("Debe ingresar un tiempo.\n");
        return;
    }

    double t = ui->Time->text().toDouble();

    // VARIABLES PARA LOS RESULTADOS

    double x = 0;
    double y = 0;
    double bateria = 0;

    int encontrado = 0;

    // RECORREMOS LOS TIEMPOS

    for(int i = 0; i < time.size(); i++)
    {
        // CASO 1: EL TIEMPO EXISTE EN EL ARCHIVO

        double diferencia = t - time[i];

        if(diferencia < 0)
        {
            diferencia = -diferencia;
        }

        if(diferencia < 0.0001)
        {
            x = coor_x[i];
            y = coor_y[i];
            bateria = batery[i];

            encontrado = 1;

            break;
        }

        // CASO 2: EL TIEMPO ESTA ENTRE DOS REGISTROS

        if(i < time.size() - 1)
        {
            if(t > time[i] && t < time[i + 1])
            {
                // FACTOR DE INTERPOLACION

                double factor = (t - time[i]) /
                                (time[i + 1] - time[i]);

                // COORDENADA X

                x = coor_x[i] +
                    factor * (coor_x[i + 1] - coor_x[i]);

                // COORDENADA Y

                y = coor_y[i] +
                    factor * (coor_y[i + 1] - coor_y[i]);

                // BATERIA

                bateria = batery[i] +
                          factor * (batery[i + 1] - batery[i]);

                encontrado = 1;

                break;
            }
        }
    }

    // VERIFICAMOS SI ENCONTRAMOS EL TIEMPO

    if(encontrado == 0)
    {
        printf("Tiempo fuera del intervalo.\n");
        return;
    }

    // MOSTRAMOS LOS RESULTADOS

    ui->X_coord->setText(QString::number(x, 'f', 2));

    ui->Y_coord->setText(QString::number(y, 'f', 2));

    ui->Bat->setText(QString::number(bateria, 'f', 2));
}


// SEGUNDO ITEM: CALCULAR EL AREA BAJO LA CURVA DE BATERIA

void MainWindow::calcularArea()
{
    // Verificamos que existan datos cargados

    if(time.size() < 2)
    {
        printf("Primero debe presionar Plot.\n");
        return;
    }

    // Leemos los tiempos ingresados

    double tmin = ui->T_min->text().toDouble();
    double tmax = ui->T_max->text().toDouble();

    // Verificamos que los tiempos sean correctos

    if(tmin >= tmax)
    {
        printf("El tiempo minimo debe ser menor al maximo.\n");
        return;
    }

    // Verificamos que esten dentro del intervalo registrado

    if(tmin < time[0] || tmax > time[time.size()-1])
    {
        printf("Los tiempos estan fuera del intervalo.\n");
        return;
    }

    // Inicializamos el area

    double area = 0;

    // Recorremos los registros

    for(int i = 0; i < time.size()-1; i++)
    {
        // Tomamos dos tiempos consecutivos

        double t1 = time[i];
        double t2 = time[i+1];

        // Tomamos sus valores de bateria

        double b1 = batery[i];
        double b2 = batery[i+1];

        // Verificamos si el tramo pertenece al intervalo

        if(t2 > tmin && t1 < tmax && t2 > t1)
        {
            double inicio = t1;
            double fin = t2;

            // Ajustamos el tiempo inicial

            if(inicio < tmin)
            {
                inicio = tmin;
            }

            // Ajustamos el tiempo final

            if(fin > tmax)
            {
                fin = tmax;
            }

            // Calculamos la bateria en el tiempo inicial

            double bateriaInicio = b1 +
                                   (b2 - b1) * (inicio - t1) / (t2 - t1);

            // Calculamos la bateria en el tiempo final

            double bateriaFin = b1 +
                                (b2 - b1) * (fin - t1) / (t2 - t1);

            // Calculamos el area del trapecio

            double trapecio = (bateriaInicio + bateriaFin)
                              * (fin - inicio) / 2.0;

            // Acumulamos el area

            area = area + trapecio;
        }
    }

    // Mostramos el resultado en la interfaz

    ui->Energy->setText(QString::number(area, 'f', 2));

    printf("Area calculada: %.2f\n", area);
}
