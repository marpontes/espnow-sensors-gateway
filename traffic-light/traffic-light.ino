#define NSTATES  4
#define S1RED    13
#define S1YELLOW 12
#define S1GREEN  11
#define S2RED    10
#define S2YELLOW 9
#define S2GREEN  8
#define TRTIME   1000
#define S1DUR    3000
#define S2DUR    3000

struct semstate {
  int duration; 
  int s1red;
  int s1yellow;
  int s1green;
  int s2red;
  int s2yellow;
  int s2green;
};

semstate ss1 = {S1DUR, HIGH, LOW, LOW, LOW, LOW, HIGH};
semstate ss2 = {TRTIME, LOW, HIGH, LOW, LOW, HIGH, LOW};
semstate ss3 = {S2DUR, LOW, LOW, HIGH, HIGH, LOW, LOW};
semstate ss4 = {TRTIME, LOW, HIGH, LOW, LOW, HIGH, LOW};

struct semstate semstates[NSTATES] = {ss1, ss2, ss3, ss4};

int startState = NSTATES;

void setup() {
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

  semstate currentState = semstates[startState];

  digitalWrite(S1RED,    currentState.s1red);
  digitalWrite(S1YELLOW, currentState.s1yellow);
  digitalWrite(S1GREEN, currentState.s1green);
  digitalWrite(S2RED,    currentState.s2red);
  digitalWrite(S2YELLOW, currentState.s2yellow);
  digitalWrite(S2GREEN,  currentState.s2green);

  delay(currentState.duration);

}
