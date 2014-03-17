
// Set up hardware pins
#include <midi_Settings.h>
#include <midi_Namespace.h>
#include <midi_Defs.h>
#include <MIDI.h>

static int btn_1 = 0;
static int btn_2 = 0;
static int btn_3 = 0;

static int rgb_red = 0;
static int rgb_green = 0;
static int rgb_blue = 0;

static int btn_1_led = 0;
static int btn_2_led = 0;

int current_mode = 1; //stores the current mode that we are in 
int bypass = 127; // whether we are bypassing or not 0 = no, 127 = yes
int undo_redo = 0; // whether we need to send undo or redo command
int inf_repeat = 0;

int counter_start = 0;
int counter_stop = 0;
int counter = 0;
int hold_length = 1000; // set to chaange the ammount of time needed for a button press to be considered a hold
int inter_command_delay =10; //delay between sending MIDI messages
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


	/* add setup code here */

	analogWrite(rgb_red, 0);
	analogWrite(rgb_green, 0);
	analogWrite(rgb_blue, 255);

	MIDI.begin(4); // Launch MIDI and listen to channel 4
}

void loop()
{
	if(current_mode == 1) mode1();
	if(current_mode == 2) mode2();
	if(current_mode == 3) mode3();
}

void mode1()
{
	analogWrite(rgb_red, 0);
	analogWrite(rgb_green, 0);
	analogWrite(rgb_blue, 255);

	while(current_mode == 1) // perform the below if we are in the red mode
	{
		if(digitalRead(btn_1) == LOW);
		{

			digitalWrite(btn_1_led, HIGH); //turn button 1 led on

			MIDI.sendControlChange(82,0,0); //Button B down
			delay(inter_command_delay);
			MIDI.sendControlChange(80,0,0); //Button A down
			delay(inter_command_delay);
			MIDI.sendControlChange(80,127,0); // Button A up			

			counter = 0; //reset interval counter

			while(digitalRead(btn_1) == LOW);
			{
				counter ++;
				delay(1);
			}

			if(counter > hold_length);
			{
				MIDI.sendControlChange(80,0,0); //Button A down
				delay(inter_command_delay);
				MIDI.sendControlChange(80,127,0); // Button A up
				delay(scroll_speed);
			}

			MIDI.sendControlChange(82,127,0); //Button B up
			digitalWrite(btn_1_led, LOW); //turn button 1 led off
		}

		if(digitalRead(btn_2) == LOW);
		{

			digitalWrite(btn_2_led, HIGH); //turn button 2 led on

			MIDI.sendControlChange(82,0,0); //Button B down
			delay(inter_command_delay);
			MIDI.sendControlChange(81,0,0); //Button Tap down
			delay(inter_command_delay);
			MIDI.sendControlChange(81,127,0); // Button Tap up			

			counter = 0; //reset interval counter

			while(digitalRead(btn_2) == LOW);
			{
				counter ++;
				delay(1);
			}

			if(counter > hold_length);
			{
				MIDI.sendControlChange(81,0,0); //Button Tap down
				delay(inter_command_delay);
				MIDI.sendControlChange(81,127,0); // Button Tap up
				delay(scroll_speed);
			}

			MIDI.sendControlChange(82,127,0); //Button B up
			digitalWrite(btn_2_led, LOW); //turn button 2 led off
		}

		if(digitalRead(btn_3) == LOW);
		{
			counter_start = millis(); //record the time the button is depressed

			while(digitalRead(btn_3) == LOW);
			{
				counter_stop = millis(); // continually update the time to snapshot as close as possible the time the button is released
			}

			if((counter_start + hold_length) > counter_stop)// button pressed
			{
				current_mode ++; //increase the current mode value
				if(current_mode >= 4) current_mode = 1; //wrap around if we have gone too high
				delay(mode_change_debounce);
			}

			if((counter_start + hold_length) < counter_stop) // button held
			{
				switch(bypass)
				{
				case 0: // if we are byp
					bypass = 127; //set variable to eng
					MIDI.sendControlChange(102,bypass,0); //send message to pedal
					break;
				case 127: //if we are eng
					bypass = 0; //set variable to byp
					MIDI.sendControlChange(102,bypass,0); //send message to pedal
					break;
				}
			}
		}
	}
} 

// This one should be done

void mode2()
{
	analogWrite(rgb_red, 0);
	analogWrite(rgb_green, 255);
	analogWrite(rgb_blue, 0);

	while(current_mode == 1) // perform the below if we are in the green mode
	{
		if(digitalRead(btn_1) == LOW);
		{
			counter_start = millis(); //record the time the button is depressed

			digitalWrite(btn_1_led, HIGH); //turn button 1 led on

			while(digitalRead(btn_1) == LOW);
			{
				counter_stop = millis(); // continually update the time to snapshot as close as possible the time the button is released
			}

			digitalWrite(btn_1_led, LOW); //turn button 1 led off

			if((counter_start + hold_length) > counter_stop)// button pressed
			{
				MIDI.sendControlChange(87,1,0); // send the record command
				delay(inter_command_delay);
			}

			if(( counter_start + hold_length) < counter_stop) // button held
			{
				// send the control message here
			}
		}

		if(digitalRead(btn_2) == LOW);
		{
			counter_start = millis(); //record the time the button is depressed

			digitalWrite(btn_2_led, HIGH); //turn button 2 led on

			while(digitalRead(btn_2) == LOW);
			{
				counter_stop = millis(); // continually update the time to snapshot as close as possible the time the button is released
			}

			digitalWrite(btn_2_led, LOW); //turn button 2 led off

			if((counter_start + hold_length) > counter_stop)// button pressed
			{
				MIDI.sendControlChange(85,1,0); // send the stop command
			}

			if(( counter_start + hold_length) < counter_stop) // button held
			{
				MIDI.sendControlChange(86,1,0); // send the start command
			}
		}

		if(digitalRead(btn_3) == LOW);
		{
			counter_start = millis(); //record the time the button is depressed

			while(digitalRead(btn_3) == LOW);
			{
				counter_stop = millis(); // continually update the time to snapshot as close as possible the time the button is released
			}

			if((counter_start + hold_length) > counter_stop)// button pressed
			{
				current_mode ++; //increase the current mode value
				if(current_mode >= 4) current_mode = 1; //wrap around if we have gone too high
				delay(mode_change_debounce);
			}

			if(( counter_start + hold_length) < counter_stop) // button held
			{
				switch(undo_redo)
				{
				case 0: // if we are set ot send undo
					undo_redo = 1; //set variable to eng
					MIDI.sendControlChange(89,1,0); //send undo message to pedal
					break;
				case 1: //if we are set to send redo
					undo_redo = 0; //set variable to byp
					MIDI.sendControlChange(90,1,0); //send redo message to pedal
					break;
				}
			}
		}
	}
}

// This one should be done

void mode3()
{
	analogWrite(rgb_red, 255);
	analogWrite(rgb_green, 0);
	analogWrite(rgb_blue, 0);

	while(current_mode == 1) // perform the below if we are in the red mode
	{
		if(digitalRead(btn_1) == LOW);
		{
			counter_start = millis(); //record the time the button is depressed

			digitalWrite(btn_1_led, HIGH); //turn button 1 led on

			while(digitalRead(btn_1) == LOW);
			{
				counter_stop = millis(); // continually update the time to snapshot as close as possible the time the button is released
			}

			digitalWrite(btn_1_led, LOW); //turn button 1 led off

			if((counter_start + hold_length) > counter_stop)// button pressed
			{
				MIDI.sendControlChange(93,1,1); // Remote Tap
			}

			if(( counter_start + hold_length) < counter_stop) // button held
			{
				// send the control message here
			}
		}

		if(digitalRead(btn_2) == LOW);
		{
			counter_start = millis(); //record the time the button is depressed

			digitalWrite(btn_2_led, HIGH); //turn button 2 led on

			while(digitalRead(btn_2) == LOW);
			{
				counter_stop = millis(); // continually update the time to snapshot as close as possible the time the button is released
			}

			digitalWrite(btn_2_led, LOW); //turn button 2 led off

			if((counter_start + hold_length) > counter_stop)// button pressed
			{
				switch(inf_repeat)
				{
				case 0: // if inf repeat is off
					inf_repeat = 127; //set variable to turn it on
					MIDI.sendControlChange(97,inf_repeat,0); //send undo message to pedal
					break;
				case 1: //if inf repeat is on
					inf_repeat = 0; //set variable to turn it off
					MIDI.sendControlChange(97,inf_repeat,0); //send redo message to pedal
					break;
				}
			}

			if(( counter_start + hold_length) < counter_stop) // button held
			{
				// send the control message here
			}
		}

		if(digitalRead(btn_3) == LOW);
		{
			counter_start = millis(); //record the time the button is depressed

			while(digitalRead(btn_3) == LOW);
			{
				counter_stop = millis(); // continually update the time to snapshot as close as possible the time the button is released
			}

			if((counter_start + hold_length) > counter_stop)// button pressed
			{
				current_mode ++; //increase the current mode value
				if(current_mode >= 4) current_mode = 1; //wrap around if we have gone too high
				delay(mode_change_debounce);
			}

			if(( counter_start + hold_length) < counter_stop) // button held
			{
				// send control message here
			}
		}
	}
}

// This one should be done
