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
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
private:
    float xRot, yRot, zRot;
    float zoomScale = 1;
    QPoint mPos;


    void drawCube(float l, float b, float zi);
    void drawPipe(float diameter, float xa, float xi = 0);
    void drawCylinder(float diameter, float xa);
    void drawCircle(float radius);

    void drawStand();
    void drawStandHolder(float a);

    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;


};


#endif // WIDGET_H
