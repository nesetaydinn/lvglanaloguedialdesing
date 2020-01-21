/**
 * @file tos_analogClock.h
 *
 */

#ifndef TOS_ANALOGCLOCK_H
#define TOS_ANALOGCLOCK_H

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


#if TOS_ANALOG_CLOCK != 0

#include "../../lvgl/src/lv_core/lv_obj.h"

	/*Degiskenler*/
	lv_obj_t * tos_analogClock_create(lv_obj_t * par, const lv_obj_t * copy);

	int tos_saat;
	int tos_dakika;
	int tos_saniye;
	/*Structlar*/
	typedef struct
	{
		int16_t seconds;
		int16_t minutes;
		int16_t hours;		
	} tos_analogClock_ext_t;

	/*Stiller*/
	enum {
		TOS_ANALOGCLOCK_STYLE_MAIN,
	};
	typedef uint8_t tos_analogClock_style_t;
	/*define'lar*/
	#define PI 3.14159265358979323846
	#define TWO_PI 6.28318530717958647692
	#define HALF_PI 1.57079632679489661923‬





	void tos_analogClock_animation(lv_obj_t * analogsaat);
	float tos_analogClock_mapValue(float value, int start1, int stop1, int start2, float stop2);
	float tos_analogClock_normalize(int value, int start, int stop);

	static inline void tos_analogClock_set_style(lv_obj_t * analogClock, tos_analogClock_style_t type, lv_style_t * style)
	{
		(void)type;
		lv_obj_set_style(analogClock, style);
	}

	static inline const lv_style_t * tos_analogClock_get_style(const lv_obj_t * analogclock, tos_analogClock_style_t type)
	{
		(void)type; /*Unused*/
		return lv_obj_get_style(analogclock);
	}


#endif /*tos_analogClock*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
