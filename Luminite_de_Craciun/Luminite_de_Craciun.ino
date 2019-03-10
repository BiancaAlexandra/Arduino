#include <IRremote.h>

const int receiver = 5;
IRrecv irrecv(receiver);
decode_results results;
const byte releuPin = 6;
volatile byte stat = 0;
const int buzzerPin = 4;

volatile byte mod1 = 0;
volatile byte mod2 = 0;
volatile byte track = 0;
volatile byte n;

const int length[2] = {26,28}; // numarul de note din fiecare piesa

const char * notes[2] = \
    {"eeeeeeegcde fffffeeeeddedg","ggagsed deggsgg ggagsed deggsgg "};
    


const int beats[2][77] = {
   {1,1,2,1,1,2,1,1,1,1,4,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2 }, { 2,2,1,1,1,1,4,2,2,2,2,2,2,4,2,2,2,2,1,1,1,1,4,2,2,2,2,2}};
   
const int tempo[2] = {150, 150}; 

void setup()
{ 
  irrecv.enableIRIn();
  pinMode(releuPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() 
{
  if (stat==1 && mod1 == 0 && mod2 == 0) 
  {
    digitalWrite(releuPin,HIGH);
    delay(1000);
    digitalWrite(releuPin,LOW);
    delay(1000);
  }
  if (mod1 == 1 && stat == 0 && mod2 == 0)
  {
    track = 0;
    for (n = 0; n < length[track]; n++)
    {
      if (notes[track][n] == ' ') delay(beats[track][n] * tempo[track]); // pauză
      else playNote(notes[track][n], beats[track][n] * tempo[track]);
    }
    // pauza intre note
    delay(tempo[track] / 2);
  }
  if (mod2 == 1 && stat == 0 && mod1 == 0)
  {
    track = 1;
    for (n = 0; n < length[track]; n++)
    {
      if (notes[track][n] == ' ') delay(beats[track][n] * tempo[track]); // pauză
      else playNote(notes[track][n], beats[track][n] * tempo[track]);
    }
    // pauza intre note
    delay(tempo[track] / 2);
  }
  if (mod1 == 1 && stat == 1 && mod2 == 0)
  {
    track = 0;
    for (n = 0; n < length[track]; n++)
    {
      digitalWrite(releuPin,HIGH);
      delay(121);
      if (notes[track][n] == ' ') delay(beats[track][n] * tempo[track]); // pauză
      else
      { 
        
        playNote(notes[track][n], beats[track][n] * tempo[track]);
      }
      digitalWrite(releuPin,LOW);
      delay(121);
    }
    // pauza intre note
    delay(tempo[track] / 2);
  }
  
  if (mod2 == 1 && stat == 1 && mod1 == 0)
  {
    track = 1;
    for (n = 0; n < length[track]; n++)
    {
      digitalWrite(releuPin,HIGH);
      delay(121);
      if (notes[track][n] == ' ') delay(beats[track][n] * tempo[track]); // pauză
      else
      { 
        
        playNote(notes[track][n], beats[track][n] * tempo[track]);
      }
      digitalWrite(releuPin,LOW);
      delay(121);
    }
    // pauza intre note
    delay(tempo[track] / 2);
  }
  if (irrecv.decode(&results))
  {
    if (results.value==0xFFC23D) stat=!stat;
    else
    {
      if (results.value==0xFF30CF) mod1=!mod1;
      else 
      {
        if(results.value==0xFF18E7) mod2=!mod2;
        else eroare(2000,500);
      }
    }
    irrecv.resume(); // receive the next value
  }
}

void eroare(int tone, int duration) 
{
  for (long i = 0; i < duration * 1000L; i += tone * 2) 
  {
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(tone);
  }
}

void playTone(int tone, int duration)
{
  for (long i = 0; i < duration * 1000L; i += tone * 2)
  {
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration)
{
  const char names[] = { 'c', 'd', 'e', 'f', 's', 'g', 'a', 'v', 'b', 'C', 'D', 'E' }; // note
  const int tones[] = { 1915, 1700, 1519, 1432, 1352, 1275, 1136, 1073, 1014, 956, 852, 758 };
  // frecvente
  for (int i = 0; i < 12; i++)
  {
    if (names[i] == note) playTone(tones[i], duration);  
  } 
}
