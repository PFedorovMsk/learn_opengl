#ifndef SCENE_H
#define SCENE_H

#include "bezier.h"
#include "bspline.h"
#include "nurbs.h"
#include "scene_base.h"

#include <QOpenGLShaderProgram>


class Scene : public SceneBase
{
public:
    Scene(QWidget *parent = nullptr);

    void enableBasePoints(bool enable);
    void enableLines(bool enable);

    void setData(const QVector<QVector3D> &curve, const QVector<QVector3D> &basePoints);
    void setCurrentBasePoint(int index);

protected:
    void init() override;
    void paint() override;

    void paintCurve();
    void paintKnots();
    void paintLines();

private:
    QOpenGLShaderProgram m_shaderProgram;

    GLuint m_curveVbo;
    GLuint m_basePointsVbo;
    GLuint m_attribPoint;

    QVector<QVector3D> m_curve;
    QVector<QVector3D> m_basePoints;

    bool m_drawBasePoints;
    bool m_drawLines;
    int  m_currentBasePointIndex;
};


#endif // SCENE_H
