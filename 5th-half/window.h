
#ifndef WINDOW_H
#define WINDOW_H
#include <QtWidgets/QtWidgets>

class Window : public QWidget
{
  Q_OBJECT

private:
  int func_id;
  int method_id = 0;
  const char *f_name;
  double a;
  double b;
  int n;
  int scale = 0;
  int delta = 0;
  double (*f) (double);

public:
  Window (QWidget *parent);

  QSize minimumSizeHint () const;
  QSize sizeHint () const;

  int parse_command_line (int argc, char *argv[]);

public slots:
  void change_func ();
  void change_method();
  void scale_plus();
  void scale_minus();
  void points_plus();
  void points_minus();
  void delta_plus();
  void delta_minus();
  

protected:
  void paintEvent (QPaintEvent *event);
};

#endif
