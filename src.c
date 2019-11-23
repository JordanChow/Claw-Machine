
const int COLOURSIZE = 3;
const float XMAX = 15.5 * 360 / (4 * PI);
const float YMAX = 4.5 * 360 / (4 * PI);
// radius of wheel = 2cm

int colourLeft(int* colours,int & coloursIndex)
{
	coloursIndex--;
	if (coloursIndex < 0)
	{
		coloursIndex += 4;
	}
	int colourChosenNum = 0;

	colourChosenNum = colours[coloursIndex];
	displayString(2,"Black | Red | Blue");

	return colourChosenNum;
}

int colourRight(int *colours,int & coloursIndex)
{
	coloursIndex++;
	if (coloursIndex > 3)
	{
		coloursIndex -= 4;
	}
	int colourChosenNum = colours[coloursIndex];
	displayString(2,"Black | Red | Blue");

	return colourChosenNum;
}


int startup(int *colours,bool *coloursPicked)
{
	int allColours = 0;
	int coloursIndex = 0;
	int colourChosenNum = colours[coloursIndex];

	while(!getButtonPress(buttonEnter))
	{
		displayString(2,"Black | Red | Blue");
		if(getButtonPress(buttonLeft))
		{
			while(getButtonPress(buttonLeft))
			{}
			eraseDisplay();
			colourChosenNum = colourLeft(colours, coloursIndex);
			displayString(3,"Selected Colour : ");
			if (colourChosenNum == 1)
				displayString(4,"Black");
			else if (colourChosenNum == 2)
				displayString(4,"Blue");
			else
				displayString(4,"Red");
		}

		else if(getButtonPress(buttonRight))
		{
			while(getButtonPress(buttonRight))
			{}
			eraseDisplay();
			colourChosenNum = colourRight(colours,coloursIndex);
			displayString(3,"Selected Colour : ");
			if (colourChosenNum == 1)
			{
				displayString(4,"Black");
			}
			else if (colourChosenNum == 2)
			{
				displayString(4,"Blue");
			}
			else
			{
				displayString(4,"Red");
			}
		}
		else if(getButtonPress(buttonUp))
		{
			displayString(9,"Please use the left, right");
			displayString(10,"and center button Only");
		}
		else if(getButtonPress(buttonDown))
		{
			displayString(9,"Please use the left, right");
			displayString(10,"and center button Only");
		}
	}
	//colour already chosen error 400
	if (coloursPicked[coloursIndex] == 1)
	{
		eraseDisplay();
		displayString(9,"Colour already chosen");
		return 400;
	}

	//all objects are gone error 404
	coloursPicked[coloursIndex] = true;
	for (int index = 0; index < 3; index++)
	{
		if (coloursPicked[index] == 1)
			allColours += 1;
	}
	if (allColours == 3)
		return 404; //terminates program

	else
		return colourChosenNum; //continues program
}

void objectPickup(int &scanx, int &scany, int powery, int powerx, int color, bool &isThere, int &row)
{
	isThere = false;

	//for (int counter = 0; counter < 2; counter++)
	//{
	if (SensorValue[S1] != color) // color is the variable
	{
		nMotorEncoder[motorB] = 0;
		while (abs(nMotorEncoder[motorB]) < YMAX)
		{
			motor[motorB] = powery;
		}
		motor[motorB] = 0;
		nMotorEncoder[motorA] = 0;
		while (abs(nMotorEncoder[motorA]) < XMAX && SensorValue[S1] != color)
		{
			motor[motorA] = powerx;
			if(SensorValue[S1] == color)
			{
				wait1Msec(500);
				isThere = true;
			}
		}

		motor[motorA] = 0;
		//powerx *= -1;
		scanx = (4 * PI * nMotorEncoder[motorA]) / 360;
		scany += 4.5;
	}
	//row++;
	//}

	if (isThere == true)
	{
		motor[motorA] = (-1)*powerx;
		wait1Msec(1000);
		motor[motorA] = 0;
		motor[motorC] = -15;
		wait1Msec(1000);
		motor[motorC] = 0;

		time1[T1] = 0;
		while(time1[T1] < 5000) // to be changed after testing
		{
			motor[motorD] = -20;
		}
		motor[motorD] = 0;

		motor[motorC] = 15;
		wait1Msec(4000);
		motor[motorC] = 0;
	}

	else
	{
		eraseDisplay();
		displayString(8, "No Object detected, is it");
		displayString(9, "out of bounds?");
	}
}

void objectTransport(int scanx, int scany, float &yTogo, float &xTogo, bool isThere, int row, int powerx)
{
	nMotorEncoder[motorB] = nMotorEncoder[motorA] = 0;
	xTogo = (17 - scanx) * 360 / (4 * PI);
	//if (row == 2)
	//{
	//	xTogo = (17 - scanx) * 360 / (4 * PI);
	//	powerx *= -1;
	//	//xTogo = scanx* 360 / (4 * PI);
	//	////powerx *= -1;
	//}

	//else
	//{
	//	xTogo = (17 - scanx) * 360 / (4 * PI);
	//	powerx *= -1;
	//}

	yTogo = (scany + 1) * 360 / (4 * PI);

	if (isThere == true)
	{
		while(abs(nMotorEncoder[motorB]) < yTogo)
		{
			motor[motorB] = 10;
		}

		motor[motorB] = 0;

		while(abs(nMotorEncoder[motorA]) < xTogo)
		{
			motor[motorA] = powerx;
		}
		motor[motorA] = 0;

		motor[motorD] = 20;
		wait1Msec(6000);
		motor[motorD] = 0;
	}
}

void reset(float yTogo, float xTogo)
{
	//nMotorEncoder[motorA] = nMotorEncoder[motorB] = 0;
	//while(abs(nMotorEncoder[motorB]) < yTogo)
	//{
	//	motor[motorB] = 20;
	//}

	//motor[motorB] = 0;

	nMotorEncoder[motorA] = 0;
	while(abs(nMotorEncoder[motorA]) < XMAX)
	{
		motor[motorA] = -20;
	}
	motor[motorA] = 0;

	motor[motorD] = -15;
	wait1Msec(6000);
	motor[motorD] = 0;
}

bool playAgain()
{
	bool status = false;

	while (SensorValue[S2] != true)
	{
		displayString(8, "Would you like to play again?");
		displayString(9, "Press touch for yes and");
		displayString(10, "any other button for no");
		if (getButtonPress(buttonAny))
		{
			return status;
		}
		status = false;
	}
	status = true;
	while(!getButtonPress(buttonAny))
	{
	}
	return status;
}

bool gameMode()// returns true if the user wants to race the robot
{
	displayString(8, "Would you like to race?");
	displayString(9, "Press enter for yes and");
	displayString(10, "any other button for no");

	if (getButtonPress(buttonEnter))
	{
		return true;
	}
	else
		return false;
}

bool isSuccesful()
{
	eraseDisplay();
	bool status = false;

	while (SensorValue[S2] != true)
	{
		displayString(8, "Would you like to play again?");
		displayString(9, "Press touch for yes and");
		displayString(10, "any other button for no");
		if (getButtonPress(buttonAny))
		{
			return status;
		}
		status = false;
	}
	status = true;
	while(!getButtonPress(buttonAny))
	{
	}
	return status;
}


int controls()
{
	eraseDisplay();
	displayString(6, "The controls are as follows");
	displayString(7, "Up and Down buttons control y");
	displayString(8, "axis and right and left for x");
	displayString(9, "Once centred above the object,");
	displayString(10, "press the enter button once, then");
	displayString(11, "the up and down buttons will raise");
	displayString(12, "and lower the claw, the right button");
	displayString(13, "will close the claw, and the left open");
	displayString(14, "GLHF");
	wait1Msec(10000);
	time1[T1] = 0;

	while (!getButtonPress(buttonEnter))
	{
		if (getButtonPress(buttonRight))
		{
			while (getButtonPress(buttonRight))
			{
				motor[motorA] = 20;
			}
			motor[motorA] = 0;
		}

		if (getButtonPress(buttonLeft))
		{
			while (getButtonPress(buttonLeft))
			{
				motor[motorA] = -20;
			}
			motor[motorA] = 0;
		}

		if (getButtonPress(buttonUp))
		{
			while (getButtonPress(buttonUp))
			{
				motor[motorB] = 20;
			}
			motor[motorB] = 0;
		}

		if (getButtonPress(buttonDown))
		{
			while (getButtonPress(buttonDown))
			{
				motor[motorB] = -20;
			}
			motor[motorB] = 0;
		}
	}

	while (!getButtonPress(buttonEnter))
	{
		if (getButtonPress(buttonRight))
		{
			while (getButtonPress(buttonRight))
			{
				motor[motorC] = 20;
			}
			motor[motorC] = 0;
		}

		if (getButtonPress(buttonLeft))
		{
			while (getButtonPress(buttonLeft))
			{
				motor[motorC] = -20;
			}
			motor[motorC] = 0;
		}

		if (getButtonPress(buttonUp))
		{
			while (getButtonPress(buttonUp))
			{
				motor[motorD] = 20;
			}
			motor[motorD] = 0;
		}

		if (getButtonPress(buttonDown))
		{
			while (getButtonPress(buttonDown))
			{
				motor[motorD] = -20;
			}
			motor[motorD] = 0;
		}
	}
	return time1[T1];
}

task main()
{
	SensorType[S1] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S1] = modeEV3Color_Color;
	wait1Msec(50);
	SensorType[S2] = sensorEV3_Touch;
	SensorType[S3] = sensorEV3_Ultrasonic;
	motor[motorA] = motor[motorB] = motor[motorC] = motor[motorD] = 0; //assuming A and B are x-y respectively, C controls z and D controls claw
	nMotorEncoder[motorA] = nMotorEncoder[motorB] = 0;

	int scanx = 0, scany = 0, powery = -10, powerx = 10, row = 1;
	int colourChosenNum = 0;
	float yTogo = 0, xTogo = 0;
	int colours[COLOURSIZE] = {1,5,2};
	bool plays = true, race = false, isThere = false;
	bool coloursPicked[COLOURSIZE] = {false,false,false};

	while(SensorValue[S3] > 100)
	{}

	while (colourChosenNum != 404 && plays == true)
	{
		while(!getButtonPress(buttonAny))
		{
			race = gameMode();// race false signifies that the "race" game mode has not been selected
		}

		if (race == false)
		{
			displayString(1,"Please select a colour: ");
			displayString(2,"Black | Red | Blue");
			displayString(3,"Selected Colour : Black");

			colourChosenNum = startup(colours, coloursPicked);
			while(colourChosenNum == 400)
			{
				wait1Msec(1000);
				int newNum = startup(colours,coloursPicked);
				colourChosenNum = newNum;
			}
			objectPickup(scanx, scany, powery, powerx, colourChosenNum, isThere, row);
			objectTransport(scanx, scany, yTogo, xTogo, isThere, row, powerx);
			eraseDisplay();
			if (isThere == true)
			{
				if(isSuccesful() == false)
				{
					reset(yTogo, xTogo);
				}
			}
			plays = playAgain();
			if (plays == true)
			{
				reset(yTogo, xTogo);
			}
		}

		else
		{
			displayString(1,"Please select a colour: ");
			displayString(2,"Black | Red | Blue");
			displayString(3,"Selected Colour : Black");
			colourChosenNum = startup(colours, coloursPicked);
			float userTime = controls()/1000.0;
			eraseDisplay();
			displayString(8, "Your time is %f", userTime);
			displayString(9, "Please return the object");
			wait1Msec(20000);
			eraseDisplay();
			colourChosenNum = startup(colours, coloursPicked);
			objectPickup(scanx, scany, powery, powerx, colourChosenNum, isThere, row);
			objectTransport(scanx, scany, yTogo, xTogo, isThere, row, powerx);

		}
	}
}
