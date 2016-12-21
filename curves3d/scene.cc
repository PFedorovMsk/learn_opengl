#include "scene.h"
#include <cmath>


Scene::Scene(QWidget *parent)
    : SceneBase(parent)
    , m_drawBasePoints(true)
    , m_drawLines(true)
    , m_currentBasePointIndex(0)
{
}

void Scene::enableBasePoints(bool enable)
{
    m_drawBasePoints = enable;
    update();
}

void Scene::enableLines(bool enable)
{
    m_drawLines = enable;
    update();
}

void Scene::setData(const QVector<QVector3D> &curve, const QVector<QVector3D> basePoints)
{
    m_curve      = curve;
    m_basePoints = basePoints;

    update();
}

void Scene::setCurrentBasePoint(int index)
{
    m_currentBasePointIndex = index;

    update();
}

void Scene::init()
{
    m_shaderProgram.removeAllShaders();
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex_shader.glsl");
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment_shader.glsl");
    m_shaderProgram.link();

    m_attribPoint = GLuint(m_shaderProgram.attributeLocation("point"));
    glEnableVertexAttribArray(m_attribPoint);
}


void Scene::paintCurve()
{
    m_shaderProgram.bind();

    m_shaderProgram.setUniformValue("mvp", mvpMatrix());
    m_shaderProgram.setUniformValue("type", 0);

    glEnableVertexAttribArray(m_attribPoint);
    glBindBuffer(GL_ARRAY_BUFFER, m_curveVbo);
    glVertexAttribPointer(m_attribPoint, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glLineWidth(3.0f);

    glDrawArrays(GL_LINES, 0, m_curve.size());
    glDrawArrays(GL_LINES, 1, m_curve.size() - 1);

    m_shaderProgram.release();

    glLineWidth(1.0f);
}

void Scene::paintKnots()
{
    m_shaderProgram.bind();

    m_shaderProgram.setUniformValue("mvp", mvpMatrix());
    m_shaderProgram.setUniformValue("type", 1);

    glEnableVertexAttribArray(m_attribPoint);
    glBindBuffer(GL_ARRAY_BUFFER, m_basePointsVbo);
    glVertexAttribPointer(m_attribPoint, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glPointSize(10.0f);

    glDrawArrays(GL_POINTS, 0, m_basePoints.size());

    m_shaderProgram.release();


    //--------------------------------------------------------------------//


    m_shaderProgram.bind();

    m_shaderProgram.setUniformValue("mvp", mvpMatrix());
    m_shaderProgram.setUniformValue("type", 2);

    glPointSize(16.0f);
    glDrawArrays(GL_POINTS, m_currentBasePointIndex, 1);

    m_shaderProgram.release();

    glPointSize(1.0f);
}

void Scene::paintLines()
{
    m_shaderProgram.bind();

    m_shaderProgram.setUniformValue("mvp", mvpMatrix());
    m_shaderProgram.setUniformValue("type", 3);

    glEnableVertexAttribArray(m_attribPoint);
    glBindBuffer(GL_ARRAY_BUFFER, m_basePointsVbo);
    glVertexAttribPointer(m_attribPoint, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_LINES, 0, m_basePoints.size());
    glDrawArrays(GL_LINES, 1, m_basePoints.size() - 1);

    m_shaderProgram.release();
}

void Scene::paint()
{
    glClearColor(float(clearColor().redF()), float(clearColor().greenF()), float(clearColor().blueF()),
                 float(clearColor().alphaF()));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);

    //
    // TODO
    // этот кусок должен быть в init(), но я хз как перевызвать initializeGL()
    // после изменения данных (вызов setData(...)).
    // [[

    QVector<float> curve(m_curve.size() * 3);
    for (int i = 0; i < m_curve.size(); ++i) {
        curve[3 * i]     = m_curve[i].x();
        curve[3 * i + 1] = m_curve[i].y();
        curve[3 * i + 2] = m_curve[i].z();
    }
    glGenBuffers(1, &m_curveVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_curveVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(curve[0]) * curve.size(), curve.data(), GL_STATIC_DRAW);

    QVector<float> basePoints(m_basePoints.size() * 3);
    for (int i = 0; i < m_basePoints.size(); ++i) {
        basePoints[3 * i]     = m_basePoints[i].x();
        basePoints[3 * i + 1] = m_basePoints[i].y();
        basePoints[3 * i + 2] = m_basePoints[i].z();
    }
    glGenBuffers(1, &m_basePointsVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_basePointsVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(basePoints[0]) * basePoints.size(), basePoints.data(), GL_STATIC_DRAW);

    // ]]
    //


    paintCurve();
    if (m_drawBasePoints) {
        paintKnots();
    }
    if (m_drawLines) {
        paintLines();
    }
}
