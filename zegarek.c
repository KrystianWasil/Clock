#include<8051.h>
#define TRUE 1
#define FALSE 0
#define KBD P3_5   // PORT

__code unsigned char WZOR[10] = {
  0b00111111,
  0b00000110,
  0b01011011,
  0b01001111,
  0b01100110,
  0b01101101,
  0b01111101,
  0b00000111,
  0b01111111,
  0b01101111

};


__code unsigned char WZOR2[10] = {
  0b10111111,
  0b10000110,
  0b11011011,
  0b11001111,
  0b11100110,
  0b11101101,
  0b11111101,
  0b10000111,
  0b11111111,
  0b11101111
};


__xdata unsigned char * led_wyb = (__xdata unsigned char * ) 0xFF30;

__xdata unsigned char * led_led = (__xdata unsigned char * ) 0xFF38;

__bit __at(0x96) SEG_ON_OFF;
__bit __at(0x97) DIODA;
__bit t0_flag = FALSE;

char hour = 23, min = 59, sec = 59;
unsigned int counter = 0;
unsigned char press = FALSE;
unsigned char enterClicked;
unsigned char key_b = 0b00000001;
unsigned char led_b = 0b00000001;
unsigned char position = 0;
unsigned char mode_flag = FALSE;
unsigned char current_key = 0b00000001;
void t0_serv();
void inc_sec();
void dec_sec();
void inc_min();
void dec_min();
void inc_hour();
void dec_hour();
void keyboard();
void display();

void timer_int() __interrupt(1){
 TH0 = 226;
 t0_flag = TRUE;
}


void main() {
     SEG_ON_OFF = FALSE;
     TH0 = 226;
     TMOD = 0b01110000;
     IE = 0b10000010;
     TR0 = TRUE;
     while (TRUE) {

	if(t0_flag == TRUE)  {
          t0_flag = FALSE;
 	  display();
 	  keyboard();
 	  t0_serv();
       }
    }
 }
 
 void t0_serv() {
    counter++;
    if (counter == 960) {
     if (mode_flag == FALSE) {
       inc_sec();
     }
     counter = 0;
    }

 }

 void inc_hour() {
   hour++;
   if (hour == 24) {
    hour = 0;
   }
}

 void inc_min() {
   min++;
   if (min == 60) {
    min = 0;
    inc_hour();
   }
}

 void inc_sec() {
   sec++;
   if (sec == 60) {
    sec = 0;
    inc_min();
   }
}

void dec_hour() {
    hour--;
    if (hour < 0) {
      hour = 23;
    }
}

void dec_min() {
     min--;
     if (min < 0) {
      min = 59;
      dec_hour();
     }
}

void dec_sec() {
     sec--;
     if (sec < 0) {
      sec = 59;
      dec_min();
     }
}

void display() {
     SEG_ON_OFF = TRUE;
     *led_wyb = led_b;
     
     if (led_b == 0b00000001) {
	if (mode_flag == TRUE && position == 0) {
	     *led_led  = WZOR2[sec%10];
	} else {
 	     *led_led  = WZOR[sec%10];
	}

     }
     if (led_b == 0b00000010) {
	if (mode_flag == TRUE && position == 0) {
	     *led_led  = WZOR2[sec/10];
	} else {
 	     *led_led  = WZOR[sec/10];
	}

     }
     if (led_b == 0b00000100) {
	if (mode_flag == TRUE && position == 1) {
	     *led_led  = WZOR2[min%10];
	} else {
 	     *led_led  = WZOR[min%10];
	}

     }
     if (led_b == 0b00001000) {
	if (mode_flag == TRUE && position == 1) {
	     *led_led  = WZOR2[min/10];
	} else {
 	     *led_led  = WZOR[min/10];
	}
     }
      if (led_b == 0b00010000) {
	if (mode_flag == TRUE && position == 2) {
	     *led_led  = WZOR2[hour%10];
	} else {
 	     *led_led  = WZOR[hour%10];
	}
     }
     if (led_b == 0b00100000) {
	if (mode_flag == TRUE && position == 2) {
	     *led_led  = WZOR2[hour/10];
	} else {
 	     *led_led  = WZOR[hour/10];
	}
     }
     SEG_ON_OFF = FALSE;
     led_b<<=1;
     if (led_b > 0b00100000) {
      led_b = 0b00000001;
     }
 }
 
 void keyboard() {
    if (KBD == TRUE) {
    	current_key = key_b;
       if (press == FALSE) {
       	  if (current_key == 0b00000001) {
            mode_flag = !mode_flag;
            press = TRUE;
    	    }
    	    if (mode_flag == TRUE) {
               if (current_key == 0b00000010) {
	       sec = 0;
	       min = 0;
	       hour = 0;
	       press = TRUE;
	       }
	       if (current_key == 0b00000100) {
	       	  if (position > 0) {
                    position--;
		  }
	       press = TRUE;
	       }
               if (current_key == 0b00001000) {
	       	  if (position == 0)
                   inc_sec();
		  if (position == 1)
		   inc_min();
	          if (position == 2)
	           inc_hour();
	       press = TRUE;
	       }
	       if (current_key == 0b00010000) {
                  if (position == 0)
                   dec_sec();
	          if (position == 1)
		   dec_min();
	          if (position == 2)
	           dec_hour();
	       press = TRUE;
	       }
       	       if (current_key == 0b00100000) {
	       	  if (position < 2) {
                    position++;
		  }
	       press = TRUE;
	       }

           }

        }

    } else if (KBD == FALSE && current_key == key_b && press == TRUE) {
      press = FALSE;
    }
    key_b<<=1;
    if (key_b > 0b00100000) 
       key_b = 0b00000001;

}
