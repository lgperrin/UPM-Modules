#include <stdint.h>
#include <math.h>
#include "HR_SpO2.h"


#define MIN_HR        30			 	    // Mínimo pulso cardiaco (latidos por minuto)
#define MAX_HR        225				    // Máximo pulso cardiaco (latidos por minuto)
#define HR_QUICK_STEP	(FRECUENCIA_MUESTREO/20)       // HR initialization value(bpm)


void Calculate_HR_SpO2(int32_t data_r_channel[VENTANA_TRABAJO], int32_t data_ir_channel[VENTANA_TRABAJO], int32_t *min_ch, float *hr, float *r, float *spo2, float *p)
{
  // Falta completar toda la implementación del algoritmo desarrollado en la parte A
  // Tal y como está genera unos valores fijos

	*min_ch= (float)100000.0;
	
	*r= (float)0.65;
	*hr= (float)75.0;

	*spo2= (float)99.50;
	*p= (float)0.87;
}
