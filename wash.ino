// Программа для бойлера в ванной
// Пока она вручную открывает и закрывает вентили и
// включает и выключает бойлер
const int Scold = 13;	          //  Switch cold pin
const int Shot = 12;	          // Switch hot pin
const int Sboil = 11;	          // Switch boiler pin
const int Sauto = 10;	          // Switch auto/manual pin
const int Lcold = 9;	          // Led cold pin
const int Lhot = 8;	          // Led hot pin
const int Lboil = 7;	          // Led boiler pin
const int Lauto = 6;	          // Led auto pin
const int RelCold = 5;	          // Relay cold 1 pin
const int RelHot = 4;	          // Relay hot 2 pin
const int RelBoil = 3;	          // Relay boiler 3 pin
const int RelOn = 2;	          // Relay power on 4 pin
const int PressMin = 100;	  // Pressure min
unsigned long DelaySleep = 600000; // 10 min test cicle
int PressCold;			  // Pressure cold variable
int PressHot;			  // Pressure hot variable
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
	pinMode(Scold, INPUT_PULLUP);    // Switch cold input pullup pin
	pinMode(Shot, INPUT_PULLUP);	  // Switch hot input pullup pin
	pinMode(Sboil, INPUT_PULLUP);	  // Switcch boiler input pullup pin
	pinMode(Sauto, INPUT_PULLUP);	  // Switch auto input pullup pin
	pinMode(Lcold, OUTPUT);	  // Cold led pin
	pinMode(Lhot, OUTPUT);	          // Hot led pin
	pinMode(Lboil, OUTPUT);	  // Boil led pin
        pinMode(Lauto, OUTPUT);	          // Auto led pin
	pinMode(RelCold, OUTPUT);	  // Cold relay pin
	pinMode(RelHot, OUTPUT);	  // Hot relay pin
	pinMode(RelBoil, OUTPUT);	  // Boil relay pin
	pinMode(RelOn, OUTPUT);		  // Power relay pin
	digitalWrite(Lcold, LOW);	  // Cold led pin off
	digitalWrite(Lhot, LOW);	  // Hot led pin off
	digitalWrite(Lboil, LOW);	  // Boiler led pin off
	digitalWrite(Lauto, LOW);	  // Auto led pin off
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
		digitalWrite(Lcold, HIGH);	// light cold led
		Cold = 1;			// cold water yes
	}
	PressHot = analogRead(SenHot);		// read hot pressure
	if (PressHot > PressMin)		// if hot water
	{
		digitalWrite(RelHot, HIGH);	// open hot water
		digitalWrite(Lhot, HIGH);	// light hot led
		Hot = 1;			// hot water yes
	}
	else					// no hot water
	{
		if (Cold == 1)			// cold water yes
		{
			digitalWrite(RelBoil, HIGH);	// switch on boiler
			digitalWrite(Lboil, HIGH);	// light led boiler
			Boiler = 1;			// boiler yes
		}
	}
	delay(DelaySwitch);                     // wait to switch water
	digitalWrite(Lauto, HIGH);		// light led auto
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
			digitalWrite(Lhot,HIGH);	// light hot led
			digitalWrite(RelBoil, HIGH);	// boiler relay on
			digitalWrite(Lboil, HIGH);	// boiler led on
			Boiler = 1;			// boiler on
			digitalWrite(RelOn,HIGH);	// power on relay
			delay(DelaySwitch);		// wait for switch water
			digitalWrite(RelOn,LOW);	// power off relay
		}
		if (Hot == 1 && PressHot < PressMin)	//Hot change from yes to no
		{
  			Hot = 0;			// Hot change from yes to no
			digitalWrite(RelHot,LOW);	// Hot close
			digitalWrite(Lhot,LOW);	// down hot led
			digitalWrite(RelBoil, LOW);	// boiler relay off
			digitalWrite(Lboil, LOW);	// boiler led off
			Boiler = 0;			// boiler off
			digitalWrite(RelOn,HIGH);	// power on relay
			delay(DelaySwitch);		// wait for switch water
			digitalWrite(RelOn,LOW);	// power off relay
		}
		delay(DelaySleep);			// sleep between measure (10 min)
	}
	if (AutoStatus == 0) // manual control
	{
		if (Cold == 0 && Scold == HIGH)        // switch on cold water
		{
			digitalWrite(RelCold, HIGH);	// cold relay on
			digitalWrite(Lcold, HIGH);	//cold led on
			Cold = 1;			// cold on
			digitalWrite(RelOn, HIGH);	// power on relay
			delay(DelaySwitch);		// wait for swich water
			digitalWrite(RelOn, LOW);	// power off relay
		}
		if (Cold == 1 && Scold == LOW)	        // swich off cold water
		{
			digitalWrite(RelCold, LOW);	// cold relay off
			digitalWrite(Lcold, LOW);	// cold led off
			Cold = 0;			// cold off
			digitalWrite(RelOn, HIGH);	// power on relay
			delay(DelaySwitch);		// wait for switch water
			digitalWrite(RelOn, LOW);	// power off relay
		}
		if (Hot == 0 && Shot == HIGH)		// switch on hot water
		{
			digitalWrite(RelHot, HIGH);	// hot relay on
			digitalWrite(Lhot, HIGH);	// hot led on
			Hot = 1;			// hot on
			digitalWrite(RelOn, HIGH);	// power on relay
			delay(DelaySwitch);		// wait for switch water
			digitalWrite(RelOn, LOW);	// power off relay
		}
		if (Boiler == 0 && Sboil == HIGH);	// switch on boiler
		{
			digitalWrite(RelBoil, HIGH);	// switch on boiler
			digitalWrite(Lboil, HIGH);	// switch on led boiler
			Boiler = 1;			// Boiler on
		}
		if (Boiler == 1 && Sboil == LOW);	// switch off boiler
		{
			digitalWrite(RelBoil, LOW);	// switch off boiler
			digitalWrite(Lboil, LOW);	// switch off led boiler
			Boiler = 0;
		}
	}
	if (AutoStatus == 0 && Sauto == HIGH) 		// switch auto on
	{
		digitalWrite(Lauto, HIGH);		// switch auto led on
		AutoStatus = 1;				// switch auto on
	}
	if (AutoStatus == 1 && Sauto == LOW)			// switch auto off
	{
		digitalWrite(Lauto, LOW);		// switch auto led off
		AutoStatus = 0;
	}
}
