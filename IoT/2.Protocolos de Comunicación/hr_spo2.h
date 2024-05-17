#ifndef HR_SPO2_H_
#define HR_SPO2_H_

#define FRECUENCIA_MUESTREO   100
#define VENTANA_TRABAJO       (4*FRECUENCIA_MUESTREO)

void Calculate_HR_SpO2(int32_t data_r_channel[VENTANA_TRABAJO], int32_t data_ir_channel[VENTANA_TRABAJO], int32_t *min_ch, float *hr, float *r, float *spo2, float *p);

#endif
