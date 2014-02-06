#ifndef SQUIRCLE_H
#define SQUIRCLE_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>

class Squircle : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)

public:
    Squircle();

    qreal t() const { return m_t; }
    void setT(qreal t);

signals:
    void tChanged();

protected:
    void itemChange(ItemChange change, const ItemChangeData &);

public slots:
    void paint();
    void cleanup();
    void sync();

private:
    QOpenGLShaderProgram *m_program;

    qreal m_t;
    qreal m_thread_t;
};

#endif // SQUIRCLE_H