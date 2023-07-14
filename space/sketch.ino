#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <OneButton.h> //buton icin gerekli kutuphane
//#include "pitches.h"
#define NOTE_DS5 622
#define NOTE_D5  587
#define NOTE_CS5 554
#define NOTE_C5  523
#define NOTE_G5  784
#define NOTE_E5  659
#define NOTE_G4  392
#define NOTE_E4  330


#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#define OLED_ADDR   0x3C

#define SPEAKER_PIN 2

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);

OneButton button(A2, true);
OneButton atesButton(A1, true);



int selected = 0;
uint8_t zorlukSelected = 0;
int entered = -1;
uint8_t zorlukEntered = 0;


uint8_t sayfa = 0; //0==menu, 1==basla, zorluk==2

uint8_t zorlukSeviyesi = 0;
int grid[8][16];


int can = 3;
bool olumsuz = 0; //ölümsüzlük
int atesHakki = 3;
unsigned int puan = 0;






//deneme baslangic

const unsigned char PROGMEM uzaygemisi [] = {


  0x3c, 0x60, 0xf0, 0x7f, 0x7f, 0xf0, 0x60, 0x3c

};

const unsigned char PROGMEM ates [] = {
  0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM meteor [] = {
  0x3c, 0x66, 0xd3, 0xaf, 0xa9, 0xfb, 0x66, 0x3c
};

const unsigned char PROGMEM cop [] = {
  0x00, 0x7e, 0x3c, 0xff, 0xff, 0x3c, 0x7e, 0x00
};

const unsigned char PROGMEM canResim [] = {
  0x18, 0x18, 0x18, 0xff, 0xff, 0x18, 0x18, 0x18
};

const unsigned char PROGMEM mermi [] = {
  0x20, 0x50, 0xf8, 0x88, 0x88, 0x88, 0x88, 0xf8
};

int ug = 0;


unsigned long startTime; // Variable to store the start time
int gecici;

unsigned long currentTime4 ;
unsigned long olumsuzlukBaslangic; // Variable to store the start time



unsigned long previousTime = 0; // Variable to store the previous time
unsigned long previousTime2 = 0;
unsigned long previousTime3 = 0;
unsigned long previousZorluk = 0;
unsigned long interval = 1000; // Interval in milliseconds (1 second)
unsigned long interval3 = 3000;
unsigned long intervalZorluk = 10000;
int zorlukIlkKontrol = 1;


//const int gameTones[] = { NOTE_G3, NOTE_C4, NOTE_E4, NOTE_G5};
void gameOver() {
  //Serial.print("Game over! your score: ");
  //Serial.println(gameIndex - 1);
  //gameIndex = 0;
  delay(200);

  // Play a Wah-Wah-Wah-Wah sound
  tone(SPEAKER_PIN, NOTE_DS5);
  delay(300);
  tone(SPEAKER_PIN, NOTE_D5);
  delay(300);
  tone(SPEAKER_PIN, NOTE_CS5);
  delay(300);
  for (byte i = 0; i < 10; i++) {
    for (int pitch = -10; pitch <= 10; pitch++) {
      tone(SPEAKER_PIN, NOTE_C5 + pitch);
      delay(5);
    }
  }
  noTone(SPEAKER_PIN);
  delay(500);
}


void playLevelUpSound() {
  tone(SPEAKER_PIN, NOTE_E4);
  delay(150);
  tone(SPEAKER_PIN, NOTE_G4);
  delay(150);
  tone(SPEAKER_PIN, NOTE_E5);
  delay(150);
  tone(SPEAKER_PIN, NOTE_C5);
  delay(150);
  tone(SPEAKER_PIN, NOTE_D5);
  delay(150);
  tone(SPEAKER_PIN, NOTE_G5);
  delay(150);
  noTone(SPEAKER_PIN);
}
// A = 49, 42, 35
// B = 48, 41, 34
// C = 47, 40, 33
// D = 46, 39, 32
// E = 45, 38, 31
// F = 44, 37, 30
// G = 43, 36, 29

//pinlerin azalan sırada olmasindan dolayi tersi yazildi
byte sayilar[10][7] = {
  {0, 1, 1, 1, 1, 1, 1}, //0
  {0, 0, 0, 0, 1, 1, 0}, //1
  {1, 0, 1, 1, 0, 1, 1}, //2
  {1, 0, 0, 1, 1, 1, 1}, //3
  {1, 1, 0, 0, 1, 1, 0}, //4
  {1, 1, 0, 1, 1, 0, 1}, //5
  {1, 1, 1, 1, 1, 0, 1}, //6
  {0, 0, 0, 0, 1, 1, 1}, //7
  {1, 1, 1, 1, 1, 1, 1}, //8
  {1, 1, 0, 1, 1, 1, 1} //9
};

void setup() {
  ug = 30;//deneme

  pinMode(53, INPUT_PULLUP);
  pinMode(51, INPUT_PULLUP);
  pinMode(5, OUTPUT);// sol yesil led
  pinMode(4, OUTPUT);// sol sari led
  pinMode(3, OUTPUT);// sol kirmizi led

  pinMode(6, OUTPUT);// sag yesil led
  pinMode(7, OUTPUT);// sag sari led
  pinMode(8, OUTPUT);// sag kirmizi led


  // A = 49, 42, 35
  // B = 48, 41, 34
  // C = 47, 40, 33
  // D = 46, 39, 32
  // E = 45, 38, 31
  // F = 44, 37, 30
  // G = 43, 36, 29

  for (int i = 29; i < 50; i++)
    pinMode(i, OUTPUT);


  button.attachDoubleClick(doubleclick);
  button.attachClick(singleclick);
  button.attachLongPressStop(longclick);
  atesButton.attachClick(atesEt);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);


  //display.clearDisplay();


  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 16; j++) {
      grid[i][j] = 0;
    }
  }
  //grid[0][0] = 9;

  //display.display();

  delay(100);
  display.clearDisplay();
}

void loop() {
  if (can > 3)
    can = 3;
  if (atesHakki > 3)
    atesHakki = 3;

  unsigned long currentTime3 = millis();
  if (olumsuz == 1 && currentTime3 - previousTime3 >= interval3) {
    previousTime3 = currentTime3;
    olumsuz = 0;
  } else if (olumsuz == 1 && currentTime3 - previousTime3 < interval3 && can > 0) {
    //previousTime3 = currentTime3;
    digitalWrite(5, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(3, HIGH);
    //delay(5);
    //digitalWrite(5, LOW);
    //digitalWrite(4, LOW);
    //digitalWrite(3, LOW);
  }


  //unsigned long currentTime4 = millis();


  menu();
  atesButton.tick();
  if (can > 3)
    can = 3;
  if (atesHakki > 3)
    atesHakki = 3;

  int birler = puan % 10;
  int onlar = (puan / 10) % 10;
  int yuzler = puan / 100;
  if (yuzler > 9) {
    puan = 0;
    yuzler = 0;
    onlar = 0;
    birler = 0;
  }


  for (int i = 29 ; i <= 49; i++) {
    if (i <= 49 && i >= 43) {
      digitalWrite(i, sayilar[yuzler][i - 43]);
    } else if (i <= 42 && i >= 36) {
      digitalWrite(i, sayilar[onlar][i - 36]);
    } else {
      digitalWrite(i, sayilar[birler][i - 29]);
    }
  }

  if (can == 3) {
    digitalWrite(5, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(3, HIGH);
  } else if (can == 2) {
    digitalWrite(5, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(3, HIGH);
  } else if (can == 1) {
    digitalWrite(5, LOW);
    digitalWrite(4, LOW);
    digitalWrite(3, HIGH);
  }
  else {
    digitalWrite(5, LOW);
    digitalWrite(4, LOW);
    digitalWrite(3, LOW);

    if (sayfa == 1)
      gameOver();
  }

  if (atesHakki == 3) {
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
  } else if (atesHakki == 2) {
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
  } else if (atesHakki == 1) {
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
  }
  else {
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
  }
}




void menu() {
  int LDRvalue = analogRead(A0);
  if (LDRvalue < 500)
    display.invertDisplay(true); // Ekran rengini tersine çevir
  else
    display.invertDisplay(false);



  /*display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);*/

  int up = digitalRead(51);
  int down = digitalRead(53);





  if (up == LOW && down == LOW) {
  };
  if (up == LOW && selected <= 0 && sayfa == 0) {
    selected = selected + 1;
    delay(200);
  };
  if (down == LOW && selected >= 1 && sayfa == 0) {
    selected = selected - 1;
    delay(200);
  };
  button.tick(); //checks the status of button
  /*if (enter == LOW) {
    entered = selected;
    };
    if (back == LOW) {
    entered = -1;
    };*/
  //delay(10);

  const char *options[2] = {
    "     Oyuna Basla     ",
    "   Zorluk Seviyesi   ",

  };



  if (entered == -1) {
    display.clearDisplay();
    display.setTextSize(1, 1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 15);

    for (int i = 0; i < 2; i++) {
      if (i == selected) {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        display.println(options[i]);
        display.println();
      } else if (i != selected) {
        display.setTextColor(SSD1306_WHITE);
        display.println(options[i]);
        display.println();
      }
    }
  } else if (entered == 0) {
    sayfa = 1;
    // Store the current time as the start time
    if (can > 0) {
      startTime = millis();
      //currentTime4 = millis();
      //previousZorluk=millis();
      oyun();
    } else {
      sayfa = 0;
      entered = -1;
    }
  } else if (entered == 1) {
    zorlukMenu();

  }


  //delay(1000);
  display.display();

}

void zorlukMenu() {
  sayfa = 2;
  int zorlukUp = digitalRead(51);
  int zorlukDown = digitalRead(53);
  button.tick();

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 16; j++) {
      grid[i][j] = 0;
    }
  }
  //grid[0][0] = 9;
  atesHakki = 3;
  can = 3;
  puan = 0;
  interval = 1000;



  if (zorlukUp == LOW && zorlukDown == LOW) {
  };
  if (zorlukUp == LOW && zorlukSelected <= 0 && sayfa == 2) {
    zorlukSelected = zorlukSelected + 1;
    delay(200);
  };
  if (zorlukDown == LOW && zorlukSelected >= 1 && sayfa == 2) {
    zorlukSelected = zorlukSelected - 1;
    delay(200);
  };

  const char *zorluk[2] = {
    "        KOLAY        ",
    "         ZOR         "
  };

  display.clearDisplay();
  display.setTextSize(1, 1);
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(0, 15);
  /* if(zorlukEntered==-1){
    for (int i = 0; i < 2; i++) {
      if (i == zorlukSelected) {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        display.println(zorluk[i]);
        display.println();
      } else if (i != zorlukSelected) {
        display.setTextColor(SSD1306_WHITE);
        display.println(zorluk[i]);
        display.println();
      }
    }
    }else*/ if (zorlukEntered == 0) {
    zorluk[0] = "-->     KOLAY        ";
    zorlukSeviyesi = 0;

    for (int i = 0; i < 2; i++) {
      if (i == zorlukSelected) {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        display.println(zorluk[i]);
        display.println();
      } else if (i != zorlukSelected) {
        display.setTextColor(SSD1306_WHITE);
        display.println(zorluk[i]);
        display.println();
      }
    }

  } else if (zorlukEntered == 1) {
    zorluk[1] = "-->      ZOR         ";
    zorlukSeviyesi = 1;
    zorlukIlkKontrol = 1;

    for (int i = 0; i < 2; i++) {
      if (i == zorlukSelected) {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        display.println(zorluk[i]);
        display.println();
      } else if (i != zorlukSelected) {
        display.setTextColor(SSD1306_WHITE);
        display.println(zorluk[i]);
        display.println();
      }
    }
  }
}

void oyun() {


  display.clearDisplay(); // OLED EKRANIN ÖNBELLEĞİNİ TEMİZLER
  display.drawPixel(51, 45, 1);
  display.drawPixel(30, 17, 1);
  display.drawPixel(60, 18, 1);
  display.drawPixel(55, 16, 1);
  display.drawPixel(25, 43, 1);
  display.drawPixel(100, 43, 1);
  display.drawPixel(117, 52, 1);
  display.drawPixel(14, 49, 1);
  display.drawPixel(24, 24, 1);
  display.drawPixel(78, 36, 1);
  display.drawPixel(80, 57, 1);
  display.drawPixel(107, 11, 1);
  display.drawPixel(123, 11, 1);
  display.drawPixel(9, 3, 1);
  display.drawPixel(2, 7, 1);
  display.drawPixel(51, 13, 1);
  display.drawPixel(60, 59, 1);
  display.drawPixel(53, 60, 1);
  display.drawPixel(51, 58, 1);
  display.drawPixel(13, 54, 1);
  display.drawPixel(65, 15, 1);
  display.setCursor(1, 1); // imleci belirtilen konuma götürür.
  display.setTextSize(1); // yazı boyutunu scale yöntemi ile büyültüp küçültür.
  display.setTextColor(1); // yazının opaklığını mat yada şefaf yapar 1=mat  0= şeffaf

  atesButton.tick();
  //display.println(atesEt());
  //display.println(ug);
  //display.display();// EKRAN ÖNBELLEĞİNDEKİ DEĞİŞİKLİKLERİ EKRANDA GÖSTER



  ug = analogRead(A3);            // reads the value of the potentiometer (value between 0 and 1023)
  ug = map(ug, 0, 1023, 0, 56) / 8;

  unsigned long currentTime = millis();


  display.drawBitmap(0, ug * 8, uzaygemisi, 8, 8, 0);
  display.drawBitmap(0, ug * 8, uzaygemisi, 8, 8, 1);
  grid[gecici][0] = 0;
  gecici = ug;
  if (grid[ug][0] == 0) {
    grid[ug][0] = 9;
  } else if (grid[ug][0] == 3) {

    can++;
    tone(2, 73, 200);
    olumsuz = 1;
    previousTime3 = currentTime;

  } else if (grid[ug][0] == 4) {

    atesHakki++;
    tone(2, 40, 200);

    //previousTime3 = currentTime;

  }
  else {
    if (olumsuz == 0)
    { can--;
      tone(2, 100, 100);
      olumsuz = 1;
      previousTime3 = currentTime;
    }
    grid[ug][0] = 9;

  }



  if (currentTime - previousTime >= interval) {
    previousTime = currentTime;
    puan++;


    for (int i = 0; i < 8; i++) {

      for (int k = 0; k <= 15; k++) {

        if (k != 0)
        {
          if (grid[i][k] == 1) {

            if (grid[i][k - 1] == 5) {
              grid[i][k] = 0;
              grid[i][k - 1] = 0;
            } else if (grid[i][k - 1] == 9) {//carpma
              grid[i][k] = 0;
              if (olumsuz == 0)
              { can--;
                tone(2, 100, 100);
                olumsuz = 1;
                previousTime3 = currentTime;
              }

              //display.setCursor(1, 1); // imleci belirtilen konuma götürür.
              //display.setTextSize(1); // yazı boyutunu scale yöntemi ile büyültüp küçültür.
              //display.setTextColor(1);
              //display.println("deneme1");
            }
            else
            { grid[i][k] = 0;
              grid[i][k - 1] = 1;
              //display.setCursor(1, 1); // imleci belirtilen konuma götürür.
              //display.setTextSize(1); // yazı boyutunu scale yöntemi ile büyültüp küçültür.
              //display.setTextColor(1);
              //display.println("deneme3");
            }
          } else if (grid[i][k] == 3) {

            if (grid[i][k - 1] == 5) {
              grid[i][k] = 0;
              grid[i][k - 1] = 0;
              can++;
              tone(2, 50, 30);
              olumsuz = 1;
              previousTime3 = currentTime;
            } else if (grid[i][k - 1] == 9) {//carpma
              grid[i][k] = 0;
              //can++;
              //if (olumsuz == 0){
              can++;
              tone(2, 50, 30);
              olumsuz = 1;
              previousTime3 = currentTime;
              // }

              //display.setCursor(1, 1); // imleci belirtilen konuma götürür.
              //display.setTextSize(1); // yazı boyutunu scale yöntemi ile büyültüp küçültür.
              //display.setTextColor(1);
              //display.println("deneme1");
            }
            else
            { grid[i][k] = 0;
              grid[i][k - 1] = 3;
              //display.setCursor(1, 1); // imleci belirtilen konuma götürür.
              //display.setTextSize(1); // yazı boyutunu scale yöntemi ile büyültüp küçültür.
              //display.setTextColor(1);
              //display.println("deneme3");
            }
          } else if (grid[i][k] == 4) {

            if (grid[i][k - 1] == 5) {
              grid[i][k] = 0;
              grid[i][k - 1] = 0;
              atesHakki++;
              tone(2, 40, 200);

              //previousTime3 = currentTime;
            } else if (grid[i][k - 1] == 9) {//carpma
              grid[i][k] = 0;
              //can++;
              //if (olumsuz == 0){
              atesHakki++;
              tone(2, 40, 200);

              //previousTime3 = currentTime;
              // }

              //display.setCursor(1, 1); // imleci belirtilen konuma götürür.
              //display.setTextSize(1); // yazı boyutunu scale yöntemi ile büyültüp küçültür.
              //display.setTextColor(1);
              //display.println("deneme1");
            }
            else
            { grid[i][k] = 0;
              grid[i][k - 1] = 4;
              //display.setCursor(1, 1); // imleci belirtilen konuma götürür.
              //display.setTextSize(1); // yazı boyutunu scale yöntemi ile büyültüp küçültür.
              //display.setTextColor(1);
              //display.println("deneme3");
            }
          }

          else if (grid[i][k] == 2) {

            if (grid[i][k - 1] == 5) {
              grid[i][k] = 1;
              grid[i][k - 1] = 0;
            } else if (grid[i][k - 1] == 9) {//carpma
              grid[i][k] = 0;
              if (olumsuz == 0)
              { can--;
                tone(2, 100, 100);
                olumsuz = 1;
                previousTime3 = currentTime;
              }
              //display.setCursor(1, 1); // imleci belirtilen konuma götürür.
              //display.setTextSize(1); // yazı boyutunu scale yöntemi ile büyültüp küçültür.
              //display.setTextColor(1);
              //display.println("deneme2");

            }
            else
            { grid[i][k] = 0;
              grid[i][k - 1] = 2;
            }

          }
        }
        else {
          if (grid[i][0] != 9)
            grid[i][0] = 0;
        }
      }



      for (int j = 14; j >= 1; j--) {
        if (grid[i][j] == 5  ) {
          if (grid[i][j + 1] == 1 ) {
            grid[i][j + 1] = 0;
            grid[i][j] = 0;
          } else if (grid[i][j + 1] == 2) {
            grid[i][j + 1] = 1;
            grid[i][j] = 0;
          }
          else if (grid[i][j + 1] == 3) {//can++
            grid[i][j + 1] = 0;
            grid[i][j] = 0;
            can++;
            tone(2, 50, 30);
            olumsuz = 1;
            previousTime3 = currentTime;
          } else if (grid[i][j + 1] == 4 ) {
            grid[i][j] = 0;
            grid[i][j + 1] = 0;
            atesHakki++;
            tone(2, 40, 200);
          }
          else {
            grid[i][j] = 0;
            grid[i][j + 1] = 5;
          }
        }


      }

    }// for i sonu
    //grid[5][15] = 1;

  }//zaman if sonu
  currentTime4 = millis();
  if (zorlukSeviyesi == 1 && zorlukIlkKontrol == 1) {
    zorlukIlkKontrol = 0;
    previousZorluk = millis();
  }
  //unsigned long yuzde;
  if (zorlukSeviyesi == 1 && (currentTime4 - previousZorluk >= 10000)) {
    interval = interval * (80.0 / 100.0);
    //interval=0;
    previousZorluk = currentTime4;
  }



  unsigned long currentTime2 = millis();
  if (currentTime2 - previousTime2 >= interval) {

    int dusmanKonum;
    int canKonum;
    int mermiKonum;

    do {
      dusmanKonum = random(0, 8);
    } while (grid[dusmanKonum][14] == 2 || grid[dusmanKonum][14] == 1 || grid[dusmanKonum][15] != 0);

    grid[dusmanKonum][15] = random(1, 3);

    if (puan % 30 == 0) {
      do {
        canKonum = random(0, 8);
      } while (grid[canKonum][14] == 2 || grid[canKonum][14] == 1 || grid[canKonum][15] != 0);

      grid[canKonum][15] = 3;
    }

    if (puan % 15 == 0) {
      do {
        mermiKonum = random(0, 8);
      } while (grid[mermiKonum][14] == 2 || grid[mermiKonum][14] == 1 || grid[mermiKonum][15] != 0  );

      grid[mermiKonum][15] = 4;
    }

    previousTime2 = currentTime2;
  }




  for (int i = 0; i < 8; i++) {
    if (grid[i][15] == 5) {
      grid[i][15] = 0;
    }
    for (int j = 0; j < 16; j++) {
      //display.print(grid[i][j]);

      if (grid[i][j] == 5) {

        display.drawBitmap( j * 8, i * 8, ates, 8, 8, 1);
      } else if (grid[i][j] == 2) {

        display.drawBitmap( j * 8, i * 8, meteor, 8, 8, 1);
      }
      else if (grid[i][j] == 1) {

        display.drawBitmap( j * 8, i * 8, cop, 8, 8, 1);
      } else if (grid[i][j] == 3) {

        display.drawBitmap( j * 8, i * 8, canResim, 8, 8, 1);
      }
      else if (grid[i][j] == 4) {

        display.drawBitmap( j * 8, i * 8, mermi, 8, 8, 1);
      }

    }
    //display.println();
  }
  /*
    display.setCursor(0, 0); // imleci belirtilen konuma götürür.
    display.setTextSize(1); // yazı boyutunu scale yöntemi ile büyültüp küçültür.
    display.setTextColor(1);
    display.println(interval);*/

  display.display();// EKRAN ÖNBELLEĞİNDEKİ DEĞİŞİKLİKLERİ EKRANDA GÖSTER
}


void atesEt() { //singleClickButton
  if (sayfa == 1 && atesHakki > 0) {
    tone(2, 300, 10);
    if (grid[ug][1] == 1)
    {
      grid[ug][1] = 0;
    } else if (grid[ug][1] == 2) {
      grid[ug][1] = 1;
    } else if (grid[ug][1] == 3) {
      grid[ug][1] = 0;
      can++;
      tone(2, 50, 30);
      olumsuz = 1;
      previousTime3 = millis();
    } else if (grid[ug][1] == 4) {
      grid[ug][1] = 0;
      atesHakki++;
      tone(2, 40, 200);
    }
    else if (grid[ug][1] == 0) {
      
      grid[ug][1] = 5;
    }
atesHakki--;

  }
}


void singleclick() {
  entered = selected;
}

void doubleclick() {
  zorlukEntered = zorlukSelected;
}

void longclick() {
  entered = -1;
  sayfa = 0;
}
