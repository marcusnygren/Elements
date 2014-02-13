
// Nonsense:
// Lambda functions:
// http://en.cppreference.com/w/cpp/language/lambda

// Qt shared data
// http://qt-project.org/doc/qt-4.8/qtbinding.html#receiving-signals
// http://qt-project.org/doc/qt-5.0/qtqml/qtqml-cppintegration-contextproperties.html


// TODO:
// READ THIS, IT'S ESSENTIAL! 
// About signals and slots:
// http://qt-project.org/doc/qt-4.8/signalsandslots.html
// About Properties:
// http://qt-project.org/doc/qt-5.0/qtcore/properties.html
// About Property binding:
// http://qt-project.org/doc/qt-4.8/propertybinding.html
// Supoorted data types:
// http://qt-project.org/doc/qt-5.0/qtqml/qtqml-cppintegration-data.html

// http://ubuntuforums.org/showthread.php?t=2147683


// http://qt-project.org/doc/qt-5.0/qtquick/qquickitem.html#ItemChange-enum

// TODO:
// Läs om rendering:
// http://qt-project.org/doc/qt-5.0/qtquick/qtquick-visualcanvas-scenegraph.html#scene-graph-and-rendering
// http://qt-project.org/doc/qt-5.0/qtquick/qquickitem.html#details



#include <QGuiApplication>
#include <QtQuick/QQuickView>

#include "Parameters.h"
#include "Engine.h"
#include "DataHandler.h"
#include "Debug.h" 

#include <QQmlContext>
#include <QQmlEngine>



int main(int argc, char** argv) 
{
	QGuiApplication app(argc, argv);

	// Make C++ class available to QML. 
	// Uniform resource identifier: Elements
	// XML version: 1.0
	// Name: Engine 
	qmlRegisterType<Engine>("ElementsEngine", 1, 0, "Engine");
	qmlRegisterType<DataHandler>("ElementsDataHandler", 1, 0, "Data");


	// QQuickView view(QUrl("qrc:///Elements/GUI.qml"), QWindow * parent = 0);
	QQuickView view;


//	DataHandler* datas = new DataHandler();
//	view.rootContext()->setContextProperty("datta", datas);
//	 qDebug() << "\nMAIN:	baseUrl() = " << view.rootContext()->baseUrl();


	view.setSource(QUrl("qrc:///Elements/GUI.qml"));
	view.show();
	
	// Returns a infinite loop which looks for events/signals.
	// It will exit the loop when exit() is called.
	return app.exec();
}


// For the linker
Console* Console::theOnlyInstance = NULL;
std::mutex Console::_instanceMutex;
std::recursive_mutex Console::_commandItemsMutex;



/*
#include "openglwindow.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>

class TriangleWindow : public OpenGLWindow
{
public:
    TriangleWindow();

    void initialize();
    void render();

private:
    GLuint loadShader(GLenum type, const char *source);

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;
    int m_frame;
};

TriangleWindow::TriangleWindow()
    : m_program(0)
    , m_frame(0)
{
}

int main(int argc, char **argv)
{
  QGuiApplication app(argc, argv);

  QSurfaceFormat format;
  format.setSamples(4);
	//format.setRenderableType(QSurfaceFormat::RenderableType::OpenGL);
	//format.setMajorVersion(3);
	//format.setMinorVersion(3);

  TriangleWindow* window = new TriangleWindow();
  window->setFormat(format);
  window->resize(640, 480);
  //window.show();

  window->setAnimating(true);


	qmlRegisterType<Engine>("ElementsEngine", 1, 0, "Engine");
	qmlRegisterType<DataHandler>("ElementsDataHandler", 1, 0, "Data");

	QQuickView view;
	view.setSource(QUrl("qrc:///Elements/GUI.qml"));
	view.show();
	// view.showFullScreen();
	window->show();

    return app.exec();
}

static const char *vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n";

GLuint TriangleWindow::loadShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}

void TriangleWindow::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");
}

void TriangleWindow::render()
{
    glViewport(0, 0, width(), height());

    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60, 4.0/3.0, 0.1, 100.0);
    matrix.translate(0, 0, -2);
    matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);

    GLfloat vertices[] = {
        0.0f, 0.707f,
        -0.5f, -0.5f,
        0.5f, -0.5f
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    m_program->release();

    ++m_frame;

}
*/