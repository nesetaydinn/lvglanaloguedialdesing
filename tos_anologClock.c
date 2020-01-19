/**
 * @file tos_anologClock.c
 *
 */

 /*********************
  *      INCLUDES
  *********************/
#include "tos_anologClock.h"
#include "math.h"

#if TOS_ANOLOG_CLOCK != 0 //lv_conf.h dosyasına ekle
/*
#if TOS_ANOLOG_CLOCK != 0
		lv_style_t * tos_anologClock;
#endif //lv_theme.h a ekle
*/


  /*********************
   *      DEFINES
   *********************/
#define LV_OBJX_NAME "tos_anolog_clock"

   /**********************
	*      TYPEDEFS
	**********************/

	/**********************
	 *  STATIC PROTOTYPES
	 **********************/
static bool tos_anologClock_design(lv_obj_t * anologclock, const lv_area_t * mask, lv_design_mode_t mode);
static lv_res_t tos_anologClock_signal(lv_obj_t * anologclock, lv_signal_t sign, void * param);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_signal_cb_t ancestor_signal;

/**********************
 *      MACROS
 **********************/
const int anologClock_x =136;
const int anologClock_y = 136;
/***********************
*STYLES
************************/
static lv_style_t clockStaticStyle1;
static lv_style_t clockStaticStyle2;
 /**********************
  *   GLOBAL FUNCTIONS
  **********************/

  /**
   * Create a line meter objects
   * @param par pointer to an object, it will be the parent of the new line meter
   * @param copy pointer to a line meter object, if not NULL then the new object will be copied from
   * it
   * @return pointer to the created line meter
   */
lv_obj_t * tos_anologClock_create(lv_obj_t * par, const lv_obj_t * copy)
{
	LV_LOG_TRACE("Anolog Clock Create Start");

	/*Anolog Saatin atalarını oluşturma*/
	lv_obj_t * new_anologClock = lv_obj_create(par, copy);
	LV_ASSERT_MEM(new_anologClock);
	if (new_anologClock == NULL) return NULL;

	if (ancestor_signal == NULL) ancestor_signal = lv_obj_get_signal_cb(new_anologClock);

	/*Anolog Saatin tipine özgü genişletilmiş verileri ayırma*/
	tos_anologClock_ext_t * ext = lv_obj_allocate_ext_attr(new_anologClock, sizeof(tos_anologClock_ext_t));
	LV_ASSERT_MEM(ext);
	if (ext == NULL) return NULL;

	/*tahsis edilen ext yi baslatir */
	ext->seconds=0;
	ext->minutes = 0;
	ext->hours = 0;


	/*Sinyal ve tasarim islemleri kopyalanmaz onlari burada ayarlayin*/
	lv_obj_set_signal_cb(new_anologClock, tos_anologClock_signal);
	lv_obj_set_design_cb(new_anologClock, tos_anologClock_design);

	
	if (copy == NULL) {
		lv_obj_set_size(new_anologClock, 272, 272);

		/*bos stili alir*/
		lv_theme_t * th = lv_theme_get_current();
		if (th) {
			tos_anologClock_set_style(new_anologClock, TOS_ANOLOGCLOCK_STYLE_MAIN, th->style.tos_anologClock);
		}
		else {
			tos_anologClock_set_style(new_anologClock, TOS_ANOLOGCLOCK_STYLE_MAIN, &lv_style_pretty_color);
		}
	}
	/*Mevcut Analog saati kopyalar*/
	else {
		tos_anologClock_ext_t * copy_ext = lv_obj_get_ext_attr(copy);
		ext->seconds = copy_ext->seconds;
		ext->minutes = copy_ext->minutes;
		ext->hours = copy_ext->hours;

		/*yeni sinyal fonksiyonu ile stili yeniler*/
		lv_obj_refresh_style(new_anologClock);
	}

	LV_LOG_INFO("Anolog Clock Oluşturuldu.");

	return new_anologClock;
}

/*=====================
 * Setter functions
 *====================*/

float tos_anologClock_mapValue(float value, int start1, int stop1, int start2, float stop2) {
	return (float)((float)value - (float)start1) / ((float)stop1 - (float)start1) * ((float)stop2 - (float)start2) + (float)start2;
}
float tos_anologClock_normalize(int value, int start, int stop) {
	return (float)mapValue(value, start, stop, 0, 1);
}



/*=====================
 * Getter functions
 *====================*/

int16_t tos_anologClock_get_sec_value(const lv_obj_t * anologclock)
{
	LV_ASSERT_OBJ(anologclock, LV_OBJX_NAME);

	tos_anologClock_ext_t * ext = lv_obj_get_ext_attr(anologclock);
	return ext->seconds;
}
int16_t tos_anologClock_get_min_value(const lv_obj_t * anologclock)
{
	LV_ASSERT_OBJ(anologclock, LV_OBJX_NAME);

	tos_anologClock_ext_t * ext = lv_obj_get_ext_attr(anologclock);
	return ext->minutes;
}

int16_t tos_anologClock_get_hour_value(const lv_obj_t * anologclock)
{
	LV_ASSERT_OBJ(anologclock, LV_OBJX_NAME);

	tos_anologClock_ext_t * ext = lv_obj_get_ext_attr(anologclock);
	return ext->hours;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static bool tos_anologClock_design(lv_obj_t * anologclock, const lv_area_t * mask, lv_design_mode_t mode)
{	
	const int margin = 5;
	const int lineWidht = 10; //Çizgi uzunluğu
	const int innermargin = 15; //kadranlar arası boşluk
	const int radiusHours = (anologClock_x - margin);
	const int radiusMinutes = radiusHours - lineWidht - innermargin;
	const int radiusSecond = radiusMinutes - lineWidht - innermargin;

	/*Return false if the object is not covers the mask_p area*/
	if (mode == LV_DESIGN_COVER_CHK) {
		return false;
	}
	/*Draw the object*/
	else if (mode == LV_DESIGN_DRAW_MAIN) {
		tos_anologClock_ext_t * ext = lv_obj_get_ext_attr(anologclock);
		lv_opa_t opa_scale = lv_obj_get_opa_scale(anologclock);

		lv_style_copy(&clockStaticStyle1, &lv_style_plain);
		clockStaticStyle1.line.width = 4;
		clockStaticStyle1.line.color = LV_COLOR_WHITE;
		clockStaticStyle1.line.rounded = 1;

		lv_style_copy(&clockStaticStyle2, &lv_style_plain);
		clockStaticStyle2.line.width = 3;
		clockStaticStyle2.line.color = LV_COLOR_GRAY;
		clockStaticStyle2.line.rounded = 0;

		lv_point_t p1;
		lv_point_t p2;
		
		int sayac = 0;
		/*Saat*/
		for (float theta = 0; theta < TWO_PI; theta += TWO_PI / 12) {
			float cosin = cos(theta);
			float sinus = sin(theta);

			p1.x = (anologClock_x + (radiusHours * cosin));
			p1.y = (anologClock_y + (radiusHours * sinus));
			p2.x = (anologClock_x + ((radiusHours - lineWidht) * cosin));
			p2.y = (anologClock_y + ((radiusHours - lineWidht) * sinus));
			sayac++;
			
			if (1 == sayac % 3) {
				
				lv_draw_line(&p1, &p2, mask, &clockStaticStyle1, opa_scale);
				
			}
			else {
				lv_draw_line(&p1, &p2, mask, &clockStaticStyle2, opa_scale);
			}
			if (12 == sayac) {
				sayac = 0;
			}
		}
		/*Dakika*/
		for (float theta = 0; theta < TWO_PI; theta += TWO_PI / 60) {
			float cosin = cos(theta);
			float sinus = sin(theta);

			p1.x = (anologClock_x + radiusMinutes * cosin);
			p1.y = (anologClock_y + radiusMinutes * sinus);
			p2.x = (anologClock_x + (radiusMinutes - lineWidht) * cosin);
			p2.y = (anologClock_y + (radiusMinutes - lineWidht) * sinus);
			sayac++;
			

			lv_draw_line(&p1, &p2, mask, &clockStaticStyle2, opa_scale);

		}
		/*Saniye*/
		for (float theta = 0; theta < TWO_PI; theta += TWO_PI / 60) {
			float cosin = cos(theta);
			float sinus = sin(theta);

			p1.x = (anologClock_x + radiusSecond * cosin);
			p1.y = (anologClock_y + radiusSecond * sinus);
			p2.x = (anologClock_x + (radiusSecond - lineWidht) * cosin);
			p2.y = (anologClock_y + (radiusSecond - lineWidht) * sinus);
			sayac++;
			

			lv_draw_line(&p1, &p2, mask, &clockStaticStyle2, opa_scale);

		}
		
	}
	/*Post draw when the children are drawn*/
	else if (mode == LV_DESIGN_DRAW_POST) {
	}

	return true;
}

/**
 * Signal function of the line meter
 * @param anologClock pointer to a line meter object
 * @param sign a signal type from lv_signal_t enum
 * @param param pointer to a signal specific variable
 * @return LV_RES_OK: the object is not deleted in the function; LV_RES_INV: the object is deleted
 */
static lv_res_t tos_anologClock_signal(lv_obj_t * anologclock, lv_signal_t sign, void * param)
{
	lv_res_t res;

	/* Include the ancient signal function */
	res = ancestor_signal(anologclock, sign, param);
	if (res != LV_RES_OK) return res;
	if (sign == LV_SIGNAL_GET_TYPE) return lv_obj_handle_get_type_signal(param, LV_OBJX_NAME);

	if (sign == LV_SIGNAL_CLEANUP) {
		/*Nothing to cleanup. (No dynamically allocated memory in 'ext')*/
	}
	else if (sign == LV_SIGNAL_STYLE_CHG) {
		lv_obj_refresh_ext_draw_pad(anologclock);
	}
	else if (sign == LV_SIGNAL_REFR_EXT_DRAW_PAD) {
		const lv_style_t * style = tos_anologClock_get_style(anologclock, TOS_ANOLOGCLOCK_STYLE_MAIN);
		anologclock->ext_draw_pad = LV_MATH_MAX(anologclock->ext_draw_pad, style->line.width);
	}

	return res;
}



#endif
