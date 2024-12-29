#ifndef TEMPLATEUTILS_H
#define TEMPLATEUTILS_H

//use this weird thing to avoid undefined order of variadic template arguments
struct PassParamPack { template<typename ...T> PassParamPack(T...) {} };

#endif