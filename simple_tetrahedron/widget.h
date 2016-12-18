#ifndef WIDGET_H
#define WIDGET_H

#include <cmath>

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>

#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>

#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTime>
#include <QTimer>
#include <QWheelEvent>


class Widget : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Widget(QOpenGLWidget *parent = nullptr);
    ~Widget();


private:
    void initializeGL() override;
    void paintGL() override;

    void measureFps();
    void init();
    void draw(QOpenGLShaderProgram *shaderProgram);

    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;


private:
    QOpenGLShaderProgram m_shaderProgram;

    QTimer m_updateTimer;

    QPointF m_mousePosition;
    float   m_rotationAngleAroundVerticalAxis;
    float   m_rotationAngleAroundLateralAxis;

    float       m_distance;
    const float m_minDistance;
    const float m_maxDistance;

    GLuint m_indexOfCoordinates;
    GLuint m_indexOfColors;
    GLuint m_indexOfVerticesVbo;
    GLuint m_indexOfColorsVbo;
    GLuint m_indexOfFacesIbo;
};


#endif // WIDGET_H
