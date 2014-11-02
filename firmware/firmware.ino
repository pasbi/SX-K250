
int keys[72] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
                 
int transpose = 101;
int velocity = 0x45;
int control = 0;

void sendCommand(int cmd, int data1, int data2) 
{
  Serial1.write(cmd);
  Serial1.write(data1);
  Serial1.write(data2);
}

int makeSliderLinear(int value)
{
  return 1024 * pow(value / 1024.0, 3);
}


void check(int note, int level)
{    
  if ( !!keys[note] != !!level ) {
    if (level) {
      keys[note] = 1;
      sendCommand( 0x90, transpose - note, velocity );
    } else {
      keys[note] = 0;
      sendCommand( 0x80, transpose - note, velocity );
    }
  }
}


void select(int group)
{
  if (group == 8) {
      digitalWrite(  4, LOW );
      digitalWrite(  5, LOW );
      digitalWrite(  6, LOW );
      digitalWrite(  7, HIGH );
  } else {
      digitalWrite(  4, group & 0x1 ? HIGH : LOW );
      digitalWrite(  5, group & 0x2 ? HIGH : LOW );
      digitalWrite(  6, group & 0x4 ? HIGH : LOW );
      digitalWrite(  7, LOW   );
  }
}

void setup()
{  
  // group selector
  pinMode(4,  OUTPUT);
  pinMode(5,  OUTPUT);
  pinMode(6,  OUTPUT);
  pinMode(7,  OUTPUT);
  
  // key return
  pinMode(8,  INPUT );
  pinMode(9,  INPUT );
  pinMode(10, INPUT );
  pinMode(11, INPUT );
  pinMode(12, INPUT );
  pinMode(13, INPUT );
  

// slider
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  
  
  // MIDI interface
  Serial1.begin(31250);
 
 // led code, control led 0-7
  pinMode(A5, OUTPUT); 
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  
}

void setLED(int n)
{
  // 1 and 2 are switched -.-
  if (n == 1) 
    n = 2;
  else if (n == 2)
    n = 1;
    
  digitalWrite(A5, n & 0x1 ? HIGH : LOW );
  digitalWrite(2,  n & 0x2 ? HIGH : LOW );
  digitalWrite(3,  n & 0x4 ? HIGH : LOW );
}


void updateTransposition()
{
  int val0 = makeSliderLinear(analogRead(A1));
  if (0 <= val0 && val0 < 205) {
    setLED(0);
    transpose = 101 - 24;
  } else if (205 <= val0 && val0 < 410) {
    setLED(1);
    transpose = 101 - 12;
  } else if (410 <= val0 && val0 < 614) {
    setLED(2);
    transpose = 101;
  } else if (614 <= val0 && val0 < 819) {
    setLED(3);
    transpose = 101 + 12;
  } else if (819 <= val0 && val0 < 1024) {
    setLED(4);
    transpose = 101 + 24;
  }
}

void updateVelocity()
{
  velocity = makeSliderLinear(analogRead(A4)) / 8;
}

void updateControl()
{
  
  int c = makeSliderLinear(analogRead(A3));
  if ( c / 8 == control / 8 )
    return;                    // do not send same command as before.
  
  int dist = c - control;
  if (dist < 0)
    dist = -dist;
    
  if (dist < 4)
    return;                    // ignore noise (< 0.4% of slider range)
    
  control = c;
  sendCommand( 0xB0, 1, c / 8 );  
}


void loop()
{  
  for (int group = 0; group <= 8; group++)
  {
    select( group );
    delay(5);
    for (int i = 0; i < 6; i++) 
    {
      check( i + group * 6, 
             digitalRead( 8 + i )   );
    }
  }
 
  updateTransposition();
  updateVelocity();
  updateControl();

}
