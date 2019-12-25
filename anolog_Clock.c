#include "anolog_Clock.h"
#include "math.h"
#include "stdio.h"
#include "time.h"

#define PI 3.14159265358979323846
#define TWO_PI 6.28318530717958647692
#define HALF_PI 1.57079632679489661923‬


const int Cx=136;
const int Cy=136;

//Saat 
int hours = 0;
int minute = 0;
int second = 0;

//Ekran Fonksiyonlar
void analog_Clock(void);
static void ekran_Yarat(void);
static void init_all_themes(uint16_t hue); //Tema Seçimi
/*Saat Fonksiyonları*/
void clockCanvas(void);
void analogClock_animation(lv_obj_t * canvas, int16_t value);
void drawStatic(lv_obj_t * analogSaat);
void drawDinamic(lv_obj_t * analogSaat);
/*Hesap Fonksiyonları*/
float mapValue(float deger, int start1, int stop1, int start2, float stop2);
float normalize(int deger, int start, int stop);
void my_getTime(int *saniye, int *dakika, int *saat);
void my_setTime(int  saat, int  dakika, int  saniye);
/**Objeler*/
static lv_theme_t * th_act;
lv_obj_t * analogsaat;
lv_obj_t * gauge1;
static lv_theme_t * themes[8];

void analog_Clock(void)
{
    init_all_themes(NULL);
    th_act = themes[5];
    if(th_act == NULL) {
    	LV_LOG_WARN("Analog Clock Etkin Degil.");
    	return;
    }
	
    lv_theme_set_current(th_act);

    lv_obj_t * scr = lv_obj_create(NULL, NULL);
    lv_disp_load_scr(scr);//Displayi oluşturur.
	
    ekran_Yarat();//Ekran oluşturur.
}
static void ekran_Yarat(void)
{
	clockCanvas();
}
static void init_all_themes(uint16_t hue)
{

    int i = 0;
#if LV_USE_THEME_NIGHT ///kahverengi
    themes[i++] = lv_theme_night_init(hue, NULL);
#endif

#if LV_USE_THEME_MATERIAL ///gri
    themes[i++] = lv_theme_material_init(hue, NULL);
#endif

#if LV_USE_THEME_ALIEN ///koyugri
    themes[i++] = lv_theme_alien_init(hue, NULL);
#endif

#if LV_USE_THEME_ZEN
    themes[i++] = lv_theme_zen_init(hue, NULL);
#endif

#if LV_USE_THEME_NEMO
    themes[i++] = lv_theme_nemo_init(hue, NULL);
#endif

#if LV_USE_THEME_MONO ///siyah
    themes[i++] = lv_theme_mono_init(hue, NULL);
#endif

#if LV_USE_THEME_DEFAULT
    themes[i++] = lv_theme_default_init(hue, NULL);
#endif
}

static lv_style_t saatstil;
static lv_style_t dakikastil;
static lv_style_t saniyestil;
static lv_style_t staticStyle1;
static lv_style_t staticStyle2;

void clockCanvas(void) {
	static lv_color_t cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(272, 272)];

	analogsaat = lv_canvas_create(lv_scr_act(), NULL);
	lv_canvas_set_buffer(analogsaat, cbuf, 272, 272, LV_IMG_CF_TRUE_COLOR);
	lv_obj_align(analogsaat, NULL, LV_ALIGN_IN_TOP_LEFT, 104, 0);
	lv_canvas_fill_bg(analogsaat, LV_COLOR_BLACK);

	lv_style_copy(&saatstil, &lv_style_plain);
	saatstil.line.width = 5;
	saatstil.line.color = LV_COLOR_RED;
	saatstil.line.rounded = 1;

	lv_style_copy(&dakikastil, &lv_style_plain);
	dakikastil.line.width = 5;
	dakikastil.line.color = LV_COLOR_GREEN;
	dakikastil.line.rounded = 1;

	lv_style_copy(&saniyestil, &lv_style_plain);
	saniyestil.line.width = 5;
	saniyestil.line.color = LV_COLOR_BLUE;
	saniyestil.line.rounded = 1;

	

#if LV_USE_ANIMATION
	lv_anim_t a;
	a.var = analogsaat;
	a.start = 0;
	a.end = 100;
	a.exec_cb = (lv_anim_exec_xcb_t)analogClock_animation;
	a.path_cb = lv_anim_path_linear;
	a.ready_cb = NULL;
	a.act_time = 0;
	a.time = 5000;
	a.playback = 0;
	a.repeat = 1;
	lv_anim_create(&a);
#endif
}
void analogClock_animation(lv_obj_t * canvas, int16_t value)
{
	LV_ASSERT_OBJ(canvas, LV_OBJX_NAME);

	lv_canvas_ext_t * ext = lv_obj_get_ext_attr(canvas);
	lv_canvas_fill_bg(analogsaat, LV_COLOR_BLACK);
	drawStatic(canvas);
	drawDinamic(canvas);

	lv_obj_invalidate(canvas);
}
void drawStatic(lv_obj_t * analogSaat) {
	lv_point_t p1;
	lv_point_t p2;
	/*Style*/
	lv_style_copy(&staticStyle1, &lv_style_plain);
	staticStyle1.line.width = 5;
	staticStyle1.line.color = LV_COLOR_WHITE;
	staticStyle1.line.rounded = 1;

	lv_style_copy(&staticStyle2, &lv_style_plain);
	staticStyle2.line.width = 3;
	staticStyle2.line.color = LV_COLOR_GRAY;
	staticStyle2.line.rounded = 0;
	/**Yarı Çaplar*/
	const int margin = 5;
	const int lineWidht = 10; //Çizgi uzunluğu
	const int innermargin = 15; //kadranlar arası boşluk
	const int radiusHours = (Cx - margin);
	const int radiusMinutes = radiusHours - lineWidht - innermargin;
	const int radiusSecond = radiusMinutes - lineWidht - innermargin;

	/*Saat*/
	int sayac = 0;
	for (float theta = 0; theta < TWO_PI; theta += TWO_PI / 12) {
		float cosin = cos(theta);
		float sinus = sin(theta);

		p1.x = (Cx + (radiusHours * cosin));
		p1.y = (Cy + (radiusHours * sinus));
		p2.x = (Cx + ((radiusHours - lineWidht) * cosin));
		p2.y = (Cy + ((radiusHours - lineWidht) * sinus));
		sayac++;
		lv_point_t noktalar[] = { p1,p2 };
		if (1 == sayac % 3) {
			lv_canvas_draw_line(analogsaat, noktalar, 2, &staticStyle1);
		}
		else {
			lv_canvas_draw_line(analogsaat, noktalar, 2, &staticStyle2);
		}
		if (12 == sayac) {
			sayac = 0;
		}

	}
	/*Dakika*/
	for (float theta = 0; theta < TWO_PI; theta += TWO_PI / 60) {
		float cosin = cos(theta);
		float sinus = sin(theta);

		p1.x = (Cx + radiusMinutes * cosin);
		p1.y = (Cy + radiusMinutes * sinus);
		p2.x = (Cx + (radiusMinutes - lineWidht) * cosin);
		p2.y = (Cy + (radiusMinutes - lineWidht) * sinus);
		sayac++;
		lv_point_t noktalar[] = { p1,p2 };

		lv_canvas_draw_line(analogsaat, noktalar, 2, &staticStyle2);

	}
	/*Saniye*/
	for (float theta = 0; theta < TWO_PI; theta += TWO_PI / 60) {
		float cosin = cos(theta);
		float sinus = sin(theta);

		p1.x = (Cx + radiusSecond * cosin);
		p1.y = (Cy + radiusSecond * sinus);
		p2.x = (Cx + (radiusSecond - lineWidht) * cosin);
		p2.y = (Cy + (radiusSecond - lineWidht) * sinus);
		sayac++;
		lv_point_t noktalar[] = { p1,p2 };

		lv_canvas_draw_line(analogsaat, noktalar, 2, &staticStyle2);

	}
}
void drawDinamic(lv_obj_t * analogSaat) {


	double secondsRadian = mapValue(second, 0, 60, 0, PI * 2) - (PI / 2);
	double minutesRadian = mapValue(minute + normalize(second, 0, 60)-1, 0, 60, 0, PI * 2) - (PI / 2);
	double hoursRadian = mapValue(hours + normalize(minute + normalize(second, 0, 60), 0, 60), 0, 12, 0, PI * 2) - (PI / 2);
	printf("%d",minute);

	lv_point_t p1;
	lv_point_t p2;

	/**Yarı Çaplar*/
	const int margin = 5;
	const int lineWidht = 10; //Çizgi uzunluğu
	const int innermargin = 15; //kadranlar arası boşluk
	const int radiusHours = (Cx - margin);
	const int radiusMinutes = radiusHours - lineWidht - innermargin;
	const int radiusSecond = radiusMinutes - lineWidht - innermargin;
	

	//Seconds
	int sayac = 0;
	float theta = 0;
	float oneRad = TWO_PI / 60;
	float maxRad = (float) secondsRadian + (PI / 2);
	while (theta < maxRad) {
		sayac += 1;
		theta += oneRad;
		float cosin = cos(theta - (PI / 2) - (PI / 30));
		float sinus = sin(theta - (PI / 2) - (PI / 30));
			
		p1.x = Cx + (radiusSecond * cosin);
		p1.y = Cy + (radiusSecond * sinus);
		p2.x = Cx + ((radiusSecond - lineWidht) * cosin);
		p2.y = Cy + ((radiusSecond - lineWidht) * sinus);
			
		lv_point_t noktalar[] = { p1,p2 };
		lv_canvas_draw_line(analogsaat, noktalar, 2, &saniyestil);
		

	}

	//Minutes
	sayac = 0;
	oneRad = TWO_PI / 60;
	theta = oneRad;
	maxRad = (float)minutesRadian + (PI / 2);
	while (theta < maxRad + oneRad) {
		sayac += 1;
		theta += oneRad;
		float cosin = cos(theta - (PI / 2) - (PI / 30));
		float sinus = sin(theta - (PI / 2) - (PI / 30));

		p1.x = Cx + (radiusMinutes * cosin);
		p1.y = Cy + (radiusMinutes * sinus);
		p2.x = Cx + ((radiusMinutes - lineWidht) * cosin);
		p2.y = Cy + ((radiusMinutes - lineWidht) * sinus);

		lv_point_t noktalar[] = { p1,p2 };
		lv_canvas_draw_line(analogsaat, noktalar, 2, &dakikastil);
	}
	

	// Hours
	sayac = 0;
	oneRad = TWO_PI / 12;
	theta = 0;
	maxRad = (float) hoursRadian + (float)(PI / 3);
	while (theta < maxRad) {
		sayac += 1;
		theta += oneRad;
		float cosin = cos(theta - (PI / 2));
		float sinus = sin(theta - (PI / 2));

		p1.x = Cx + (radiusHours * cosin);
		p1.y = Cy + (radiusHours * sinus);
		p2.x = Cx + ((radiusHours - lineWidht) * cosin);
		p2.y = Cy + ((radiusHours - lineWidht) * sinus);

		lv_point_t noktalar[] = { p1,p2 };
		lv_canvas_draw_line(analogsaat, noktalar, 2, &saatstil);
	}
}
float mapValue(float value, int start1, int stop1, int start2, float stop2) {
	return (float)((float)value - (float)start1) / ((float)stop1 - (float)start1) * ((float)stop2 - (float)start2) + (float)start2;
}
float normalize(int value, int start, int stop) {
	return (float)mapValue(value, start, stop, 0, 1);
}
void my_getTime(int *saniye, int *dakika, int *saat) {
	time_t t;
	struct tm ptm;

	time(&t);

	ptm = *localtime(&t);
	*saniye = ptm.tm_sec;
	*dakika = ptm.tm_min;
	*saat = ptm.tm_hour;
}
void my_setTime(int saat, int dakika, int saniye) {
	if (saat >= 0 && saat < 12) {
		hours = saat;
	}
	else {
		switch (saat) {
		case 12:
			hours = 0;
			break;
		case 13:
			hours = 1;
			break;
		case 14:
			hours = 2;
			break;
		case 15:
			hours = 3;
			break;
		case 16:
			hours = 4;
			break;
		case 17:
			hours = 5;
			break;
		case 18:
			hours = 6;
			break;
		case 19:
			hours = 7;
			break;
		case 20:
			hours = 8;
			break;
		case 21:
			hours = 9;
			break;
		case 22:
			hours = 10;
			break;
		case 23:
			hours = 11;
			break;

		default:
			break;
		}
	}

	minute = dakika;
	second = saniye;
}