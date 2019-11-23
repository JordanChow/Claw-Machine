
task main()
{
	SensorType[S1] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S1] = modeEV3Color_Color;
	wait1Msec(50);

	motor[motorD] = 20;
	wait1Msec(4500);
	motor[motorD] = 0;
	while (!getButtonPress(buttonAny))
	{
		if (SensorValue[S1] != (int)colorWhite)
		{
			displayString(8, "not red");
		}
		else
		{
			eraseDisplay();
		}
	}
}
