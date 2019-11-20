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
boolean ColdStatus = HIGH;			  // Status cold relay
boolean ColdOld;			// Last status COLD
boolean ColdPress;			// previously press COLD button
boolean PredCold;			  // Предыдущее состояние холодного реле
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
	digitalWrite(Lcold, HIGH);		// Cold led on
	digitalWrite(Lhot, HIGH);		// Hot led on
	digitalWrite(Lboil, LOW);		// Boiler led off
	digitalWrite(Lauto, HIGH);		// Auto led on
	digitalWrite(RelCold, HIGH);	// Cold relay on
	digitalWrite(RelHot, HIGH);		// Hot relay on
	digitalWrite(RelBoil, LOW);		// Boil relay on

	
//	digitalWrite(RelOn, HIGH); 	  // relay power on
//	delay(DelaySwitch);               // 10 sec for run switch
	PressCold = analogRead(A0);  // read cold pressure
	if (PressCold > PressMin)	  // if cold water
	{
		digitalWrite(RelCold, HIGH);	// open cold water
		digitalWrite(Lcold, HIGH);	// light cold led
		ColdStatus = HIGH;			// cold water yes
	}
	PressHot = analogRead(A1);		// read hot pressure
	if (PressHot > PressMin)		// if hot water
	{
		digitalWrite(RelHot, HIGH);	// open hot water
		digitalWrite(Lhot, HIGH);	// light hot led
		HotStatus = HIGH;			// hot water yes
	}
	else					// no hot water
	{
		if (ColdStatus == HIGH)			// cold water yes
		{
			digitalWrite(RelBoil, HIGH);	// switch on boiler
			digitalWrite(Lboil, HIGH);	// light led boiler
			BoilerStatus = HIGH;			// boiler yes
		}
	}
	delay(DelaySwitch);                     // wait to switch water
	digitalWrite(Lauto, HIGH);		// light led auto
	digitalWrite(RelOn, LOW);		// switch off relay power
//        Serial.begin(9600);  // для отладки
        AutoStatus = HIGH;   // auto on    
		OldPress = HIGH;
		OldStatus = HIGH;
}



void loop () {
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
	digitalWrite(RelBoil, BoilerStatus);		// Boil relay off 3
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
	digitalWrite(Lhot, HotStatus);		// Boiler led off 8
	digitalWrite(RelHot, HotStatus);		// Boil relay off 4
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
	digitalWrite(Lcold, ColdStatus);		// Boiler led off 9
	digitalWrite(RelCold, ColdStatus);		// Boil relay off 5	
	PressCold = analogRead(A0);// Read cold pressure
	PressHot = analogRead(A1); // Read hot pressure
  	if (AutoStatus == HIGH) 				// AutoStatus option on
	{


		if (PressCold > PressMin) {
			if (PressHot > PressMin) {
				if (BoilerStatus == LOW) {
					goto endloop;
				}
				else {
					digitalWrite(Lboil, LOW);		// Boiler led off 7
					digitalWrite(RelBoil, LOW);		// Boil relay off 3
				}
			}
			
			
		}
		else { // no cold water
		
//			for ( byte i=0; i < 5; i++)
//			{
				digitalWrite(Lcold, HIGH); // 9
				delay(100);
				digitalWrite(Lcold, LOW);
				delay(100);
//			}
		}
	}
	
	
	endloop: ;
}
