#include "widget.h"

Widget::Widget(QOpenGLWidget *parent)
    : QOpenGLWidget(parent)
    , m_rotationAngleAroundVerticalAxis(0.0f)
    , m_rotationAngleAroundLateralAxis(0.0f)
    , m_distance(30.f)
    , m_minDistance(10.0f)
    , m_maxDistance(150.0f)
{
}

Widget::~Widget()
{
}

void Widget::initializeGL()
{
    initializeOpenGLFunctions();

    qDebug("OpenGL version: %s", glGetString(GL_VERSION));
    qDebug("OpenGL vendor: %s", glGetString(GL_VENDOR));
    qDebug("OpenGL renderer: %s", glGetString(GL_RENDERER));
    qDebug("OpenGL shading language version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex_shader.glsl");
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment_shader.glsl");
    m_shaderProgram.link();

    init();

    m_updateTimer.setInterval(20);
    connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(update()));
    m_updateTimer.start();
}

void Widget::init()
{
    //равносторонний треугольник с высотой h = 1.0 (h = r + R; R = 2 * r):
    const float r          = 1.0f / 3.0f;
    const float R          = 2.0f * r;
    GLfloat     vertices[] = {0.0f, R, 0.0f, -R, -r, 0.0f, r, -r, -R, r, -r, R};

    glGenBuffers(1, &m_indexOfVerticesVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_indexOfVerticesVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLfloat colors[] = {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    glGenBuffers(1, &m_indexOfColorsVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_indexOfColorsVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLushort faces[] = {0, 1, 2, 0, 2, 3, 0, 3, 1, 1, 2, 3};
    glGenBuffers(1, &m_indexOfFacesIbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexOfFacesIbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);

    m_indexOfCoordinates = GLuint(m_shaderProgram.attributeLocation("coordinate"));
    m_indexOfColors      = GLuint(m_shaderProgram.attributeLocation("vertColor"));
    glEnableVertexAttribArray(m_indexOfCoordinates);
    glEnableVertexAttribArray(m_indexOfColors);
}

void Widget::draw(QOpenGLShaderProgram *shaderProgram)
{
    QMatrix4x4 animationMatrix;
    animationMatrix.rotate(m_rotationAngleAroundVerticalAxis, QVector3D(0, 1, 0));
    animationMatrix.rotate(m_rotationAngleAroundLateralAxis, QVector3D(1, 0, 0));

    QMatrix4x4 modelMatrix;
    modelMatrix.translate(QVector3D(0.0, 0.0, 3.0));

    QMatrix4x4 viewMatrix;
    viewMatrix.lookAt(QVector3D(0.0, 1.0, 0.0), QVector3D(0.0, 0.0, 3.0), QVector3D(0.0, 1.0, 0.0));

    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(m_distance, float(width()) / height(), 0.1f, 10.0f);

    QMatrix4x4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix * animationMatrix;
    shaderProgram->setUniformValue("mvp", modelViewProjectionMatrix);

    glEnableVertexAttribArray(m_indexOfCoordinates);
    glBindBuffer(GL_ARRAY_BUFFER, m_indexOfVerticesVbo);
    glVertexAttribPointer(m_indexOfCoordinates, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(m_indexOfColors);
    glBindBuffer(GL_ARRAY_BUFFER, m_indexOfColorsVbo);
    glVertexAttribPointer(m_indexOfColors, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexOfFacesIbo);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(m_indexOfCoordinates);
    glDisableVertexAttribArray(m_indexOfColors);
}

void Widget::paintGL()
{
    makeCurrent();

    glClearColor(0.175f, 0.35f, 0.35f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    m_shaderProgram.bind();
    draw(&m_shaderProgram);
    m_shaderProgram.release();

    doneCurrent();
    measureFps();
}

void Widget::measureFps()
{
    static int   frames       = 0;
    static QTime previousTime = QTime::currentTime();

    frames++;
    int delta = QTime::currentTime().msecsSinceStartOfDay() - previousTime.msecsSinceStartOfDay();
    if (delta > 1000) {
        setWindowTitle(QString("FPS: %1").arg(1000.0 * frames / delta));
        frames       = 0;
        previousTime = QTime::currentTime();
    }
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    if (e->button()) {
        m_mousePosition.setX(e->x());
        m_mousePosition.setY(e->y());
    }
}

void Widget::wheelEvent(QWheelEvent *e)
{
    if (e->delta() < 0.0f && m_distance < m_maxDistance) {
        m_distance += 1.0f;
    } else if (m_distance > m_minDistance) {
        m_distance -= 1.0f;
    }

    paintGL();
}

void Widget::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons()) {
        m_rotationAngleAroundVerticalAxis += (e->x() - m_mousePosition.x());
        m_rotationAngleAroundLateralAxis -= (e->y() - m_mousePosition.y());
    }

    m_mousePosition.setX(e->x());
    m_mousePosition.setY(e->y());

    paintGL();
}
