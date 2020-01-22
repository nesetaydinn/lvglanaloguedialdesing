/**
 * @file tos_analogClock.c
 *
 */

 /*********************
  *      INCLUDES
  *********************/
#include "tos_analogClock.h"
#include "math.h"
#include "../../lvgl/src/lv_draw/lv_draw.h"
#include "../../lvgl/src/lv_themes/lv_theme.h"
#include "../../lvgl/src/lv_core/lv_group.h"
#include "../../lvgl/src/lv_misc/lv_math.h"

#if TOS_ANALOG_CLOCK != 0 
  
  /*lv_conf.h dosyasinda;
  *
  #define TOS_ANALOG_CLOCK 1 olacak 
  *
  * lv_theme.h dosyasinda;
  #if TOS_ANALOG_CLOCK != 0
		  lv_style_t * tos_analogClock;
  #endif 
  */


#define LV_OBJX_NAME "tos_analog_clock"


/*Fonksiyonlar*/
static void tos_analogClock_static(lv_obj_t * analogclock, const lv_area_t * mask, lv_design_mode_t mode);
static void tos_analogClock_dynamic(lv_obj_t * analogclock, const lv_area_t * mask, lv_design_mode_t mode);
static bool tos_analogClock_design(lv_obj_t * analogclock, const lv_area_t * mask, lv_design_mode_t mode);

/*Sabitler*/


const int analogClock_x = 136;
const int analogClock_y = 136;
/*Tanimlamalar*/
static lv_signal_cb_t ancestor_signal;
lv_point_t p1;
lv_point_t p2;
lv_opa_t opa_scale;
lv_coord_t x_ofs;
lv_coord_t y_ofs;
tos_analogClock_ext_t * ext;
/*Stil Tanimlamalari*/
static lv_style_t clockStaticStyle1;
static lv_style_t clockStaticStyle2;
static lv_style_t tos_saatstil;
static lv_style_t tos_dakikastil;
static lv_style_t tos_saniyestil;
/*Analog Kadran Olusturma*/
lv_obj_t * tos_analogClock_create(lv_obj_t * par, const lv_obj_t * copy)
{
	LV_LOG_TRACE("Analog Clock Create Start");

	/*Analog Saatin atalarını oluşturma*/
	lv_obj_t * new_analogClock = lv_obj_create(par, copy);
	lv_mem_assert(new_analogClock);
	if (new_analogClock == NULL) return NULL;

	if (ancestor_signal == NULL) ancestor_signal = lv_obj_get_signal_cb(new_analogClock);

	/*Analog Saatin tipine özgü genişletilmiş verileri ayırma*/
	 ext = lv_obj_allocate_ext_attr(new_analogClock, sizeof(tos_analogClock_ext_t));
	lv_mem_assert(ext);
	if (ext == NULL) return NULL;

	/*tahsis edilen ext yi baslatir */
	ext->seconds = 0;
	ext->minutes = 0;
	ext->hours = 0;


	/*Sinyal ve tasarim islemleri kopyalanmaz onlari burada ayarlayin*/
	lv_obj_set_design_cb(new_analogClock, tos_analogClock_design);


	if (copy == NULL) {
		lv_obj_set_size(new_analogClock, 272, 272);

		/*bos stili alir*/
		lv_theme_t * th = lv_theme_get_current();
		if (th) {
			tos_analogClock_set_style(new_analogClock, TOS_ANALOGCLOCK_STYLE_MAIN, th->style.tos_analogClock);
		}
		else {
			tos_analogClock_set_style(new_analogClock, TOS_ANALOGCLOCK_STYLE_MAIN, &lv_style_pretty_color);
		}
	}
	/*Mevcut Analog saati kopyalar*/
	else {
		tos_analogClock_ext_t * copy_ext = lv_obj_get_ext_attr(copy);
		ext->seconds = copy_ext->seconds;
		ext->minutes = copy_ext->minutes;
		ext->hours = copy_ext->hours;

		/*yeni sinyal fonksiyonu ile stili yeniler*/
		lv_obj_refresh_style(new_analogClock);
	}

	LV_LOG_INFO("Analog Clock Oluşturuldu.");

	return new_analogClock;
}


/*Animasyon Fonksiyonu*/
void tos_analogClock_animation(lv_obj_t * analogsaat)
{
	lv_mem_assert(analogsaat);
	lv_canvas_ext_t * ext = lv_obj_get_ext_attr(analogsaat);
	lv_obj_invalidate(analogsaat);
}
/*Hesaplama Fonksiyonlari*/
float tos_analogClock_mapValue(float value, int start1, int stop1, int start2, float stop2) {
	return (float)((float)value - (float)start1) / ((float)stop1 - (float)start1) * ((float)stop2 - (float)start2) + (float)start2;
}
float tos_analogClock_normalize(int value, int start, int stop) {
	return (float)tos_analogClock_mapValue(value, start, stop, 0, 1);
}
/*Cizim Fonksiyonlari*/
static void tos_analogClock_static(lv_obj_t * analogclock, const lv_area_t * mask, lv_design_mode_t mode) {
	opa_scale = lv_obj_get_opa_scale(analogclock);

	lv_style_copy(&clockStaticStyle1, &lv_style_plain);
	clockStaticStyle1.line.width = 4;
	clockStaticStyle1.line.color = LV_COLOR_WHITE;
	clockStaticStyle1.line.rounded = 1;

	lv_style_copy(&clockStaticStyle2, &lv_style_plain);
	clockStaticStyle2.line.width = 3;
	clockStaticStyle2.line.color = LV_COLOR_GRAY;
	clockStaticStyle2.line.rounded = 0;

	x_ofs = lv_obj_get_width(analogclock) / 2 + analogclock->coords.x1;
	y_ofs = lv_obj_get_height(analogclock) / 2 + analogclock->coords.y1;

	const int margin = 5;
	const int lineWidht = 10; //Çizgi uzunluğu
	const int innermargin = 15; //kadranlar arası boşluk
	const int radiusHours = (analogClock_x - margin);
	const int radiusMinutes = radiusHours - lineWidht - innermargin;
	const int radiusSecond = radiusMinutes - lineWidht - innermargin;

	int sayac = 0;
	/*Saat*/
	for (float theta = 0; theta < TWO_PI; theta += TWO_PI / 12) {
		float cosin = cos(theta);
		float sinus = sin(theta);

		p1.x = (x_ofs + (radiusHours * cosin));
		p1.y = (y_ofs + (radiusHours * sinus));
		p2.x = (x_ofs + ((radiusHours - lineWidht) * cosin));
		p2.y = (y_ofs + ((radiusHours - lineWidht) * sinus));
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

		p1.x = (x_ofs + radiusMinutes * cosin);
		p1.y = (y_ofs + radiusMinutes * sinus);
		p2.x = (x_ofs + (radiusMinutes - lineWidht) * cosin);
		p2.y = (y_ofs + (radiusMinutes - lineWidht) * sinus);
		sayac++;


		lv_draw_line(&p1, &p2, mask, &clockStaticStyle2, opa_scale);

	}
	/*Saniye*/
	for (float theta = 0; theta < TWO_PI; theta += TWO_PI / 60) {
		float cosin = cos(theta);
		float sinus = sin(theta);

		p1.x = (x_ofs + radiusSecond * cosin);
		p1.y = (y_ofs + radiusSecond * sinus);
		p2.x = (x_ofs + (radiusSecond - lineWidht) * cosin);
		p2.y = (y_ofs + (radiusSecond - lineWidht) * sinus);
		sayac++;


		lv_draw_line(&p1, &p2, mask, &clockStaticStyle2, opa_scale);

	}


}
static void tos_analogClock_dynamic(lv_obj_t * analogclock, const lv_area_t * mask, lv_design_mode_t mode) {
	ext = lv_obj_get_ext_attr(analogclock);
	opa_scale = lv_obj_get_opa_scale(analogclock);
	double hoursRadian = tos_analogClock_mapValue(tos_saat + tos_analogClock_normalize(tos_dakika + tos_analogClock_normalize(tos_saniye, 0, 60), 0, 60), 0, 12, 0, PI * 2) - (PI / 2);
	double minutesRadian = tos_analogClock_mapValue(tos_dakika + tos_analogClock_normalize(tos_saniye, 0, 60) - 1, 0, 60, 0, PI * 2) - (PI / 2);
	double secondsRadian = tos_analogClock_mapValue(tos_saniye, 0, 60, 0, PI * 2) - (PI / 2);


	x_ofs = lv_obj_get_width(analogclock) / 2 + analogclock->coords.x1;
	y_ofs = lv_obj_get_height(analogclock) / 2 + analogclock->coords.y1;

	lv_style_copy(&tos_saatstil, &lv_style_plain);
	tos_saatstil.line.width = 5;
	tos_saatstil.line.color = LV_COLOR_RED;
	tos_saatstil.line.rounded = 1;

	lv_style_copy(&tos_dakikastil, &lv_style_plain);
	tos_dakikastil.line.width = 5;
	tos_dakikastil.line.color = LV_COLOR_GREEN;
	tos_dakikastil.line.rounded = 1;

	lv_style_copy(&tos_saniyestil, &lv_style_plain);
	tos_saniyestil.line.width = 5;
	tos_saniyestil.line.color = LV_COLOR_BLUE;
	tos_saniyestil.line.rounded = 1;



	const int margin = 5;
	const int lineWidht = 10; //Çizgi uzunluğu
	const int innermargin = 15; //kadranlar arası boşluk
	const int radiusHours = (analogClock_x - margin);
	const int radiusMinutes = radiusHours - lineWidht - innermargin;
	const int radiusSecond = radiusMinutes - lineWidht - innermargin;

	
	
	// Hours
	float hours_maxRad = (float)hoursRadian + (float)(PI / 3);

	for (float theta = 0; theta < hours_maxRad; theta += TWO_PI / 12) {
		float cosin = cos(theta - (PI / 2));
		float sinus = sin(theta - (PI / 2));

		p1.x = x_ofs + (radiusHours * cosin);
		p1.y = y_ofs + (radiusHours * sinus);
		p2.x = x_ofs + ((radiusHours - lineWidht) * cosin);
		p2.y = y_ofs + ((radiusHours - lineWidht) * sinus);
		lv_draw_line(&p1, &p2, mask, &tos_saatstil, opa_scale);
	}

	//Minutes
	float minutes_maxRad = (float)minutesRadian + (float)(PI / 2);

	for (float theta = 0; theta < minutes_maxRad; theta += TWO_PI / 60) {
		float cosin = cos(theta - (PI / 2) - (PI / 30));
		float sinus = sin(theta - (PI / 2) - (PI / 30));

		p1.x = x_ofs + (radiusMinutes * cosin);
		p1.y = y_ofs + (radiusMinutes * sinus);
		p2.x = x_ofs + ((radiusMinutes - lineWidht) * cosin);
		p2.y = y_ofs + ((radiusMinutes - lineWidht) * sinus);
		lv_draw_line(&p1, &p2, mask, &tos_dakikastil, opa_scale);
	}

	//Seconds
	float seconds_maxRad = (float)secondsRadian + (float)(PI / 2);

	for (float theta = 0; theta < seconds_maxRad; theta += TWO_PI / 60) {
		float cosin = cos(theta - (PI / 2) - (PI / 30));
		float sinus = sin(theta - (PI / 2) - (PI / 30));

		p1.x = x_ofs + (radiusSecond * cosin);
		p1.y = y_ofs + (radiusSecond * sinus);
		p2.x = x_ofs + ((radiusSecond - lineWidht) * cosin);
		p2.y = y_ofs + ((radiusSecond - lineWidht) * sinus);
		lv_draw_line(&p1, &p2, mask, &tos_saniyestil, opa_scale);
	}

}
static bool tos_analogClock_design(lv_obj_t * analogclock, const lv_area_t * mask, lv_design_mode_t mode)
{


	/*Return false if the object is not covers the mask_p area*/
	if (mode == LV_DESIGN_COVER_CHK) {
		return false;
	}
	/*Draw the object*/
	else if (mode == LV_DESIGN_DRAW_MAIN) {
		 tos_analogClock_static(analogclock, mask, mode); /*Statik cizim fonksiyonu*/
		 tos_analogClock_dynamic(analogclock, mask, mode); /*Dinamik cizim fonksiyonu*/
	}
	return true;
}

#endif
