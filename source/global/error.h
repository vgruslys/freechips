#ifndef __ERROR__
#define __ERROR__

#include <exception>
#include <cstdio>

class HistoryError : public std::exception {
    public:
        HistoryError(const char*);
        const char* what() const throw();

    private:
        const char* _message;
};

class CardError : public std::exception {
    public:
        CardError(const char*);
        const char* what() const throw();

    private:
        const char* _message;
};

class CommunicatorError : public std::exception {
    public:
        CommunicatorError(const char*);
        const char* what() const throw();

    private:
        const char* _message;
};

class LoggerError : public std::exception {
    public:
        LoggerError(const char*);
        const char* what() const throw();

    private:
        const char* _message;
};

class DefinableError : public std::exception {
    public:
        DefinableError(const char*);
        const char* what() const throw();

    private:
        const char* _message;
};

class BlindsError : public std::exception {
    public:
        BlindsError(const char*);
        const char* what() const throw();

    private:
        const char* _message;
};

#endif
