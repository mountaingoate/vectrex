/* 
  Arduino IR glasses control
   
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

  
  * Glasses : Sainsonic Rainbow 3D active shutter glasses (96-144 Hz)
   
  * Circuit : Analog pin 1 --- 220 Ohm --- IR LED --- GND
             IR LED : 850 nm, 1.5V, 50mA
  
  * IR protocol reference:
     https://cmst.curtin.edu.au/local/docs/pubs/2011-17-woods-helliwell-3D-Sync-IR.pdf
  
   Subtractin 2uS from each delay timing due to Arduino speed
   On-board LED (pin 13) is used for testing - only on for left eye to see
   
   After 1000 iterations, timing is printed on serial console it is average milliseconds 
   for left-right iteration period.
 */
 
long startTime = 0;
long endTime = 0;
long total = 0;
int  count = 0;

// Play with this to make glasses go faster or slower, 8000-10500 seem to work
#define DELAY 10500

void setup() {
  Serial.begin(115200);
  
  pinMode(A1, OUTPUT);
  digitalWrite(A1, LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loopCrystal() {
  // Crystal eyes - WORKS

  digitalWrite(13, HIGH);

  // Open left, close right
  digitalWrite(A1, HIGH);
  delayMicroseconds(118);
  digitalWrite(A1, LOW); 

  delayMicroseconds(DELAY);    

  digitalWrite(13, LOW);

  // Open right, close left
  digitalWrite(A1, HIGH);
  delayMicroseconds(58);
  digitalWrite(A1, LOW);  

  delayMicroseconds(DELAY);

}

  
void loopNuvision() {
  // Nuvision - WORKS

  digitalWrite(13, HIGH); 
  
  // Open right, close left
  digitalWrite(A1, HIGH);
  delayMicroseconds(2); 
  digitalWrite(A1, LOW); 
  delayMicroseconds(6); 
  digitalWrite(A1, HIGH);
  delayMicroseconds(2); 
  digitalWrite(A1, LOW); 
  delayMicroseconds(6); 
  digitalWrite(A1, HIGH);
  delayMicroseconds(2); 
  digitalWrite(A1, LOW); 
  
  delayMicroseconds(DELAY);    

  digitalWrite(13, LOW);

  // Open left, close right
  digitalWrite(A1, HIGH);
  delayMicroseconds(2);
  digitalWrite(A1, LOW);  
  delayMicroseconds(16);
  digitalWrite(A1, HIGH);
  delayMicroseconds(2);
  digitalWrite(A1, LOW);  
  
  delayMicroseconds(DELAY);

}


void loopXpand() {
  // Xpand
  
  digitalWrite(13, HIGH); 
  
  // Open right, close left
  digitalWrite(A1, HIGH);
  delayMicroseconds(16); 
  digitalWrite(A1, LOW); 
  delayMicroseconds(18); 
  digitalWrite(A1, HIGH);
  delayMicroseconds(16); 
  digitalWrite(A1, LOW); 
  delayMicroseconds(18); 
  digitalWrite(A1, HIGH);
  delayMicroseconds(16); 
  digitalWrite(A1, LOW); 
  
  delayMicroseconds(DELAY);    

  digitalWrite(13, LOW);

  // Open left, close right
  digitalWrite(A1, HIGH);
  delayMicroseconds(16);
  digitalWrite(A1, LOW);  
  delayMicroseconds(58);
  digitalWrite(A1, HIGH);
  delayMicroseconds(16);
  digitalWrite(A1, LOW);  
  
  delayMicroseconds(DELAY);

}

void loopPanasonic() {
  // Panasonic - DOES NOT WORK

  digitalWrite(13, HIGH); 

  // Close right
  digitalWrite(A1, HIGH);
  delayMicroseconds(18);
  digitalWrite(A1, LOW); 
  delayMicroseconds(58);
  digitalWrite(A1, HIGH);
  delayMicroseconds(18);
  digitalWrite(A1, LOW); 
  delayMicroseconds(18);
  digitalWrite(A1, HIGH);
  delayMicroseconds(18);
  digitalWrite(A1, LOW); 
  delayMicroseconds(58);
  digitalWrite(A1, HIGH);
  delayMicroseconds(18);
  digitalWrite(A1, LOW); 
  delayMicroseconds(161);

  // Open left
  digitalWrite(A1, HIGH);
  delayMicroseconds(18);
  digitalWrite(A1, LOW); 
  delayMicroseconds(58);
  digitalWrite(A1, HIGH);
  delayMicroseconds(18);
  digitalWrite(A1, LOW); 
  delayMicroseconds(58);
  digitalWrite(A1, HIGH);
  delayMicroseconds(18);
  digitalWrite(A1, LOW); 
  delayMicroseconds(18);
  digitalWrite(A1, HIGH);
  delayMicroseconds(18);
  digitalWrite(A1, LOW); 
  
  delayMicroseconds(DELAY);

  digitalWrite(13, HIGH); 

  // Close left
  digitalWrite(A1, HIGH);
  delayMicroseconds(18);
  digitalWrite(A1, LOW); 
  delayMicroseconds(18);
  digitalWrite(A1, HIGH);
  delayMicroseconds(18);
  digitalWrite(A1, LOW); 
  delayMicroseconds(58);
  digitalWrite(A1, HIGH);
  delayMicroseconds(18);
  digitalWrite(A1, LOW); 
  delayMicroseconds(58);
  digitalWrite(A1, HIGH);
  delayMicroseconds(18);
  digitalWrite(A1, LOW); 
  delayMicroseconds(161);

  // Open right
  digitalWrite(A1, HIGH);
  delayMicroseconds(18);
  digitalWrite(A1, LOW); 
  delayMicroseconds(18);
  digitalWrite(A1, HIGH);
  delayMicroseconds(18);
  digitalWrite(A1, LOW); 
  delayMicroseconds(98);
  digitalWrite(A1, HIGH);
  delayMicroseconds(18);
  digitalWrite(A1, LOW); 
  delayMicroseconds(18);
  digitalWrite(A1, HIGH);
  delayMicroseconds(18);
  digitalWrite(A1, LOW); 
  
  delayMicroseconds(DELAY);

  count++;
  endTime = millis();

}

void loop() {
  startTime = millis();

  loopCrystal();
  // loopNuvision();
  // loopXpand();

  count++;
  endTime = millis();

  total += (endTime - startTime);
  if (1000 == count) {
    Serial.println(total / 1000);
    total = 0;
    count = 0;
  }
}

