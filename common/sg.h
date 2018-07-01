#ifndef SG_H
#define SG_H


#include <stddef.h>
/**
 * 单例模式
 */
template<typename T>
class sg
{
public:
    static T* get();
    static void release();
private:
    sg(){}
    static T* __instance__;
};

template<typename T>
T* sg<T>::__instance__ ;

template<typename T>
T* sg<T>::get(){
    if(! sg<T>::__instance__){
        sg<T>::__instance__ = new T;
    }
    return __instance__;
}

template<typename T>
void sg<T>::release(){
    if(sg<T>::__instance__){
        delete sg<T>::__instance__;
        sg<T>::__instance__ = NULL;
    }
}



#endif // SG_H
