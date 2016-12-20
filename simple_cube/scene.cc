#include "scene.h"
#include <cmath>


Scene::Scene(QWidget *parent)
    : SceneBase(parent)
{
}

void Scene::init()
{
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex_shader.glsl");
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment_shader.glsl");
    m_shaderProgram.link();

    float a = std::sqrt(3.0f) / 3.0f;

    GLfloat vertices[] = {+a, -a, -a, -a, -a, -a, -a, +a, -a, +a, +a, -a,
                          +a, -a, +a, -a, -a, +a, -a, +a, +a, +a, +a, +a};

    glGenBuffers(1, &m_indexOfVerticesVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_indexOfVerticesVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLfloat colors[] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    glGenBuffers(1, &m_indexOfColorsVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_indexOfColorsVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLushort faces[] = {0, 2, 1, 0, 2, 3, 0, 7, 3, 0, 7, 4, 0, 5, 1, 0, 5, 4,
                        6, 2, 1, 6, 2, 3, 6, 7, 3, 6, 7, 4, 6, 5, 1, 6, 5, 4};
    glGenBuffers(1, &m_indexOfFacesIbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexOfFacesIbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);

    m_indexOfCoordinates = GLuint(m_shaderProgram.attributeLocation("coordinate"));
    m_indexOfColors      = GLuint(m_shaderProgram.attributeLocation("vertColor"));
    glEnableVertexAttribArray(m_indexOfCoordinates);
    glEnableVertexAttribArray(m_indexOfColors);

    m_updateTimer.setInterval(20);
    connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(update()));
    m_updateTimer.start();
}

void Scene::paint()
{
    glClearColor(float(clearColor().redF()), float(clearColor().greenF()), float(clearColor().blueF()),
                 float(clearColor().alphaF()));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    m_shaderProgram.bind();

    m_shaderProgram.setUniformValue("mvp", mvpMatrix());

    glEnableVertexAttribArray(m_indexOfCoordinates);
    glBindBuffer(GL_ARRAY_BUFFER, m_indexOfVerticesVbo);
    glVertexAttribPointer(m_indexOfCoordinates, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(m_indexOfColors);
    glBindBuffer(GL_ARRAY_BUFFER, m_indexOfColorsVbo);
    glVertexAttribPointer(m_indexOfColors, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexOfFacesIbo);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(m_indexOfCoordinates);
    glDisableVertexAttribArray(m_indexOfColors);

    m_shaderProgram.release();
}
