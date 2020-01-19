/**
 * @file tos_anologClock.h
 *
 */

#ifndef TOS_ANOLOGCLOCK_H
#define TOS_ANOLOGCLOCK_H

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


#if TOS_ANOLOG_CLOCK != 0

#include "../../lvgl/src/lv_core/lv_obj.h"

	 /*********************
	  *      DEFINES
	  *********************/

	  /**********************
	   *      TYPEDEFS
	   **********************/
	
	typedef struct
	{
		int16_t seconds;
		int16_t minutes;
		int16_t hours;		
	} tos_anologClock_ext_t;

	/*Styles*/
	enum {
		TOS_ANOLOGCLOCK_STYLE_MAIN,
	};
	typedef uint8_t tos_anologClock_style_t;

#define PI 3.14159265358979323846
#define TWO_PI 6.28318530717958647692
#define HALF_PI 1.57079632679489661923‬
	/**********************
	 * GLOBAL PROTOTYPES
	 **********************/
	lv_obj_t * tos_anologClock_create(lv_obj_t * par, const lv_obj_t * copy);

	/*=====================
	 * Setter functions
	 *====================*/

	float tos_anologClock_mapValue(float value, int start1, int stop1, int start2, float stop2);
	float tos_anologClock_normalize(int value, int start, int stop);

	static inline void tos_anologClock_set_style(lv_obj_t * anologClock, tos_anologClock_style_t type, lv_style_t * style)
	{
		(void)type; /*Unused*/
		lv_obj_set_style(anologClock, style);
	}

	/*=====================
	 * Getter functions
	 *====================*/


	/**
	*tos_anologClock_get_sec_value(); Saniye değerini,
	*tos_anologClock_get_min_value(); Dakika değerini,
	*tos_anologClock_get_hour_value(); Saat Değerini Alır.
	*/
	int16_t tos_anologClock_get_sec_value(const lv_obj_t * anologclock);
	int16_t tos_anologClock_get_min_value(const lv_obj_t * anologclock);
	int16_t tos_anologClock_get_hour_value(const lv_obj_t * anologclock);

	
	/**
	 * Get the style of a line meter
	 * @param anologClock pointer to a line meter object
	 * @param type which style should be get (can be only `TOS_ANOLOGCLOCK_STYLE_MAIN`)
	 * @return pointer to the line meter's style
	 */
	static inline const lv_style_t * tos_anologClock_get_style(const lv_obj_t * anologclock, tos_anologClock_style_t type)
	{
		(void)type; /*Unused*/
		return lv_obj_get_style(anologclock);
	}

	/**********************
	 *      MACROS
	 **********************/

#endif /*tos_anologClock*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_LMETER_H*/
