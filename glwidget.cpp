#include "glwidget.h"
#include "gl-matrix.h"
#include <QMessageBox>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
    program(0),
    frame(0)
{
    timer = new QTimer(this);
    timer->start(1000 / 30);
    connect(timer,SIGNAL(timeout()),this,SLOT(repaint()));
}

void GLWidget::initializeGL()
{    
    static const GLbyte vertexShaderSrc[] =
        "attribute vec4 aVertex;   "
        "void main(void) {         "
        "	gl_Position = aVertex; "
        "}                         ";

    static const GLbyte fragmentShaderSrc[] =
        "void main(void) {                             "
        "	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);   "
        "}                                             ";

    int r = 0x00;
    int g = 0x44;
    int b = 0x99;

    glClearColor(r / 256.0f, g / 256.0f, b / 256.0f, 1.0);

    //glEnable(GL_CULL_FACE);
    //glEnable(GL_DEPTH_TEST);

    GLuint shaderIds[2] = { 0, 0 };
    int flags[2] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
    const GLbyte* sources[2] = { vertexShaderSrc, fragmentShaderSrc };

    for(int i = 0; i != 2; i++) {
        int compileStatus;

        GLuint shader = glCreateShader(flags[i]);
        glShaderSource(shader, 1, (const GLchar**)(&sources[i]), NULL);
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

        if(!compileStatus) {
            GLchar message[1024];
            GLsizei size;
            glGetShaderInfoLog(shader, 1024, &size, &message[0]);

            QMessageBox::warning(this, "[ERROR]", message);
            glDeleteShader(shader);
            return;
        }

        shaderIds[i] = shader;
    }

    program = glCreateProgram();

    GLuint vertexShader = shaderIds[0];
    GLuint fragmentShader = shaderIds[1];

    qDebug() << vertexShader << " " << fragmentShader << " " << program;

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

    if(!linkStatus) {
        GLint infoLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1)
        {
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(program, infoLen, NULL, infoLog);
            QMessageBox::warning(this, "[ERROR]", infoLog);
            free(infoLog);
        }
        glDeleteProgram(program);
        return;
    }

    /*GLuint vtBuffer;
    glGenBuffers(1, &vtBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vtBuffer);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), tri, GL_STATIC_DRAW);*/

    //glBindAttribLocation(program, 0, "aVertex");       

    this->resizeGL(this->parentWidget()->size().width(), this->parentWidget()->size().height());
}

void GLWidget::resizeGL(int width, int height)
{
    qDebug() << "SIZE: " << width << " " << height;
    glViewport(0, 0, (GLint)width, (GLint)height);
}

void GLWidget::paintGL()
{

    GLfloat tri[] = {
        +0.0f, +0.5f, +0.0f,
        -0.5f, -0.5f, +0.0f,
        +0.5f, -0.5f, +0.0f
    };

    glClear(GL_COLOR_BUFFER_BIT);

    //GLint aVertex = glGetAttribLocation(program, "aVertex");
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, tri);
    glEnableVertexAttribArray(0);

    //qDebug() << "aVertex : " << aVertex;

    /*
    float modelview[16];
    float projection[16];

    mat4_identity(modelview);

    float cameraPos[3] = {0.0f, 0.0f, 0.0f};
    float cameraNormal[3] = {0.0f, 0.0f, -1.0f};
    float cameraUp[3] = {0.0f, 1.0f, 0.0f};
    float cameraCenter[3];
    vec3_add(cameraPos, cameraNormal, cameraCenter);

    float camera[16];

    mat4_lookAt(cameraPos, cameraCenter, cameraUp, camera);

    mat4_perspective(75.0f, 4.0f / 3.0f, 0.1f, 1000.0f, projection);
    */

    //GLint uModelview = glGetUniformLocation(program, "uModelview");
    //GLint uProjection = glGetUniformLocation(program, "uProjection");
    //GLint uTexture = glGetUniformLocation(program, "uTexture");

    //qDebug() << uModelview << " " << uProjection;
    //char s[1024];
    //mat4_str(projection, s);
    //qDebug() << s;

//    mat4_multiply(modelview, camera, modelview);
//    float translate[3] = {0.0f, 0.0f, 0.1f * frame};
//    mat4_translate(modelview, translate, modelview);

    //glUniformMatrix4fv(uModelview, 1, GL_FALSE, modelview);
    //glUniformMatrix4fv(uProjection, 1, GL_FALSE, projection);


    glDrawArrays(GL_TRIANGLES, 0, 3);

    frame++;
}

void GLWidget::repaint()
{
    paintGL();
}
