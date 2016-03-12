/*
  Vectrex 3d imager with 3d active shutter glasses and an L293D chip
  Built with Panasonic Viera TY-ER3D4MU glasses powered at 12V through
  the H-bridge
  
  Copyright (c) 2016, Mountain Goat Entertainment
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
  3. All advertising materials mentioning features or use of this software
     must display the following acknowledgement:
     This product includes software developed by the <organization>.
  4. Neither the name of the <organization> nor the
     names of its contributors may be used to endorse or promote products
     derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


// Define the pins for the H-bridge
#define PIN_1A 2
#define PIN_2A 3
#define PIN_3A 4
#define PIN_4A 5

// To Vectrex controller 2 pin 4 (button 4)
#define PIN_TO_VECX 7

// From Vectrex controller 2 pin3 - currently not used
#define PIN_FROM_VECX 6

// Press to button to start - eliminate player 2 button 4 at start
#define PIN_START  12

#define LENS_LEFT  0
#define LENS_RIGHT 1

#define LENS_LIGHT 0
#define LENS_DARK  1

// Easier to access pin numbers in a foor loop
int hBridgePins[4] = { PIN_1A, PIN_2A, PIN_3A, PIN_4A  };

// Keep track of which way we turned the H-bridge on the last time
// Does not really matter how these get initialized - they will be switching
// all the time
int currentDir[2] = {1, 1};

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 4; i++) {
    pinMode(hBridgePins[i], OUTPUT);
  }

  pinMode(PIN_FROM_VECX, INPUT);
  pinMode(PIN_TO_VECX, OUTPUT);

  pinMode(PIN_START, INPUT_PULLUP);

  digitalWrite(PIN_TO_VECX, 1);

  Serial.println("Ready...");

  // With multicarts, it may be bad if the Arduino "pressed" button 4 right away
  // So hold the line high until someone presses a button to start the glasses
  // and Vectrex signal
  int start = digitalRead(PIN_START);

  while (1 == start) {
    delay(1);
    start = digitalRead(PIN_START);
  }

  Serial.println("Starting glasses + Vectrex signal!");
}


/* 
 *  From http://www.ti.com/lit/ds/symlink/l293.pdf
 * 
 *  EN   1A   1A   Function
 *  ------------------------------
 *  H    L    H    Turn rigt
 *  H    H    L    Turn left
 *  H    L    L    Fast motor stop
 *  H    H    H    Fast motor stop
 *  L    X    X    Free-running motor stop
 */

void LensOnOff(int lens, int state)
{
  // Lens is 0 for left, 1 for right lens
  // state is 0 for off, 1 for on

  if (0 == state) {
//    Serial.print("Off ");
//    Serial.println(lens);
    
    digitalWrite(hBridgePins[lens * 2], 0);
    digitalWrite(hBridgePins[lens * 2 + 1], 0);

    return;
  }

//  Serial.print("On ");
//  Serial.println(lens);
//  Serial.print(lens * 2);
//  Serial.print(" - ");
//  Serial.print(currentDir[lens]);
//  Serial.print(", ");
//  Serial.print(lens * 2 + 1);
//  Serial.print(" - ");
//  Serial.print(1 - currentDir[lens]);
//  Serial.println("");
  
  // Every time the lens turns on, switch the current direction around
  digitalWrite(hBridgePins[lens * 2], currentDir[lens]);
  digitalWrite(hBridgePins[lens * 2 + 1], 1 - currentDir[lens]);

  currentDir[lens] = 1 - currentDir[lens];
}

int count = 0;
/* 
 * The timing for the glasses 
 * 
 * 37 ms cycle (ends up being 37.46 with all the code
 * 
 *  0 - 18 Right lens open, left dark
 * 18 - 19 Both lenses dark
 * 19 - 36 Left open, right dark
 * 36 - 37 Borh lenses dark
 * 
 * Also, in the 36-37 period is when we send the sync signal to the Vectrex
 */
void loop() {
  count++;

  if (18 == count) {
    // Almost alfway through the cycle darken both
    LensOnOff(LENS_RIGHT, LENS_DARK);
  }

  if (19 == count) {
    // Left lens open
    LensOnOff(LENS_LEFT, LENS_LIGHT);
  }

  if (36 == count) {
    // Close left
    LensOnOff(LENS_LEFT, LENS_DARK);

    // Drop the signal for 1 ms
    digitalWrite(PIN_TO_VECX, 0);
  }

  if (37 == count) {
    // Open right
    LensOnOff(LENS_RIGHT, LENS_LIGHT);

    // Raise the signal back up
    digitalWrite(PIN_TO_VECX, 1);

    count = 0;
  }

  delay(1);
  
}

