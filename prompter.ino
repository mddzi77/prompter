// columns
#define C1 13
#define C2 12
#define C3 11
#define C4 10
#define C5 9
#define C6 8
#define C7 7
#define C8 6
//rows
#define R1 5
#define R2 4
#define R3 3
#define R4 2
#define R5 A0
#define R6 A1
#define R7 A2
#define R8 A3
//
uint8_t COLS[] = {C1, C2, C3, C4, C5, C6, C7, C8};
uint8_t ROWS[] = {R1, R2, R3, R4, R5, R6, R7, R8};
int package = 0;
String data;
int image[8][8];
int temp_img[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 0, 1, 1},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
  };
//
void display();
void copy();
void selectColumn(int, int);
void selectRow(int, int);
//
void setup() {
  // setup pins
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
  pinMode(R5, OUTPUT);
  pinMode(R6, OUTPUT);
  pinMode(R7, OUTPUT);
  pinMode(R8, OUTPUT);

  pinMode(C1, OUTPUT);
  pinMode(C2, OUTPUT);
  pinMode(C3, OUTPUT);
  pinMode(C4, OUTPUT);
  pinMode(C5, OUTPUT);
  pinMode(C6, OUTPUT);
  pinMode(C7, OUTPUT);
  pinMode(C8, OUTPUT);

  // default values
  digitalWrite(R1, HIGH);
  digitalWrite(R2, HIGH);
  digitalWrite(R3, HIGH);
  digitalWrite(R4, HIGH);
  digitalWrite(R5, HIGH);
  digitalWrite(R6, HIGH);
  digitalWrite(R7, HIGH);
  digitalWrite(R8, HIGH);

  digitalWrite(C1, LOW);
  digitalWrite(C2, LOW);
  digitalWrite(C3, LOW);
  digitalWrite(C4, LOW);
  digitalWrite(C5, LOW);
  digitalWrite(C6, LOW);
  digitalWrite(C7, LOW);
  digitalWrite(C8, LOW);

  Serial.begin(38400);
  copy();
}

void loop() {

  while (Serial.available() > 0) {
    String data = Serial.readStringUntil('!');

    display(); // display image to 8x8 matrix led

    for (int j = 0; j < 8; j++) {
      if (data[j] == '#') {
        temp_img[package][j] = 1;
      }
      else {
        temp_img[package][j] = 0;
      }
    }

    // increment package count
    // if all packages are received copy received data to output image
    if (package == 7) {
      package = 0;
      copy();
    }
    else {
      package++;
      }
    
  }

  display(); // display image to 8x8 matrix led
}

void selectColumn(int col, int state) {
  int columnPIN = COLS[col];
  digitalWrite(columnPIN, state);
}

void selectRow(int row_iter, int row_selected) {
  if (row_iter == row_selected) {
    digitalWrite(ROWS[row_iter], 0);
  }
  else {
    digitalWrite(ROWS[row_iter], 1);
  }
}

void display() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      selectRow(j, i);
      selectColumn(j, image[i][j]);
    }
    delay(1);
  }
}

void copy() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      image[i][j] = temp_img[i][j];
    }
  }
}
