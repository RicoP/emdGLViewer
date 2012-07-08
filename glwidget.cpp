#include "glwidget.h"
#include "gl-matrix.h"
#include "gpubuffer.h"
#include <QMessageBox>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
    program(0),
    frame(0),
    m_angleX(0.0f),
    m_angleY(0.0f)
{
    static trianglevtn trivt = {
        //P1
        {
            //Vertex1
            { +0.0f, +0.5f, +0.0f, 1.0f },
            //Texture1
            { +0.0f, +1.0f },
            //Normal1
            { +0.0f, +0.0f, +1.0f, +0.0f }

        },
        //P2
        {
            //Vertex2
            { -0.5f, -0.5f, +0.0f, 1.0f },
            //Texture2
            { +1.0f, +1.0f },
            //Normal2
            { +0.0f, +0.0f, +1.0f, +0.0f }
        },
        //P3
        {
            //Vertex3
            { +0.5f, -0.5f, +0.0f, 1.0f },
            //Texture3
            { +0.5f, +0.0f },
            //Normal3
            { +0.0f, +0.0f, +1.0f, +0.0f }
        }
    };

    objectvtn* object = new objectvtn();
    object->numTriangles = 1;
    object->triangles = &trivt;

    this->objects = new EmdObjectSet(object, 1);

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

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    GLuint vertexShader = getShader(this, ":/shader/shader.vert", GL_VERTEX_SHADER);
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

    /*GLuint vtBuffer;
    glGenBuffers(1, &vtBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vtBuffer);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), tri, GL_STATIC_DRAW);*/



    //this->resizeGL(this->parentWidget()->size().width(), this->parentWidget()->size().height());
}

void GLWidget::resizeGL(int width, int height)
{
    qDebug() << "SIZE: " << width << " " << height;
    glViewport(0, 0, (GLint)width, (GLint)height);
    paintGL();
}

void GLWidget::paintGL()
{
    trianglevtn trivt = {
        //P1
        {
            //Vertex1
            { +0.0f, +0.5f, +0.0f, 1.0f },
            //Texture1
            { +0.0f, +1.0f },
            //Normal1
            { +0.0f, +0.0f, +1.0f, +0.0f }

        },
        //P2
        {
            //Vertex2
            { -0.5f, -0.5f, +0.0f, 1.0f },
            //Texture2
            { +1.0f, +1.0f },
            //Normal2
            { +0.0f, +0.0f, +1.0f, +0.0f }
        },
        //P3
        {
            //Vertex3
            { +0.5f, -0.5f, +0.0f, 1.0f },
            //Texture3
            { +0.5f, +0.0f },
            //Normal3
            { +0.0f, +0.0f, +1.0f, +0.0f }
        }
    };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    void* blob = &trivt;

//    GLint aVertex = glGetAttribLocation(program, "aVertex");
//    glVertexAttribPointer(aVertex, 3, GL_FLOAT, GL_FALSE, 0, tri);
//    glEnableVertexAttribArray(aVertex);

    GLint aVertex = glGetAttribLocation(program, "aVertex");
    glVertexAttribPointer(aVertex, sizeof(vector4) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertexvtn), blob);
    glEnableVertexAttribArray(aVertex);

    GLint aTextureuv = glGetAttribLocation(program, "aTextureuv");
    glVertexAttribPointer(aTextureuv, sizeof(textureuv) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertexvtn), blob + offsetof(vertexvtn, uv));
    glEnableVertexAttribArray(aTextureuv);

    //qDebug() << "aVertex : " << aVertex;

    float modelview[16];
    float projection[16];

    mat4_identity(modelview);

    float cameraPos[3] = {0.0f, 0.0f, 2.0f};
    float cameraNormal[3] = {0.0f, 0.0f, -1.0f};
    float cameraUp[3] = {0.0f, 1.0f, 0.0f};
    float cameraCenter[3];
    vec3_add(cameraPos, cameraNormal, cameraCenter);

    float camera[16];

    mat4_lookAt(cameraPos, cameraCenter, cameraUp, camera);

    mat4_perspective(75.0f, 4.0f / 3.0f, 0.1f, 1000.0f, projection);    

    GLint uModelview = glGetUniformLocation(program, "uModelview");
    GLint uProjection = glGetUniformLocation(program, "uProjection");
    //GLint uTexture = glGetUniformLocation(program, "uTexture");

    mat4_multiply(modelview, camera, modelview);    
    mat4_rotateY(modelview, angleY(), modelview);
    mat4_rotateX(modelview, angleX(), modelview);

    //mat4_identity(modelview);
    //mat4_identity(projection);

    glUniformMatrix4fv(uModelview, 1, GL_FALSE, modelview);
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, projection);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    frame++;
}

void GLWidget::repaint()
{
    paintGL();
}
