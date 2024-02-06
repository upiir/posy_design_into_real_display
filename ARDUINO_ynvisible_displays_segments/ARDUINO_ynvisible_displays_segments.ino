// a simple sketch to drive custom made displays from Ynvisible, based on designs from youtuber posy
// created by upir - 2023 - www.youtube.com/upir_upir
//
// YouTube Video: https://youtu.be/jz01j1TpM84
// Source files: https://github.com/upiir/posy_design_into_real_display
//
// Links from the video:
// Displays from the Future: https://www.youtube.com/watch?v=lxeH1ilL2T0
// Segmented Displays video from Posy - https://www.youtube.com/watch?v=RTB5XhjbgZA
// Ynvisible Guidelines - https://www.ynvisible.com/news-inspiration/making-a-custom-display-everything-you-need-to-know
// Segmented Displays from Posy SVG - http://www.michieldb.nl/other/segments/
// My other videos with segmented displays - https://www.youtube.com/playlist?list=PLjQRaMdk7pBZYAlQeaKdKJS1i52py4Ar2
// Jumper wires / dupont wires - https://s.click.aliexpress.com/e/_DnCVKOZ
// FFC/FPC Connector board - https://s.click.aliexpress.com/e/_DD2w1yp
// 
// Other videos using segment displays:
// Segment LED ring light: https://youtu.be/sE3LSYoCqLQ
// Arduino + Ynvisible Displays: https://youtu.be/lxeH1ilL2T0
// Arduino E-ink: https://youtu.be/Rf-E-7fFop0
// Raspberry PI Pico Displays: https://youtu.be/52DDySZQQBI


int arduino_pins[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, A1, A2, A0}; // to which Arduino pins is the display connected
int segments_current[15]; // current segments
int segments_new[15]; // new segments

int delay_turn_on = 400; // delay when turning the segment ON
int delay_turn_refresh = 100; // delay when refreshing the display
int delay_turn_off = 400; // delay when turning the segment OFF

// demo sequences
int segments_demo[2][15] = { 
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

int segments_demo_counter = 0;

// when the display should not be set, the pins should be set to inputs
void set_all_pins_to_inputs() {
  for (int i = 0; i < 16; i++) {
    pinMode(arduino_pins[i], INPUT);
  }
}

void setup() {
  analogWriteResolution(8); // 0-255 
  set_all_pins_to_inputs(); // set all pins to inputs
}

void loop() {

set_all_pins_to_inputs(); // set all pins to inputs

// get a new segments
for (int i = 0; i < 15; i++) {
  segments_new[i] = segments_demo[segments_demo_counter][i];
}

// part 1 of updating - update the segments that were ON and should be OFF
pinMode(DAC, OUTPUT); // DAC = A0
analogWrite(DAC, 76); // set the COM to be around 1.5V 

// all the pins that should be turned off shall be set to LOW OUTPUTS 
for (int i = 0; i < 15; i++) {
  if (segments_current[i] == 1 && segments_new[i] == 0) {
    pinMode(arduino_pins[i], OUTPUT);
    digitalWrite(arduino_pins[i], LOW);
  }
}

delay(delay_turn_off); // wait to turn all off
// -----------------------------------------------------

set_all_pins_to_inputs();  // set all pins to inputs

// part 2 of updating - update the segments that were OFF and should be ON
pinMode(DAC, OUTPUT); // DAC = A0
analogWrite(DAC, 255-76); // set the COM to be around 5.0 - 1.5V = 3.5V

// all the pins that should be turned on shall be set to HIGH OUTPUTS 
for (int i = 0; i < 15; i++) {
  if (segments_current[i] == 0 && segments_new[i] == 1) {
    pinMode(arduino_pins[i], OUTPUT);
    digitalWrite(arduino_pins[i], HIGH);
  }
}

delay(delay_turn_on); // wait to turn all on
// -----------------------------------------------------

set_all_pins_to_inputs(); // set all pins to inputs

// part 3 of updating - if the segment was ON and should still be ON, add a short pulse to refresh it
pinMode(DAC, OUTPUT); // DAC = A0
analogWrite(DAC, 255-76); // set the COM to be around 5.0 - 1.5V = 3.5V

// all the pins that should be turned on shall be set to HIGH OUTPUTS 
for (int i = 0; i < 15; i++) {
  if (segments_current[i] == 1 && segments_new[i] == 1) {
    pinMode(arduino_pins[i], OUTPUT);
    digitalWrite(arduino_pins[i], HIGH);
  }
}

delay(delay_turn_refresh); // wait to turn all on

set_all_pins_to_inputs(); // set all pins to inputs

// update the current segment array with new segments
for (int i = 0; i < 15; i++) {
  segments_current[i] = segments_new[i];
}

// jump into the next demo segments
segments_demo_counter = segments_demo_counter + 1;
if (segments_demo_counter >= 2) {segments_demo_counter = 0;}

// wait a while to keep the content displayed
delay(300);
}
