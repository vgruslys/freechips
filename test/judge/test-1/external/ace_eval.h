#include <stdint.h>
#define Card uint32_t

Card E(Card h[]); 

#define ACEHAND 5
static inline Card ACE_makecard(int i){return 1<<(2*(i%13)+6)|1<<(i/13);}
#define ACE_addcard(h,c)  h[c&7]+=c,h[3]|=c 
#define ACE_evaluate(h)   E((h))
#define ACE_rank(r)       ((r)>>28)

