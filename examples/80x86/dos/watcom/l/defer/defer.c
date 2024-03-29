/*****************************************************************************
* Product: Deferred Event state pattern example
* Last Updated for Version: 5.0.0
* Date of the Last Update:  Sep 03, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qp_port.h"
#include "bsp.h"

#include <stdio.h>           /* this example uses printf() to report status */

Q_DEFINE_THIS_FILE

/*..........................................................................*/
enum TServerSignals {
    NEW_REQUEST_SIG = Q_USER_SIG,                 /* the new request signal */
    RECEIVED_SIG,                          /* the request has been received */
    AUTHORIZED_SIG,                      /* the request has been authorized */
    TERMINATE_SIG                              /* terminate the application */
};
/*..........................................................................*/
typedef struct RequestEvtTag {
    QEvt super;                                     /* derive from QEvt */
    uint8_t ref_num;                     /* reference number of the request */
} RequestEvt;

/*..........................................................................*/
typedef struct TServerTag {             /* Transaction Server active object */
    QActive super;                                   /* derive from QActive */

    QEQueue requestQueue;    /* native QF queue for deferred request events */
    QEvt const *requestQSto[3];      /* storage for deferred queue buffer */

    QTimeEvt receivedEvt;                   /* private time event generator */
    QTimeEvt authorizedEvt;                 /* private time event generator */
} TServer;

void TServer_ctor(TServer *me);                         /* the default ctor */
                                          /* hierarchical state machine ... */
static QState TServer_initial    (TServer *me, QEvt const *e);
static QState TServer_idle       (TServer *me, QEvt const *e);
static QState TServer_busy       (TServer *me, QEvt const *e);
static QState TServer_receiving  (TServer *me, QEvt const *e);
static QState TServer_authorizing(TServer *me, QEvt const *e);
static QState TServer_final      (TServer *me, QEvt const *e);


/*..........................................................................*/
void TServer_ctor(TServer *me) {                        /* the default ctor */
    QActive_ctor(&me->super, (QStateHandler)&TServer_initial);
    QEQueue_init(&me->requestQueue,
                 me->requestQSto, Q_DIM(me->requestQSto));
    QTimeEvt_ctor(&me->receivedEvt,   RECEIVED_SIG);
    QTimeEvt_ctor(&me->authorizedEvt, AUTHORIZED_SIG);
}
/* HSM definition ----------------------------------------------------------*/
QState TServer_initial(TServer *me, QEvt const *e) {
    (void)e;           /* avoid the compiler warning about unused parameter */
    return Q_TRAN(&TServer_idle);
}
/*..........................................................................*/
QState TServer_final(TServer *me, QEvt const *e) {
    (void)me;          /* avoid the compiler warning about unused parameter */
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("final-ENTRY;\nBye!Bye!\n");
            QF_stop();                         /* terminate the application */
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState TServer_idle(TServer *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("idle-ENTRY;\n");
                                /* recall the request from the requestQueue */
            if (QActive_recall((QActive *)me, &me->requestQueue)) {
                printf("Request recalled\n");
            }
            else {
                printf("No deferred requests\n");
            }
            return Q_HANDLED();
        }
        case NEW_REQUEST_SIG: {
            printf("Processing request #%d\n",
                   (int)((RequestEvt const *)e)->ref_num);
            return Q_TRAN(&TServer_receiving);
        }
        case TERMINATE_SIG: {
            return Q_TRAN(&TServer_final);
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState TServer_busy(TServer *me, QEvt const *e) {
    switch (e->sig) {
        case NEW_REQUEST_SIG: {
            if (QActive_defer((QActive *)me, &me->requestQueue, e)) {
                printf("Request #%d deferred;\n",
                       (int)((RequestEvt const *)e)->ref_num);
            }
            else {
                printf("Request #%d IGNORED;\n",
                       (int)((RequestEvt const *)e)->ref_num);
                /* notify the request sender that his request was denied... */
            }
            return Q_HANDLED();
        }
        case TERMINATE_SIG: {
            return Q_TRAN(&TServer_final);
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState TServer_receiving(TServer *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("receiving-ENTRY;\n");
                                            /* one-shot timeout in 1 second */
            QTimeEvt_postIn(&me->receivedEvt, (QActive *)me,
                            BSP_TICKS_PER_SEC);
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            QTimeEvt_disarm(&me->receivedEvt);
            return Q_HANDLED();
        }
        case RECEIVED_SIG: {
            return Q_TRAN(&TServer_authorizing);
        }
    }
    return Q_SUPER(&TServer_busy);
}
/*..........................................................................*/
QState TServer_authorizing(TServer *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("authorizing-ENTRY;\n");
                                           /* one-shot timeout in 2 seconds */
            QTimeEvt_postIn(&me->authorizedEvt, (QActive *)me,
                            2*BSP_TICKS_PER_SEC);
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            QTimeEvt_disarm(&me->authorizedEvt);
            return Q_HANDLED();
        }
        case AUTHORIZED_SIG: {
            return Q_TRAN(&TServer_idle);
        }
    }
    return Q_SUPER(&TServer_busy);
}

// test harness ============================================================*/

// Local-scope objects -------------------------------------------------------
static TServer l_tserver;                      /* the TServer active object */
static QEvt const *l_tserverQSto[10];  /* Event queue storage for TServer */
static RequestEvt l_smlPoolSto[10];       /* buffer for the small-size pool */

/*..........................................................................*/
int main(int argc, char *argv[]) {
    printf("Deferred Event state pattern\nQEP version: %s\nQF  version: %s\n"
           "Press n to generate a new request\n"
           "Press ESC to quit...\n",
           QEP_getVersion(), QF_getVersion());

    BSP_init(argc, argv);                             /* initialize the BSP */

    QF_init();       /* initialize the framework and the underlying RTOS/OS */


    /* publish-subscribe not used, no call to QF_psInit() */

                                               /* initialize event pools... */
    QF_poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));

                                             /* start the active objects... */
    TServer_ctor(&l_tserver);
    QActive_start((QActive *)&l_tserver, 1,
                  l_tserverQSto, Q_DIM(l_tserverQSto),
                  (void *)0, 0, (QEvt *)0);

    return QF_run();                              /* run the QF application */
}
/*..........................................................................*/
void BSP_onKeyboardInput(uint8_t key) {
    switch (key) {
        case 177: {                                    /* 'n': new request? */
            static uint8_t reqCtr = 0;                /* count the requests */
            RequestEvt *e = Q_NEW(RequestEvt, NEW_REQUEST_SIG);
            e->ref_num = (++reqCtr);            /* set the reference number */
                                  /* post directly to TServer active object */
            QActive_postFIFO((QActive *)&l_tserver, (QEvt *)e);
            break;
        }
        case 129: {                                         /* ESC pressed? */
            static QEvt const terminateEvt = { TERMINATE_SIG, 0, 0 };
            QActive_postFIFO((QActive *)&l_tserver, &terminateEvt);
            break;
        }
    }
}
