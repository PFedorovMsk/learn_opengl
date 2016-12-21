#ifndef FORM_H
#define FORM_H

#include "bezier.h"
#include "bspline.h"
#include "nurbs.h"
#include <QWidget>


namespace Ui
{
class Form;
}


class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();


protected:
    void changeEvent(QEvent *e);


private slots:
    void on_radionBspline_toggled(bool checked);
    void on_radioNurbs_toggled(bool checked);
    void on_radioBezier_toggled(bool checked);
    void on_cbDrawPoints_toggled(bool checked);
    void on_cbDrawLines_toggled(bool checked);
    void on_btnLeft_clicked();
    void on_btnRight_clicked();
    void on_dsbX_valueChanged(double arg1);
    void on_dsbY_valueChanged(double arg1);
    void on_dsbZ_valueChanged(double arg1);
    void on_dsbW_valueChanged(double arg1);
    void on_doubleSpinBox_5_valueChanged(double arg1);


private:
    void initCurves();
    void buildCurves();
    void drawCurves();
    void modifyPoint(char coord, double value);
    void fillSpinBoxes(int currentIndex = false);


private:
    Ui::Form *ui;

    Bezier  m_bezier;
    BSpline m_bspline;
    Nurbs   m_nurbs;

    bool m_modificationEnabled;
};

#endif // FORM_H
