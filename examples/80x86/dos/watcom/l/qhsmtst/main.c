/*****************************************************************************
* Product: QHsmTst Example, DOS, Large model
* Last Updated for Version: 5.2.0
* Date of the Last Update:  Dec 24, 2013
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
#include "qp_port.h"                                         /* the QP port */
#include "qhsmtst.h"

#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

Q_DEFINE_THIS_FILE

/* local objects -----------------------------------------------------------*/
static FILE *l_outFile = (FILE *)0;
static void dispatch(QSignal sig);

/*..........................................................................*/
int main(int argc, char *argv[]) {
    QHsmTst_ctor();                       /* instantiate the QHsmTst object */

    if (argc > 1) {                                  /* file name provided? */
        l_outFile = fopen(argv[1], "w");
    }

    if (l_outFile == (FILE *)0) {                   /* interactive version? */
        l_outFile = stdout;            /* use the stdout as the output file */

        printf("QHsmTst example, built on %s at %s,\n"
               "QEP: %s.\nPress ESC to quit...\n",
               __DATE__, __TIME__, QEP_getVersion());

        QHsm_init(the_hsm, (QEvt *)0);      /* the top-most initial tran. */

        for (;;) {                                            /* event loop */
            QEvt e;
            int c;

            printf("\n>");
            c = _getche();    /* get a character from the console with echo */
            printf(": ");

            if ('a' <= c && c <= 'i') {                        /* in range? */
                e.sig = (QSignal)(c - 'a' + A_SIG);
            }
            else if ('A' <= c && c <= 'I') {                   /* in range? */
                e.sig = (QSignal)(c - 'A' + A_SIG);
            }
            else if (c == '\33') {                          /* the ESC key? */
                e.sig = TERMINATE_SIG;    /* terminate the interactive test */
            }
            else {
                e.sig = IGNORE_SIG;
            }

            QHsm_dispatch(the_hsm, &e);               /* dispatch the event */
        }
    }
    else {                                                 /* batch version */
        printf("QHsmTst example, built on %s at %s, QEP %s\n"
               "output saved to %s\n",
               __DATE__, __TIME__, QEP_getVersion(),
               argv[1]);

        fprintf(l_outFile, "QHsmTst example, QEP %s\n",
                QEP_getVersion());

        QHsm_init(the_hsm, (QEvt *)0);      /* the top-most initial tran. */

                                       /* testing of dynamic transitions... */
        dispatch(A_SIG);
        dispatch(B_SIG);
        dispatch(D_SIG);
        dispatch(E_SIG);
        dispatch(I_SIG);
        dispatch(F_SIG);
        dispatch(I_SIG);
        dispatch(I_SIG);
        dispatch(F_SIG);
        dispatch(A_SIG);
        dispatch(B_SIG);
        dispatch(D_SIG);
        dispatch(D_SIG);
        dispatch(E_SIG);
        dispatch(G_SIG);
        dispatch(H_SIG);
        dispatch(H_SIG);
        dispatch(C_SIG);
        dispatch(G_SIG);
        dispatch(C_SIG);
        dispatch(C_SIG);

        fclose(l_outFile);
    }

    return 0;
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    exit(-1);
}
/*..........................................................................*/
void BSP_display(char const *msg) {
    fprintf(l_outFile, msg);
}
/*..........................................................................*/
void BSP_exit(void) {
    printf("Bye, Bye!");
    _exit(0);
}
/*..........................................................................*/
static void dispatch(QSignal sig) {
    QEvt e;
    Q_REQUIRE((A_SIG <= sig) && (sig <= I_SIG));
    e.sig = sig;
    fprintf(l_outFile, "\n%c:", 'A' + sig - A_SIG);
    QHsm_dispatch(the_hsm, &e);                       /* dispatch the event */
}

