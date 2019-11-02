// Программа для бойлера в ванной
// Пока она вручную открывает и закрывает вентили и
// включает и выключает бойлер
const int S-cold = 13;	          //  Switch cold pin
const int S-hot = 12;	          // Switch hot pin
const int S-boil = 11;	          // Switch boiler pin
const int S-auto = 10;	          // Switch auto/manual pin
const int L-cold = 9;	          // Led cold pin
const int L-hot = 8;	          // Led hot pin
const int L-boil = 7;	          // Led boiler pin
const int L-auto = 6;	          // Led auto pin
const int RelCold = 5;	          // Relay cold 1 pin
const int RelHot = 4;	          // Relay hot 2 pin
const int RelBoil = 3;	          // Relay boiler 3 pin
const int RelOn = 2;	          // Relay power on 4 pin
const int PressMin = 100;	  // Pressure min
unsigned long DelaySleep = 600000; // 10 min test cicle
int PressCold;			  // Pressure cold variable
int pressHot;			  // Pressure hot variable
int SenCold = 0;		  //  Sensor cold A0 pin
int SenHot = 1;			  // Sensor hot A1 pin
int DelaySwitch = 10000;	  //  Время для переключения вентиля
int Cold = 0;			  // Status cold relay
int Hot = 0;			  // Status hot relay
int Boiler = 0;			  // Status boiler relay
int AutoStatus = 1;			  // Status auto

// ?2 - ????? ???????? ???????? ????
// ?3 - ????? ???????? ??????? ????
void setup ()
{
	pinMode(S-cold, INPUT_PULLUP);    // Switch cold input pullup pin
	pinMode(S-hot, INPUT_PULLUP);	  // Switch hot input pullup pin
	pinMode(S-boil, INPUT_PULLUP);	  // Switcch boiler input pullup pin
	pinMode(S-auto, INPUT_PULLUP);	  // Switch auto input pullup pin
	pinMode(L-cold, OUTPUT);	  // Cold led pin
	pinMode(L-hot, OUTPUT);	          // Hot led pin
	pinMode(L-boil, OUTPUT);	  // Boil led pin
  pinMode(L-auto, OUTPUT);	          // Auto led pin
	pinMode(RelCold, OUTPUT);	  // Cold relay pin
	pinMode(RelHot, OUTPUT);	  // Hot relay pin
	pinMode(RelBoil, OUTPUT);	  // Boil relay pin
	pinMode(RelOn, OUTPUT);		  // Power relay pin
	digitalWrite(L-cold, LOW);	  // Cold led pin off
	digitalWrite(L-hot, LOW);	  // Hot led pin off
	digitalWrite(L-boil, LOW);	  // Boiler led pin off
	digitalWrite(L-auto, LOW);	  // Auto led pin off
	digitalWrite(RelCold, LOW);	  // Cold relay off
	digitalWrite(RelHot, LOW);	  // Hot relay off
	digitalWrite(RelBoil, LOW);	  // Boil relay off

	// ????? ????????? ????? ?????, ????? ????? ????????? ? ????????? ????? ??????? ????
	// ?????, ???? ??????? ?????????? ????, ?? ???????? ?????
	// ???? ?????? ?????????? ?????????? ???? - ????? ????????? ???????? ????
	digitalWrite(RelOn, HIGH); 	  // relay power on
	delay(DelaySwitch);               // 10 sec for run switch
	PressCold = analogRead(SenCold);  // read cold pressure
	if (PressCold > PressMin)	  // if cold water
	{
		digitalWrite(RelCold, HIGH);	// open cold water
		digitalWrite(L-cold, HIGH);	// light cold led
		Cold = 1;			// cold water yes
	}
	PressHot = analogRead(SenHot);		// read hot pressure
	if (PressHot > PressMin)		// if hot water
	{
		digitalWrite(RelHot, HIGH);	// open hot water
		digitalWrite(L-hot, HIGH);	// light hot led
		Hot = 1;			// hot water yes
	}
	else					// no hot water
	{
		if (Cold == 1)			// cold water yes
		{
			digitalWrite(RelBoil, HIGH);	// switch on boiler
			digitalWrite(L-boil, HIGH);	// light led boiler
			Boiler = 1;			// boiler yes
		}
	}
	delay(DelaySwitch);                     // wait to switch water
	digitalWrite(L-auto, HIGH);		// light led auto
	digitalWrite(RelOn, LOW);		// switch off relay power
}
void loop ()
{
	if (AutoStatus == 1) 				// AutoStatus option on
	{
		PressCold = analogRead(SenCold);// Read cold pressure
		if (Cold == 0)		        // was not before cold water
		{
			if (PressCold > PressMin)	// Cold water yes
			{
				Cold = 1;		// Cold change fron no to yes
			}
		}
		if (Cold == 1 && PressCold < PressMin)	// Cold change from yes to no
		{
			Cold = 0;			// Cold change from yes to no
		}
		PressHot = analogRead(SenHot);		// Читаю холодное давление
		if (Hot == 0 && PressHot > PressMin)	// Hot change from no to yes
		{
			Hot = 1;			// Hot change from no to yes
			digitalWrite(RelHot,HIGH);	// Open Hot water
			digitalWrite(L-hot,HIGH);	// light hot led
			digitalWrite(RelBoil, HIGH);	// boiler relay on
			digitalWrite(L-boil, HIGH);	// boiler led on
			Boiler = 1;			// boiler on
			digitalWrite(RelOn,HIGH);	// power on relay
			delay(DelaySwitch);		// wait for switch water
			digitalWrite(RelOn,LOW);	// power off relay
		}
		if (Hot == 1 && PressHot < PressMin)	//Hot change from yes to no
		{
  			Hot = 0;			// Hot change from yes to no
			digitalWrite(RelHot,LOW);	// Hot close
			digitalWrite(L-hot,LOW);	// down hot led
			digitalWrite(RelBoil, LOW);	// boiler relay off
			digitalWrite(L-boil, LOW);	// boiler led off
			Boiler = 0;			// boiler off
			digitalWrite(RelOn,HIGH);	// power on relay
			delay(DelaySwitch);		// wait for switch water
			digitalWrite(RelOn,LOW);	// power off relay
		}
		delay(DelaySleep);			// sleep between measure (10 min)
	}
	if (AutoStatus == 0) // manual control
	{
		if (Cold == 0 && S-cold == HIGH)        // switch on cold water
		{
			digitalWrite(RelCold, HIGH);	// cold relay on
			digitalWrite(L-cold, HIGH);	//cold led on
			Cold = 1;			// cold on
			digitalWrite(RelOn, HIGH);	// power on relay
			delay(DelaySwitch);		// wait for swich water
			digitalWrite(RelOn, LOW):	// power off relay
		}
		if (Cold == 1 && S-cold == LOW)	        // swich off cold water
		{
			digitalWrite(RelCold, LOW);	// cold relay off
			digitalWrite(L-cold, LOW);	// cold led off
			Cold = 0;			// cold off
			digitalWrite(RelOn, HIGH);	// power on relay
			delay(DelaySwitch);		// wait for switch water
			digitalWrite(RelOn, LOW);	// power off relay
		}
		if (Hot == 0 && S-hot == HIGH)		// switch on hot water
		{
			digitalWrite(RelHot, HIGH);	// hot relay on
			digitalWrite(L-Hot, HIGH);	// hot led on
			Hot = 1;			// hot on
			digitalWrite(RelOn, HIGH);	// power on relay
			delay(DelaySwitch);		// wait for switch water
			digitalWrite(RelOn, LOW);	// power off relay
		}
		if (Boiler == 0 && S-boil == HIGH);	// switch on boiler
		{
			digitalWrite(RelBoil, HIGH);	// switch on boiler
			digitalWrite(L-Boil, HIGH);	// switch on led boiler
			Boiler = 1;			// Boiler on
		}
		if (Boiler == 1 && S-boil == LOW);	// switch off boiler
		{
			digitalWrite(RelBoil, LOW);	// switch off boiler
			digitalWrite(L-Boil, LOW);	// switch off led boiler
			Boiler = 0;
		}
	}
	if (AutoStatus == 0 && S-auto == HIGH) 		// switch auto on
	{
		digitalWrite(L-auto, HIGH);		// switch auto led on
		AutoStatus = 1;				// switch auto on
	}
	if (AutoStatus == 1 && S-auto == LOW)			// switch auto off
	{
		digitalWrite(L-auto, LOW);		// switch auto led off
		AutoStatus = 0;
	}
}
