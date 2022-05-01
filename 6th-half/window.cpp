#include "window.h"
#include "method.h"
#include <QPainter>
#include <QtCore>
#include <QtGui>
#include <QtOpenGL>
#include <cstdio>
#include <math.h>

#define DEFAULT_A -1
#define DEFAULT_B 1
#define DEFAULT_C -1
#define DEFAULT_D 1
#define DEFAULT_N 40
#define DEFAULT_M 40
#define DEFAULT_ID 5

int WIDTH, HEIGHT;

static double f_0(double x, double y)
{
    return 1;
}
static double f_1(double x, double y)
{
    return x;
}
static double f_2(double x, double y)
{
    return y;
}
static double f_3(double x, double y)
{
    return x + y;
}
static double f_4(double x, double y)
{
    return sqrt(x * x + y * y);
}
static double f_5(double x, double y)
{
    return x * x + y * y;
}
static double f_6(double x, double y)
{
    return exp(x * x - y * y);
}
static double f_7(double x, double y)
{
    return 1 / (25 * (x * x + y * y) + 1);
}

window::window(QWidget *parent) : QGLWidget(parent)
{
    xRot = -90;
    yRot = zRot = zTra = 0;
    nSca = 1;

    a = DEFAULT_A;
    b = DEFAULT_B;
    c = DEFAULT_C;
    d = DEFAULT_D;
    n = DEFAULT_N;
    m = DEFAULT_M;
    Gmax = 0;
    max = 0;
    mode = 0;
    s = 0;
    res = 0;
    func_id = DEFAULT_ID;
    pribl = 0;
    change_func();
}

window::~window()
{
    free(x);
    free(y);
    free(Pf_ij);
    free(Pf);
    free(f_xy);
}

int window::prepare()
{
    double hx, hy;
    x = (double *)malloc(n * sizeof(double));
    y = (double *)malloc(m * sizeof(double));
    f_xy = (double *)malloc(m * n * sizeof(double));
    Pf_ij = (double *)malloc(16 * sizeof(double));
    Pf = (double *)malloc(16 * (n - 1) * (m - 1) * sizeof(double));
    x_left = a;
    x_right = b;
    y_left = c;
    y_right = d;
    max = f(a, c);
    hx = (b - a) / (n - 1);
    hy = (d - c) / (m - 1);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            x[i] = a + i * hx;
            y[j] = c + j * hy;
            f_xy[i * m + j] = f(x[i], y[j]);
            if (max < fabs(f_xy[i * m + j]))
                max = fabs(f_xy[i * m + j]);
        }
    }
    f_xy[(n / 2) * m + (m / 2)] += pribl * 0.1 * max;
    return 0;
}

int window::parse_command_line(int argc, char *argv[])
{
    if (argc == 1)
    {
        return 0;
    }

    if (argc == 2)
    {
        return -1;
    }

    if (sscanf(argv[1], "%lf", &a) != 1 || sscanf(argv[2], "%lf", &b) != 1 || b - a < 1.e-6 || sscanf(argv[3], "%lf", &c) != 1 || sscanf(argv[4], "%lf", &d) != 1 || sscanf(argv[5], "%d", &n) != 1 || sscanf(argv[6], "%d", &m) != 1 || (argc > 7 && sscanf(argv[7], "%i", &func_id) != 1) || n <= 3 || m <= 3)
    {
        return -2;
    }

    func_id -= 1;
    change_func();
    return 0;
}
void window::increase_n()
{
    n *= 2;
}
void window::increase_m()
{
    m *= 2;
}
void window::decrease_n()
{
    if (n >= 6)
        n /= 2;
}
void window::decrease_m()
{
    if (m >= 6)
        m /= 2;
}
void window::increase_p()
{
    pribl += 1;
}

void window::decrease_p()
{
    pribl -= 1;
}

void window::change_func()
{
    func_id = (func_id + 1) % 8;
    switch (func_id)
    {
    case 0:
        f_name = "f(x, y) = 1";
        f = f_0;
        break;
    case 1:
        f_name = "f(x, y) = x";
        f = f_1;
        break;
    case 2:
        f_name = "f(x, y) = y";
        f = f_2;
        break;
    case 3:
        f_name = "f(x, y) = x + y";
        f = f_3;
        break;
    case 4:
        f_name = "f(x, y) = sqrt(x * x + y * y)";
        f = f_4;
        break;
    case 5:
        f_name = "f(x, y) = x * x + y * y";
        f = f_5;
        break;
    case 6:
        f_name = "f(x, y) = exp(x * x - y * y)";
        f = f_6;
        break;
    case 7:
        f_name = "f(x, y) = 1/(25*(x * x + y * y)+1)";
        f = f_7;
        break;
    }
}

void window::change_mode()
{
    mode = (mode + 1) % 2;
}

double window::compute(double t_x, double t_y)
{
    return method_compute(n, m, t_x, t_y, x, y, Pf);
}

void window::initializeGL()
{
    qglClearColor(Qt::white);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
}

void window::resizeGL(int nWidth, int nHeight)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    WIDTH = nWidth;
    HEIGHT = nHeight;

    GLfloat ratio = (GLfloat)nHeight / (GLfloat)nWidth;
    if (nWidth >= nHeight)
        glOrtho(-1.0 / ratio, 1.0 / ratio, -1.0, 1.0, -100.0, 100.0);
    else
        glOrtho(-1.0, 1.0, -1.0 * ratio, 1.0 * ratio, -100.0, 100.0);
    glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
}

void window::paintGL()
{
    double x1, x2, y1, y2, z1;
    double t1, t2, t3, t4;
    double delta_x, delta_y, delta_z;
    double special;
    double hx, hy;
    char text[80];

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    prepare();
    delta_x = (x_right - x_left) / 20;
    delta_y = (y_right - y_left) / 20;
    hx = (b - a) / 100;
    hy = (d - c) / 100;
    special = f(x[n / 2], y[m / 2]) + pribl * 0.1 * max;

    QFont shr("Fira Code", 8, QFont::DemiBold);
    QFont big_shr("Fira Code", 12, QFont::ExtraBold);
    glColor3f(0, 0, 0);
    renderText(-1.9, 0.95, 0, "ШОРТКАТЫ", shr);
    renderText(-1.9, 0.9, 0, "0: сменить функцию", shr);
    renderText(-1.9, 0.85, 0, "1: сменить mode отображения", shr);
    renderText(-1.84, 0.8, 0, "(0: график + апроксимация, 1: погрешность)", shr);
    renderText(-1.9, 0.75, 0, "2/3: приблизить/отдалить", shr);
    renderText(-1.9, 0.7, 0, "4/5: увеличить/уменьшить n (число точек по оси x)", shr);
    renderText(-1.9, 0.65, 0, "6/7: увеличить/уменьшить m (число точек по оси y)", shr);
    renderText(-1.9, 0.6, 0, "8/9: увеличить/уменьшить p (возмущения)", shr);
    renderText(-1.9, 0.55, 0, "стрелки: вращать график", shr);
    renderText(-1.9, 0.5, 0, "Space: изначальная позиция", shr);
    renderText(-1.9, 0.45, 0, "Esc: выход", shr);
    renderText(-1.9, 0.4, 0, "(мышкой можно вращать и приближать)", shr);
    renderText(1, 0.95, 0, "ИНФОРМАЦИЯ", shr);

    PF(n, m, x, y, f_xy, Pf_ij, Pf); // вычисление метода

    glLineWidth(1.0);
    if (mode == 0)
    {
        max_z = f(x_left, y_left);
        min_z = f(x_left, y_left);
        for (x1 = x_left; x1 < x_right + 1e-6; x1 += delta_x)
        {
            for (y1 = y_left; y1 < y_right + 1e-6; y1 += delta_y)
            {
                z1 = f(x1, y1);
                if (equal(x1, x[n / 2]) && equal(y1, y[m / 2]))
                    z1 = special;
                if (z1 > max_z)
                    max_z = z1;
                if (z1 < min_z)
                    min_z = z1;
            }
        }
        delta_z = 0.01 * (max_z - min_z) + 0.01; // без добавления 0.01 не отрисовывается график = 1
        min_z -= delta_z;
        max_z += delta_z;

        sprintf(text, "Метод 20");
        renderText(1, 0.9, 0, text, shr);
        sprintf(text, "k = %d: %s", func_id, f_name);
        renderText(1, 0.85, 0, text, shr);
        sprintf(text, "mode = %d", mode);
        renderText(1, 0.8, 0, text, shr);
        sprintf(text, "n = %i  m = %i p = %i", n, m, pribl);
        renderText(1, 0.75, 0, text, shr);
        sprintf(text, "scale: %i0%% ", s);
        renderText(1, 0.7, 0, text, shr);

        glColor3f(0.3, 0.5, 1.0);
        renderText(1, 0.65, 0, "изначальная функция", shr);
        glColor3f(1, 0.4, 0.3);
        renderText(1, 0.6, 0, "аппроксимированная функция", shr);

        glScalef(nSca / (x_right - x_left), nSca / (y_right - y_left), nSca / (max_z - min_z));
        glTranslatef(0.0f, zTra, 0.0f);
        glRotatef(xRot, 1.0f, 0.0f, 0.0f);
        glRotatef(yRot, 0.0f, 1.0f, 0.0f);
        glRotatef(zRot, 0.0f, 0.0f, 1.0f);

        // изначальный график
        x1 = x_left;
        y1 = y_left;
        for (x2 = x1 + delta_x; x2 - x_right < 1e-6; x2 += delta_x)
        {
            for (y2 = y1 + delta_y; y2 - y_right < 1e-6; y2 += delta_y)
            {
                glColor4f(0.3, 0.5, 1.0, 0.3);
                if (equal(x1, x[n / 2]) && equal(y1, y[m / 2]))
                {
                    t1 = special;
                }
                else
                {
                    t1 = f(x1, y1);
                }
                if (equal(x1, x[n / 2]) && equal(y2, y[m / 2]))
                {
                    t2 = special;
                }
                else
                {
                    t2 = f(x1, y2);
                }
                if (equal(x2, x[n / 2]) && equal(y2, y[m / 2]))
                {
                    t3 = special;
                }
                else
                {
                    t3 = f(x2, y2);
                }
                if (equal(x2, x[n / 2]) && equal(y1, y[m / 2]))
                    t4 = special;
                else
                    t4 = f(x2, y1);
                glBegin(GL_LINE_STRIP);
                glVertex3f(x1, y1, t1);
                glVertex3f(x1, y2, t2);
                glVertex3f(x2, y2, t3);
                glVertex3f(x1, y1, t1);
                glVertex3f(x2, y1, t4);
                glVertex3f(x2, y2, t3);
                glEnd();
                y1 = y2;
            }
            y1 = y_left;
            x1 = x2;
        }

        // аппроксимированная график
        x1 = x_left;
        y1 = y_left;
        for (x2 = x1 + delta_x; x2 - x_right < 1.e-6; x2 += delta_x)
        {
            for (y2 = y_left + delta_y; y2 - y_right < 1e-6; y2 += delta_y)
            {
                glColor4f(1, 0.4, 0.3, 0.3);
                glBegin(GL_LINE_STRIP);
                glVertex3f(x1, y1, compute(x1, y1));
                glVertex3f(x1, y2, compute(x1, y2));
                glVertex3f(x2, y2, compute(x2, y2));
                glVertex3f(x1, y1, compute(x1, y1));
                glVertex3f(x2, y1, compute(x2, y1));
                glVertex3f(x2, y2, compute(x2, y2));
                glEnd();
                y1 = y2;
            }
            y1 = y_left;
            x1 = x2;
        }
    }
    else if (mode == 1)
    {
        Gmax = 0;
        min_z = max_z = f(a, c) - compute(a, c);
        for (x1 = a; x1 < b; x1 += hx)
        {
            for (y1 = c; y1 < d; y1 += hy)
            {
                z1 = f(x1, y1) - compute(x1, y1);
                if (equal(x1, x[n / 2]) && equal(y1, y[m / 2]))
                    z1 = special - compute(x1, y1);
                if (z1 > max_z)
                    max_z = z1;
                if (z1 < min_z)
                    min_z = z1;
            }
        }
        if (fabs(max_z) > fabs(min_z))
        {
            Gmax = max_z;
        }
        else
        {
            Gmax = min_z;
        }
        delta_z = 0.01 * (max_z - min_z) + 1;
        min_z -= delta_z;
        max_z += delta_z;

        sprintf(text, "Метод 20, погрешность");
        renderText(1, 0.9, 0, text, shr);
        sprintf(text, "k = %d: %s", func_id, f_name);
        renderText(1, 0.85, 0, text, shr);
        sprintf(text, "mode = %d", mode);
        renderText(1, 0.8, 0, text, shr);
        sprintf(text, "n = %i  m = %i p = %i", n, m, pribl);
        renderText(1, 0.75, 0, text, shr);
        sprintf(text, "scale: %i0%% ", s);
        renderText(1, 0.7, 0, text, shr);
        glColor3f(0.3, 0.5, 1.0);
        renderText(1, 0.65, 0, "изначальная функция", shr);

        glScalef(nSca / (x_right - x_left), nSca / (y_right - y_left), nSca / (max_z - min_z));
        glTranslatef(0.0f, zTra, 0.0f);
        glRotatef(xRot, 1.0f, 0.0f, 0.0f);
        glRotatef(yRot, 0.0f, 1.0f, 0.0f);
        glRotatef(zRot, 0.0f, 0.0f, 1.0f);

        // график погрешностей
        x1 = x_left;
        y1 = y_left;
        for (x2 = x1 + delta_x; x2 - x_right < 1.e-6; x2 += delta_x)
        {
            for (y2 = y1 + delta_y; y2 - y_right < 1.e-6; y2 += delta_y)
            {
                if (equal(x1, x[n / 2]) && equal(y1, y[m / 2]))
                    t1 = special - compute(x1, y1);
                else
                    t1 = f(x1, y1) - compute(x1, y1);
                if (equal(x1, x[n / 2]) && equal(y2, y[m / 2]))
                    t2 = special - compute(x1, y2);
                else
                    t2 = f(x1, y2) - compute(x1, y2);
                if (equal(x2, x[n / 2]) && equal(y2, y[m / 2]))
                    t3 = special - compute(x2, y2);
                else
                    t3 = f(x2, y2) - compute(x2, y2);
                if (equal(x2, x[n / 2]) && equal(y1, y[m / 2]))
                    t4 = special - compute(x2, y1);
                else
                    t4 = f(x2, y1) - compute(x2, y1);
                glBegin(GL_LINE_STRIP);
                glColor4f(0.3, 0.5, 1.0, 0.3);
                glVertex3f(x1, y1, t1);
                glVertex3f(x1, y2, t2);
                glVertex3f(x2, y2, t3);
                glVertex3f(x1, y1, t1);
                glVertex3f(x2, y1, t4);
                glVertex3f(x2, y2, t3);
                glEnd();
                y1 = y2;
            }
            y1 = y_left;
            x1 = x2;
        }
    }

    drawAxis();
}

void window::mousePressEvent(QMouseEvent *pe)
{
    ptrMousePosition = pe->pos();
}

void window::mouseMoveEvent(QMouseEvent *pe)
{
    xRot += 180 / nSca * (GLfloat)(pe->y() - ptrMousePosition.y()) / height();
    zRot += 180 / nSca * (GLfloat)(pe->x() - ptrMousePosition.x()) / width();
    ptrMousePosition = pe->pos();
    updateGL();
}

void window::wheelEvent(QWheelEvent *pe)
{
    if (pe->delta() > 0)
        scale_plus();
    else if (pe->delta() < 0)
        scale_minus();
    updateGL();
}

void window::keyPressEvent(QKeyEvent *pe)
{
    switch (pe->key())
    {
    case Qt::Key_0:
        change_func();
        break;
    case Qt::Key_1:
        change_mode();
        break;
    case Qt::Key_2:
        scale_plus();
        break;
    case Qt::Key_3:
        scale_minus();
        break;
    case Qt::Key_4:
        increase_n();
        break;
    case Qt::Key_5:
        decrease_n();
        break;
    case Qt::Key_6:
        increase_m();
        break;
    case Qt::Key_7:
        decrease_m();
        break;
    case Qt::Key_8:
        increase_p();
        break;
    case Qt::Key_9:
        decrease_p();
        break;
    case Qt::Key_Up:
        rotate_up();
        break;
    case Qt::Key_Down:
        rotate_down();
        break;
    case Qt::Key_Left:
        rotate_left();
        break;
    case Qt::Key_Right:
        rotate_right();
        break;
    case Qt::Key_Escape:
        this->close();
        break;
    case Qt::Key_Space:
        defaultScene();
        break;
    }
    updateGL();
}
void window::scale_plus()
{
    nSca *= 1.1;
    s += 1;
}
void window::scale_minus()
{
    nSca /= 1.1;
    s -= 1;
}
void window::rotate_up()
{
    xRot += 1.0;
}
void window::rotate_down()
{
    xRot -= 1.0;
}
void window::rotate_left()
{
    zRot += 1.0;
}
void window::rotate_right()
{
    zRot -= 1.0;
}
void window::translate_up()
{
    zTra += 0.05;
}
void window::translate_down()
{
    zTra -= 0.05;
}
void window::defaultScene()
{
    xRot = -90;
    yRot = 0;
    zRot = 0;
    zTra = 0;
    nSca = 1;
    s = 0;
}

void window::drawAxis()
{
    QFont big_shr("Fira Code", 8, QFont::ExtraBold);
    glLineWidth(3.0);

    glColor4f(0.0, 0.1, 0.0, 1.0);
    renderText(x_right + 1, 0.0, 0.0, "X", big_shr);
    glBegin(GL_LINES);
    glVertex3f(x_left - 1, 0.0, 0.0);
    glVertex3f(x_right + 1, 0.0, 0.0);
    glEnd();

    glColor4f(0.0, 0.0, 1.0, 1.0);
    renderText(0.0, y_right + 1, 0.0, "Y", big_shr);
    glBegin(GL_LINES);
    glVertex3f(0.0, y_left - 1, 0.0);
    glVertex3f(0.0, y_right + 1, 0.0);
    glEnd();

    glColor4f(1.0, 0.0, 0.0, 1.0);
    renderText(0.0, 0.0, max_z + 1, "Z", big_shr);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, max_z + 1);
    glVertex3f(0.0, 0.0, min_z - 1);
    glEnd();
}
