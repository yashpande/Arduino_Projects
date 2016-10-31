int ledpins[] = {2,4,6,7,8,10,12,13};
int pbpin = 0;
int ptpin = 2;
int speakerPin = 11;
int length = 15; // the number of notes
char notes[] = "ccggaagffeeddc "; // a space represents a rest
char notesbad[] = "c c c c c c c c "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  
  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}


void playmusic()
{
  for (int i = 0; i < length; i++) {
    if (i%2==0)
    {
      for (int j=0; j<8; j++)
        digitalWrite(ledpins[j],HIGH);
    }
    else
    {
      for (int j=0; j<8; j++)
        digitalWrite(ledpins[j],LOW);
    }
    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
    } else {
      playNote(notes[i], beats[i] * tempo);
    }
    // pause between notes
    delay(tempo / 2); 
  }
}

void playmusicbad(int rand)
{
  for (int i = 0; i < length; i++) {
    if (i%2==0)
    {
      digitalWrite(ledpins[rand],HIGH);
    }
    else
    {
      digitalWrite(ledpins[rand],LOW);
    }
    if (notesbad[i] == ' ') {
      delay(beats[i] * tempo); // rest
    } else {
      playNote(notesbad[i], beats[i] * tempo);
    }
    // pause between notes
    delay(tempo / 2); 
  }
}

void setup() {
  // initialize digital pin 13 as an output.
  Serial.begin(9600);
  for (int i=0; i<8; i++)
  {
      pinMode(ledpins[i], OUTPUT);
  }
  pinMode(speakerPin, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  int rand = random(8);
  digitalWrite(ledpins[rand],HIGH);
  while(analogRead(pbpin)<1023) {}
  digitalWrite(ledpins[rand],LOW);
  int stime = millis();
  int curled = 0;
  delay(1000);
  while(analogRead(pbpin)<1023)
  {
    digitalWrite(ledpins[curled],HIGH);
    if(millis()-stime>analogRead(ptpin))
    {
      digitalWrite(ledpins[curled],LOW);
      curled++;
      curled = curled%8;
      stime = millis();
    }
  }
  if(curled==rand)
  {
    playmusic();
  }
  else
  {
    playmusicbad(rand);
    digitalWrite(ledpins[curled],LOW);
  }
}

