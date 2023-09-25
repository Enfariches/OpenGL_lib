#include "widget.h"

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setWindowTitle("Ambiewnt");
    setGeometry(400,200,800,600);
}
Widget::~Widget()
{

}


void Widget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
}

void Widget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int k = w / h;
    glFrustum(-k,k, -1,1, 1,7);

}

void Widget::paintGL()
{
    glClearColor(0,0.49,0.83,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0,0,-2.5);

    glRotatef(xRot,1,0,0);
    glRotatef(yRot,0,1,0);
    glScalef(zoomScale,zoomScale,zoomScale);
    drawStand();
    drawStandHolder(1);

    glRotatef(90,0,1,0);
    //drawCube(1,1,1); //глубина, высота, ширина
    glTranslatef(0,0,0.35);
    //drawCylinder(2,1);
    drawPipe(5,0.2,-5);

}

void Widget::mousePressEvent(QMouseEvent* mo){
    mPos = mo->pos();
}

void Widget::mouseMoveEvent(QMouseEvent *mo){
    xRot = 1 / M_PI*(mo->pos().y() - mPos.y());
    yRot = 1 / M_PI*(mo->pos().x() - mPos.x());
    update();
}

void Widget::wheelEvent(QWheelEvent *mo){
    QPoint wheel = mo->angleDelta();
    if(wheel.y() > 0){
        zoomScale *= 1.1f;
    }
    else if(wheel.y() < 0){
        zoomScale /= 1.1f;
    }
    update();
}

void Widget::drawCube(float b, float l, float zi)
{
    float arr[] = {-1, -1, zi,  b, -1, zi,  b, l, zi,  -1, l, zi, // передняя
            -1, -1, -zi,  b, -1, -zi,  b, l, -zi,  -1, l, -zi, //задняя
            -1, -1, -zi,  -1, -1, zi,  -1, l, zi,   -1, l, -zi, //левая
             b, -1, zi,  b, -1, -zi,   b, l, -zi,  b, l, zi, //правая
             -1, -1, zi,  -1, -1, -zi,  b, -1, -zi,  b, -1, zi, //нижняя
             -1, l, zi,  b, l, zi,  b, l, -zi,  -1, l, -zi}; //верхняя

    float color_arr[] = {
        1,0,0,  1,0,0,  1,0,0,  1,0,0,
        0,0,1,  0,0,1,  0,0,1,  0,0,1,
        1,1,0,  1,1,0,  1,1,0,  1,1,0,
        0,1,1,  0,1,1,  0,1,1,  0,1,1,
        1,0,1,  1,0,1,  1,0,1,  1,0,1,
        1,0.5,0.5,  1,0.5,0.5,  1,0.5,0.5,  1,0.5,0.5
    };

    glVertexPointer(3, GL_FLOAT, 0, &arr);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColorPointer(3, GL_FLOAT, 0, &color_arr);
    glEnableClientState(GL_COLOR_ARRAY);

        glDrawArrays(GL_QUADS, 0, 26);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Widget::drawPipe(float diameter, float xa, float xi){

        float radius = diameter / 2;
        const float steps = 40;
        const float angle = 3.1415926 * 2.f / steps;
        float color[3] = {0,1,0};

        if(xi == 0)
        {
            glBegin(GL_TRIANGLE_STRIP);

            float x = radius * sin(angle * 0);
            float y = -radius * cos(angle * 0);

            glColor3f(0,1,0);
            glVertex3f(x,y,xa);
            glColor3f(0,0,1);
            glVertex3f(x,y,-xa);

            for (int i = 1; i <= steps; i++)
            {
              float newX = radius * sin(angle * i);
              float newY = -radius * cos(angle * i);

              glColor3f(0,1,0);
              glVertex3f(newX,newY,xa);
              glColor3f(0,0,1);
              glVertex3f(newX,newY,-xa);
            }
            glEnd();
        }

        else
        {
            for(int j = 0; j < 2; ++j)
            {
                if(j == 1)
                {
                    radius += xi;
                }
                glBegin(GL_TRIANGLE_STRIP);

                float x = radius * sin(angle * 0);
                float y = -radius * cos(angle * 0);

                glColor3f(0,1,0);
                glVertex3f(x,y,xa);
                glColor3f(0,0,1);
                glVertex3f(x,y,-xa);

                for (int i = 1; i <= steps; i++)
                {
                  float newX = radius * sin(angle * i);
                  float newY = -radius * cos(angle * i);

                  glColor3f(0,1,0);
                  glVertex3f(newX,newY,xa);
                  glColor3f(0,0,1);
                  glVertex3f(newX,newY,-xa);
                }
                glEnd();

            }

            radius -= xi;
            for (int i = 0; i < 2; ++i)
            {
                if(i == 1)
                {
                    xa *= -1;
                    color[1] = 0;
                    color[2] = 1;
                }
                glBegin(GL_TRIANGLE_STRIP);
                float mainX = radius * sin(angle * 0);
                float upY = -radius * cos(angle * 0);
                float downY = -(radius + xi) * cos(angle * 0);

                glColor3fv(color);
                glVertex3f(mainX,downY,xa);
                glColor3fv(color);
                glVertex3f(mainX,upY,xa);

                for (int j = 1; j <= steps; ++j)
                {
                   float upX = radius * sin(angle * j);
                   float upY = -radius * cos(angle * j);

                   float downX = (radius + xi) * sin(angle * j);
                   float downY = -(radius + xi) * cos(angle * j);

                   glColor3fv(color);
                   glVertex3f(downX,downY,xa);
                   glColor3fv(color);
                   glVertex3f(upX,upY,xa);
                }
                glEnd();
            }
        }
}

void Widget::drawCircle(float radius){

    const float steps = 40;
    const float angle = 3.1415926 * 2.f / steps;

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0,0.0,0.0);

    for (int i = 0; i <= steps; i++)
    {
        float x = radius * sin(angle * i);
        float y = -radius * cos(angle * i);
        glVertex3f(x, y, 0);
    }
    glEnd();
}

void Widget::drawCylinder(float diameter, float xa){

    float radius = diameter / 2;
    const float steps = 40;
    const float angle = 3.1415926 * 2.f / steps;

    for(int i = 0; i < 2; ++i)
    {
        if(i == 1)
        {
            xa *= -1;
        }
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0,1,1);
        glVertex3f(0.0,0.0,xa);

        for (int i = 0; i <= steps; i++)
        {
            float x = radius * sin(angle * i);
            float y = -radius * cos(angle * i);
            glVertex3f(x, y, xa);
        }
        glEnd();
    }

    glBegin(GL_TRIANGLE_STRIP);

    float x = radius * sin(angle * 0);
    float y = -radius * cos(angle * 0);

    glColor3f(0,0.3,0);
    glVertex3f(x,y,xa);
    glColor3f(0,0.3,0.3);
    glVertex3f(x,y,-xa);

    for (int i = 1; i <= steps; i++)
    {
      float newX = radius * sin(angle * i);
      float newY = -radius * cos(angle * i);

      glColor3f(0,0.3,0);
      glVertex3f(newX,newY,xa);
      glColor3f(0,0.3,0.3);
      glVertex3f(newX,newY,-xa);
    }
    glEnd();
}

void Widget::drawStand(){
    //-2.8,2.8,-1, -2.8,-2.8,-1, -2.5,-2.8,-1, -2.5,2.8,-1
    float stand_vert[] = {-3,-3,-1, -2.9,-3,-1, -2.9,3,-1, -3,3,-1,
                         -2.9,3,-1, -3,3,-1, -3,3,1, -2.9,3,1,
                         -3,-3,1, -2.9,-3,1, -2.9,3,1, -3,3,1,
                         -3,-3,-1, -2.9,-3,-1, -2.9,-3,1, -3,-3,1,
                         -2.9,-3,-1, -2.9,-3,1, -2.9,3,1, -2.9,3,-1,
                         -3,-3,-1, -3,-3,1, -3,3,1, -3,3,-1,

                         -2.9,2.9,-1, -2.9,-2.9,-1, -2.7,-2.9,-1, -2.7,2.9,-1,
                          -2.9,2.9,-1, -2.7,2.9,-1, -2.7,2.9,1, -2.9,2.9,1,
                         -2.9,2.9,1, -2.9,-2.9,1, -2.7,-2.9,1, -2.7,2.9,1,
                         -2.9,-2.9,-1, -2.7,-2.9,-1, -2.7,-2.9,1, -2.9,-2.9,1,
                         -2.9,-2.9,-1, -2.7,-2.9,-1, -2.7,-2.9, 1, -2.9,-2.9,1,

                         -2.8,2.5,-1, -2,2.5,-1, -2,-2.5,-1, -2.8,-2.5,-1,
                         -2.8,2.5,1, -2,2.5,1, -2,-2.5,1, -2.8,-2.5,1,
                         -2.8,2.5,-1, -2,2.5,-1, -2,2.5,1, -2.8,2.5,1,
                         -2.8,-2.5,-1, -2,-2.5,-1, -2,-2.5,1, -2.8,-2.5,1,
                         -2,-2.5,-1, -2,-2.5,1, -2,2.5,1, -2,2.5,-1,};

    glVertexPointer(3,GL_FLOAT,0,&stand_vert);
    glEnableClientState(GL_VERTEX_ARRAY);
        glColor3f(0.28,0.28,0.28);
        glDrawArrays(GL_QUADS,0,64);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Widget::drawStandHolder(float a){
    float positionZ = a / 2;
    float holder_arr[] = {-2,2.3,positionZ, -1.8,2.3,positionZ, -1.8,a,positionZ, -2,a,positionZ,
                         -2,2.3,-positionZ, -1.8,2.3,-positionZ, -1.8,a,-positionZ, -2,a,-positionZ,
                          -2,2.3,positionZ, -1.8,2.3,positionZ, -1.8,2.3,-positionZ, -2,2.3,-positionZ,


                         -2,1.5,positionZ, -2,a,positionZ, 0,a,positionZ, 0,1.5,positionZ,
                         -2,1.5,-positionZ, -2,a,-positionZ, 0,a,-positionZ, 0,1.5,-positionZ,
                         0,a,-positionZ, 0,a,positionZ, 0,1.5,positionZ, 0,1.5,-positionZ,
                         -2,1.5,positionZ, 0,1.5,positionZ, 0,1.5,-positionZ, -2,1.5,-positionZ,
                         -1.8,2.3,positionZ, -1.8,2.3,-positionZ, -1.8,1.5,-positionZ, -1.8,1.5,positionZ,
                         };

    glVertexPointer(3,GL_FLOAT,0,&holder_arr);
    glEnableClientState(GL_VERTEX_ARRAY);
        glColor3f(0.5,0.5,0.5);
        glDrawArrays(GL_QUADS,0,32);
    glDisableClientState(GL_VERTEX_ARRAY);
}
