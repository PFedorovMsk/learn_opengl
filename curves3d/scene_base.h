#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include <QColor>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>


class SceneBase : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    SceneBase(QWidget *parent = nullptr);
    virtual ~SceneBase();


    // setters:

    void setXRotAngle(float angle);
    void setYRotAngle(float angle);
    void setZRotAngle(float angle);

    void setProjectionVerticalAngle(float angle);

    void setClearColor(const QColor &color);

    void setTranslationVector(const QVector3D &vec);
    void setEyeVector(const QVector3D &vec);
    void setCenterVector(const QVector3D &vec);
    void setUpVector(const QVector3D &vec);


    // getters:

    float xRotAngle() const;
    float yRotAngle() const;
    float zRotAngle() const;

    float projectionVerticalAngle() const;

    const QColor &clearColor() const;

    const QVector3D &translationVector() const;
    const QVector3D &eyeVector() const;
    const QVector3D &centerVector() const;
    const QVector3D &upVector() const;


protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    const QMatrix4x4 &mvpMatrix();

    virtual void init()  = 0;
    virtual void paint() = 0;


private:
    float normalizeAngle(float angle);


private:
    float m_xRotAngle;
    float m_yRotAngle;
    float m_zRotAngle;

    // for projection:
    float       m_projectionVerticalAngle;
    const float m_minProjectionVerticalAngle;
    const float m_maxProjectionVerticalAngle;

    QColor m_clearColor;

    // for model:
    QVector3D m_translationVector;

    // for view:
    QVector3D m_eyeVector;
    QVector3D m_centerVector;
    QVector3D m_upVector;

    QMatrix4x4 m_mvpMatrix;

    QPoint m_mousePrevPos;
};

#endif // SCENE_BASE_H
