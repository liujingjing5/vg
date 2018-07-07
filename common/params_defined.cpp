#include "params_defined.h"

namespace ParamsDefined
{

QString getTypeName(QString type)
{
    if(type == Type::text){
        return "单文本";
    }else if(type == Type::area){
        return "多文本";
    }else if(type == Type::select){
        return "单选框";
    }else if(type == Type::checkbox){
        return "多选框";
    }else{
        return "";
    }
}

}
