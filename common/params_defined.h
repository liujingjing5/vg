#ifndef PARAMS_DEFINED_H
#define PARAMS_DEFINED_H

#include <QString>




namespace ParamsDefined
{
namespace Type{
    const static char* text = "text";
    const static char* area = "area";
    const static char* select = "select";
    const static char* checkbox = "checkbox";
}


QString getTypeName(QString type);

}

#endif // PARAMS_DEFINED_H
