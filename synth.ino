#define INTERVAL_MS_LED       1     // TODO might need adjustment when 12 LEDS on
#define INTERVAL_MS_BUTTONS   200
#define SERIAL_RATE           9600

// TODO: rewrite when all added
#define BUTTON_1_PIN          2
#define BUTTON_CHORD_PIN      0
#define BUTTON_OCTAVE_DOWN    1
#define BUTTON_OCTAVE_UP      4
#define LED_SELECT_PIN_6      6
#define LED_SELECT_PIN_7      7
#define LED_SELECT_PIN_8      8

#define LED_PIN_R             9
#define LED_PIN_G             10
#define LED_PIN_B             11

#define MIDI_CMD_NOTE_OFF    0x80
#define MIDI_CMD_NOTE_ON     0x90
#define MIDI_CMD_CHORDS      0xF4

#define MIDI_BASE_TONE       0x36    // C3
#define MIDI_VELOCITY        0x45
#define TONE_COUNT           12


unsigned long currentTime;
unsigned long previousTimeLeds;
unsigned long previousTimeButtons;

const uint8_t crtlButtons[] = {BUTTON_CHORD_PIN, BUTTON_OCTAVE_DOWN, BUTTON_OCTAVE_UP};
const uint8_t toneButtons[] = {BUTTON_1_PIN};
const uint8_t outLeds[] = {LED_SELECT_PIN_8, LED_SELECT_PIN_7, LED_SELECT_PIN_6};
const uint8_t colourLeds[] = {LED_PIN_R, LED_PIN_G, LED_PIN_B};

const int outLedCount = sizeof(outLeds) / sizeof(outLeds[0]);
uint8_t led = -1;
uint8_t octaveFactor = 0;

const int colours[12][3] = {{255, 0, 0},      // RED
                            {255, 0, 25},     // PINK 1
                            {255, 150, 0},    // YELLOW
                            {150, 255, 0},    // GREEN-YELLOW
                            {0, 255, 0},      // GREEN
                            {0, 255, 100},    // AQUA
                            {0, 255, 255},    // TEAL
                            {0, 0, 255},      // BLUE
                            {100, 0, 255},    // PURPLE
                            {255, 0, 255},    // PINK 2
                            {255, 0, 100},    // MAGENTA
                            {255, 255, 255}}; // WHITE


void setup() {
  Serial.begin(SERIAL_RATE);

  for(int i = 0; i < sizeof(crtlButtons); i++) {
    pinMode(crtlButtons[i], INPUT);
  }

  for(int i = 0; i < sizeof(toneButtons); i++) {
    pinMode(toneButtons[i], INPUT);
  }

  for(int i = 0; i < sizeof(colourLeds); i++) {
    pinMode(colourLeds[i], OUTPUT);
  }

  for(int i = 0; i < sizeof(outLeds); i++) {
    pinMode(outLeds[i], OUTPUT);
    digitalWrite(outLeds[i], LOW);
  }

  previousTimeLeds, previousTimeButtons = millis();
}


// send midi control command
uint8_t sendCtrl(int cmd) {
  if(cmd < 127) return 1;

  Serial.write(cmd);
  return 0;
}

// send note via midi as command, pitch, velocity
uint8_t sendNote(int cmd, int pitch, int velocity) {
  if(cmd < 127) return 1;
  if(pitch >= 127) return 2;

  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
  return 0;
}


int8_t readButtons() {  // TODO handle returned value errors (ovo se neće dešavat, ali svejedno)
  if (digitalRead(BUTTON_CHORD_PIN) == HIGH) {  
    sendCtrl(MIDI_CMD_CHORDS);
  }

  if (digitalRead(BUTTON_OCTAVE_DOWN) == HIGH) 
    if(octaveFactor - 1 >= 0) octaveFactor--;

  if (digitalRead(BUTTON_OCTAVE_UP) == HIGH) 
    if(octaveFactor + 1 <= 9) octaveFactor++;

  
  for(int i = 0; i < sizeof(toneButtons); i++) {
    if (digitalRead(toneButtons[i]) == HIGH) {
      sendNote(MIDI_CMD_NOTE_ON,
              (MIDI_BASE_TONE + i) * octaveFactor,
              MIDI_VELOCITY);
    } 
  }

  return 0;   // no error
}


void loop() { 
  currentTime = millis(); 

  /* -------------------- LED colours -------------------- */
  if ((currentTime - previousTimeLeds) > INTERVAL_MS_LED) {  
    led = (led + 1) % outLedCount;
    uint8_t prev = (led - 1 >= 0) ? led-1 : outLedCount-1;
    
    digitalWrite(outLeds[prev], LOW);      

    analogWrite(LED_PIN_R, colours[led][0]);
    analogWrite(LED_PIN_G, colours[led][1]);
    analogWrite(LED_PIN_B, colours[led][2]);

    digitalWrite(outLeds[led], HIGH);
    
    previousTimeLeds = millis();
  }

  /* -------------------- BUTTONS -------------------- */
  if ((currentTime - previousTimeButtons) > INTERVAL_MS_BUTTONS) {  
    readButtons();
    previousTimeButtons = millis();
  }
}