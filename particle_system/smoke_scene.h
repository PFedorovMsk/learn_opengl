#ifndef SCENE_H
#define SCENE_H

#include "scene_base.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QTimer>


class SmokeScene : public SceneBase
{
public:
    SmokeScene(QWidget *parent = nullptr);
    ~SmokeScene();


protected:
    void init() override;
    void paint() override;


private:
    void initFloor();
    void paintFloor();

    void initSmoke();
    void paintSmoke();

private:
    QOpenGLShaderProgram m_floorShaderProgram;
    QOpenGLShaderProgram m_smokeShaderProgram;
    QOpenGLTexture *     m_texture;

    GLuint m_floorVerticesVbo;
    GLuint m_floorColorsVbo;
    GLuint m_floorFacesIbo;
    GLuint m_attribFloorCoordinate;
    GLuint m_attribFloorColor;

    QTimer m_timer;
    int    m_frame;
    int    m_particlesCount;

    GLuint m_particlesVbo;
    GLuint m_randomVbo;
    GLuint m_velocitiesVbo;
};


#endif // SCENE_H
