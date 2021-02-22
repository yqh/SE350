/**
 * @brief timer.h - Timer header file
 * @author Y. Huang
 * @date 2021/02/20
 */
#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>

extern uint32_t timer_init  (uint8_t n_timer);  /* initialize timer n_timer */
extern void     timer0_iproc(void);

#endif /* ! _TIMER_H_ */
