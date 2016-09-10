#ifndef __COMMUNICATOR__
#define __COMMUNICATOR__

#include "error.h"
#include "global_types.h"
#include "history.h"
#include "logger.h"

#include <cstdio>
#include <utility>
#include <sys/time.h>

typedef std::pair<Command, Command> CommandPair;


class Communicator {
    public:
        Communicator(FILE*, FILE*, History&, const Logger&);

        History& getHistory();

        void check();
        void call();
        void fold();
        void raise(int);
        void awaitInstructions();

        long long getCurrTime() const;

    private:
        CommandPair readHeader() const;
        int doCommand(Command, Command);

        void setRequestTime(long long);
        long long getRequestTime() const;

        void setResponseTime(long long);
        long long getResponseTime() const;

        void setOwnName(const char*);
        const char* getOwnName() const;

        void setOppName(const char*);
        const char* getOppName() const;

        void setHistory(History& history);

        const Logger& getLogger() const;

        FILE* getStreamIn() const;
        FILE* getStreamOut() const;

        int hasOwnName() const;
        int hasOppName() const;

        int readInt() const;
        char* readStr() const;
        void dumpInt() const;
        void dumpStr() const;
        
        void restartTimer();
        int readTimer();


        FILE* _stream_in;
        FILE* _stream_out;

        History& _history;
        const Logger& _logger;

        long long _request_time;
        long long _response_time;

        char _own_name[256];
        char _opp_name[256];
        int _own_name_saved;
        int _opp_name_saved;
};

#endif
