/*
 * comb
 * 
 * This code has been extracted from the Csound opcode "comb".
 * It has been modified to work as a Soundpipe module.
 * 
 * Original Author(s): Barry Vercoe, John ffitch
 * Year: 1991
 * Location: OOps/ugens6.c
 *
 */

#include <math.h>
#include <stdlib.h>
#include "soundpipe.h"

#define log001 (-(SPFLOAT)6.9078)    /* log(.001) */

int sp_comb_create(sp_comb **p)
{
    *p = malloc(sizeof(sp_comb));
    return SP_OK;
}

int sp_comb_destroy(sp_comb **p)
{
    sp_comb *pp = *p;
    sp_auxdata_free(&pp->aux);
    free(*p);
    return SP_OK;
}

int sp_comb_init(sp_data *sp, sp_comb *p, SPFLOAT looptime)
{
    p->revtime = 3.5;
    p->looptime = looptime;
    p->bufsize = (uint32_t) (0.5 + looptime * sp->sr);
    sp_auxdata_alloc(&p->aux, p->bufsize * sizeof(SPFLOAT));
    p->prvt = 0.0;
    p->coef = 0.0;
    p->bufpos = 0;
    return SP_OK;
}

int sp_comb_compute(sp_data *sp, sp_comb *p, SPFLOAT *in, SPFLOAT *out)
{
    SPFLOAT tmp = 0;
    SPFLOAT coef = p->coef;
    SPFLOAT outsamp = 0;

    if(p->prvt != p->revtime) {
        p->prvt = p->revtime;
        SPFLOAT exp_arg = (SPFLOAT) (log001 * p->looptime / p->prvt);
        if(exp_arg < -36.8413615) {
            coef = p->coef = 0;
        } else {
            coef = p->coef = exp(exp_arg);
        }
    }
    sp_auxdata_getbuf(&p->aux, p->bufpos, &outsamp);
    *out = outsamp;
    tmp = outsamp;
    tmp *= coef;
    tmp += *in;
    sp_auxdata_setbuf(&p->aux, p->bufpos, &tmp);

    p->bufpos++;
    p->bufpos %= p->bufsize; 
    return SP_OK;
}
