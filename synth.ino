#define LED_INTERVAL_MS       1     // TODO might need adjustment when 12 LEDS on
#define SERIAL_RATE           9600

// TODO: rewrite when all LEDs added
#define LED_SELECT_PIN_6  6
#define LED_SELECT_PIN_7  7
#define LED_SELECT_PIN_8  8

#define LED_PIN_R         9
#define LED_PIN_G         10
#define LED_PIN_B         11


unsigned long currentTime;
unsigned long previousTimeLeds;

const uint8_t outLeds[] = {LED_SELECT_PIN_8, LED_SELECT_PIN_7, LED_SELECT_PIN_6};
const uint8_t colourLeds[] = {LED_PIN_R, LED_PIN_G, LED_PIN_B};

const int outLedCount = sizeof(outLeds) / sizeof(outLeds[0]);
unsigned int outLedPrevs[12] = {0};
uint8_t led = -1;

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

  for(int i = 0; i < sizeof(colourLeds); i++) {
    pinMode(colourLeds[i], OUTPUT);
  }

  for(int i = 0; i < sizeof(outLeds); i++) {
    pinMode(outLeds[i], OUTPUT);
    digitalWrite(outLeds[i], LOW);
  }

  previousTimeLeds = millis();
}


void loop() { 
  currentTime = millis(); 

  /* -------------------- LED colours -------------------- */
  Serial.println(currentTime - previousTimeLeds);
  if ((currentTime - previousTimeLeds) > LED_INTERVAL_MS) {  
    led = (led + 1) % outLedCount;
    uint8_t prev = (led - 1 >= 0) ? led-1 : outLedCount-1;
    
    digitalWrite(outLeds[prev], LOW);      

    analogWrite(LED_PIN_R, colours[led][0]);
    analogWrite(LED_PIN_G, colours[led][1]);
    analogWrite(LED_PIN_B, colours[led][2]);

    digitalWrite(outLeds[led], HIGH);
    
    previousTimeLeds = millis();
  }
}