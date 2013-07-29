#include <QApplication>
#include <gl_widget.h>
#include <X11/Xlib.h>

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        printf("\nUsage\n%s [V4L2 camera path] [width] [height]", argv[0]);
        printf("\nEg:- %s /dev/video0 640 480\n", argv[0]);
        exit(0);
    }
    int w, h;
    XInitThreads();
    QApplication app(argc, argv);
    QGLFormat format;
    format.setVersion(3, 1);
    format.setProfile(QGLFormat::CoreProfile);
    format.setSampleBuffers(true);
    w = atoi(argv[2]);
    h = atoi(argv[3]);
    Gl_widget window(w, h, QString(argv[1]), format);
    window.resize(w, h);
    window.show();
    return app.exec();
}

