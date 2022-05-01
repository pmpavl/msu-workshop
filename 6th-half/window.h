#ifndef WINDOW_H
#define WINDOW_H
#include <QGLWidget>
#include <QtOpenGL>

class window : public QGLWidget
{
private:
    GLfloat xRot;
    GLfloat yRot;
    GLfloat zRot;
    GLfloat zTra;
    GLfloat nSca;
    QPoint ptrMousePosition;

    void scale_plus();
    void scale_minus();
    void rotate_up();
    void rotate_down();
    void rotate_left();
    void rotate_right();
    void translate_down();
    void translate_up();
    void defaultScene();
    void drawAxis();

    const char *f_name;
    double x_left, x_right, y_left, y_right;
    double max_z, min_z;
    double *x, *y, *f_xy, *Pf_ij, *Pf;
    int res, mode, pribl;
    double max;
    double (*f)(double x, double y);

protected:
    void initializeGL();
    void resizeGL(int nWidth, int nHeight);
    void paintGL();
    void mousePressEvent(QMouseEvent *pe);
    void mouseMoveEvent(QMouseEvent *pe);
    void wheelEvent(QWheelEvent *pe);
    void keyPressEvent(QKeyEvent *pe);

public:
    window(QWidget *parent = 0);
    ~window();
    int prepare();
    void start_22();
    double compute(double t_x, double t_y);
    int parse_command_line(int argc, char *argv[]);
    double a, b, c, d;
    int n, m, s;
    int func_id;
    double Gmax;

    void change_func();
    void change_mode();
    void increase_n();
    void decrease_n();
    void increase_m();
    void decrease_m();
    void increase_p();
    void decrease_p();
};

#endif
