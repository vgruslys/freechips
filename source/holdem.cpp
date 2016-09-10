#include "error.h"
#include "global_types.h"
#include "history.h"
#include "communicator.h"
#include "logger.h"

#include <cstdio>

int main()
{
    Logger logger(stderr);
    logger.setShowAllLevels(1);
    logger.log(L_SYSTEM, "Logger is ON\n");
    
    History history(logger);
    Communicator communicator(stdin, stdout, history, logger);

    while(1) {
        communicator.awaitInstructions();
        communicator.call();
        fflush(stdout);
    }
    return 0;
}

