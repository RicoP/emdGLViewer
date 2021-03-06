#ifndef EMDOBJECTSET_H
#define EMDOBJECTSET_H

#include "gpubuffer.h"
#include <QByteArray>
#include <QFile>

class EmdObjectSet
{
public:
    EmdObjectSet();
    EmdObjectSet(QByteArray blob, bool* success);
    ~EmdObjectSet();

    bool writeObjFile(QString path);

    bool isEmpty() const {
        return m_numParts == 0;
    }

    objectvtn* parts() const {
        return m_parts;
    }

    int numParts() const {
        return m_numParts;
    }

private:
    objectvtn* m_parts;
    int m_numParts;
};

#endif // EMDOBJECTSET_H
