#include "leveling.h"

//zmienne z aktualnymi danymi akcelerometru wyrazonymi w stopniach
uint16_t roll = 0;	//obrot wzgledem osi X
float pitch = 0;	//obrot wzgledem osi Y

//tolerancja bledu pomiaru akcelerometru
int tolerancja_roll = 1;
float tolerancja_pitch = 0.5f;

void leveling( int16_t akcelero_x, int16_t akcelero_y, int16_t akcelero_z )
{
	//dane z akcelerometru wyrazone w stopniach
	roll = atan2( akcelero_x, akcelero_z ) * 180 / M_PI;
	pitch = atan2( akcelero_y, akcelero_z ) * 180 / M_PI;
	
	//sterowanie silnikiem krokowym na podstawie danych z akcelerometru
	if( ( roll < -tolerancja_roll ) || ( roll > tolerancja_roll ) )
	{
		//wlaczenie ukadu sterujacego silnikiem krokowym
		StepperXEnable( MOTOR1 );
		
		if( roll > 0 )
		{		
			//wykonanie odpowiedniej ilosci krokow
			StepperXExecute( MOTOR1, roll, 0 );
		}
		else
		{
			//wykonanie odpowiedniej ilosci krokow
			StepperXExecute( MOTOR1, -roll, 1 );
		}
		
		if( pitch < 0 )
		{
			//wykonanie obrotu o okolo 180 stopni, aby kat pitch byl dodatni
			StepperXExecute( MOTOR1, 100, 0 );
		}
		else
		{
		}
		
		//wylaczenie ukladu sterujacego silnikiem krokowym
		StepperXDisable( MOTOR1 );
	}
	else
	{
	}
	
	//sterowanie serwomechanizmami (przy zalozeniu, ze sterujemy dwoma przy poziomowaniu) na podstawie danych z akcelerometru
	if( pitch > tolerancja_pitch )
	{
		//zadanie kata dla serwomechanizmow
		ServoSetAngle( SERVO1, pitch );
		ServoSetAngle( SERVO2, pitch );
	}
	else
	{
	}
}
