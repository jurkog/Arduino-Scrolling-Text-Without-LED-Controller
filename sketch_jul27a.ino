#include "letters_resource.h"

/* Pins of rows and columns numbered 0-7 */
const int rows[8] = {2, 7, 19, 5, 10, 18, 11, 16};
const int cols[8] = {6, 12, 13, 3, 17, 4, 8, 9};

/* Message buffer that is used to manipulate current display */
int message_buffer[8] = {B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000};

/* Test message I am using for this demonstration */
int *message[16] = {letter_i, space, heart, space, letter_s, letter_a, letter_n, letter_d, letter_r, letter_a, space, letter_p, letter_a, letter_p, letter_o, space};
const int MESSAGE_SIZE = 16; // TODO: Replace with sizeof
const int MESSAGE_SCROLL_SPEED = 100;
 
/* Variables to help determine current column to be appended */
int counter = 0;
long long timeVar;
                      
/* Setup Helper Functions */
void pinModeOutputForRowsAndColumns() {
  for (int i = 2; i < 20; i++) {
    pinMode(i, OUTPUT);
  }
}

/* Loop Helper Functions */

int ledLitOrNot(int ledValue, int col) {
  ledValue >>= 7 - col;
  if (ledValue & 1 == 1)
    return LOW;
    
  return HIGH;
}

void turnOnRow(int ledArray[8], int row) {
  digitalWrite(rows[row], HIGH);
  for (int col = 0; col < 8; col++)
    digitalWrite(cols[col], ledLitOrNot(ledArray[row], col));
}

void turnOffRow(int ledArray[8], int row) {
  digitalWrite(rows[row], LOW);
  for (int col = 0; col < 8; col++)
    digitalWrite(cols[col], LOW);
}

void toggleRow(int ledArray[8], int row) {
  turnOnRow(ledArray, row);
  delay(1);
  turnOffRow(ledArray, row);
}

void drawImage() {
  for (int row = 0; row < 8; row++) {
    toggleRow(message_buffer, row);
  }
}

/* Main Functions */
void setup() {
  Serial.begin(9600);
  pinModeOutputForRowsAndColumns();  
  timeVar = millis();
}

void updateCounterAndTime() {
  counter = (counter + 1) % (MESSAGE_SIZE * 8);
  timeVar = millis();
}

void shiftBuffer() {
  for (int row = 0; row < 8; row++)
    message_buffer[row] <<= 1;
}

void appendBufferWithCurrentCharacterColumn(int imageArray[8]) {
  int colToShift = counter % 8;
  for (int row = 0; row < 8; row++) {
    int currentImageRow = imageArray[row];
    currentImageRow >>= 7 - colToShift;
    currentImageRow &= 1;
    message_buffer[row] |= currentImageRow;
  }
}

void advanceBuffer(int *messageArray[8]) {
    updateCounterAndTime();
    shiftBuffer();
    appendBufferWithCurrentCharacterColumn(messageArray[counter/8]);
}

void advanceBufferIfCurrentTimeInvalidatesDisplay(int *messageArray[8]) {
  int timeDifference = millis() - timeVar;
  
  if (timeDifference > MESSAGE_SCROLL_SPEED)
    advanceBuffer(messageArray);
}

void drawMessage(int *messageArray[8]) {
  advanceBufferIfCurrentTimeInvalidatesDisplay(messageArray);
  drawImage(); 
}

void loop() {
  drawMessage(message);
}





