char keypadPort at PORTD;
unsigned char kp;
unsigned char ch[5];
unsigned char vh[5];
unsigned char i = 0, NB;
unsigned char tem;
unsigned char a = 0;
unsigned char nc = 0;
unsigned char  ncv=0;
unsigned char  re=0;
unsigned char  j=0;
sbit LCD_RS at RC2_bit;
sbit LCD_EN at RC3_bit;
sbit LCD_D4 at RC4_bit;
sbit LCD_D5 at RC5_bit;
sbit LCD_D6 at RC6_bit;
sbit LCD_D7 at RC7_bit;
sbit LCD_RS_Direction at TRISC2_bit;
sbit LCD_EN_Direction at TRISC3_bit;
sbit LCD_D4_Direction at TRISC4_bit;
sbit LCD_D5_Direction at TRISC5_bit;
sbit LCD_D6_Direction at TRISC6_bit;
sbit LCD_D7_Direction at TRISC7_bit;
void main() {
    TRISC = 0xFC;
    TRISB = 0xC1;
    TRISA = 0x11;
    Keypad_Init();
    PORTB.RB2 = 0;
    PORTB.RB3 = 0;
    PORTC.RC0 = 0;
    PORTC.RC1 = 0;
    PORTB.RB1 = 0;
    PORTB.RB4= 0;
    ADC_Init();
    Lcd_Init();
    Lcd_Cmd(_LCD_CURSOR_OFF);
    Lcd_out(1, 4, "bienvenue");
    Lcd_out(2, 2, "Saisir le code:");
    EEPROM_Write(0x00, '0');
    EEPROM_Write(0x01, '0');
    EEPROM_Write(0x02, '0');
    EEPROM_Write(0x03, '0');
    INTCON.GIE = 1;
    INTCON.INTE = 1;
    OPTION_REG.INTEDG = 1;
    INTCON.T0IE = 1;
    OPTION_REG.T0SE = 0;
    OPTION_REG.T0CS = 1;
    TMR0 = 253;
    NB = 152;
    ADCON1=0x08;
    while (1) {
        kp = 0;
        while (!kp) {
            kp = Keypad_Key_Click();
        }

        switch (kp) {
            case 1: kp = 49; break;  // 1
            case 2: kp = 50; break;  // 2
            case 3: kp = 51; break;  // 3
            case 4: kp = 65; break;  // A
            case 5: kp = 52; break;  // 4
            case 6: kp = 53; break;  // 5
            case 7: kp = 54; break;  // 6
            case 8: kp = 66; break;  // B
            case 9: kp = 55; break;  // 7
            case 10: kp = 56; break; // 8
            case 11: kp = 57; break; // 9
            case 13: kp = 42; break; // *
            case 14: kp = 48; break; // 0
            case 15: kp = 35; break; // #
            case 16: kp = 68; break; // D
        }
        if (i < 4) {
            ch[i] = kp;
            i++;
            Lcd_Cmd(_LCD_CLEAR);
            Lcd_out(1, 2, "Saisir le code:");
            Lcd_out(2, 1, ch);
        }
        if (kp == 35) {
            ch[i] = '\0';
            vh[0] = EEPROM_Read(0x00);
            vh[1] = EEPROM_Read(0x01);
            vh[2] = EEPROM_Read(0x02);
            vh[3] = EEPROM_Read(0x03);
            if ((strcmp(ch, vh) == 0)) {
                Lcd_Cmd(_LCD_CLEAR);
                Lcd_out(2, 3, "Acces autorise");
                PORTB.RB2 = 1;
                Delay_ms(5000);
                PORTB.RB2 = 0;
                PORTC.RC0 = 0;
                PORTC.RC1 = 1;
                tem = ADC_Read(0) * 0.488;
                if (tem < 15) {
                    Lcd_Cmd(_LCD_CLEAR);
                    Lcd_out(2, 3, "Chauffage");
                    PORTB.RB4 = 1;
                    Delay_ms(5000);
                    PORTB.RB4 = 0;

                } else if (tem > 28) {
                    Lcd_Cmd(_LCD_CLEAR);
                    Lcd_out(2, 3, "Climatiseur");
                    PORTB.RB4 = 1;
                    Delay_ms(5000);
                    PORTB.RB4 = 0;
                }
            } else {
                Lcd_Cmd(_LCD_CLEAR);
                Lcd_out(1, 3, "Acces refuse");
                Lcd_out(2, 4, "click STOP");
                PORTB.RB3 = 1;
                PORTB.RB1 = 1;
                PORTC.RC0 = 0;
                PORTC.RC1 = 0;
            }
        }
        if (a == 1) {
            PORTC.RC0 = 0;
            PORTC.RC1 = 0;
            Lcd_Cmd(_LCD_CLEAR);
            Lcd_out(1, 1, "Mode modification");
            INTCON.GIE = 1;
            INTCON.RBIE =1;
            /*INTCON.T0IE=1;
            TMR0 = 0;
            OPTION_REG=0b00000111;
            NB = 152;*/
            if(nc==1)/*&&(NB>0))*/{
                 ch[j]=kp;
                 j++;
                 Lcd_Cmd(_LCD_CLEAR);
                 Lcd_out(1, 1,"nouveau code:");
                 Lcd_out(2, 1, ch);
                 EEPROM_Write(0x00, ch[0]);
                 EEPROM_Write(0x01, ch[1]);
                 EEPROM_Write(0x02, ch[2]);
                 EEPROM_Write(0x03, ch[3]);
            }/*else{
                 Lcd_Cmd(_LCD_CLEAR);
                 Lcd_out(1, 1, "time out");
            }  */

        }
    }
}
void interrupt() {
    if (INTCON.INTF) {
        PORTB.RB3 = 0;
        PORTB.RB1 = 0;
        PORTC.RC0 = 0;
        PORTC.RC1 = 0;
        ch[0]='\0';
        ch[1]='\0';
        ch[2]='\0';
        ch[3]='\0';
        ch[4]='\0';
        i = 0;
        INTCON.INTF = 0;
    }
    if (INTCON.T0IF) {
        INTCON.T0IF = 0;
        ch[0]='\0';
        ch[1]='\0';
        ch[2]='\0';
        ch[3]='\0';
        ch[4]='\0';
        a = 1;
        TMR0 = 253;
        /*NB--;
        if(NB==0){
          INTCON.T0IE=0;
        }*/
    }
    if (INTCON.RBIF) {
        if (PORTB.RB6) {
            ch[0]='\0';
            ch[1]='\0';
            ch[2]='\0';
            ch[3]='\0';
            ch[4]='\0';
            nc = 1;
        }
        if (PORTB.RB7){
            ch[0]='\0';
            ch[1]='\0';
            ch[2]='\0';
            ch[3]='\0';
            ch[4]='\0';
             a=0;
             i=0;
        }
        INTCON.RBIF = 0;
    }
}