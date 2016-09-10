#ifndef __DEFINABLE__
#define __DEFINABLE__

#include "error.h"

template <class T>
class Definable {
    public:
        Definable();
        const T& get() const;
        void set(T);
        int isDefined() const;
        
    private:
        T _value;
        int _defined;
};

template <class T>
const T& Definable<T> :: get() const
{
    if (!isDefined())
        throw DefinableError("Accessing undefined variable");
    else
        return _value;
}

template <class T>
void Definable<T> :: set(T value)
{
    if (isDefined())
        throw DefinableError("Redefining a variable");
    else {
        _value = value;
        _defined = 1;
    }
}

template <class T>
int Definable<T> :: isDefined() const
{
    return _defined;
}

template <class T>
Definable<T> :: Definable()
{
    _defined = 0;
}

#endif
