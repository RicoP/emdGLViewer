#include "glwidget.h"
#include "gl-matrix.h"
#include "gpubuffer.h"
#include <QMessageBox>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
    program(0),
    frame(0),
    m_angleX(0.0f),
    m_angleY(0.0f),
    m_objects(0),
    m_tim(0),
    m_part(0),
    texture(0),
    m_showWireframe(0),
    distance(2.0f)
{    
    timer = new QTimer(this);
    timer->start(1000 / 30);
    connect(timer,SIGNAL(timeout()),this,SLOT(repaint()));
}

float& GLWidget::angleX() {
    return this->m_angleX;
}

float GLWidget::angleX() const {
    return this->m_angleX;
}

float& GLWidget::angleY() {
    return this->m_angleY;
}

float GLWidget::angleY() const {
    return this->m_angleY;
}

EmdObjectSet*& GLWidget::objects() {
    return m_objects;
}

EmdObjectSet* GLWidget::objects() const {
    return m_objects;
}

int& GLWidget::part() {
    return m_part;
}

int GLWidget::part() const {
    return m_part;
}

EmdTimBitmap*& GLWidget::tim() {
    return m_tim;
}

EmdTimBitmap* GLWidget::tim() const {
    return m_tim;
}

bool& GLWidget::showWireframe() {
    return m_showWireframe;
}

bool GLWidget::showWireframe() const {
    return m_showWireframe;
}

void GLWidget::zoom(float difference) {
    if( distance + difference > 3.0f ) {
        distance = 3.0f;
    }
    else if( distance + difference < 0.5f ) {
        distance = 0.5f;
    }
    else {
        distance += difference;
    }
}

static GLuint getShader(QWidget* caller, QString path, int type) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return 0;

    QByteArray array = file.readAll();
    const GLchar* text = array.data();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &text, NULL);

    glCompileShader(shader);

    int compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

    if(!compileStatus) {
        GLchar message[1024];
        GLsizei size;
        glGetShaderInfoLog(shader, 1024, &size, &message[0]);

        QMessageBox::warning(caller, "[ERROR]", message);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

void GLWidget::initializeGL()
{
    glClearColor(0.0f / 255.0f, 68.0f / 255.0f, 153.0f / 255.0f, 1.0);

    glEnable(GL_DEPTH_TEST);    

    GLuint vertexShader   = getShader(this, ":/shader/shader.vert", GL_VERTEX_SHADER);
    GLuint fragmentShader = getShader(this, ":/shader/shader.frag", GL_FRAGMENT_SHADER);

    program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glBindAttribLocation(program, 0, "aVertex");

    glLinkProgram(program);

    int linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

    if(!linkStatus) {
        qDebug() << "!linkStatus " << linkStatus;
        GLint infoLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1) {
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(program, infoLen, NULL, infoLog);
            QMessageBox::warning(this, "[ERROR]", infoLog);
            free(infoLog);
        }
        else {
            QMessageBox::warning(this, "[ERROR]", "Unknown Error");
        }
        glDeleteProgram(program);
        return;
    }
}

void GLWidget::resizeGL(int width, int height) {
    glViewport(0, 0, (GLint)width, (GLint)height);
    paintGL();
}

void GLWidget::refreshData() {   
    glDeleteTextures(1, &texture);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->tim()->width(), this->tim()->height(), 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, this->tim()->raw());    
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(!this->m_objects) {
        return;
    }

    glUseProgram(program);

    objectvtn part = this->m_objects->parts()[this->part()];

    void* blob = part.triangles;

    GLint aVertex = glGetAttribLocation(program, "aVertex");
    glVertexAttribPointer(aVertex, sizeof(vector4) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertexvtn), blob);
    glEnableVertexAttribArray(aVertex);

    GLint aTextureuv = glGetAttribLocation(program, "aTextureuv");
    glVertexAttribPointer(aTextureuv, sizeof(textureuv) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertexvtn), (void*)(((int)blob) + offsetof(vertexvtn, uv)));
    glEnableVertexAttribArray(aTextureuv);

    float modelview[16];
    float projection[16];

    mat4_identity(modelview);

    float cameraPos[3] = {-distance, 0.0f, 0.0f};
    float cameraNormal[3] = {1.0f, 0.0f, 0.0f};
    float cameraUp[3] = {0.0f, 1.0f, 0.0f};
    float cameraCenter[3];
    vec3_add(cameraPos, cameraNormal, cameraCenter);

    float camera[16];

    mat4_lookAt(cameraPos, cameraCenter, cameraUp, camera);   

    mat4_perspective(75.0f, (1.0f * this->size().width()) / (1.0f * this->size().height()), 0.1f, 1000.0f, projection);

    GLint uModelview = glGetUniformLocation(program, "uModelview");
    GLint uProjection = glGetUniformLocation(program, "uProjection");
    GLint uTexture = glGetUniformLocation(program, "uTexture");

    mat4_multiply(modelview, camera, modelview);    
    mat4_rotateY(modelview, angleY(), modelview);
    mat4_rotateZ(modelview, angleX(), modelview);

    glUniformMatrix4fv(uModelview, 1, GL_FALSE, modelview);
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, projection);

    //qDebug()<< texture;

    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(uTexture, 0);

    glDrawArrays( showWireframe() ? GL_LINES : GL_TRIANGLES, 0, part.numTriangles * 3);

    frame++;
}

void GLWidget::repaint()
{
    paintGL();
}
