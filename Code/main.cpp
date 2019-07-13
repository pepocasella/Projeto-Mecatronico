
/* Includes ------------------------------------------------------------------*/

/* mbed specific header files. */
#include "mbed.h"

#include "TCS3200.h"

/*------------------------- Implementação LCD --------------------------------*/

#include "TextLCD.h"

// Host PC Communication channels
Serial pc(USBTX, USBRX); // tx, rx

// I2C Communication
I2C i2c_lcd(PB_9,PB_8); // SDA, SCL

// TCS3200


TextLCD_I2C lcd(&i2c_lcd, 0x7E, TextLCD::LCD20x4);

//include rgb
TCS3200 color(PB_13,PB_14,PB_15,PC_10,PA_11);
//            S0    S1    S2     S3     OUT
/* Definitions ---------------------------------------------------------------*/



/* Variables -----------------------------------------------------------------*/
//INTERRUPCOES
InterruptIn BOTAO_FIM(PC_0);
//InterruptIn Botao_FIM_X_MAX(PC_1);
InterruptIn Botao_positivo(PC_2);
InterruptIn Botao_negativo(PC_3);
InterruptIn Botao_Z(PA_5);
InterruptIn Botao_Y(PA_6);
InterruptIn Botao_X(PC_7);
InterruptIn Botao_Ref_Go(PC_8);
InterruptIn Botao_Ref(PC_12);


//ENTRADAS MOTOR
DigitalOut Eixo_X_Pulso(PA_10);
DigitalOut Eixo_Y_Pulso(PB_3);
DigitalOut Eixo_Z_Pulso(PB_5);
DigitalOut Eixo_X_Dir(PB_4);
DigitalOut Eixo_Y_Dir(PB_10);
DigitalOut Eixo_Z_Dir(PA_8);
DigitalOut Garra(PA_4);
//ENTRADAS MOTOR

//ENTRADAS ANALOGICAS
AnalogIn sensor_distancia(PB_0);

//VARIAVEIS
float position_x = 0;
float position_y = 0;
float position_z = 0;
float position_x_max = 0;
float position_x_min = 0;
float position_y_max = 0;
float position_y_min = 0;
float position_z_max = 0;
float position_z_min = 0;
int botao_x = 0;
int botao_y = 0;
int botao_z = 0;
int pos_x1 = 0;
int pos_x2 = 0;
int pos_x3 = 0;
int pos_y1 = 0;
int pos_y2 = 0;
int pos_y3 = 0;
int pos_z1 = 0;
int pos_z2= 0;
int pos_z3 = 0;
int marcador_bt = 0;
int botao_ref = 0;

//var do motor
bool dir = false;
int cont_pulses = 0;
int cur_pos_x = 0;
int cur_pos_y = 0;
int cur_pos_z = 0;
float velocidade = 0.00032;//de 0.01 a 0.0004
float velocidade2 = 0.001;
bool auto_rot = false;
//var do motor

//VAR RGB
int contadorRGB=0;
int valor_base=0;
int diferenca=0;
int soma=0;
long verde, vermelho;
int cor;
//VAR RGB

//------------------------ Declaracao Variaveis sensor distancia --------------------------------

float raio = 0;
float distancia_x = 0;
float media_leitura_sensor = 0;
float media = 0;

int n_amostras = 50;
//fim


/*------------------------ Funções --------------------------*/

//RGB
void CalibracaoRGB()
{
    color.SetMode(TCS3200::SCALE_20);
    printf("Aguarde..");
    while(contadorRGB<20) {
        vermelho=color.ReadRed();
        verde = color.ReadGreen();
        diferenca=verde-vermelho;
        soma=soma+diferenca;
        contadorRGB=contadorRGB+1;

        printf("Soma, %d Contador, %d", soma, contadorRGB);
    }
    valor_base=soma/contadorRGB;
    valor_base=valor_base-10;
    printf("Soma final, %d ", valor_base);
    contadorRGB=0;
}

void MedicaoRGB()
{

    while(contadorRGB <20) {

        vermelho=color.ReadRed();
        verde = color.ReadGreen();
        diferenca=verde-vermelho;
        soma=soma+diferenca;
        printf("Diferenca. %d \r\n", diferenca);
        contadorRGB=contadorRGB+1;

        wait(0.1);

    }

    soma=soma/contadorRGB;
    if(diferenca >valor_base) {
        printf("Produto IRREGULAR.COR DIFERENTE DE VERDE %d \r\n", soma);
        cor = 0;
    }

    else {
        printf("Produto REGULAR.COR VERDE %d \r\n", soma);
        cor = 1;
    }
    contadorRGB=0;
    wait(0.1);
}
//FIM RGB

// SENSOR DISTANCIA
float sensor_distancia_TCRT5000()
{

    media_leitura_sensor = 0;

    for (int i = 0; i<n_amostras; i++) {
        //printf("%f \r\n", sensor_distancia.read());
        media_leitura_sensor = media_leitura_sensor + sensor_distancia.read();
        wait(0.03);
    }

    media = media_leitura_sensor/n_amostras;

    printf("media:%f \r\n", media);


    return media;
}

void Sensor_Distancia()
{
    sensor_distancia.read();
    sensor_distancia_TCRT5000();

    printf("------------------------------------------\r\n");
    printf("------------------------------------------\r\n");

    distancia_x = 77.526*pow(media,2) - 55.603*media + 28.994; // y=77,526x2 - 55,603x + 28,994

    raio = sqrt(distancia_x*distancia_x-(0.5*distancia_x*(0.5*distancia_x)));
    printf("d:%f \r\n", distancia_x);
    printf("raio:%f \r\n", raio);

    printf("------------------------------------------\r\n");
    printf("------------------------------------------\r\n");

    wait(0.01);
}
//FIM SENSOR DISTANCIA

void motor_off()
{
    dir = false;
}


void direcao_positiva(int motor)
{
    dir = true ;
    switch(motor) {
            while(dir == true) {
            case 1:
                Eixo_X_Dir = 1;
                Eixo_X_Pulso = 1;
                wait(velocidade);
                Eixo_X_Pulso = 0;
                wait(velocidade);
                cur_pos_x += 1;
            }
            break;


        case 2:
            while(dir==true) {
                Eixo_Y_Dir = 1;
                Eixo_Y_Pulso = 1;
                wait(velocidade);
                Eixo_Y_Pulso = 0;
                wait(velocidade);
                cur_pos_y += 1;
            }
            break;


        case 3:
            while(dir == true) {
                Eixo_Z_Dir = 1;
                Eixo_Z_Pulso = 1;
                wait(velocidade2);
                Eixo_Z_Pulso = 0;
                wait(velocidade2);
                cur_pos_z += 1;
            }
            break;



    }
    return;
}


void direcao_negativa(int motor)
{
    dir = true ;
    switch(motor) {
            while(dir == true) {
            case 1:
                Eixo_X_Dir = 0;
                Eixo_X_Pulso = 1;
                wait(velocidade);
                Eixo_X_Pulso = 0;
                wait(velocidade);
                cur_pos_x -= 1;
            }
            break;


        case 2:
            while(dir==true) {
                Eixo_Y_Dir = 0;
                Eixo_Y_Pulso = 1;
                wait(velocidade);
                Eixo_Y_Pulso = 0;
                wait(velocidade);
                cur_pos_y -= 1;
            }
            break;


        case 3:
            while(dir == true) {
                Eixo_Z_Dir = 0;
                Eixo_Z_Pulso = 1;
                wait(velocidade2);
                Eixo_Z_Pulso = 0;
                wait(velocidade2);
                cur_pos_z -= 1;
            }
            break;



    }
    return;
}



void home(int motor)
{
    switch(motor) {
        case 1:
            cur_pos_x = 0;
            break;




        case 2:
            cur_pos_y = 0;
            break;

        case 3:
            cur_pos_z = 0;
            break;
    }
}
//fim das funcoes motor
void motor_tap(){
    int cont = 0;
    while(cont != 20){
        Eixo_Z_Dir = 0;
        Eixo_Z_Pulso = 1;
        wait(velocidade2);
        Eixo_Z_Pulso = 0;
        wait(velocidade2);
        cur_pos_z += 1;
        }
    cont = 0;
    return;
    }
void referenciamento_z()
{
    while(BOTAO_FIM != 0) {
        Eixo_Z_Dir = 0;
        Eixo_Z_Pulso = 1;
        wait(velocidade2);
        Eixo_Z_Pulso = 0;
        wait(velocidade2);
        cur_pos_z -= 1;
    }
    Eixo_Z_Pulso = 0;
    wait(0.1);
    position_z_min = cur_pos_z;
    home(3);
    
    while(BOTAO_FIM != 0) {
        Eixo_Z_Dir = 1;
        Eixo_Z_Pulso = 1;
        wait(velocidade2);
        Eixo_Z_Pulso = 0;
        wait(velocidade2);
        cur_pos_z += 1;
    position_z_max = cur_pos_z;
    return;
}
}
// FIM DO REFERENCIAMENTO Z
//INICIO REFERENCIAMENTO X
void referenciamento_x()
{
    while(BOTAO_FIM != 0) {
        Eixo_X_Dir = 0;
        Eixo_X_Pulso = 1;
        wait(velocidade2);
        Eixo_X_Pulso = 0;
        wait(velocidade2);
        cur_pos_x -= 1;
    }
    Eixo_X_Pulso = 0;
    wait(0.1);
    position_x_min = cur_pos_x;
    home(3);
    
    while(BOTAO_FIM != 0) {
        Eixo_X_Dir = 1;
        Eixo_X_Pulso = 1;
        wait(velocidade2);
        Eixo_X_Pulso = 0;
        wait(velocidade2);
        cur_pos_x += 1;
    position_x_max = cur_pos_x;
    return;
}
}
//FIM DO REFERENCIAMENTO X
void referenciamento_y()
{
    while(BOTAO_FIM != 0) {
        Eixo_Y_Dir = 0;
        Eixo_Y_Pulso = 1;
        wait(velocidade2);
        Eixo_Y_Pulso = 0;
        wait(velocidade2);
        cur_pos_y -= 1;
    }
    Eixo_Y_Pulso = 0;
    wait(0.1);
    position_y_min = cur_pos_y;
    home(3);
    
    while(BOTAO_FIM != 0) {
        Eixo_Y_Dir = 1;
        Eixo_Y_Pulso = 1;
        wait(velocidade2);
        Eixo_Y_Pulso = 0;
        wait(velocidade2);
        cur_pos_y += 1;
    position_y_max = cur_pos_y;
    return;
}
}
//FIM DOS REFERENCIAMENTOS

//CALIBRAÇÃO DA POSIÇÃO DOS EIXOS PARA MILIMETROS
float position_to_mm(int passo)
{
    return (3*passo)/400;
}

float position_to_mm_garra(int passo)
{
    return (5*passo)/400;
}
//FIM DAS CALIBRAÇÕES

//Função dos botoes on/off

void botao_x_on_off()
{
    botao_y = 0;
    botao_z = 0;
    if(Botao_X == 1) {
        if(botao_x == 0) {
            botao_x = 1;
        } else {
            botao_x = 0;
        }
    }
}

void botao_y_on_off()
{
    botao_x = 0;
    botao_z = 0;
    if(Botao_Y == 1) {
        if(botao_y == 0) {
            botao_y = 1;
        } else {
            botao_y = 0;
        }
    }
}

void botao_z_on_off()
{
    botao_x = 0;
    botao_y = 0;
    if(Botao_Z == 1) {
        if(botao_z == 0) {
            botao_z = 1;
        } else {
            botao_z = 0;
        }
    }
}

void botao_ref_on_off()
{
    if(Botao_Ref_Go == 1) {
        if(botao_ref == 0) {
            botao_ref = 1;
        } else {
            botao_ref = 0;
        }
    }

}
//fim da função dos botoes on/off
//botao_referenciar posição
void bt_ref()
{
    marcador_bt = marcador_bt + 1;
    if(marcador_bt > 4) {
        marcador_bt = 0;
        pos_x1 = 0;
        pos_y1 = 0;
        pos_z1 = 0;

        pos_x2 = 0;
        pos_y2 = 0;
        pos_z2 = 0;

        pos_x3 = 0;
        pos_y3 = 0;
        pos_z3 = 0;
        lcd.setAddress(0,3);
        printf("POSICOES RESETADAS!");
        lcd.printf("POSICOES RESETADAS!");
        wait(1);
        lcd.setAddress(0,3);
        lcd.printf("                   ");
    } else if(marcador_bt == 2) {
        pos_x1 = cur_pos_x;
        pos_y1 = cur_pos_y;
        pos_z1 = cur_pos_z;
        printf("POS PICK SALVA!");
        lcd.setAddress(0,3);
        lcd.printf("POS PICK SALVA!");
        wait(1);
        lcd.setAddress(0,3);
        lcd.printf("               ");
    }

    else if(marcador_bt == 3) {
        pos_x2 = cur_pos_x;
        pos_y2 = cur_pos_y;
        pos_z2 = cur_pos_z;
        printf("POS PLC1 SALVA!");
        lcd.setAddress(0,3);
        lcd.printf("POS PLC1 SALVA!");
        wait(1);
        lcd.setAddress(0,3);
        lcd.printf("               ");
    }

    else if(marcador_bt == 4) {
        pos_x3 = cur_pos_x;
        pos_y3 = cur_pos_y;
        pos_z3 = cur_pos_z;
        printf("POS PLC2 SALVA!");
        lcd.setAddress(0,3);
        lcd.printf("POS PLC2 SALVA!");
        wait(1);
        lcd.setAddress(0,3);
        lcd.printf("               ");
    }



    printf("POSIcaoO MARCADA: P %d position_x.\r\n ",pos_x1);
    printf("POSIcaoO MARCADA: P %d position_x.\r\n ",pos_x2);
    printf("POSIcaoO MARCADA: P %d position_x.\r\n ",pos_x3);
    wait(0.1);
}
//fim referenciar posição
//INICIO ir para posicao
//inicio rotina automatica

void go_to2(int pos)
{
    switch(pos) {
        case 1:
            while(cur_pos_x != pos_x1) {
                if(cur_pos_x < pos_x1) {
                    Eixo_X_Dir = 1;
                    Eixo_X_Pulso = 1;
                    wait(velocidade);
                    Eixo_X_Pulso = 0;
                    wait(velocidade);
                    cur_pos_x += 1;
                }
                if(cur_pos_x > pos_x1) {
                    Eixo_X_Dir = 0;
                    Eixo_X_Pulso = 1;
                    wait(velocidade);
                    Eixo_X_Pulso = 0;
                    wait(velocidade);
                    cur_pos_x -= 1;
                }
            }
            //wait(0.1);

            while(cur_pos_y != pos_y1) {
                if(cur_pos_y < pos_y1) {
                    Eixo_Y_Dir = 1;
                    Eixo_Y_Pulso = 1;
                    wait(velocidade);
                    Eixo_Y_Pulso = 0;
                    wait(velocidade);
                    cur_pos_y += 1;
                }
                if(cur_pos_y > pos_y1) {
                    Eixo_Y_Dir = 0;
                    Eixo_Y_Pulso = 1;
                    wait(velocidade);
                    Eixo_Y_Pulso = 0;
                    wait(velocidade);
                    cur_pos_y -= 1;
                }
            }

            while(cur_pos_z != pos_z1) {

                if(cur_pos_z < pos_z1) {
                    Eixo_Z_Dir = 1;
                    Eixo_Z_Pulso = 1;
                    wait(velocidade2);
                    Eixo_Z_Pulso = 0;
                    wait(velocidade2);
                    cur_pos_z += 1;
                }
                if(cur_pos_z > pos_z1) {
                    Eixo_Z_Dir = 0;
                    Eixo_Z_Pulso = 1;
                    wait(velocidade2);
                    Eixo_Z_Pulso = 0;
                    wait(velocidade2);
                    cur_pos_z -= 1;
                }
            }
            wait(0.1);


            //wait(0.1);
            break;

        case 2:

            while(cur_pos_x != pos_x2) {
                if(cur_pos_x < pos_x2) {
                    Eixo_X_Dir = 1;
                    Eixo_X_Pulso = 1;
                    wait(velocidade);
                    Eixo_X_Pulso = 0;
                    wait(velocidade);
                    cur_pos_x += 1;
                }
                if(cur_pos_x > pos_x2) {
                    Eixo_X_Dir = 0;
                    Eixo_X_Pulso = 1;
                    wait(velocidade);
                    Eixo_X_Pulso = 0;
                    wait(velocidade);
                    cur_pos_x -= 1;
                }
            }



            //wait(0.1);

            while(cur_pos_y != pos_y2) {
                if(cur_pos_y < pos_y2) {
                    Eixo_Y_Dir = 1;
                    Eixo_Y_Pulso = 1;
                    wait(velocidade);
                    Eixo_Y_Pulso = 0;
                    wait(velocidade);
                    cur_pos_y += 1;
                }
                if(cur_pos_y > pos_y2) {
                    Eixo_Y_Dir = 0;
                    Eixo_Y_Pulso = 1;
                    wait(velocidade);
                    Eixo_Y_Pulso = 0;
                    wait(velocidade);
                    cur_pos_y -= 1;
                }
            }

            while(cur_pos_z != pos_z2) {
                if(cur_pos_z < pos_z2) {
                    Eixo_Z_Dir = 1;
                    Eixo_Z_Pulso = 1;
                    wait(velocidade2);
                    Eixo_Z_Pulso = 0;
                    wait(velocidade2);
                    cur_pos_z += 1;
                }
                if(cur_pos_z > pos_z2) {
                    Eixo_Z_Dir = 0;
                    Eixo_Z_Pulso = 1;
                    wait(velocidade2);
                    Eixo_Z_Pulso = 0;
                    wait(velocidade2);
                    cur_pos_z -= 1;
                }
            }
            break;


        case 3:

            while(cur_pos_x != pos_x3) {
                if(cur_pos_x < pos_x3) {
                    Eixo_X_Dir = 1;
                    Eixo_X_Pulso = 1;
                    wait(velocidade);
                    Eixo_X_Pulso = 0;
                    wait(velocidade);
                    cur_pos_x += 1;
                }
                if(cur_pos_x > pos_x3) {
                    Eixo_X_Dir = 0;
                    Eixo_X_Pulso = 1;
                    wait(velocidade);
                    Eixo_X_Pulso = 0;
                    wait(velocidade);
                    cur_pos_x -= 1;
                }
            }

            //wait(0.1);

            while(cur_pos_y != pos_y3) {
                if(cur_pos_y < pos_y3) {
                    Eixo_Y_Dir = 1;
                    Eixo_Y_Pulso = 1;
                    wait(velocidade);
                    Eixo_Y_Pulso = 0;
                    wait(velocidade);
                    cur_pos_y += 1;
                }
                if(cur_pos_y > pos_y3) {
                    Eixo_Y_Dir = 0;
                    Eixo_Y_Pulso = 1;
                    wait(velocidade);
                    Eixo_Y_Pulso = 0;
                    wait(velocidade);
                    cur_pos_y -= 1;
                }
            }

            while(cur_pos_z != pos_z3) {
                if(cur_pos_z < pos_z3) {
                    Eixo_Z_Dir = 1;
                    Eixo_Z_Pulso = 1;
                    wait(velocidade2);
                    Eixo_Z_Pulso = 0;
                    wait(velocidade2);
                    cur_pos_z += 1;
                }
                if(cur_pos_z > pos_z3) {
                    Eixo_Z_Dir = 0;
                    Eixo_Z_Pulso = 1;
                    wait(velocidade2);
                    Eixo_Z_Pulso = 0;
                    wait(velocidade2);
                    cur_pos_z -= 1;
                }
            }


            break;
    }
    return;
}

//fim go_to
void kill_routine()
{
    auto_rot = false;
    return;
}

void automatic_routine(int rotina)
{
    Botao_Ref_Go.rise(&kill_routine);
    switch(rotina) {
        case 1:
            auto_rot = true;
            lcd.setCursor(TextLCD::CurOff_BlkOff);
            while(auto_rot == true) {
                lcd.setAddress(5,0);
                lcd.printf("Indo para:");
                lcd.setAddress(4,1);
                lcd.printf("Posicao PICK");
                go_to2(1);
                wait(1);
                lcd.setAddress(4,1);
                lcd.printf("Posicao PLC1");
                go_to2(2);
                wait(1);
            }
            break;
        case 2:
            Garra = 0;
            lcd.setCursor(TextLCD::CurOff_BlkOff);
            while(cur_pos_z != 5500) {
                    Eixo_Z_Dir = 1;
                    Eixo_Z_Pulso = 1;
                    wait(velocidade2);
                    Eixo_Z_Pulso = 0;
                    wait(velocidade2);
                    cur_pos_z += 1;
                }
            auto_rot = true;    
            while(auto_rot == true) {
                go_to2(1);
                Garra = 1;
                while(cur_pos_z != 5500) {
                    Eixo_Z_Dir = 1;
                    Eixo_Z_Pulso = 1;
                    wait(velocidade2);
                    Eixo_Z_Pulso = 0;
                    wait(velocidade2);
                    cur_pos_z += 1;
                }
                wait(1);
                Sensor_Distancia();
                MedicaoRGB();
                if(raio>19.5 & raio<21 & cor == 1) {
                    go_to2(2);
                    while(cur_pos_z != 5500) {
                        Eixo_Z_Dir = 1;
                        Eixo_Z_Pulso = 1;
                        wait(velocidade2);
                        Eixo_Z_Pulso = 0;
                        wait(velocidade2);
                        cur_pos_z += 1;
                        Garra = 0;
                    }
                } else {
                    go_to2(3);
                    Garra = 0;
                    while(cur_pos_z != 5500) {
                        Eixo_Z_Dir = 1;
                        Eixo_Z_Pulso = 1;
                        wait(velocidade2);
                        Eixo_Z_Pulso = 0;
                        wait(velocidade2);
                        cur_pos_z += 1;
                    }
                }
            }
            break;
    }
}

//INICIO DA FUNÇÃO JOG
void JOG()
{
    position_x = cur_pos_x;
    position_x = position_to_mm(position_x);
    position_y = cur_pos_y;
    position_y = position_to_mm(position_y);
    position_z = cur_pos_z;
    position_z = position_to_mm_garra(position_z);
    lcd.setCursor(TextLCD::CurOff_BlkOff);
    lcd.setAddress(0,0);
    lcd.printf("Posx:");
    lcd.setAddress(12,0);
    lcd.printf("mm");
    lcd.setAddress(0,1);
    lcd.printf("Posy:");
    lcd.setAddress(12,1);
    lcd.printf("mm");
    lcd.setAddress(0,2);
    lcd.printf("Posz:");
    lcd.setAddress(12,2);
    lcd.printf("mm");
    lcd.setAddress(5,0);
    lcd.printf("%0.2f",position_x);
    lcd.setAddress(5,1);
    lcd.printf("%0.2f",position_y);
    lcd.setAddress(5,2);
    lcd.printf("%0.2f",position_z);

    botao_x = 0;
    bool JOG = true;

    while(JOG == true) {
        if(botao_x == 1) {
            lcd.setAddress(3,0);
            lcd.printf("X");
        } else {
            lcd.setAddress(3,0);
            lcd.printf("x");
        }
        if(botao_y == 1) {
            lcd.setAddress(3,1);
            lcd.printf("Y");
        } else {
            lcd.setAddress(3,1);
            lcd.printf("y");
        }
        if(botao_z == 1) {
            lcd.setAddress(3,2);
            lcd.printf("Z");
        } else {
            lcd.setAddress(3,2);
            lcd.printf("z");
        }

        if(Botao_Ref_Go == 1) {
            JOG = false;
        }
        if(Botao_Ref == 1) {
            bt_ref();
        }





        /*----- Implementação Botão Fim de Curso -----*/


        /*----- JOG EIXO -----*/

        if(Botao_positivo == 1 & botao_x == 1) {
            direcao_positiva(1);
            //wait(wait_time);
            position_x = cur_pos_x;
            position_x = position_to_mm(position_x);
            //conversão da leitura get_position para milimetros
            printf("Position: P %f3 position_x.\r\n ",position_x);
            printf("Position: P %f3 position_y.\r\n ",position_y);
            printf("Position: P %f3 position_y.\r\n ",position_z);
            lcd.setAddress(5,0);
            lcd.printf("%0.2f",position_x);
            lcd.setAddress(5,1);
            lcd.printf("%0.2f",position_y);
            lcd.setAddress(5,2);
            lcd.printf("%0.2f",position_z);
            //wait(wait_time);
        }


        if(Botao_negativo == 1 & botao_x == 1) {
            direcao_negativa(1);
            //wait(wait_time);
            position_x = cur_pos_x;
            position_x = position_to_mm(position_x);
            //conversão da leitura get_position para milimetros
            printf("Position: P %f3 position_x.\r\n ",position_x);
            printf("Position: P %f3 position_y.\r\n ",position_y);
            printf("Position: P %f3 position_y.\r\n ",position_z);
            lcd.setAddress(5,0);
            lcd.printf("%0.2f",position_x);
            lcd.setAddress(5,1);
            lcd.printf("%0.2f",position_y);
            lcd.setAddress(5,2);
            lcd.printf("%0.2f",position_z);
            //wait(wait_time);
        }

        if(Botao_positivo == 1 & botao_y == 1) {
            direcao_positiva(2);
            position_y = cur_pos_y;
            position_y = position_to_mm(position_y);
            //conversão da leitura get_position para milimetros
            printf("Position: P %f3 position_x.\r\n ",position_x);
            printf("Position: P %f3 position_y.\r\n ",position_y);
            printf("Position: P %f3 position_y.\r\n ",position_z);
            lcd.setAddress(5,0);
            lcd.printf("%0.2f",position_x);
            lcd.setAddress(5,1);
            lcd.printf("%0.2f",position_y);
            lcd.setAddress(5,2);
            lcd.printf("%0.2f",position_z);
        }



        if(Botao_negativo == 1 & botao_y == 1) {
            direcao_negativa(2);
            //wait(wait_time);
            position_y = cur_pos_y;
            position_y = position_to_mm(position_y);
            //conversão da leitura get_position para milimetros
            printf("Position: P %f3 position_x.\r\n ",position_x);
            printf("Position: P %f3 position_y.\r\n ",position_y);
            printf("Position: P %f3 position_y.\r\n ",position_z);
            lcd.setAddress(5,0);
            lcd.printf("%0.2f",position_x);
            lcd.setAddress(5,1);
            lcd.printf("%0.2f",position_y);
            lcd.setAddress(5,2);
            lcd.printf("%0.2f",position_z);
            //wait(wait_time);
        }

        if(Botao_negativo == 1 & botao_z == 1) {
            direcao_negativa(3);
            //wait(wait_time);
            position_z = cur_pos_z;
            position_z = position_to_mm(position_z);
            //conversão da leitura get_position para milimetros
            printf("Position: P %f3 position_x.\r\n ",position_x);
            printf("Position: P %f3 position_y.\r\n ",position_y);
            printf("Position: P %f3 position_y.\r\n ",position_z);
            lcd.setAddress(5,0);
            lcd.printf("%0.2f",position_x);
            lcd.setAddress(5,1);
            lcd.printf("%0.2f",position_y);
            lcd.setAddress(5,2);
            lcd.printf("%0.2f",position_z);
            //wait(wait_time);

        }

        if(Botao_positivo == 1 & botao_z == 1) {
            direcao_positiva(3);
            //wait(wait_time);
            position_z = cur_pos_z;
            position_z = position_to_mm(position_z);
            //conversão da leitura get_position para milimetros
            printf("Position: P %f3 position_x.\r\n ",position_x);
            printf("Position: P %f3 position_y.\r\n ",position_y);
            printf("Position: P %f3 position_y.\r\n ",position_z);
            lcd.setAddress(5,0);
            lcd.printf("%0.2f",position_x);
            lcd.setAddress(5,1);
            lcd.printf("%0.2f",position_y);
            lcd.setAddress(5,2);
            lcd.printf("%0.2f",position_z);
        }
        //fim
    }
    return;
}
//FIM DA FUNÇÃO JOG
// inicio funcao menu
void menu()
{
    int marcador_cursor = 0;
    int marcador_tela = 1;//n_tela
    int marcador_tela_atual = marcador_tela;
    lcd.setCursor(TextLCD::CurOn_BlkOn);
    lcd.setBacklight(TextLCD::LightOn);
    bool menu = true;
    while(menu==true) {

        if(Botao_positivo == 1) {
            marcador_cursor += 1;
            if (marcador_cursor < 0 && marcador_tela == 1) {
                marcador_cursor = 4;
                marcador_tela += 1;
            }
            if (marcador_cursor > 4 && marcador_tela == 1) {
                marcador_cursor = 0;
                marcador_tela += 1;
            }
            if (marcador_cursor < 0 && marcador_tela == 2) {
                marcador_cursor = 4;
                marcador_tela -= 1;
            }
            if (marcador_cursor > 4 && marcador_tela == 2) {
                marcador_cursor = 0;
                marcador_tela -= 1;
            }

        }

        if(Botao_negativo == 1) {
            marcador_cursor -= 1;
            if (marcador_cursor < 0 && marcador_tela == 1) {
                marcador_cursor = 4;
                marcador_tela += 1;
            } else if (marcador_cursor > 4 && marcador_tela == 1) {
                marcador_cursor = 0;
                marcador_tela += 1;
            }



            else if (marcador_cursor < 0 && marcador_tela == 2) {
                marcador_cursor = 4;
                marcador_tela -= 1;
            } else if (marcador_cursor > 4 && marcador_tela == 2) {
                marcador_cursor = 0;
                marcador_tela -= 1;
            }



        }
        lcd.setAddress(20,marcador_cursor);
        wait(0.1);
        if(marcador_tela != marcador_tela_atual) {
            lcd.cls();
            marcador_tela_atual = marcador_tela;
        }
        if(marcador_tela == 1) {
            lcd.setCursor(TextLCD::CurOn_BlkOn);
            lcd.setAddress(0,0);
            lcd.printf("->JOG");
            lcd.setAddress(0,1);
            lcd.printf("->Referenciamento");
            lcd.setAddress(0,2);
            lcd.printf("->Rotina automatica");
            lcd.setAddress(0,3);
            lcd.printf("->Calibrar Cor");
            lcd.setAddress(14,0);
        }
        if(marcador_tela == 2) {
            lcd.setCursor(TextLCD::CurOn_BlkOn);
            lcd.setAddress(0,0);
            lcd.printf("->Posicao Place2");
            lcd.setAddress(0,1);
            lcd.printf("->Posicao Place3");
            lcd.setAddress(14,0);
        }

        if(Botao_Ref == 1) {
            //ENTRA NO JOG A PARTIR DO PRIMEIRO ITEM DO MENU
            if(marcador_cursor == 0 && marcador_tela == 1) {
                lcd.cls();
                JOG();
                lcd.cls();
            }
            if(marcador_cursor == 1 && marcador_tela ==1) {
                lcd.cls();
                lcd.setCursor(TextLCD::CurOff_BlkOff);
                lcd.setAddress(2,0);
                lcd.printf("REFERENCIAMENTO");
                lcd.setAddress(0,1);
                lcd.printf("EIXO X:");
                lcd.setAddress(0,2);
                lcd.printf("EIXO Y:");
                lcd.setAddress(0,3);
                lcd.printf("EIXO Z:");
                referenciamento_z();// EIXO x na verdade
                lcd.setAddress(7,1);
                lcd.printf("OK");
               referenciamento_x();//EIXO y
                lcd.setAddress(7,2);
                lcd.printf("OK");
                referenciamento_y();//EIXO Z
                lcd.setAddress(7,3);
                lcd.printf("OK");
                wait(1);
                lcd.cls();
            }
            if(marcador_cursor == 2 && marcador_tela == 1) {
                lcd.cls();
                lcd.setAddress(0,0);
                lcd.printf("ROTINA");
                lcd.setAddress(0,1);
                lcd.printf("AUTOMATICA");
                lcd.setAddress(0,2);
                lcd.printf("HABILITADA");
                automatic_routine(2);
                lcd.cls();
            }

            if(marcador_cursor == 3 && marcador_tela == 1) {
                lcd.cls();
                lcd.setAddress(0,0);
                lcd.printf("CALIBRANDO COR");
                CalibracaoRGB();
                lcd.setAddress(0,0);
                lcd.printf("              ");
                lcd.setAddress(0,0);
                lcd.printf("COR CALIBRADA");
                wait(1);
                lcd.cls();
            }
            if(marcador_cursor == 0 && marcador_tela == 2) {
                lcd.cls();
                go_to2(1);
                lcd.cls();
            }
        }

    }
}
//fim funcao menu
//inicio funcao menu_init
void menu_inicialization()
{
    lcd.setBacklight(TextLCD::LightOn);
    lcd.setCursor(TextLCD::CurOff_BlkOff);
    //lcd.setBacklight(TextLCD::LightOff);
    lcd.setAddress(3,0);//coluna e linha
    lcd.printf("Bem Vindo ao:");
    lcd.setAddress(2,1); //setando o cursor
    lcd.printf("Desktop Robot-2R");
    wait(4);
    lcd.cls();
}

//fim funcao menu init



/* Main ----------------------------------------------------------------------*/

int main()
{





    /*------------------------------ JOG ------------------------------*/
    Botao_X.rise(&botao_x_on_off);
    Botao_Y.rise(&botao_y_on_off);
    Botao_Z.rise(&botao_z_on_off);
    //Botao_Ref.rise(&bt_ref);


    Botao_Ref_Go.rise(&botao_ref_on_off);
    Botao_positivo.fall(&motor_off);
    Botao_negativo.fall(&motor_off);
    BOTAO_FIM.rise(&motor_tap);
    //Botao_FIM_Z_MIN.rise(&motor_off);
    //Botao_FIM_Z_MAX.rise(&motor_off);
    //BOTAO_FIM.rise(&motor_off);
    //Botao_FIM_X_MAX.rise(&motor_off);
//JOG();
    //menu_inicialization();
    menu();
    //referenciamento_z();

    //eh importante realizar essa leitura uma vez que a
    //primeira leitura do sensor se mostrou com um erro bastante alto
//CalibracaoRGB();
}

