#include "mbed.h"

AnalogIn sensor_distancia(A0);

//------------------------ Declaracao Variaveis --------------------------------
/*
       diametro 35:
       bits_tarugo_35_min = 0.7880
       bits_tarugo_35_max = 0.8000
       
       diametro 37:
       bits_tarugo_35_min = 0.7880
       bits_tarugo_35_max = 0.8000
       
       diametro 40:
       bits_tarugo_35_min = 0.7880
       bits_tarugo_35_max = 0.8000
       
       diametro 43:
       bits_tarugo_35_min = 0.7880
       bits_tarugo_35_max = 0.8000
       
       
       diametro 45:
       bits_tarugo_45_min = 0.8290
       bits_tarugo_45_max = 0.840
       */
       
float bits_tarugo_35_min = 0.7880;
float bits_tarugo_35_max = 0.8000; 

float bits_tarugo_37_min = 0;
float bits_tarugo_37_max = 0;

float bits_tarugo_40_min = 0;
float bits_tarugo_40_max = 0;

float bits_tarugo_43_min = 0;
float bits_tarugo_43_max = 0;

float bits_tarugo_45_min = 0.8290;
float bits_tarugo_45_max = 0.840;

float bits_garra_aberta = 0.76717716;
float bits_garra_fechada = 0;

float leitura_calibracao = 0;
float delta_calibracao = 0;

float media_leitura_sensor = 0;
float media = 0;

int n_amostras = 10;

//------------------------ Criacao Funcao --------------------------------------

float sensor_distancia_TCRT5000() {
    
    media_leitura_sensor = 0;
    
    for (int i = 0; i<n_amostras; i++){
        media_leitura_sensor = media_leitura_sensor + sensor_distancia.read(); 
        wait(0.3);
        }
    
    media = media_leitura_sensor/n_amostras;
    
    printf("media:%f \r\n", media);
    
    
    
    return media;}

int main() {
    
    /*
    eh importante realizar essa leitura uma vez que a 
    primeira leitura do sensor se mostrou com um erro bastante alto
    */
    
    sensor_distancia.read();
    
    /*
    calibracao garra aberta
    */
    
    sensor_distancia_TCRT5000();
    leitura_calibracao = media;
    //if(leitura_calibracao > bits_garra_aberta){
        delta_calibracao = leitura_calibracao - bits_garra_aberta;
        //}
    printf("delta_calibracao:%f leitura_cali: %f \r\n", delta_calibracao, leitura_calibracao);
    printf("------------------------------------------\r\n");
    //if(leitura_calibracao < bits_garra_aberta){
        //delta_calibracao = leitura_calibracao - bits_garra_aberta;
        //}
        
    sensor_distancia_TCRT5000();
        
    media = media - delta_calibracao;
    
    printf("delta_calibracao:%f \r\n", delta_calibracao);
    printf("media:%f \r\n", media);
    printf("------------------------------------------\r\n");
    printf("------------------------------------------\r\n");    
     
//------------------------ Fim da calibracao --------------------------------------     
    
    while(1) {
       sensor_distancia_TCRT5000();
       media = media - delta_calibracao; // ajustando o valor a curva de calibracao
       
       printf("------------------------------------------\r\n");
       printf("------------------------------------------\r\n");
       
       if ( media > bits_tarugo_35_min and media < bits_tarugo_35_max){
           printf("Diametro tarugo: 35mm\r\n");
           }
       if ( media > bits_tarugo_37_min and media < bits_tarugo_37_max){
           printf("Diametro tarugo: 37mm\r\n");
           }
        if ( media > bits_tarugo_40_min and media < bits_tarugo_40_max){
           printf("Diametro tarugo: 40mm\r\n");
           }
       
       if ( media > bits_tarugo_43_min and media < bits_tarugo_43_max){
           printf("Diametro tarugo: 43mm\r\n");
           }
       if ( media > bits_tarugo_45_min and media < bits_tarugo_45_max) {
           printf("Diametro tarugo: 45mm\r\n");
           }
       
       printf("------------------------------------------\r\n");
       printf("------------------------------------------\r\n");
       wait(0.3);
    
    }
    
}
