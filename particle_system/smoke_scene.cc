#include "smoke_scene.h"
#include "help_math.h"


SmokeScene::SmokeScene(QWidget *parent)
    : SceneBase(parent)
    , m_particlesCount(1500)
{
    setClearColor(Qt::black);
    setTranslationVector(QVector3D(0.0f, 0.0f, -1.0f));
    setYRotAngle(15);

    std::srand(std::time(0));
}

SmokeScene::~SmokeScene()
{
}


// floor:

void SmokeScene::initFloor()
{
    m_floorShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/floor.vs");
    m_floorShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/floor.fs");
    m_floorShaderProgram.link();

    GLfloat vertices[] = {-2.0f, -1.0f, -2.0f, -2.0f, -1.0f, 2.0f,  2.0f, -1.0f,
                          2.0f,  2.0f,  -1.0f, -2.0f, 0.0f,  -1.0f, 0.0f};
    glGenBuffers(2, &m_floorVerticesVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_floorVerticesVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLfloat colors[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};
    glGenBuffers(2, &m_floorColorsVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_floorColorsVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLushort faces[] = {0, 4, 1, 0, 4, 3, 2, 4, 1, 2, 4, 3};
    glGenBuffers(2, &m_floorFacesIbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_floorFacesIbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);

    m_attribFloorCoordinate = GLuint(m_floorShaderProgram.attributeLocation("coordinate"));
    m_attribFloorColor      = GLuint(m_floorShaderProgram.attributeLocation("color"));
    glEnableVertexAttribArray(m_attribFloorCoordinate);
    glEnableVertexAttribArray(m_attribFloorColor);
}

void SmokeScene::paintFloor()
{
    m_floorShaderProgram.bind();

    m_floorShaderProgram.setUniformValue("mvp", mvpMatrix());

    glEnableVertexAttribArray(m_attribFloorCoordinate);
    glBindBuffer(GL_ARRAY_BUFFER, m_floorVerticesVbo);
    glVertexAttribPointer(m_attribFloorCoordinate, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(m_attribFloorColor);
    glBindBuffer(GL_ARRAY_BUFFER, m_floorColorsVbo);
    glVertexAttribPointer(m_attribFloorColor, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_floorFacesIbo);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(m_attribFloorCoordinate);
    glDisableVertexAttribArray(m_attribFloorColor);

    m_floorShaderProgram.release();
}


// smoke:

void SmokeScene::initSmoke()
{
    m_smokeShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/smoke.vs");
    m_smokeShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/smoke.fs");
    m_smokeShaderProgram.link();

    GLsizeiptr pCount = GLsizeiptr(m_particlesCount);
    GLsizeiptr fSize  = GLsizeiptr(sizeof(float));

    QVector<float> particles(m_particlesCount);
    for (int i = 0; i < m_particlesCount; ++i) {
        particles[i] = float(i);
    }
    glGenBuffers(1, &m_particlesVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_particlesVbo);
    glBufferData(GL_ARRAY_BUFFER, pCount * fSize, particles.data(), GL_STATIC_DRAW);

    QVector<float> randoms(m_particlesCount * 3);
    for (int i = 0; i < m_particlesCount * 3; ++i) {
        randoms[i] = mix(-1.0, 1.0, randFloat());
    }
    glGenBuffers(1, &m_randomVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_randomVbo);
    glBufferData(GL_ARRAY_BUFFER, pCount * fSize * 3, randoms.data(), GL_STATIC_DRAW);

    QVector<float> velocities(3 * m_particlesCount);
    QVector3D      vec;
    for (int i = 0; i < m_particlesCount; i++) {
        float theta    = mix(0.0f, pi() / 1.5f, randFloat());
        float phi      = mix(0.0f, twoPi(), randFloat());
        float velocity = mix(0.1f, 0.2f, randFloat());

        vec.setX(sinf(theta) * cosf(phi));
        vec.setY(cosf(theta));
        vec.setZ(sinf(theta) * sinf(phi));
        vec.normalize();

        velocities[3 * i]     = vec.x() * velocity;
        velocities[3 * i + 1] = vec.y() * velocity;
        velocities[3 * i + 2] = vec.z() * velocity;
    }
    glGenBuffers(1, &m_velocitiesVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_velocitiesVbo);
    glBufferData(GL_ARRAY_BUFFER, pCount * fSize * 3, velocities.data(), GL_STATIC_DRAW);

    m_texture = new QOpenGLTexture(QImage(":/smoke_texture.png"));

    m_timer.setInterval(15);
    m_timer.setSingleShot(false);
    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer.start();
}

void SmokeScene::paintSmoke()
{
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(20);

    m_texture->bind();
    m_smokeShaderProgram.bind();

    m_smokeShaderProgram.setUniformValue("time", float(m_frame));
    m_smokeShaderProgram.setUniformValue("sampler", 0);
    m_smokeShaderProgram.setUniformValue("mvp", mvpMatrix());

    GLuint attribRandom = GLuint(m_smokeShaderProgram.attributeLocation("random"));
    glEnableVertexAttribArray(attribRandom);
    glBindBuffer(GL_ARRAY_BUFFER, m_randomVbo);
    glVertexAttribPointer(attribRandom, 3, GL_FLOAT, GL_TRUE, 0, 0);

    GLuint attribVelocity = GLuint(m_smokeShaderProgram.attributeLocation("velocity"));
    glEnableVertexAttribArray(attribVelocity);
    glBindBuffer(GL_ARRAY_BUFFER, m_velocitiesVbo);
    glVertexAttribPointer(attribVelocity, 3, GL_FLOAT, GL_TRUE, 0, 0);

    GLuint attribParticles = GLuint(m_smokeShaderProgram.attributeLocation("vertex"));
    glEnableVertexAttribArray(attribParticles);
    glBindBuffer(GL_ARRAY_BUFFER, m_particlesVbo);
    glVertexAttribPointer(attribParticles, 1, GL_FLOAT, GL_TRUE, 0, 0);

    glDrawArrays(GL_POINTS, 0, m_particlesCount);

    m_smokeShaderProgram.release();
    m_texture->release();

    m_frame = (m_frame + 1) % 2000;

    glDisable(GL_POINT_SPRITE);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}


// override methods:

void SmokeScene::init()
{
    initFloor();
    initSmoke();
}

void SmokeScene::paint()
{
    glClearColor(float(clearColor().redF()), float(clearColor().greenF()), float(clearColor().blueF()),
                 float(clearColor().alphaF()));
    paintFloor();
    paintSmoke();
}
