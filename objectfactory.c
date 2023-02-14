#include "objectfactory.h"

#include "sentry.h"
#include "door.h"

void CreateObject(char* name, float x, float y)
{
	if (strcmp(name, "player") == 0)
	{
		CreatePlayer(x, y);
	}
	else if (strcmp(name, "door") == 0)
	{
		CreateDoor(x, y);
	}
	else if (strcmp(name, "key") == 0)
	{

	}
	else if (strcmp(name, "sentry") == 0)
	{
		printf("Sentry initialized at %f %f\n", x, y);
		CreateSentry(x, y);
	}
}

static void DoorOnHit(void)
{

}