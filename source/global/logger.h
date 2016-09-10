#ifndef __LOGGER__
#define __LOGGER__

#include "error.h"
#include "global_types.h"
#include "card.h"
#include "sets.h"

#include <cstdarg>
#include <cstdio>

class Logger {
    public:
        Logger(FILE*);

        void setShowAllLevels(int);
        void setShowLevel(LogLevel, int);
        
        int getShowLevel(LogLevel) const;
        
        void log(LogLevel, const char*, ...) const;

        const char* commandToStr(Command) const;
        const char* streetToStr(Street) const;
        const char* sideToStr(Side) const;
        const char* actionTypeToStr(ActionType) const;
        const char* logLevelToStr(LogLevel) const; 
        const char* handValueToStr(HandValue) const;

    private:
        FILE* getStream() const;

        FILE* _stream;

        int _show_system;
        int _show_debug;
        int _show_mundane;
};


#endif
