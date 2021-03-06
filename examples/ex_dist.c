#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_fosc *osc;
    sp_ftbl *ft;
    sp_dist *ds;
    sp_osc *lfo;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT osc = 0, lfo = 0;
    sp_osc_compute(sp, ud->lfo, NULL, &lfo);
    lfo = 7 * (0.5 * (lfo + 1));
    ud->osc->indx = lfo;
    sp_fosc_compute(sp, ud->osc, NULL, &osc);
    sp_dist_compute(sp, ud->ds, &osc, &sp->out[0]);
}

int main() {
    srand(time(NULL));
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_fosc_create(&ud.osc);
    sp_dist_create(&ud.ds);
    sp_osc_create(&ud.lfo);

    sp_gen_sine(sp, ud.ft);
    sp_fosc_init(sp, ud.osc, ud.ft);
    ud.osc->freq = 60;
    sp_dist_init(sp, ud.ds);
    ud.ds->pregain = 10;
    sp_osc_init(sp, ud.lfo, ud.ft, 0);
    ud.lfo->freq = 0.5;

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_ftbl_destroy(&ud.ft);
    sp_fosc_destroy(&ud.osc);
    sp_dist_destroy(&ud.ds);
    sp_osc_destroy(&ud.lfo);
    sp_destroy(&sp);
    return 0;
}
