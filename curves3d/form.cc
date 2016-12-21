#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Form)
    , m_modificationEnabled(true)
{
    ui->setupUi(this);

    initCurves();
    buildCurves();
    drawCurves();

    ui->glScene->setTranslationVector(QVector3D(0.0, 0.0, -25.0));
}

Form::~Form()
{
    delete ui;
}

void Form::initCurves()
{
    QVector<QVector3D> basePoints;
    basePoints.append(QVector3D(-11.0f, 11.0f, 0.0f));
    basePoints.append(QVector3D(-8.0f, -6.0f, 4.2f));
    basePoints.append(QVector3D(-2.0f, 9.5f, 7.0f));
    basePoints.append(QVector3D(1.0f, -11.0f, -12.0f));
    basePoints.append(QVector3D(5.5f, 11.0f, -2.0f));
    basePoints.append(QVector3D(9.0f, -5.5f, 7.0f));
    basePoints.append(QVector3D(12.0f, -3.0f, 5.5f));

    for (int i = 0; i < basePoints.size(); ++i) {
        m_bezier.append(basePoints[i]);
        m_bspline.append(basePoints[i]);
        m_nurbs.append(basePoints[i]);

        float randWeight = 0.1f * (5 + qrand() % 11); // w ~ uniform (0.5, 1.5);
        m_nurbs.modifyWeight(i, randWeight);
    }
}

void Form::buildCurves()
{
    m_bezier.build();
    m_bspline.build();
    m_nurbs.build();
}

void Form::drawCurves()
{
    if (ui->radioBezier->isChecked()) {
        ui->glScene->setData(m_bezier.data(), m_bezier.basePoints());
    } else if (ui->radionBspline->isChecked()) {
        ui->glScene->setData(m_bspline.data(), m_bspline.basePoints());
    } else {
        ui->glScene->setData(m_nurbs.data(), m_nurbs.basePoints());
    }
    fillSpinBoxes(ui->sbIndex->value());
}

void Form::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Form::on_radionBspline_toggled(bool checked)
{
    if (checked) {
        drawCurves();
    }
}

void Form::on_radioNurbs_toggled(bool checked)
{
    if (checked) {
        ui->dsbW->setEnabled(true);
        drawCurves();
    } else {
        ui->dsbW->setEnabled(false);
    }
}

void Form::on_radioBezier_toggled(bool checked)
{
    if (checked) {
        drawCurves();
    }
}

void Form::on_cbDrawPoints_toggled(bool checked)
{
    ui->glScene->enableBasePoints(checked);
}

void Form::on_cbDrawLines_toggled(bool checked)
{
    ui->glScene->enableLines(checked);
}

void Form::on_btnLeft_clicked()
{
    int currentIndex = ui->sbIndex->value() - 1;

    if (currentIndex < 0) {
        currentIndex = m_bezier.basePoints().size() - 1;
    }

    ui->glScene->setCurrentBasePoint(currentIndex);
    fillSpinBoxes(currentIndex);
}

void Form::on_btnRight_clicked()
{
    int currentIndex = ui->sbIndex->value() + 1;

    if (currentIndex > m_bezier.basePoints().size() - 1) {
        currentIndex = 0;
    }

    ui->glScene->setCurrentBasePoint(currentIndex);
    fillSpinBoxes(currentIndex);
}

void Form::fillSpinBoxes(int currentIndex)
{
    m_modificationEnabled = false;

    double x = double(m_bezier.basePoints()[currentIndex].x());
    double y = double(m_bezier.basePoints()[currentIndex].y());
    double z = double(m_bezier.basePoints()[currentIndex].z());
    double w = 1.0;

    if (ui->radioNurbs->isEnabled()) {
        w = double(m_nurbs.weights()[currentIndex]);
    }

    ui->dsbX->setValue(x);
    ui->dsbY->setValue(y);
    ui->dsbZ->setValue(z);
    ui->dsbW->setValue(w);

    ui->sbIndex->setValue(currentIndex);

    m_modificationEnabled = true;
}

void Form::modifyPoint(char coord, double value)
{
    if (m_modificationEnabled == false) {
        return;
    }

    int       index = ui->sbIndex->value();
    QVector3D point = m_bezier.basePoints()[index];

    if (coord == 'x' || coord == 'X') {
        point.setX(float(value));
    } else if (coord == 'y' || coord == 'Y') {
        point.setY(float(value));
    } else {
        point.setZ(float(value));
    }

    m_bezier.modify(index, point);
    m_bspline.modify(index, point);
    m_nurbs.modify(index, point);

    buildCurves();
    drawCurves();
}

void Form::on_dsbX_valueChanged(double arg1)
{
    modifyPoint('x', arg1);
}

void Form::on_dsbY_valueChanged(double arg1)
{
    modifyPoint('y', arg1);
}

void Form::on_dsbZ_valueChanged(double arg1)
{
    modifyPoint('z', arg1);
}

void Form::on_dsbW_valueChanged(double arg1)
{
    if (m_modificationEnabled == false) {
        return;
    }

    m_nurbs.modifyWeight(ui->sbIndex->value(), float(arg1));

    buildCurves();
    drawCurves();
}

void Form::on_doubleSpinBox_5_valueChanged(double arg1)
{
    float step = float(arg1);

    m_bezier.setApproximationStep(step);
    m_bspline.setApproximationStep(step);
    m_nurbs.setApproximationStep(step);

    buildCurves();
    drawCurves();
}
