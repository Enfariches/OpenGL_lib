#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QEvent>
#include <QDebug>
#include <OpenGL/glu.h>

class Widget : public QOpenGLWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    float color[3] = {0, 0.51, 0.85}; //Стоковый цвет деталей

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    float xRot, yRot, zRot;
    float zoomScale = 1;
    float turnScale = 20;
    QPoint mPos;

    float color_first[3] = {0.16,0.16,0.16};
    float color_second[3] = {0.21,0.21,0.21};
    float color_third[3] = {0.38,0.38,0.38};

    float lt_ambient[4] = {0.3,0.3,0.3,0.3}; // Фоновое освещение
    float lt_diffuse[4] = {0.2,0.2,0.2,1}; //Диффузное освещение

    void drawParallelepiped(float l, float b, float zi); // Параллелепипед (высота, ширина, глубина, цвет)
    void drawPipe(float diameter, float xa, float xi = 0); // Труба (диаметр, длина, высота стенки)
    void drawCylinder(float diameter, float xa, float color[3]); // Цилиндр (диаметр, длина, цвет)
    void drawCircle(float radius); // Круг 2D (Радиус)
    void drawPolyhedron(float x, float y, float z); // Многоугольник (6-льник в данном случае, без параметров)

    void drawStand(); //Стенд из цилиндров
    void drawStandHolder(float a); //Кулочки не готовы
    void drawSH(float a, float h, float c);
    void setColor(int r, int g, int b); //Изменение цвета
    void trigger_change(bool trigger); //Триггер переключения камеры

    void wheelEvent(QWheelEvent*) override;

};


#endif // WIDGET_H
