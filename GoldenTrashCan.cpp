// Do not remove the include below
#include "GoldenTrashCan.h"

void initialize();
void timerIsr();
void updateMotorSpeed();
uint8_t readMotorSpeed();

static LedWrapper ledController;


// Indicates we have a command waiting for processing
static volatile uint8_t commandAvailable = false;

uint16_t volatile gHbCounter;

Bounce sw1 = Bounce(SW1, 20);
Bounce sw2 = Bounce(SW2, 20);
uint8_t volatile gSw1 = false;
uint8_t volatile gSw2 = false;

uint8_t gRunMotor = false;
LedMode gLedMode = None;


/**
 * Initialize function
 *
 */
void initialize()
{
	gHbCounter = 0;
	gLedMode = None;

	pinMode(MY_LED_PIN, OUTPUT); // LEDs

	pinMode(SW1, INPUT_PULLUP); // SW1
	pinMode(SW2, INPUT_PULLUP); // SW2

	pinMode(MOTOR_SPEED, INPUT_PULLUP); // Potentiometer
	pinMode(MOTOR_CTRL, OUTPUT); // Pot pull down
	digitalWrite(MOTOR_CTRL, LOW); // Enable Pot

	motor.initialize(MOTORA, MOTORB);
	motor.stop();

	if ( ledController.initialize(NUM_LEDS, DEFAULT_INTENSITY) )
	{
		Serial.print(F("\nLED Controller initialized..."));
//		ledController.fill(CRGB::White, true);
//		delay(250);
//		ledController.fill(CRGB::Black, true);
//		delay(250);
//		ledController.fill(CRGB::Red, true);
//		delay(250);
//		ledController.fill(CRGB::Black, true);
//		Serial.println(F("LED Module Configured."));
	}
	else
	{
		Serial.println(F("\nERROR - failed to configure LED module"));
	}

	FlexiTimer2::set(10, timerIsr); // 10ms period
	FlexiTimer2::start();
}

void timerIsr()
{
	gHbCounter++;
	sw1.update();
	sw2.update();

	// Adjust speed of motor based on analog input
	if(motor.isRunning() || gRunMotor == true)
	{
		uint8_t gNewSpeed = readMotorSpeed();
		if( motor.getSpeed() != gNewSpeed )
		{
			updateMotorSpeed();
		}
	}

	if( (gSw1 == false) && (sw1.fell() == true) )
	{
		gSw1 = true;
		commandAvailable = true;
	}
	if( (gSw2 == false) && (sw2.fell() == true) )
	{
		gSw2 = true;
		commandAvailable = true;
	}

	switch(gHbCounter)
	{
		case 10:
			digitalWrite(13, HIGH);
			break;
		case 20:
			digitalWrite(13, LOW);
			break;
		case 40:
			digitalWrite(13, HIGH);
			break;
		case 50:
			digitalWrite(13, LOW);
			break;
		case 150:
			gHbCounter = 0;
			break;
	}
}

//The setup function is called once at startup of the sketch
void setup()
{
	// set led pin
	pinMode(13, OUTPUT);
	digitalWrite(13, LOW);

	// set serial
	Serial.begin(9600);

	Serial.println("Hello World!");

	initialize();

	Serial.println("Initialization Complete!");

}

// The loop function is called in an endless loop
void loop()
{
	if(gLedMode != None)
	{
		switch(gLedMode)
		{
		case Chase:
			ledController.rotatePattern(0, 0, 0x77, Forward, WHITE, BLACK, 400, 400);
			break;
		case Fade:
			ledController.confetti(0, CRGB::OrangeRed, 25, 50, 5);
			break;
		case Random:
			ledController.randomFlash(0, 0, 75, 50, PURPLE, BLACK, 6);
			break;
		default:
			break;
		}
	}

	if(gSw1)
	{
		setCommandAvailable(false);
		Serial.println("SW1 Pressed");
		gSw1 = false;

		if( gRunMotor )
		{
			gRunMotor = false;
			motor.stop();
			Serial.println("Motor Stopped");
		}
		else
		{
			gRunMotor = true;
			motor.forward( 255 ); // quick pulse of full speed to get going (isr will set speed in 10ms)
			Serial.println("Motor Started");
		}
	}

	if(gSw2)
	{
		setCommandAvailable(false);
		Serial.println("SW2 Pressed");
		gSw2 = false;

		switch(gLedMode)
		{
		case None:
			gLedMode = Chase;
			break;
		case Chase:
			gLedMode = Fade;
			break;
		case Fade:
			gLedMode = Random;
			break;
		case Random:
			gLedMode = None;
			break;
		}
	}


}
uint8_t readMotorSpeed()
{
	return ( analogRead(MOTOR_SPEED)/4);
}

void updateMotorSpeed()
{
	uint8_t newSpeed = readMotorSpeed();

	if( newSpeed > 50)
	{
		motor.forward( newSpeed );
		Serial.print("Updating Speed");
		Serial.println(newSpeed, DEC);

	}
	else
	{
		motor.stop();
		Serial.println("Motor Stop");
	}

}


/**
 * Returns flag; true=new command is available
 */
uint8_t isCommandAvailable()
{
	return commandAvailable;
}

/**
 * Sets command available flag
 *
 */
void setCommandAvailable(boolean flag)
{
	commandAvailable = flag;
}


/**
 * Delay function with command check
 */
uint8_t commandDelay(uint32_t time)
{
	boolean cmd = isCommandAvailable();
	if( time == 0 ) return cmd;

	if( !cmd ) // if no command, pause
	{
		for (uint32_t i = 0; i < time; i++)
		{
			delay(1); // delay
			cmd = isCommandAvailable();
			if (cmd)
			{
				break;
			}
		}
	}
	return cmd;
}
