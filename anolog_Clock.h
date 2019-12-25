

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


struct timeRadian
{
	double nanoSecondsRadian;
	double secondsRadian;
	double minutesRadian;
	double hoursRadian;
};
/*Ana Fonksiyonlarý*/
void analog_Clock(void);
static void ekran_Yarat(void);
static void init_all_themes(uint16_t hue); //Tema Seçimi
/*Saat Fonksiyonlarý*/
void clockCanvas(void);
void analogClock_animation(lv_obj_t * canvas, int16_t value);
void drawStatic(lv_obj_t * analogSaat);
void drawDinamic(lv_obj_t * analogSaat);
/*Hesap Fonksiyonlarý*/
float mapValue(float deger,int start1,int stop1,int start2,float stop2);
float normalize(int deger,int start,int stop);
void my_getTime(int *saniye, int *dakika, int *saat);
void my_setTime(int  saat, int  dakika, int  saniye);

int my_seconds;
int my_minute;
int my_hours;

double nanoSaniyeRadyan, saniyeRadyan, dakikaRadyan, saatRadyan;
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
