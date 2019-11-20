
const int COLOURSIZE = 4;
const float xMax = 15 * 360 / (4 * PI);
const float yMax = 6 * 360 / (4 * PI);
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
    displayString(2,"Black | White | Red | Blue");

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
    displayString(2,"Black | White | Red | Blue");

    return colourChosenNum;
}


int startup(int *colours,bool *coloursPicked)
{
    int allColours = 0;
    int coloursIndex = 0;
    int colourChosenNum = colours[coloursIndex];

    while(!getButtonPress(buttonEnter))
    {
        displayString(2,"Black | White | Red | Blue");
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
            else if (colourChosenNum == 6)
                displayString(4,"White");
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
            else if (colourChosenNum == 6)
            {
                displayString(4,"White");
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
    for (int index = 0; index < 4; index++)
    {
        if (coloursPicked[index] == 1)
            allColours += 1;
    }
    if (allColours == 4)
        return 404; //terminates program

    else
        return colourChosenNum; //continues program
}

void objectPickup(int &scanx, int &scany, int powery, int &powerx, int color)
{
	for (int counter = 0; counter < 3; counter++)
	{
		if (SensorValue[S1] != color) // color is the variable
		{
			nMotorEncoder[motorB] = 0;
			while (nMotorEncoder[motorB] < yMax)
			{
				motor[motorB] = powery;
			}
			motor[motorB] = 0;
			nMotorEncoder[motorA] = 0;
			while (abs(nMotorEncoder[motorA]) < xMax && SensorValue[S1] != color)
			{
				motor[motorA] = powerx;
			}
			motor[motorA] = 0;
			powerx *= -1;
			scanx = (4 * PI * nMotorEncoder[motorA]) / 360;
			scany += 6;
		}
	}

	//wait1Msec(1000);
	//motor[motorA] = 0;

	motor[motorC] = 15;
	wait1Msec(1500);
	motor[motorC] = 0;

	time1[T1] = 0;
	while(time1[T1] < 3000) // to be changed after testing
	{
		motor[motorD] = -20;
  }
  motor[motorD] = 0;

  motor[motorC] = -15;
	wait1Msec(1500);
	motor[motorC] = 0;
}

void objectTransport(int scanx, int scany)
{
	float xTogo = (xMax - scanx) * 360 / (4 * PI);

	nMotorEncoder[motorB] = 0;
	float yTogo = scany * 360 / (4 * PI);

	while(nMotorEncoder[motorB] < yTogo)
	{
		motor[motorB] = -20;
	}

	motor[motorB] = 0;

	nMotorEncoder[motorA] = 0;
	while(nMotorEncoder[motorA] < xTogo)
	{
		motor[motorA] = 20;
	}
	motor[motorA] = 0;

	motor[motorD] = 20;
	wait1Msec(3000);
	motor[motorD] = 0;
}

bool playAgain()
{
	displayString(8, "Would you like to play again?");
	displayString(9, "Press enter for yes and");
	displayString(10, "any other button for no");

	return getButtonPress(buttonEnter);
}

bool gameMode()// returns true if the user wants to race the robot
{
	displayString(8, "Would you like to play again?");
	displayString(9, "Press enter for yes and");
	displayString(10, "any other button for no");

	return getButtonPress(buttonEnter);
}

void controls()
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

	int scanx = 0, scany = 0, powery = 20, powerx = 20;
  int colourChosenNum = 0;
	int colours[COLOURSIZE] = {1,6,5,2};
	bool plays = true;
  bool coloursPicked[COLOURSIZE] = {false,false,false,false};

  while(SensorValue[S3] > 100)
  {}

	bool race = gameMode();// race false signifies that the "race" game mode has not been selected

  if (race == false)
  {
  	displayString(1,"Please select a colour: ");
  	displayString(2,"Black | White | Red | Blue");
  	displayString(3,"Selected Colour : Black");

		while (colourChosenNum != 404 && plays == 1)
		{
			colourChosenNum = startup(colours, coloursPicked);
			while(colourChosenNum == 400)
		  {
		    wait1Msec(1000);
		    int newNum = startup(colours,coloursPicked);
		    colourChosenNum = newNum;
		  }
			objectPickup(scanx, scany, powery, powerx, colourChosenNum);
			objectTransport(scanx, scany);
			if (SensorValue[S2] != 1)
			{
				motor[motorC] = 15;
				wait1Msec(1500);
				motor[motorC] = 0;
				objectPickup(scanx, scany, powery, powerx, colourChosenNum);
				if(SensorValue[S2] != 1)
				{
					displayString(8, "Object fell out of bounds");
					wait1Msec(5000);
					eraseDisplay();
				}
			}
			plays = playAgain();
		}
	}

	else
	{
		displayString(1,"Please select a colour: ");
 	 	displayString(2,"Black | White | Red | Blue");
  	displayString(3,"Selected Colour : Black");
  	colourChosenNum = startup(colours, coloursPicked);
  	controls();
	}
}
