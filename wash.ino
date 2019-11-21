// Программа для бойлера в ванной
// Пока она вручную открывает и закрывает вентили и
// включает и выключает бойлер
const byte Scold = 13;	          //  Switch cold pin
const byte Shot = 12;	          // Switch hot pin
const byte Sboil = 11;	          // Switch boiler pin
const byte Sauto = 10;	          // Switch auto/manual pin
const byte Lcold = 9;	          // Led cold pin
const byte Lhot = 8;	          // Led hot pin
const byte Lboil = 7;	          // Led boiler pin
const byte Lauto = 6;	          // Led auto pin
const byte RelCold = 5;	          // Relay cold 1 pin
const byte RelHot = 4;	          // Relay hot 2 pin
const byte RelBoil = 3;	          // Relay boiler 3 pin
const byte RelOn = 2;	          // Relay power on 4 pin
const byte PressMin = 100;	  // Pressure min
unsigned long DelaySleep = 600; // 10 min test circle
int PressCold;			  // Pressure cold variable
int PressHot;			  // Pressure hot variable
int SenCold = 0;		  //  Sensor cold A0 pin
int SenHot = 1;			  // Sensor hot A1 pin
int DelaySwitch = 1000;	  //  Время для переключения вентиля
boolean Cold;				// Cold water yes
boolean ColdStatus = HIGH;			  // Status cold relay
boolean ColdOld;			// Last status COLD
boolean ColdPress;			// previously press COLD button
boolean PredCold;			  // Предыдущее состояние холодного реле
boolean Hot;				// Hot water yes
boolean HotStatus = HIGH;			  // Status hot relay
boolean HotPress = HIGH;		// previously press HOT button
boolean HotOld;					// Last status HOT
boolean BoilerStatus = LOW;			  // Status boiler relay
boolean BoilerOld;			// Last status BOILER
boolean BoilerPress;		// previously press BOILER button
boolean AutoStatus = HIGH;			  // current Status auto
boolean Status;				// Readed Status
boolean OldStatus = HIGH;			// Last status Auto
boolean OldPress = HIGH;          // previously press Auto button



void setup () {

	pinMode(Scold, INPUT_PULLUP);   // Switch cold input pull up pin
	pinMode(Shot, INPUT_PULLUP);	// Switch hot input pull up pin
	pinMode(Sboil, INPUT_PULLUP);	// Switch boiler input pull up pin
	pinMode(Sauto, INPUT_PULLUP);	// Switch auto input pull up pin
	pinMode(Lcold, OUTPUT);			// Cold led pin
	pinMode(Lhot, OUTPUT);	        // Hot led pin
	pinMode(Lboil, OUTPUT);			// Boil led pin
    pinMode(Lauto, OUTPUT);			// Auto led pin
	pinMode(RelCold, OUTPUT);		// Cold relay pin
	pinMode(RelHot, OUTPUT);		// Hot relay pin
	pinMode(RelBoil, OUTPUT);		// Boil relay pin
	pinMode(RelOn, OUTPUT);			// Power relay pin


	
//	digitalWrite(RelOn, HIGH); 	  // relay power on
//	delay(DelaySwitch);               // 10 sec for run switch
	if ( analogRead(A0) > PressMin) {
		PressCold = HIGH;						// Cold water yes
	}
	else PressCold = LOW;					// Cold water no
	if (analogRead(A1) > PressMin)
	{
		PressHot = HIGH;					// Hot Water yes
	}
	else PressHot = LOW;					// Hot water no	PressCold = analogRead(A0);  // read cold pressure
	
	delay(DelaySwitch);                     // wait to switch water
//        Serial.begin(9600);  // для отладки
        AutoStatus = HIGH;   // auto on
		BoilerStatus = LOW;	// boiler off
		BoilerOld = LOW;
		ColdStatus = HIGH;	// cold water on
		ColdOld = HIGH;
		HotStatus = HIGH;	// how water on    
		HotOld = HIGH;
		OldPress = HIGH;
		OldStatus = HIGH;
	digitalWrite(Lauto, AutoStatus);		// light led auto
//	digitalWrite(RelOn, LOW);		// switch off relay power
		digitalWrite(Lcold, ColdStatus);		// Cold led off 9
		digitalWrite(RelCold, ColdStatus);		// Cold relay off 5
		digitalWrite(Lboil, BoilerStatus);		// Boiler led off 7
		digitalWrite(RelBoil, BoilerStatus);	// Boil relay off 3
		digitalWrite(Lhot, HotStatus);		// Boiler led off 8
		digitalWrite(RelHot, HotStatus);		// Boil relay off 4
}



void loop () {
	delay(300);
	digitalWrite(Lcold, ColdStatus);		// Cold led off 9
	digitalWrite(RelCold, ColdStatus);		// Cold relay off 5
	digitalWrite(Lboil, BoilerStatus);		// Boiler led off 7
	digitalWrite(RelBoil, BoilerStatus);	// Boil relay off 3
	digitalWrite(Lhot, HotStatus);		// Boiler led off 8
	digitalWrite(RelHot, HotStatus);		// Boil relay off 4
	Status = digitalRead(Sauto);
	if (Status == LOW && OldPress == HIGH)
	{
		OldStatus = !OldStatus;
		OldPress = LOW; // press button	
	}
	if (Status == HIGH)
	{
		OldPress = HIGH;
	}
	AutoStatus = OldStatus;	
	digitalWrite(Lauto, AutoStatus);		// Auto led on  6

	if ( analogRead(A0) > PressMin) {
		PressCold = HIGH;						// Cold water yes
	}
	else PressCold = LOW;					// Cold water no
	if (analogRead(A1) > PressMin)
	{
		PressHot = HIGH;					// Hot Water yes
	}
	else PressHot = LOW;					// Hot water no
//	PressCold = analogRead(A0);// Read cold pressure
//	PressHot = analogRead(A1); // Read hot pressure
  	if (AutoStatus == HIGH) 				// AutoStatus option on
	{
		if (PressCold) {
			if (PressHot) {
				if (BoilerStatus == HIGH) {
					digitalWrite(Lboil, LOW);		// Boiler led off 7
					digitalWrite(RelBoil, LOW);		// Boil relay off 3
					BoilerStatus = LOW;
				}
				HotStatus = HIGH; //hot water on
			}
			else { // no hot water
				BoilerStatus = HIGH; // boiler on
				HotStatus = LOW; // hot water off
			} 
			ColdStatus = HIGH; // cold water must be on

		}
		else { // no cold water
				digitalWrite(Lcold, HIGH); // 9
				delay(100);
				digitalWrite(Lcold, LOW);
				delay(100);
				if (BoilerStatus == HIGH)
				{
					BoilerStatus = LOW;		// boiler off
//					digitalWrite(Lboil, LOW);		// Boiler led off 7
//					digitalWrite(RelBoil, LOW);		// Boil relay off 3
				}
		}
	}
	else {	// Autostatus == LOW
		// read button
		Status = digitalRead(Sboil);	// pin 11
		if (Status == LOW && BoilerPress == HIGH)
		{
			BoilerOld = !BoilerOld;
			BoilerPress = LOW; // press button
		}
		if (Status == HIGH)
		{
			BoilerPress = HIGH;
		}
		BoilerStatus = BoilerOld;

		digitalWrite(Lboil, BoilerStatus);		// Boiler led off 7
		digitalWrite(RelBoil, BoilerStatus);	// Boil relay off 3
		if (BoilerStatus == HIGH)
		{
			if (PressCold == HIGH && ColdStatus == HIGH)
			{
				if (HotStatus == HIGH) // hot water up
				{
					HotStatus = LOW;       // in next loop hot water down

				}
			}
			else {
				BoilerStatus = LOW;
				BoilerOld = LOW;
			}
			if (PressCold == LOW)			// No cold water
			{
				BoilerStatus = LOW;			// switch off boiler
				digitalWrite(Lboil, BoilerStatus);		// Boiler led off 7
				digitalWrite(RelBoil, BoilerStatus);		// Boil relay off 3			

			}
		}
	else { // boiler off
		Status = digitalRead(Shot);			// pin 12
		if (Status == LOW && HotPress == HIGH)
		{
			HotOld = !HotOld;
			HotPress = LOW; // press button
		}
		if (Status == HIGH)
		{
			HotPress = HIGH;
		}
		HotStatus = HotOld;
		Status = digitalRead(Scold);			// pin 12
		if (Status == LOW && ColdPress == HIGH)
		{
			ColdOld = !ColdOld;
			ColdPress = LOW; // press button
		}
		if (Status == HIGH)
		{
			ColdPress = HIGH;
		}
		ColdStatus = ColdOld;
		if (PressCold == LOW && ColdStatus == HIGH)
		{
			digitalWrite(Lcold, HIGH); // 9
			delay(100);
			digitalWrite(Lcold, LOW);
			delay(100);
		}
	}
	}
	
}

