
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

int current_mode = 1;

int counter_start = 0;
int counter_stop = 0;
int hold_length = 3000; // set to chaange the ammount of time needed for a button press to be considered a hold
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
	if(current_mode == 4) mode4();
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
			counter_start = millis(); //record the time the button is depressed

			digitalWrite(btn_1_led, HIGH); //turn button 1 led on

			while(digitalRead(btn_1) == LOW);
			{
				counter_stop = millis(); // continually update the time to snapshot as close as possible the time the button is released
			}

			digitalWrite(btn_1_led, LOW); //turn button 1 led off

			if((counter_start + hold_length) > counter_stop)// button pressed
			{
				// send the control message here
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
				// send the control message here
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
				if(current_mode >= 5) current_mode = 1; //wrap around if we have gone too high
				delay(mode_change_debounce);
			}

			if(( counter_start + hold_length) < counter_stop) // button held
			{
				// send control message here
			}
		}
	}
}

void mode2()
{
	analogWrite(rgb_red, 0);
	analogWrite(rgb_green, 255);
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
				// send the control message here
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
				// send the control message here
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
				if(current_mode >= 5) current_mode = 1; //wrap around if we have gone too high
				delay(mode_change_debounce);
			}

			if(( counter_start + hold_length) < counter_stop) // button held
			{
				// send control message here
			}
		}
	}
}

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
				MIDI.sendControlChange(94,1,1); // Reverse toggle
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
				MIDI.sendControlChange(95,1,1); // Half / Full speed toggle
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
				if(current_mode >= 5) current_mode = 1; //wrap around if we have gone too high
				delay(mode_change_debounce);
			}

			if(( counter_start + hold_length) < counter_stop) // button held
			{
				// send control message here
			}
		}
	}
}

void mode4()

{
	analogWrite(rgb_red, 255);
	analogWrite(rgb_green, 255);
	analogWrite(rgb_blue, 255);

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
				// send the control message here
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
				// send the control message here
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
				if(current_mode >= 5) current_mode = 1; //wrap around if we have gone too high
				delay(mode_change_debounce);
			}

			if(( counter_start + hold_length) < counter_stop) // button held
			{
				// send control message here
			}
		}
	}
}
