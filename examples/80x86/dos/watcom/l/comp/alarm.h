/*****************************************************************************
* Product: Orthogonal Component state pattern example
* Last Updated for Version: 4.5.00
* Date of the Last Update:  May 18, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
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
#ifndef alarm_h
#define alarm_h

typedef struct AlarmTag {         /* the HSM version of the Alarm component */
    QFsm super;                                         /* derive from QFsm */
    uint32_t alarm_time;
} Alarm;

void Alarm_ctor(Alarm *me);
#define Alarm_init(me_)           QFsm_init    ((QFsm *)(me_), (QEvt *)0)
#define Alarm_dispatch(me_, e_)   QFsm_dispatch((QFsm *)(me_), e_)

#endif                                                           /* alarm_h */
