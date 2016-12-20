#ifndef SCENE_H
#define SCENE_H

#include "scene_base.h"

#include <QOpenGLShaderProgram>
#include <QTime>
#include <QTimer>


class Scene : public SceneBase
{
public:
    Scene(QWidget *parent = nullptr);

protected:
    void init() override;
    void paint() override;

private:
    QOpenGLShaderProgram m_shaderProgram;

    QTimer m_updateTimer;

    GLuint m_indexOfCoordinates;
    GLuint m_indexOfColors;
    GLuint m_indexOfVerticesVbo;
    GLuint m_indexOfColorsVbo;
    GLuint m_indexOfFacesIbo;
};


#endif // SCENE_H
