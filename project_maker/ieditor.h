#ifndef IEDITOR_H
#define IEDITOR_H

#include <QString>

class IEditor{
public:
    virtual bool editorClose()=0;
    virtual QString editorFilePath()=0;
    virtual bool editorSave()=0;
};

#endif // IEDITOR_H
