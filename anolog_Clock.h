

#ifndef LV_ANALOG_CLOCK
#define LV_ANALOG_CLOCK

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lvgl.h"
#include "lv_ex_conf.h"
#else
#include "../../lvgl/lvgl.h"
#include "../../lv_ex_conf.h"
#endif
#include "tosbaga.h"
#if TOS_TEST

/*Ana Fonksiyonlari*/
void analog_Clock(void);
void tos_ex_analogclktest(void);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
#endif
