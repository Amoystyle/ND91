#ifndef __CWEBRESOURCEMACROS_H__
#define __CWEBRESOURCEMACROS_H__

#define PROPERTY(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void);\
public: virtual void set##funName(varType var);

#define PROPERTYREALONLY(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void);

#define PROPERTYWRITEONLY(varType, varName, funName)\
protected: varType varName;\
public: virtual void set##funName(varType var);

#ifndef todo
#   define __STR2__(x)      #x
#   define __STR1__(x)      __STR2__(x)
#   define __LOC__          __FILE__ "("__STR1__(__LINE__)") : Warning Msg: "
#   define __LOC2__         __FILE__ "("__STR1__(__LINE__)") : <TODO> "
#   define todo(str)        message (__LOC2__ str)
#endif
#endif