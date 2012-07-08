#ifndef EMDOBJECTSET_H
#define EMDOBJECTSET_H

#include "gpubuffer.h"
#include <QByteArray>

class EmdObjectSet
{
public:
    EmdObjectSet();
    EmdObjectSet(QByteArray blob, bool* success);
    EmdObjectSet(objectvtn* parts, int numParts);
    ~EmdObjectSet();

    bool isEmpty() const {
        return numParts == 0;
    }

private:
    objectvtn* parts;
    int numParts;
};

#endif // EMDOBJECTSET_H
