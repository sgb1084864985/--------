#ifndef _CONN_EXCEPTION_H
#define _CONN_EXCEPTION_H

#include <stdexcept>

class ConnException:public std::logic_error{
public:
	ConnException(const std::string& msg):std::logic_error(msg){}
};

class ceNullPtr :public ConnException {
public:
	ceNullPtr() :ConnException("null ptr"){}
};

class ceNullServer :public ceNullPtr {
public:
	ceNullServer(){}
};

class ceNullSocket :public ceNullPtr {
public:
	ceNullSocket() {}
};

class ceBranchEx :public ConnException {
public:
	ceBranchEx() :ConnException("impossible case"){}
};

class ceIOError :public ConnException {
public:
	ceIOError(const std::string& s):ConnException(s){}
};

class ceStateEx :public ConnException {
public:
	ceStateEx() :ConnException("impossible state"){}
};

class ceReadInt :public ConnException {
public:
	ceReadInt() :ConnException("read interruption"){}
};

class ceNetEx :public ConnException {
public:
	ceNetEx(const std::string& s) :ConnException(s) {}
};
#endif
