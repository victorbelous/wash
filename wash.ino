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
const int ColdPin = A0;			// Analog Pin cold pressure
const int HotPin = A1;			// Analog Pin hot pressure
const byte TonePin = A2;		// Audio signal
const byte TENpin = A3;			// Relay TEN
//const byte TonePin = 16;		// Audio signal
const byte PressMin = 100;	  // Pressure min
unsigned long DelaySleep = 600; // 10 min test circle
unsigned long OldTime;		// running time
int PressCold;			  // Pressure cold variable
int PressHot;			  // Pressure hot variable
int SenCold = 0;		  //  Sensor cold A0 pin
int SenHot = 1;			  // Sensor hot A1 pin
int DelaySwitch = 1000;	  //  Время для переключения вентиля
boolean Cold;				// Cold water yes
boolean ColdStatus = HIGH;			  // Status cold relay
boolean ColdOld;			// Last status COLD
boolean ColdPress;			// previously press COLD button
boolean Cold2;				// for Valve
boolean PredCold;			  // Предыдущее состояние холодного реле
boolean Hot;				// Hot water yes
boolean HotStatus = HIGH;			  // Status hot relay
boolean HotPress = HIGH;		// previously press HOT button
boolean Hot2;				// for Valve
boolean HotOld;					// Last status HOT
boolean BoilerStatus = LOW;			  // Status boiler relay
boolean BoilerOld;			// Last status BOILER
boolean Boiler2;			// for ValveStatus
boolean BoilerPress;		// previously press BOILER button
boolean AutoStatus = HIGH;			  // current Status auto
boolean Status;				// Readed Status
boolean OldStatus = HIGH;			// Last status Auto
boolean OldPress = HIGH;          // previously press Auto button
boolean ValveStatus;			// status any Valve



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
	pinMode(ColdPin, INPUT_PULLUP);	// Analog pin input
	pinMode(HotPin, INPUT_PULLUP);	// Analog pin hot pressure input
	pinMode(TonePin, OUTPUT);		// Buzzer
	pinMode(TENpin, OUTPUT);		// TEN relay

	tone(TonePin, 500);  	
//	digitalWrite(TonePin, LOW);
//	digitalWrite(TonePin, HIGH);
//	digitalWrite(RelOn, HIGH); 	  // relay power on
//	delay(DelaySwitch);               // 10 sec for run switch

//        Serial.begin(9600);  // для отладки
    AutoStatus = HIGH;   // auto on
	BoilerStatus = HIGH;	// boiler on
	BoilerOld = LOW;
	ColdStatus = LOW;	// cold water on
	ColdOld = HIGH;
	HotStatus = HIGH;	// how water on    
	HotOld = HIGH;
	OldPress = HIGH;
	OldStatus = HIGH;
	digitalWrite(Lauto, AutoStatus);		// light led auto
	digitalWrite(RelOn, HIGH);		// switch on relay power
	digitalWrite(Lboil, BoilerStatus);		// Boiler led off 7
	digitalWrite(RelBoil, BoilerStatus);	// Boil relay off 3
	digitalWrite(Lcold, ColdStatus);		// Cold led off 9
	digitalWrite(RelCold, ColdStatus);		// Cold relay off 5
	digitalWrite(Lhot, HotStatus);		// Boiler led off 8
	digitalWrite(RelHot, HotStatus);		// Boil relay off 4
	delay(DelaySwitch);							// wait to end
	tone(TonePin, 1000);
	ColdStatus = HIGH;
	HotStatus = HIGH;
	BoilerStatus = LOW;
	Boiler2 = LOW;
	digitalWrite(Lboil, BoilerStatus);		// Boiler led off 7
	digitalWrite(RelBoil, BoilerStatus);	// Boil relay off 3
	digitalWrite(Lcold, ColdStatus);		// Cold led off 9
	digitalWrite(RelCold, ColdStatus);		// Cold relay off 5
	digitalWrite(Lhot, HotStatus);		// Boiler led off 8
	digitalWrite(RelHot, HotStatus);		// Boil relay off 4
	delay(DelaySwitch);
	digitalWrite(RelOn, LOW);				// Power off
//	tone(TonePin, 0);
//	digitalWrite(TonePin, HIGH);			// Sound off
//		digitalWrite(TonePin, LOW);			// Sound off
//		digitalWrite(TonePin, HIGH);			// Sound off
		pinMode(TonePin, INPUT_PULLUP);		// Sound off
		ValveStatus = LOW;					// Valve don't switch
		if ( analogRead(ColdPin) > PressMin) {
			PressCold = HIGH;						// Cold water yes
		}
		else PressCold = LOW;					// Cold water no
		if (analogRead(HotPin) > PressMin)
		{
			PressHot = HIGH;					// Hot Water yes
		}
		else PressHot = LOW;					// Hot water no	PressCold = analogRead(A0);  // read cold pressure
		delay(DelaySwitch);                     // wait to switch water
}



void loop () {
	delay(500);
// Обновляю состояние пинов только при изменении условий
	if (OldStatus != AutoStatus)
			ValveStatus = HIGH;					// Auto status change
	else if (BoilerStatus != Boiler2)
			ValveStatus = HIGH;
	else if (HotStatus != Hot2)
			ValveStatus = HIGH;
	else if (ColdStatus != Cold2)
			ValveStatus = HIGH;
	else ValveStatus = LOW;
	if (ValveStatus == HIGH)	
	{
		digitalWrite(RelOn, HIGH);				// питание на вентили включить
		digitalWrite(Lcold, ColdStatus);		// Cold led  9
		digitalWrite(RelCold, ColdStatus);		// Cold relay  5
		digitalWrite(Lboil, BoilerStatus);		// Boiler led  7
		digitalWrite(RelBoil, BoilerStatus);	// Boil relay  3 реле переключения потока воды из бойлера в горячий/холодный трубопровод
		digitalWrite(TENpin, BoilerStatus);		// TEN relay A3 реле включение тена
		digitalWrite(Lhot, HotStatus);			// Hot led  8
		digitalWrite(RelHot, HotStatus);		// Hot relay  4
		pinMode(TonePin, OUTPUT);				// sound on
		delay(DelaySwitch);
		pinMode(TonePin, INPUT_PULLUP);			// sound off
		digitalWrite(RelOn, LOW);				// питание на вентили отключить, потому что какой-то вентиль щелкает
		Boiler2 = BoilerStatus;					// запоминаю состояние пинов
		OldStatus = AutoStatus;
		Hot2 = HotStatus;
		Cold2 = ColdStatus;
	}
	Status = digitalRead(Sauto);				// читаю кнопку АВТО
	if (Status == LOW && OldPress == HIGH)
	{
		OldStatus = !OldStatus;
		OldPress = LOW; // press button	
	}
	if (Status)									// Status == HIGH
	{
		OldPress = HIGH;
	}
	AutoStatus = OldStatus;	
	digitalWrite(Lauto, AutoStatus);		// Auto led on  6 состояние триггера АВТО отображаю светодиодом

	if ( analogRead(ColdPin) > PressMin) {	// Проверяю давление холодной воды
		PressCold = HIGH;						// Cold water yes
	}
	else PressCold = LOW;					// Cold water no
	if (analogRead(HotPin) > PressMin)		// Проверяю давление горячей воды
	{
		PressHot = HIGH;					// Hot Water yes
	}
	else PressHot = LOW;					// Hot water no
  	if (AutoStatus) 				// AutoStatus option on Цикл при включённом триггере АВТО
	{
		if (PressCold) {
			if (PressHot) {
				if (BoilerStatus) { // Есть горячая и холодная вода, нужно выключить бойлер
					BoilerStatus = LOW;
				}
				HotStatus = HIGH; //hot water on Есть горячая, включить вентиль горячей
			}
			else { // no hot water нет горячей воды
				BoilerStatus = HIGH; // boiler on включить бойлер
				HotStatus = LOW; // hot water off выключить вентиль горячей
			} 
			ColdStatus = HIGH; // cold water must be on есть давление холодной, включить вентиль

		}
		else { // no cold water
				digitalWrite(Lcold, HIGH); // 9
				delay(100);
				digitalWrite(Lcold, LOW);
				delay(100);
				if (BoilerStatus)
				{
					BoilerStatus = LOW;		// boiler off
				}
		}
		delay(20);
	}
	else {	// Autostatus == LOW
		// read button
		Status = digitalRead(Sboil);	// pin 11
		if (Status == LOW && BoilerPress == HIGH)
		{
			BoilerOld = !BoilerOld;
			BoilerPress = LOW; // press button
		}
		if (Status)
		{
			BoilerPress = HIGH;
		}
		BoilerStatus = BoilerOld;
		if (BoilerStatus == HIGH)
		{
			if (PressCold == HIGH && ColdStatus == HIGH)
			{
				if (HotStatus) // hot water up
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

