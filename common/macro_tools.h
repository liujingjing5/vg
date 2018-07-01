#ifndef MACRO_TOOLS_H
#define MACRO_TOOLS_H

#define GET_VALUE(access_permission,type, name)\
    access_permission:\
        type name;\
    public:\
    inline type get_##name() {\
        return this->name;\
    }\

#define SET_VALUE(access_permission,type, name)\
    access_permission:\
        type name;\
    public:\
    inline void set_##name(type v) {\
        this->name = v;\
    }\

#define GETSET_VALUE(access_permission,type, name)\
    access_permission:\
        type name;\
    public:\
    inline void set_##name(type v) {\
        this->name = v;\
    }\
    inline type get_##name() {\
        return this->name;\
    }\



#define GET_PTR(access_permission,type, name)\
    access_permission:\
        type* name;\
    public:\
        inline type* get_##name(){\
            return this->name;\
        }\

#define SET_PTR(access_permission,type, name)\
    access_permission:\
        type* name;\
    public:\
        inline void set_##name(type* v){\
            this->name = v;\
        }\

#define GETSET_PTR(access_permission,type, name)\
    access_permission:\
        type* name;\
    public:\
        inline void set_##name(type* v){\
            name = v;\
        }\
        inline type* get_##name(){\
            return this->name;\
        }\

#endif // MACRO_TOOLS_H
