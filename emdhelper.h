#ifndef EMDHELPER_H
#define EMDHELPER_H

#include <QByteArray>

class EmdHelper
{
public:
    struct Dictionary {
        int skeleton;
        int animation;
        int mesh;
        int tim;
    };

    static Dictionary getDictionary(QByteArray blob);


private:
    EmdHelper();
};

#endif // EMDHELPER_H
