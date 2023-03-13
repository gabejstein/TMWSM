#include "objectfactory.h"

#include "sentry.h"
#include "door.h"
#include "pickup.h"

void CreateObject(char* name, float x, float y)
{
	if (strcmp(name, "player") == 0)
	{
		CreatePlayer(x, y);
	}
	else if (strcmp(name, "greendoor") == 0)
	{
		CreateGreenDoor(x,  y);
	}
	else if (strcmp(name, "reddoor") == 0)
	{
		CreateRedDoor(x,  y);
	}
	else if (strcmp(name, "bluedoor") == 0)
	{
		CreateBlueDoor(x, y);
	}
	else if (strcmp(name, "redkey") == 0)
	{
		CreateRedKey(x, y);
	}
	else if (strcmp(name, "bluekey") == 0)
	{
		CreateBlueKey(x, y);
	}
	else if (strcmp(name, "greenkey") == 0)
	{
		CreateGreenKey(x, y);
	}
	else if (strcmp(name, "money") == 0)
	{
		CreateMoney(x, y);
	}
	else if (strcmp(name, "ammopickup") == 0)
	{
		CreateAmmoPickup(x, y);
	}
	else if (strcmp(name, "sentry") == 0)
	{
		printf("Sentry initialized at %f %f\n", x, y);
		CreateSentry(x, y);
	}
	else if (strcmp(name, "shooter") == 0)
	{
		printf("Shooter initialized at %f %f\n", x, y);
		CreateShooter(x, y);
	}
}