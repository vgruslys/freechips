#include "error.h"

HistoryError :: HistoryError(const char* message)
{
    _message = message;
}

const char* HistoryError :: what() const throw()
{
    return _message;
}



CardError :: CardError(const char* message)
{
    _message = message;
}

const char* CardError :: what() const throw()
{
    return _message;
}

RandomError :: RandomError(const char* message)
{
    _message = message;
}

const char* RandomError :: what() const throw()
{
    return _message;
}

DefinableError :: DefinableError(const char* message)
{
    _message = message;
}

const char* DefinableError :: what() const throw()
{
    return _message;
}



BlindsError :: BlindsError(const char* message)
{
    _message = message;
}

const char* BlindsError :: what() const throw()
{
    return _message;
}



CommunicatorError :: CommunicatorError(const char* message)
{
    _message = message;
}

const char* CommunicatorError :: what() const throw()
{
    return _message;
}



LoggerError :: LoggerError (const char* message)
{
    _message = message;
}

const char* LoggerError :: what() const throw()
{
    return _message;
}



