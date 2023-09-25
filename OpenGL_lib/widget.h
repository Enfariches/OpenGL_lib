#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QEvent>
#include <math.h>
#include <GL/glu.h>

class Widget : public QOpenGLWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    float color[3] = {0, 0.51, 0.85};

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    float xRot, yRot, zRot;
    float zoomScale = 1;
    QPoint mPos;

    float color_first[3] = {0.16,0.16,0.16};
    float color_second[3] = {0.21,0.21,0.21};
    float color_third[3] = {0.38,0.38,0.38};

    void drawParallelepiped(float l, float b, float zi); // Параллелепипед (высота, ширина, глубина, цвет)
    void drawPipe(float diameter, float xa, float xi = 0); // Труба (диаметр, длина, высота стенки)
    void drawCylinder(float diameter, float xa, float color[3]); // Цилиндр (диаметр, длина, цвет)
    void drawCircle(float radius); // Круг 2D (Радиус)
    void drawPolyhedron(float x, float y, float z); // Многоугольник (6-льник в данном случае, без параметров)

    void drawStand(); //Стенд из цилиндров
    void drawStandHolder(float a); //Кулочки не готовы
    void setColor(int r, int g, int b);

    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;

};


#endif // WIDGET_H
