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
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
}

void Widget::resizeGL(int w, int h)
{
    int k = w / h;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-k,k, -1,1, 1,7);

}

void Widget::paintGL()
{
    glClearColor(0.95,0.95,0.95,0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0,0,-2.5);
    trigger_change(true);
    glScalef(0.2,0.2,0.2);
    glRotatef(90,0,1,0);

    glPushMatrix(); //Освещение
        glRotatef(zoomScale,0,1,0);
        qDebug() << zoomScale;
        float position[] = {0,0,2,0};
        glLightfv(GL_LIGHT0,GL_POSITION,position);

        glTranslatef(0,0,2);
        glScalef(0.5,0.5,0.5);
        //drawCircle(2);
    glPopMatrix();

    glPushMatrix(); // Стенд
        glTranslatef(0,0,-3); // Перемещение к левому краю
        drawStand();
    glPopMatrix();

    glPushMatrix(); // Объект
        glTranslatef(0,0,-0.5);// Отделение стойки от фигуры
        drawCylinder(2,1,color);
    glPopMatrix();
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
        zoomScale += 10.0f;
    }
    else if(wheel.y() < 0){
        zoomScale -= 10.0f;
    }
    update();
}

void Widget::drawParallelepiped(float l, float b, float zi) //высота, ширина, глубина
{
    float vertex[] = {-1, -1, b,  zi, -1, b,  zi, l, b,  -1, l, b, // передняя
            -1, -1, -b,  zi, -1, -b,  zi, l, -b,  -1, l, -b, //задняя
            -1, -1, -b,  -1, -1, b,  -1, l, b,   -1, l, -b, //левая
             zi, -1, b,  zi, -1, -b,   zi, l, -b,  zi, l, b, //правая
             -1, -1, b,  -1, -1, -b,  zi, -1, -b,  zi, -1, b, //нижняя
             -1, l, b,  zi, l, b,  zi, l, -b,  -1, l, -b};//верхняя

    float normals[] = {0,0,1, 0,0,1, 0,0,1, 0,0,1,
                       0,0,-1, 0,0,-1, 0,0,-1, 0,0,-1,
                      -1,0,0, -1,0,0, -1,0,0, -1,0,0,
                      1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0,
                      0,-1,0, 0,-1,0, 0,-1,0, 0,-1,0,
                      0,1,0, 0,1,0, 0,1,0, 0,1,0,};

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &vertex);

        glColor3fv(color);
        glNormalPointer(GL_FLOAT, 0, &normals);
        glDrawArrays(GL_QUADS, 0, 26);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void Widget::drawPipe(float diameter, float xa, float xi){

        float radius = diameter / 2;
        const float steps = 40;
        const float angle = 3.1415926 * 2.f / steps;
        float normals[] = {0,1,0, 0,0,1, 0,1,0, 0,1,1};

        glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_FLOAT, 0, &normals);

        if(xi == 0)
        {
            glBegin(GL_TRIANGLE_STRIP);

            float x = radius * sin(angle * 0);
            float y = -radius * cos(angle * 0);

            glColor3fv(color);
            glVertex3f(x,y,xa);
            glVertex3f(x,y,-xa);

            for (int i = 1; i <= steps; i++)
            {
              float newX = radius * sin(angle * i);
              float newY = -radius * cos(angle * i);

              glColor3fv(color);
              glVertex3f(newX,newY,xa);
              glVertex3f(newX,newY,-xa);
            }
            glEnd();
        }

        else
        {
            for(int j = 0; j < 2; ++j)
            {
                if(j == 1)
                    radius += xi;

                glBegin(GL_TRIANGLE_STRIP);

                float x = radius * sin(angle * 0);
                float y = -radius * cos(angle * 0);

                glColor3fv(color);
                glVertex3f(x,y,xa);
                glVertex3f(x,y,-xa);

                for (int i = 1; i <= steps; i++)
                {
                  float newX = radius * sin(angle * i);
                  float newY = -radius * cos(angle * i);

                  glColor3fv(color);
                  glVertex3f(newX,newY,xa);
                  glVertex3f(newX,newY,-xa);
                }
                glEnd();

            }

            radius -= xi;
            for (int i = 0; i < 2; ++i)
            {
                if(i == 1)
                    xa *= -1;

                glBegin(GL_TRIANGLE_STRIP);
                float mainX = radius * sin(angle * 0);
                float upY = -radius * cos(angle * 0);
                float downY = -(radius + xi) * cos(angle * 0);

                glColor3fv(color);
                glVertex3f(mainX,downY,xa);
                glVertex3f(mainX,upY,xa);

                for (int j = 1; j <= steps; ++j)
                {
                   float upX = radius * sin(angle * j);
                   float upY = -radius * cos(angle * j);

                   float downX = (radius + xi) * sin(angle * j);
                   float downY = -(radius + xi) * cos(angle * j);

                   glColor3fv(color);
                   glVertex3f(downX,downY,xa);
                   glVertex3f(upX,upY,xa);
                }
                glEnd();
            }
        }
        glDisableClientState(GL_NORMAL_ARRAY);
}

void Widget::drawCircle(float radius){

    const float steps = 40;
    const float angle = 3.1415926 * 2.f / steps;

    glBegin(GL_TRIANGLE_FAN);
    glColor3fv(color);
    glVertex3f(0.0,0.0,0.0);

    for (int i = 0; i <= steps; i++)
    {
        float x = radius * sin(angle * i);
        float y = -radius * cos(angle * i);
        glVertex3f(x, y, 0);
    }
    glEnd();
}

void Widget::drawPolyhedron(float x, float y, float z)
{
    float arr[18] = {-x,0,z, -x/2,y,z, -x/2,-y,z, x/2,y,z, x/2,-y,z, x,0,z}; // верхняя
    float arr_second[18] = {-x,0,-z, -x/2,y,-z, -x/2,-y,-z, x/2,y,-z, x/2,-y,-z, x,0,-z,}; // нижняя
    float arr_general[42] = {-x,0,z, -x,0,-z, -x/2,-y,z, -x/2,-y,-z, x/2,-y,z, x/2,-y,-z,
                           x,0,z, x,0,-z, x/2,y,z, x/2,y,-z, -x/2,y,z, -x/2,y,-z, -x,0,z, -x,0,-z}; //общая

    float normals[] = {0,0,1, 0,0,1, 0,0,0, 0,0,1};

    glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, &normals);

   glVertexPointer(3, GL_FLOAT, 0, &arr);
   glEnableClientState(GL_VERTEX_ARRAY);
    glColor3fv(color);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
   glDisableClientState(GL_VERTEX_ARRAY);

   glVertexPointer(3, GL_FLOAT, 0, &arr_second);
   glEnableClientState(GL_VERTEX_ARRAY);
    glColor3fv(color);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
   glDisableClientState(GL_VERTEX_ARRAY);

   glVertexPointer(3, GL_FLOAT, 0, &arr_general);
   glEnableClientState(GL_VERTEX_ARRAY);
    glColor3fv(color);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);
   glDisableClientState(GL_VERTEX_ARRAY);
   glDisableClientState(GL_NORMAL_ARRAY);
}

void Widget::drawCylinder(float diameter, float xa, float color[3]){

    const float radius = diameter / 2;
    const float steps = 40;
    const float angle = 3.1415926 * 2.f / steps;

    for(int i = 0; i < 2; ++i)
    {
        if(i == 1)
        {
            xa *= -1;
        }
        glBegin(GL_TRIANGLE_FAN);
        glColor3fv(color);
        glVertex3f(0,0,xa);
        glNormal3f(0,0,xa);

        for (int i = 0; i <= steps; i++)
        {
            float x = radius * sin(angle * i);
            float y = -radius * cos(angle * i);
            glVertex3f(x, y, xa);
            glNormal3f(x, y, xa);
        }
        glEnd();
    }

    glBegin(GL_TRIANGLE_STRIP);

    float x = radius * sin(angle * 0);
    float y = -radius * cos(angle * 0);

    glColor3fv(color);

    glVertex3f(x,y,xa);
    glNormal3f(x,y,xa);

    glVertex3f(x,y,-xa);
    glNormal3f(x,y,-xa);


    for (int i = 1; i <= steps; i++)
    {
      float newX = radius * sin(angle * i);
      float newY = -radius * cos(angle * i);

      glColor3fv(color);
      glVertex3f(newX,newY,xa);
      glNormal3f(newX,newY,xa);

      glVertex3f(newX,newY,-xa);
      glNormal3f(newX,newY,-xa);
    }
    glEnd();
}

void Widget::drawStand(){

    drawCylinder(5,0.21,color_first);
    glTranslatef(0,0,0.2);
    drawCylinder(4.8,0.2,color_second);
    glTranslatef(0,0,0.2);
    drawCylinder(4,0.6,color_third);

}

void Widget::drawStandHolder(float a){
    float z = a / 2;
    float holder_arr[] = {-2,2.3,z, -1.8,2.3,z, -1.8,a,z, -2,a,z,
                         -2,2.3,-z, -1.8,2.3,-z, -1.8,a,-z, -2,a,-z,
                          -2,2.3,z, -1.8,2.3,z, -1.8,2.3,-z, -2,2.3,-z,


                         -2,1.5,z, -2,a,z, 0,a,z, 0,1.5,z,
                         -2,1.5,-z, -2,a,-z, 0,a,-z, 0,1.5,-z,
                         0,a,-z, 0,a,z, 0,1.5,z, 0,1.5,-z,
                         -2,1.5,z, 0,1.5,z, 0,1.5,-z, -2,1.5,-z,
                         -1.8,2.3,z, -1.8,2.3,-z, -1.8,1.5,-z, -1.8,1.5,z,
                         };

    glVertexPointer(3,GL_FLOAT,0,&holder_arr);
    glEnableClientState(GL_VERTEX_ARRAY);
        glColor3f(0.5,0.5,0.5);
        glDrawArrays(GL_QUADS,0,32);
        glDisableClientState(GL_VERTEX_ARRAY);
}

void Widget::setColor(int r, int g, int b)
{
    color[0] = r;
    color[1] = g;
    color[2] = b;
}

void Widget::trigger_change(bool trigger)
{
    if (trigger)
    {
        glRotatef(11,1,0,0);
        glRotatef(-22,0,1,0);

    }
}
