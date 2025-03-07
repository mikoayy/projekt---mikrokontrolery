#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"
#include "mfrc522.h"
#include "hd44780pcf8574.h"
#include "lib_servo.h"

  int i = 17;
  int j = 17;
  char addr = 0x27;
  uint8_t byte;
	uint8_t str[MAX_LEN];
	uint8_t person_recognised = 0;
	uint8_t UID[4]  = {0x53, 0x41, 0x79, 0xB7}; 
  bool card_is_valid = false;

 void set_cursor(int x1, int x2){
    //ustawia wartosci i,j
    i = x1;
    j = x2;
 }

  void LCD_draw(){
    // funkcja prostej animacji tekstu
    if(!card_is_valid){
      if(i>=0){
        HD44780_PCF8574_DisplayClear(addr); // wyczyszczenie ekranu
        HD44780_PCF8574_PositionXY(addr, i,0 ); //ustawienie kursora na pozycji 0,0 (kolumna, wiersz) na ekranie lcd 
        HD44780_PCF8574_DrawString(addr, "Przyloz karte"); // napisanie tekstu na ekranie (zawsze na pozycji kursora)
        _delay_ms(50);
        i--;
      }
    }
    

    else{
      if(j>=0){
      HD44780_PCF8574_DisplayClear(addr);
      HD44780_PCF8574_PositionXY(addr, j,0 );
      HD44780_PCF8574_DrawString(addr, "Witaj Miki!");
      HD44780_PCF8574_PositionXY(addr, j,1 );
      HD44780_PCF8574_DrawString(addr, "wjezdzaj");
      _delay_ms(50);
      j--;
      }
      
    }
    
      
}

void open_gate(){
  if(i <0 & j<0 & card_is_valid)
  {
    _delay_ms(200);// delay po wyswietleniu tekstu
    set_angle(90); // obrot serwo do pozycji 90 stopni 
    _delay_ms(5000); // delay w celu umozliwenia wjazdu auta 
    set_angle(0); // obrot serwo do pozycji 0 stopni
    set_cursor(17,17); // ustawienie zmiennych kursora
    card_is_valid = false; // ustawienie zmiennej karty
    
  }
}

void LCD_init(){
  //funkcja inicjalizująca ekran LCD
  HD44780_PCF8574_Init(addr); // inicjalizacja LCD
  HD44780_PCF8574_DisplayClear(addr); //wyczyszczenie ekranu
  HD44780_PCF8574_DisplayOn(addr); //włączenie ekranu
}

int main(){
	spi_init(); // inicjalizacja spi
	mfrc522_init(); // inicjalizacja rfid
  LCD_init(); 
  servo_init(PB1); // inicjalizacja serwomechanizmu
  set_angle(0); // ustawienie serwo w pozytcji początkowej 



  while(1){
    byte = mfrc522_request(PICC_REQALL,str); // funkjca zwracająca status kraty / PICC_REQALL typ karty / funkcja zwraca CARD_FOUND przypisana wartość to 1, może także zwrócić CARD_NOT_FOUND - 2, ERROR- 3
    if(byte == CARD_FOUND){
      byte = mfrc522_get_card_serial(str); // funkcja przypisuje do tablicy str UID odczytanej karty, oraz zwraca status karty
      if(byte == CARD_FOUND){
        for (byte = 0;byte<4;byte++){
        // pętla porównuje pokolei wszystkie elementy obu tablic jest któreś elementy nie są równe, pętla jest przerywana
        if(UID[byte] != str[byte]) break;
      }
      if(byte == 4){
        // jeśli wszystkie elementy były równe byte ma wartość 4 co ustawia wartość zmiennej card_is_valid na true
        card_is_valid = true;
      }
      else{
        // przypadek gdy wartość byte jest różna od 4
        card_is_valid = false;
        HD44780_PCF8574_DisplayClear(addr);
        HD44780_PCF8574_DrawString(addr, "odmowa dostepu");
        set_cursor(17,17);
        _delay_ms(2000);
        

      }
      }
    }
    LCD_draw();
    open_gate();
  }

}


