#include <QGuiApplication>
#include <QtQml>

#include <QtQuick/QQuickView>

#include "squircle.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Squircle>("OpenGLUnderQML", 1, 0, "Squircle");

    QQuickView view;
    view.setSource(QUrl("qrc:///scenegraph/openglunderqml/main.qml"));
    view.show();

    return app.exec();

}
