#include "notes.h"

int mario[] = {

  NOTE_E5,8, NOTE_E5,8, REST,8, NOTE_E5,8, REST,8, NOTE_C5,8, NOTE_E5,8, //1
  NOTE_G5,4, REST,4, NOTE_G4,8, REST,4, 
  NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4, // 3
  NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
  NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
  REST,8, NOTE_E5,4,NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,
  NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4, // repeats from 3
  NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
  NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
  REST,8, NOTE_E5,4,NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,

  
  REST,4, NOTE_G5,8, NOTE_FS5,8, NOTE_F5,8, NOTE_DS5,4, NOTE_E5,8,//7
  REST,8, NOTE_GS4,8, NOTE_A4,8, NOTE_C4,8, REST,8, NOTE_A4,8, NOTE_C5,8, NOTE_D5,8,
  REST,4, NOTE_DS5,4, REST,8, NOTE_D5,-4,
  NOTE_C5,2, REST,2,

  REST,4, NOTE_G5,8, NOTE_FS5,8, NOTE_F5,8, NOTE_DS5,4, NOTE_E5,8,//repeats from 7
  REST,8, NOTE_GS4,8, NOTE_A4,8, NOTE_C4,8, REST,8, NOTE_A4,8, NOTE_C5,8, NOTE_D5,8,
  REST,4, NOTE_DS5,4, REST,8, NOTE_D5,-4,
  NOTE_C5,2, REST,2,

  NOTE_C5,8, NOTE_C5,4, NOTE_C5,8, REST,8, NOTE_C5,8, NOTE_D5,4,//11
  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2,

  NOTE_C5,8, NOTE_C5,4, NOTE_C5,8, REST,8, NOTE_C5,8, NOTE_D5,8, NOTE_E5,8,//13
  REST,1, 
  NOTE_C5,8, NOTE_C5,4, NOTE_C5,8, REST,8, NOTE_C5,8, NOTE_D5,4,
  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2,
  NOTE_E5,8, NOTE_E5,8, REST,8, NOTE_E5,8, REST,8, NOTE_C5,8, NOTE_E5,4,
  NOTE_G5,4, REST,4, NOTE_G4,4, REST,4, 
  NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4, // 19
  
//  NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
//  NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
//  REST,8, NOTE_E5,4, NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,
//
//  NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4, // repeats from 19
//  NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
//  NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
//  REST,8, NOTE_E5,4, NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,
//
//  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,//23
//  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
//  NOTE_D5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_G5,-8, NOTE_F5,-8,
//  
//  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2, //26
//  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,
//  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
//  NOTE_B4,8, NOTE_F5,4, NOTE_F5,8, NOTE_F5,-8, NOTE_E5,-8, NOTE_D5,-8,
//  NOTE_C5,8, NOTE_E4,4, NOTE_E4,8, NOTE_C4,2,
//
//  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,//repeats from 23
//  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
//  NOTE_D5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_G5,-8, NOTE_F5,-8,
//  
//  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2, //26
//  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,
//  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
//  NOTE_B4,8, NOTE_F5,4, NOTE_F5,8, NOTE_F5,-8, NOTE_E5,-8, NOTE_D5,-8,
//  NOTE_C5,8, NOTE_E4,4, NOTE_E4,8, NOTE_C4,2,
//  NOTE_C5,8, NOTE_C5,4, NOTE_C5,8, REST,8, NOTE_C5,8, NOTE_D5,8, NOTE_E5,8,
//  REST,1,
//
//  NOTE_C5,8, NOTE_C5,4, NOTE_C5,8, REST,8, NOTE_C5,8, NOTE_D5,4, //33
//  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2,
//  NOTE_E5,8, NOTE_E5,8, REST,8, NOTE_E5,8, REST,8, NOTE_C5,8, NOTE_E5,4,
//  NOTE_G5,4, REST,4, NOTE_G4,4, REST,4, 
//  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,
//  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
//  NOTE_D5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_G5,-8, NOTE_F5,-8,
//  
//  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2, //40
//  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,
//  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
//  NOTE_B4,8, NOTE_F5,4, NOTE_F5,8, NOTE_F5,-8, NOTE_E5,-8, NOTE_D5,-8,
//  NOTE_C5,8, NOTE_E4,4, NOTE_E4,8, NOTE_C4,2,
//  
//  //game over sound
//  NOTE_C5,-4, NOTE_G4,-4, NOTE_E4,4, //45
//  NOTE_A4,-8, NOTE_B4,-8, NOTE_A4,-8, NOTE_GS4,-8, NOTE_AS4,-8, NOTE_GS4,-8,
//  NOTE_G4,8, NOTE_D4,8, NOTE_E4,-2,  

};


int jigglypuffsong[] = {

  // Jigglypuff's Song
  // Score available at https://musescore.com/user/28109683/scores/5044153
  
  NOTE_D5,-4, NOTE_A5,8, NOTE_FS5,8, NOTE_D5,8,
  NOTE_E5,-4, NOTE_FS5,8, NOTE_G5,4,
  NOTE_FS5,-4, NOTE_E5,8, NOTE_FS5,4,
  NOTE_D5,-2,
  NOTE_D5,-4, NOTE_A5,8, NOTE_FS5,8, NOTE_D5,8,
  NOTE_E5,-4, NOTE_FS5,8, NOTE_G5,4,
  NOTE_FS5,-1,
  NOTE_D5,-4, NOTE_A5,8, NOTE_FS5,8, NOTE_D5,8,
  NOTE_E5,-4, NOTE_FS5,8, NOTE_G5,4,
  
  NOTE_FS5,-4, NOTE_E5,8, NOTE_FS5,4,
  NOTE_D5,-2,
  NOTE_D5,-4, NOTE_A5,8, NOTE_FS5,8, NOTE_D5,8,
  NOTE_E5,-4, NOTE_FS5,8, NOTE_G5,4,
  NOTE_FS5,-1,
  
};



int babyElephant[] = {

  // Baby Elephant Walk
  // Score available at https://musescore.com/user/7965776/scores/1862611

  
  NOTE_C4,-8, NOTE_E4,16, NOTE_G4,8, NOTE_C5,8, NOTE_E5,8, NOTE_D5,8, NOTE_C5,8, NOTE_A4,8,
  NOTE_FS4,8, NOTE_G4,8, REST,4, REST,2,
  NOTE_C4,-8, NOTE_E4,16, NOTE_G4,8, NOTE_C5,8, NOTE_E5,8, NOTE_D5,8, NOTE_C5,8, NOTE_A4,8,
  NOTE_G4,-2, NOTE_A4,8, NOTE_DS4,1,
  
  NOTE_A4,8,
  NOTE_E4,8, NOTE_C4,8, REST,4, REST,2,
  NOTE_C4,-8, NOTE_E4,16, NOTE_G4,8, NOTE_C5,8, NOTE_E5,8, NOTE_D5,8, NOTE_C5,8, NOTE_A4,8,
  NOTE_FS4,8, NOTE_G4,8, REST,4, REST,4, REST,8, NOTE_G4,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_B4,8, NOTE_G4,8, REST,8, NOTE_G4,8,
   
  NOTE_C5,4, NOTE_C5,4, NOTE_AS4,16, NOTE_C5,16, NOTE_AS4,16, NOTE_G4,16, NOTE_F4,8, NOTE_DS4,8,
  NOTE_FS4,4, NOTE_FS4,4, NOTE_F4,16, NOTE_G4,16, NOTE_F4,16, NOTE_DS4,16, NOTE_C4,8, NOTE_G4,8,
  NOTE_AS4,8, NOTE_C5,8, REST,4, REST,2,
};

int birthday[] = {

  // Happy Birthday
  // Score available at https://musescore.com/user/8221/scores/26906

  NOTE_C4,4, NOTE_C4,8, 
  NOTE_D4,-4, NOTE_C4,-4, NOTE_F4,-4,
  NOTE_E4,-2, NOTE_C4,4, NOTE_C4,8, 
  NOTE_D4,-4, NOTE_C4,-4, NOTE_G4,-4,
  NOTE_F4,-2, NOTE_C4,4, NOTE_C4,8,

  NOTE_C5,-4, NOTE_A4,-4, NOTE_F4,-4, 
  NOTE_E4,-4, NOTE_D4,-4, NOTE_AS4,4, NOTE_AS4,8,
  NOTE_A4,-4, NOTE_F4,-4, NOTE_G4,-4,
  NOTE_F4,-2,
 
};


int quietnight[] = {

  // Silent Night, Original Version
  // Score available at https://musescore.com/marcsabatella/scores/3123436

  NOTE_G4,-4, NOTE_A4,8, NOTE_G4,4,
  NOTE_E4,-2, 
  NOTE_G4,-4, NOTE_A4,8, NOTE_G4,4,
  NOTE_E4,-2, 
  NOTE_D5,2, NOTE_D5,4,
  NOTE_B4,-2,
  NOTE_C5,2, NOTE_C5,4,
  NOTE_G4,-2,

  NOTE_A4,2, NOTE_A4,4,
  NOTE_C5,-4, NOTE_B4,8, NOTE_A4,4,
  NOTE_G4,-4, NOTE_A4,8, NOTE_G4,4,
  NOTE_E4,-2, 
  NOTE_A4,2, NOTE_A4,4,
  NOTE_C5,-4, NOTE_B4,8, NOTE_A4,4,
  NOTE_G4,-4, NOTE_A4,8, NOTE_G4,4,
  NOTE_E4,-2, 
  
  NOTE_D5,2, NOTE_D5,4,
  NOTE_F5,-4, NOTE_D5,8, NOTE_B4,4,
  NOTE_C5,-2,
  NOTE_E5,-2,
  NOTE_C5,4, NOTE_G4,4, NOTE_E4,4,
  NOTE_G4,-4, NOTE_F4,8, NOTE_D4,4,
  NOTE_C4,-2,
  NOTE_C4,-1,
  
};


int lullaby[] = {

  // Wiegenlied (Brahms' Lullaby)
  // Score available at https://www.flutetunes.com/tunes.php?id=54

  NOTE_G4, 4, NOTE_G4, 4, //1
  NOTE_AS4, -4, NOTE_G4, 8, NOTE_G4, 4,
  NOTE_AS4, 4, REST, 4, NOTE_G4, 8, NOTE_AS4, 8,
  NOTE_DS5, 4, NOTE_D5, -4, NOTE_C5, 8,
  NOTE_C5, 4, NOTE_AS4, 4, NOTE_F4, 8, NOTE_G4, 8,
  NOTE_GS4, 4, NOTE_F4, 4, NOTE_F4, 8, NOTE_G4, 8,
  NOTE_GS4, 4, REST, 4, NOTE_F4, 8, NOTE_GS4, 8,
  NOTE_D5, 8, NOTE_C5, 8, NOTE_AS4, 4, NOTE_D5, 4,

  NOTE_DS5, 4, REST, 4, NOTE_DS4, 8, NOTE_DS4, 8, //8
  NOTE_DS5, 2, NOTE_C5, 8, NOTE_GS4, 8,
  NOTE_AS4, 2, NOTE_G4, 8, NOTE_DS4, 8,
  NOTE_GS4, 4, NOTE_AS4, 4, NOTE_C5, 4,
  NOTE_AS4, 2, NOTE_DS4, 8, NOTE_DS4, 8,
  NOTE_DS5, 2, NOTE_C5, 8, NOTE_GS4, 8,
  NOTE_AS4, 2, NOTE_G4, 8, NOTE_DS4, 8,
  NOTE_AS4, 4, NOTE_G4, 4, NOTE_DS4, 4,
  NOTE_DS4, 2

};
