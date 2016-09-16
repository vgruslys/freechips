#include "test.h"

#include <cstdio>

int runTest(FILE* stream_in, FILE* stream_out, Randomizer* randomizer)
{
    char cmd;
    while (fscanf(stream_in, " %c", &cmd) > 0) {
        
        switch (cmd) {
            
            int result;
            /*
            case 'S': {
                int seed;
                if (fscanf(stream_in, "%d", &seed) < 1)
                    return 1;
                
                try {
                    randomizer->setSeed(seed);
                }
                catch (...) {
                    fprintf(stream_out, "E");
                    return 0;
                }
                
                break;
            }
            */
            
            case 'I': {
                
                int lb, ub;
                if (fscanf(stream_in, "%d%d", &lb, &ub) < 2)
                    return 1;
                
                try {
                    result = randomizer->randInt(lb, ub);
                }
                catch (...) {
                    fprintf(stream_out, "E\n");
                    return 0;
                }
                fprintf(stream_out, "%d\n", result);
                
                break;
            }
            
            case 'W': {
                
                int t_weights;
                if (fscanf(stream_in, "%d", &t_weights) < 1)
                    return 1;
                if (t_weights < 1)
                    return 1;

                int* weights = new int[t_weights];
                for (int i = 0; i < t_weights; ++i) {
                    if (fscanf(stream_in, "%d", &weights[i]) < 1) {
                        delete weights;
                        return 0;
                    }
                }

                try {
                    result = randomizer->randWeights(weights, t_weights);
                }
                catch (...) {
                    delete weights;
                    fprintf(stream_out, "E\n");
                    return 0;
                }
                delete weights;
                fprintf(stream_out, "%d\n", result);

                break;
            }

            default:
                return 1;
        }
    }
    return 0;
}

int main()
{
    Randomizer* randomizer = selectRandomizer();
    int exit_code = runTest(stdin, stdout, randomizer);
    return exit_code;
}
