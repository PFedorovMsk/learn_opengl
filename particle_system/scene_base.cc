#include "scene_base.h"


SceneBase::SceneBase(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_xRotAngle(0.0f)
    , m_yRotAngle(0.0f)
    , m_zRotAngle(0.0f)
    , m_projectionVerticalAngle(60.0f)
    , m_minProjectionVerticalAngle(1.0f)
    , m_maxProjectionVerticalAngle(180.0f)
    , m_clearColor(QColor::fromRgbF(0.175, 0.35, 0.35, 1.0))
    , m_translationVector(0.0f, 0.0f, 1.0f)
    , m_eyeVector(0.0f, 0.0f, 3.0f)
    , m_centerVector(0.0f, 0.0f, 0.0f)
    , m_upVector(0.0f, 1.0f, 0.0f)
{
}

SceneBase::~SceneBase()
{
}


void SceneBase::setXRotAngle(float angle)
{
    m_xRotAngle = normalizeAngle(angle);
    update();
}

void SceneBase::setYRotAngle(float angle)
{
    m_yRotAngle = normalizeAngle(angle);
    update();
}

void SceneBase::setZRotAngle(float angle)
{
    m_zRotAngle = normalizeAngle(angle);
    update();
}

void SceneBase::setClearColor(const QColor &color)
{
    m_clearColor = color;
    update();
}

void SceneBase::setProjectionVerticalAngle(float angle)
{
    if (angle < m_minProjectionVerticalAngle) {
        m_projectionVerticalAngle = m_minProjectionVerticalAngle;
    } else if (angle > m_maxProjectionVerticalAngle) {
        m_projectionVerticalAngle = m_minProjectionVerticalAngle;
    } else {
        m_projectionVerticalAngle = angle;
    }
    update();
}

void SceneBase::setTranslationVector(const QVector3D &vec)
{
    m_translationVector = vec;
    update();
}

void SceneBase::setEyeVector(const QVector3D &vec)
{
    m_eyeVector = vec;
    update();
}

void SceneBase::setCenterVector(const QVector3D &vec)
{
    m_centerVector = vec;
    update();
}

void SceneBase::setUpVector(const QVector3D &vec)
{
    m_upVector = vec;
    update();
}

float SceneBase::xRotAngle() const
{
    return m_xRotAngle;
}

float SceneBase::yRotAngle() const
{
    return m_yRotAngle;
}

float SceneBase::zRotAngle() const
{
    return m_zRotAngle;
}

float SceneBase::projectionVerticalAngle() const
{
    return m_projectionVerticalAngle;
}

const QColor &SceneBase::clearColor() const
{
    return m_clearColor;
}

const QVector3D &SceneBase::translationVector() const
{
    return m_translationVector;
}

const QVector3D &SceneBase::eyeVector() const
{
    return m_eyeVector;
}

const QVector3D &SceneBase::centerVector() const
{
    return m_centerVector;
}

const QVector3D &SceneBase::upVector() const
{
    return m_upVector;
}

void SceneBase::initializeGL()
{
    initializeOpenGLFunctions();
    init();
}

void SceneBase::paintGL()
{
    paint();
}

void SceneBase::resizeGL(int width, int height)
{
    int minSide = qMin(width, height);
    glViewport((width - minSide) / 2, (height - minSide) / 2, minSide, minSide);
    update();
}

void SceneBase::mousePressEvent(QMouseEvent *event)
{
    m_mousePrevPos = event->pos();
}

void SceneBase::mouseMoveEvent(QMouseEvent *event)
{
    QPoint mouseCurrentPos = event->pos();

    int dx = mouseCurrentPos.x() - m_mousePrevPos.x();
    int dy = mouseCurrentPos.y() - m_mousePrevPos.y();

    m_xRotAngle = normalizeAngle(m_xRotAngle + 0.35f * dy);
    if (event->buttons() & Qt::LeftButton) {
        m_yRotAngle = normalizeAngle(m_yRotAngle + 0.35f * dx);
    } else if (event->buttons() & Qt::RightButton) {
        m_zRotAngle = normalizeAngle(m_zRotAngle + 0.35f * dx);
    }
    m_mousePrevPos = mouseCurrentPos;

    update();
}

void SceneBase::wheelEvent(QWheelEvent *event)
{
    float step = 0.2f;
    float z    = m_translationVector.z();

    if (event->delta() < 0.0f) {
        z -= step;
    } else if (z + step <= 2.0f) {
        z += step;
    }

    m_translationVector.setZ(z);

    update();
}

const QMatrix4x4 &SceneBase::mvpMatrix()
{
    QMatrix4x4 rotationMatrix;
    rotationMatrix.rotate(m_xRotAngle, 1.0f, 0.0f, 0.0f);
    rotationMatrix.rotate(m_yRotAngle, 0.0f, 1.0f, 0.0f);
    rotationMatrix.rotate(m_zRotAngle, 0.0f, 0.0f, 1.0f);

    QMatrix4x4 modelMatrix;
    modelMatrix.translate(m_translationVector);

    QMatrix4x4 viewMatrix;
    viewMatrix.lookAt(m_eyeVector, m_centerVector, m_upVector);

    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(m_projectionVerticalAngle, float(width()) / height(), 0.1f, 100.0f);

    m_mvpMatrix = projectionMatrix * viewMatrix * modelMatrix * rotationMatrix;

    return m_mvpMatrix;
}

float SceneBase::normalizeAngle(float angle)
{
    if (angle < 0.0f) {
        angle += 360.0f;
    } else if (angle > 360.0f) {
        angle -= 360.0f;
    }
    return angle;
}
