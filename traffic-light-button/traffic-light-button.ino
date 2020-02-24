#define NSTATES  4
#define S1RED    13
#define S1YELLOW 12
#define S1GREEN  11
#define S2RED    10
#define S2YELLOW 9
#define S2GREEN  8
#define TRTIME   1000
#define BPIN     4
#define FOREVER  10000

struct semstate {
  int duration;
  int s1red;
  int s1yellow;
  int s1green;
  int s2red;
  int s2yellow;
  int s2green;
};

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 500;
int buttonState;
int lastButtonState = LOW;

semstate ss1 = {FOREVER, HIGH, LOW, LOW, LOW, LOW, HIGH};
semstate ss2 = {TRTIME, LOW, HIGH, LOW, LOW, HIGH, LOW};
semstate ss3 = {FOREVER, LOW, LOW, HIGH, HIGH, LOW, LOW};
semstate ss4 = {TRTIME, LOW, HIGH, LOW, LOW, HIGH, LOW};

struct semstate semstates[NSTATES] = {ss1, ss2, ss3, ss4};

int startState = NSTATES;

void setup() {
  pinMode(BPIN,     INPUT );
  pinMode(S1RED,    OUTPUT);
  pinMode(S1YELLOW, OUTPUT);
  pinMode(S1GREEN,  OUTPUT);
  pinMode(S2RED,    OUTPUT);
  pinMode(S2YELLOW, OUTPUT);
  pinMode(S2GREEN,  OUTPUT);
}

void loop() {
  startState++;

  if (startState > NSTATES - 1) {
    startState = 0;
  }
  // read the current button pin state
  int buttonValue = digitalRead(BPIN); 
  
  semstate currentState = semstates[startState];

  writeLights(currentState);
  if(currentState.duration == FOREVER){
     while(buttonValue == LOW){
       buttonValue = digitalRead(BPIN);
       if(buttonValue != LOW){
          delay(200);
       }
     }
  }else{
    delay(currentState.duration);
  }
  
 
}

void writeLights(struct semstate state){
  digitalWrite(S1RED,    state.s1red);
  digitalWrite(S1YELLOW, state.s1yellow);
  digitalWrite(S1GREEN,  state.s1green);
  digitalWrite(S2RED,    state.s2red);
  digitalWrite(S2YELLOW, state.s2yellow);
  digitalWrite(S2GREEN,  state.s2green);
}
