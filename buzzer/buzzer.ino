#include <Bounce2.h>
#include "songs.h"
#define BUZZER 11
#define BPIN      7
#define MAXIDX 5

Bounce debouncer = Bounce(); // Instantiate a Bounce object

int idx = 5;

void setup() {
  pinMode(BPIN,     INPUT );
  debouncer.attach(BPIN,INPUT); 
  debouncer.interval(120); 
}

void play(){
  idx = idx + 1 > MAXIDX
                ? 0
                : idx + 1;
  switch(idx){
     case 0: 
        playSong(mario, sizeof(mario));
        break;
     case 1: 
        playSong(jigglypuffsong, sizeof(jigglypuffsong));
        break;
     case 2: 
        playSong(babyElephant, sizeof(babyElephant));
        break;
     case 3: 
        playSong(birthday, sizeof(birthday));
        break;
     case 4: 
        playSong(quietnight, sizeof(quietnight));
        break;
     case 5: 
        playSong(lullaby, sizeof(lullaby));
        break;

  }
}

void playSong(int melody[], int sze){
  
  int tempo = 140;
  int wholenote = (60000 * 4) / tempo;
  int divider = 0, noteDuration = 0;
  int notes = sze / sizeof(melody[0]) / 2;
  
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
     if ( digitalRead(BPIN) == HIGH ) { 
       delay(500);
       break;
     }
    divider = melody[thisNote + 1];
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; 
    }

    tone(BUZZER, melody[thisNote], noteDuration * 0.9);

    int start = millis();
    while(true){
      int curr = millis();
      if(curr - start  >= noteDuration){
         if ( digitalRead(BPIN) == HIGH ) { 
           delay(500);
           return;
         }
         break;
      }
    }

    noTone(BUZZER);
  }
}

void loop() {    
    debouncer.update();
     if ( debouncer.fell() ) { 
       tone(BUZZER, NOTE_CS5, 90 * 0.9);
       delay(500);
       play();
//       playSong(babyElephant, sizeof(babyElephant));
     }
  
}
