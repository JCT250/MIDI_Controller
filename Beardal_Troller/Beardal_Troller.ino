
// Set up hardware pins
#include <midi_Settings.h>
#include <midi_Namespace.h>
#include <midi_Defs.h>
#include <MIDI.h>

int btn_1 = 4;
int btn_2 = 2;
int btn_3 = 3;

int rgb_red = 9;
int rgb_green = 10;
int rgb_blue = 11;

int btn_1_led = 5;
int btn_2_led = 6;

int current_mode = 1; //stores the current mode that we are in 
int bypass = 127; // whether we are bypassing or not 0 = no, 127 = yes
int undo_redo = 0; // whether we need to send undo or redo command
int inf_repeat = 0;

unsigned long counter_start = 0;
unsigned long counter_stop = 0;
unsigned long counter = 0;
int enter1 = 1; // used to check whether we should enter a loop again (button needs to go high before going low again in order to re enter a held loop)
int enter2 = 1;
int enter3 = 1;
int hold_length = 750; // set to chaange the ammount of time needed for a button press to be considered a hold
int inter_command_delay = 50; //delay between sending MIDI messages
int scroll_speed = 500; //speed between scroll repeats
int mode_change_debounce = 500; // debounce between mode changes (not sure if this is needed but is not then just set to 0)

// Set up midi message variables


void setup()
{
  pinMode(btn_1, INPUT_PULLUP);
  pinMode(btn_2, INPUT_PULLUP);
  pinMode(btn_3, INPUT_PULLUP);
  pinMode(rgb_red, OUTPUT);
  pinMode(rgb_green, OUTPUT);
  pinMode(rgb_blue, OUTPUT);
  pinMode(btn_1_led, OUTPUT);
  pinMode(btn_2_led, OUTPUT);
  
  digitalWrite(rgb_red, HIGH);
  digitalWrite(rgb_green, HIGH);
  digitalWrite(rgb_blue, HIGH);
  
  delay(5000);

  MIDI.begin(); // Launch MIDI

  /* add setup code here */

  digitalWrite(rgb_red, HIGH);
  digitalWrite(rgb_green, HIGH);
  digitalWrite(rgb_blue, LOW);


}

void loop()
{
  if(current_mode == 1) mode1();
  if(current_mode == 2) mode2();
  if(current_mode == 3) mode3();
}

void mode1()
{
  digitalWrite(rgb_red, LOW);
  digitalWrite(rgb_green, HIGH);
  digitalWrite(rgb_blue, HIGH);
  enter1 = 1;
  enter2 = 1;
  enter3 = 1;

  while(current_mode == 1) // perform the below if we are in the red mode
  {
    if(digitalRead(btn_1) == LOW)
    {
      delay(5);
      if(digitalRead(btn_1) == LOW)
      {

        digitalWrite(btn_1_led, HIGH); //turn button 1 led on

        MIDI.sendControlChange(82,0,1); //Button B down
        delay(inter_command_delay);
        MIDI.sendControlChange(80,0,1); //Button A down
        delay(inter_command_delay);
        MIDI.sendControlChange(80,127,1); // Button A up			

        counter = 0; //reset interval counter

        while(digitalRead(btn_1) == LOW)
        {
          counter ++;
          delay(1);
          if(counter >= hold_length)
          {
            MIDI.sendControlChange(80,0,1); //Button A down
            delay(inter_command_delay);
            MIDI.sendControlChange(80,127,1); // Button A up
            delay(scroll_speed);
          }
        }
        MIDI.sendControlChange(82,127,1); //Button B up
        digitalWrite(btn_1_led, LOW); //turn button 1 led off
      }
    }

    if(digitalRead(btn_2) == LOW)
    {
      delay(5);
      if(digitalRead(btn_2) == LOW)
      {

        digitalWrite(btn_2_led, HIGH); //turn button 2 led on

        MIDI.sendControlChange(82,0,1); //Button B down
        delay(inter_command_delay);
        MIDI.sendControlChange(81,0,1); //Button Tap down
        delay(inter_command_delay);
        MIDI.sendControlChange(81,127,1); // Button Tap up			

        counter = 0; //reset interval counter

        while(digitalRead(btn_2) == LOW)
        {
          counter ++;
          delay(1);
          if(counter >= hold_length)
          {
            MIDI.sendControlChange(81,0,1); //Button Tap down
            delay(inter_command_delay);
            MIDI.sendControlChange(81,127,1); // Button Tap up
            delay(scroll_speed);
          }
        }

        MIDI.sendControlChange(82,127,1); //Button B up
        digitalWrite(btn_2_led, LOW); //turn button 2 led off
      }
    }

    if(digitalRead(btn_3) == LOW)
    {
      delay(5);
      if(digitalRead(btn_3) == LOW)
      {
        if(enter3 == 1)
        {
          counter_start = millis(); //record the time the button is depressed

          while(digitalRead(btn_3) == LOW)
          {
            delay(1);
            counter_stop = millis(); // continually update the time
            if(counter_stop > (counter_start + hold_length)) break;
          }

          if(counter_stop < (counter_start + hold_length))// button pressed
          {
            current_mode ++; //increase the current mode value
            if(current_mode >= 4) current_mode = 1; //wrap around if we have gone too high
            delay(mode_change_debounce);
          }

          if(counter_stop > (counter_start + hold_length)) // button held
          {
            switch(bypass)
            {
            case 0: // if we are byp
              bypass = 127; //set variable to eng
              MIDI.sendControlChange(102,bypass,1); //send message to pedal
              break;
            case 127: //if we are eng
              bypass = 0; //set variable to byp
              MIDI.sendControlChange(102,bypass,1); //send message to pedal
              break;
            }
            enter3 = 0;
          }
        }
      }
    }
    if(digitalRead(btn_3) == HIGH) enter3 = 1;
  }
} 









// This one should be done

void mode2()
{
  digitalWrite(rgb_red, HIGH);
  digitalWrite(rgb_green, LOW);
  digitalWrite(rgb_blue, HIGH);
  enter1 = 1;
  enter2 = 1;
  enter3 = 1;

  while(current_mode == 2) // perform the below if we are in the green mode
  {
    if(digitalRead(btn_1) == LOW)
    {
      delay(5);
      if(digitalRead(btn_1) == LOW)
      {
        counter_start = millis(); //record the time the button is depressed

        digitalWrite(btn_1_led, HIGH); //turn button 1 led on

        while(digitalRead(btn_1) == LOW)
        {
          delay(1);
          counter_stop = millis(); // continually update the time to snapshot as close as possible the time the button is released
        }

        digitalWrite(btn_1_led, LOW); //turn button 1 led off

        if((counter_start + hold_length) > counter_stop)// button pressed
        {
          MIDI.sendControlChange(87,1,1); // send the record command
          delay(inter_command_delay);
        }

        if(( counter_start + hold_length) < counter_stop) // button held
        {
          // send the control message here
        }
      }
    }

    if(digitalRead(btn_2) == LOW)
    {
      delay(5);
      if(digitalRead(btn_2) == LOW)
      {
        if(enter2 == 1)
        {
          counter_start = millis(); //record the time the button is depressed

          digitalWrite(btn_2_led, HIGH); //turn button 2 led on

          while(digitalRead(btn_2) == LOW)
          {
            delay(1);
            counter_stop = millis(); // continually update the time to snapshot as close as possible the time the button is released
            if(counter_stop > (counter_start + hold_length)) break;
          }

          digitalWrite(btn_2_led, LOW); //turn button 2 led off

          if((counter_start + hold_length) >= counter_stop)// button pressed
          {
            MIDI.sendControlChange(85,1,1); // send the stop command
          }

          if(( counter_start + hold_length) < counter_stop) // button held
          {
            MIDI.sendControlChange(86,1,1); // send the start command
          }
          enter2 = 0;
        }
      }
    }

    if(digitalRead(btn_2) == HIGH) enter2 = 1;

    if(digitalRead(btn_3) == LOW)
    {
      delay(5);
      if(digitalRead(btn_3) == LOW)
      {
        counter_start = millis(); //record the time the button is depressed

        while(digitalRead(btn_3) == LOW)
        {
          delay(1);
          counter_stop = millis(); // continually update the time to snapshot as close as possible the time the button is released
          if((counter_start + hold_length) < counter_stop) // button held
          {
            switch(undo_redo)
            {
            case 0: // if we are set ot send undo
              undo_redo = 1; //set variable to eng
              MIDI.sendControlChange(89,1,1); //send undo message to pedal
              break;
            case 1: //if we are set to send redo
              undo_redo = 0; //set variable to byp
              MIDI.sendControlChange(90,1,1); //send redo message to pedal
              break;
            }
            counter_stop = counter_stop + 10;
            break;
          }
        }

        if((counter_start + hold_length) > counter_stop)// button pressed
        {
          current_mode ++; //increase the current mode value
          if(current_mode >= 4) current_mode = 1; //wrap around if we have gone too high
          delay(mode_change_debounce);
        }
      }
    }
  }
}









// This one should be done

void mode3()
{
  digitalWrite(rgb_red, HIGH);
  digitalWrite(rgb_green, HIGH);
  digitalWrite(rgb_blue, LOW);
  enter1 = 1;
  enter2 = 1;
  enter3 = 1;
  while(current_mode == 3) // perform the below if we are in the blue mode
  {
    if(digitalRead(btn_1) == LOW)
    {
      delay(5);
      if(digitalRead(btn_1) == LOW)
      {
        if(enter1 == 1)
        {
          digitalWrite(btn_1_led, HIGH); //turn button 1 led on

          digitalWrite(btn_1_led, LOW); //turn button 1 led off

          MIDI.sendControlChange(93,1,1); // Remote Tap

          enter1 = 0;
        }
      }
    }

    if(digitalRead(btn_1) == HIGH) enter1 = 1;

    if(digitalRead(btn_2) == LOW)
    {
      delay(5);
      if(digitalRead(btn_2) == LOW)
      {
        if(enter2 == 1)
        {
          digitalWrite(btn_2_led, HIGH); //turn button 2 led on

          MIDI.sendControlChange(97,127,1); //send undo message to pedal

          while(digitalRead(btn_2) == LOW)
          {
            delay(1); 
          }

          digitalWrite(btn_2_led, LOW); //turn button 2 led off

          MIDI.sendControlChange(97,0,1); //send undo message to pedal

          enter2 = 0;
        }
      }
    }

    if(digitalRead(btn_2) == HIGH) enter2 = 1;

    if(digitalRead(btn_3) == LOW)
    {
      delay(5);
      if(digitalRead(btn_3) == LOW)
      {
        current_mode ++; //increase the current mode value
        if(current_mode >= 4) current_mode = 1; //wrap around if we have gone too high
        delay(mode_change_debounce);
      }
    }
  }
}

// This one should be done























