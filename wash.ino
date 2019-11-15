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
boolean Cold = LOW;			  // Status cold relay
boolean PredCold;			  // Предыдущее состояние холодного реле
boolean Hot = LOW;			  // Status hot relay
boolean Boiler = LOW;			  // Status boiler relay
boolean AutoStatus = HIGH;			  // Status auto



void setup ()
{
	pinMode(Scold, INPUT_PULLUP);    // Switch cold input pull up pin
	pinMode(Shot, INPUT_PULLUP);	  // Switch hot input pull up pin
	pinMode(Sboil, INPUT_PULLUP);	  // Switch boiler input pull up pin
	pinMode(Sauto, INPUT_PULLUP);	  // Switch auto input pull up pin
	pinMode(Lcold, OUTPUT);	  // Cold led pin
	pinMode(Lhot, OUTPUT);	          // Hot led pin
	pinMode(Lboil, OUTPUT);	  // Boil led pin
        pinMode(Lauto, OUTPUT);	          // Auto led pin
	pinMode(RelCold, OUTPUT);	  // Cold relay pin
	pinMode(RelHot, OUTPUT);	  // Hot relay pin
	pinMode(RelBoil, OUTPUT);	  // Boil relay pin
	pinMode(RelOn, OUTPUT);		  // Power relay pin
	digitalWrite(Lcold, LOW);	  // Cold led off
	digitalWrite(Lhot, LOW);	  // Hot led off
	digitalWrite(Lboil, LOW);	  // Boiler led off
	digitalWrite(Lauto, HIGH);	  // Auto led on
	digitalWrite(RelCold, LOW);	  // Cold relay off
	digitalWrite(RelHot, LOW);	  // Hot relay off
	digitalWrite(RelBoil, LOW);	  // Boil relay off

	
//	digitalWrite(RelOn, HIGH); 	  // relay power on
//	delay(DelaySwitch);               // 10 sec for run switch
	PressCold = analogRead(SenCold);  // read cold pressure
	if (PressCold > PressMin)	  // if cold water
	{
		digitalWrite(RelCold, HIGH);	// open cold water
		digitalWrite(Lcold, HIGH);	// light cold led
		Cold = HIGH;			// cold water yes
	}
	PressHot = analogRead(SenHot);		// read hot pressure
	if (PressHot > PressMin)		// if hot water
	{
		digitalWrite(RelHot, HIGH);	// open hot water
		digitalWrite(Lhot, HIGH);	// light hot led
		Hot = HIGH;			// hot water yes
	}
	else					// no hot water
	{
		if (Cold == HIGH)			// cold water yes
		{
			digitalWrite(RelBoil, HIGH);	// switch on boiler
			digitalWrite(Lboil, HIGH);	// light led boiler
			Boiler = HIGH;			// boiler yes
		}
	}
	delay(DelaySwitch);                     // wait to switch water
	digitalWrite(Lauto, HIGH);		// light led auto
	digitalWrite(RelOn, LOW);		// switch off relay power
        Serial.begin(9600);  // для отладки
        AutoStatus = HIGH;   // auto on    
}
void loop ()

{

  Serial.print("SenCold=");  
  Serial.println(SenCold);
  Serial.print("AutoStatus=");
  Serial.println(AutoStatus);
    AutoStatus = pressButton(Sauto, AutoStatus);
      Serial.print("AutoStatus=");
  Serial.println(AutoStatus);
  	if (AutoStatus == HIGH) 				// AutoStatus option on
	{
          Serial.println("HIGH");
		PressCold = analogRead(SenCold);// Read cold pressure
          Serial.print("PressCold=");
          Serial.println(PressCold);
          Serial.print("Cold=");
          Serial.println(Cold);

		if (Cold == 0)		        // was not before cold water
		{
			if (PressCold > PressMin)	// Cold water yes
			{
				Cold = 1;		// Cold change from no to yes
			}
		}
		if (Cold == 1 && PressCold < PressMin)	// Cold change from yes to no
		{
			Cold = 0;			// Cold change from yes to no
		}
		PressHot = analogRead(SenHot);		// Читаю холодное давление
          Serial.print("Hot=");
          Serial.println(Hot);
          Serial.print("PressHot=");
          Serial.println(PressHot);
          
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
	if (AutoStatus == LOW) // manual control
	{
                         Serial.println("LOW");
                         Cold = pressButton(Scold, Cold);
		if (Cold == LOW)        // switch on cold water
		{
			digitalWrite(RelCold, HIGH);	// cold relay on
			digitalWrite(Lcold, HIGH);	//cold led on

			digitalWrite(RelOn, HIGH);	// power on relay
			delay(DelaySwitch);		// wait for swich water
			digitalWrite(RelOn, LOW);	// power off relay
		}
		else	        // swich off cold water
		{
			digitalWrite(RelCold, LOW);	// cold relay off
			digitalWrite(Lcold, LOW);	// cold led off

			digitalWrite(RelOn, HIGH);	// power on relay
			delay(DelaySwitch);		// wait for switch water
			digitalWrite(RelOn, LOW);	// power off relay
		}
                         Hot = pressButton(Shot, Hot);
		if (Hot == LOW)		// switch on hot water
		{
			digitalWrite(RelHot, HIGH);	// hot relay on
			digitalWrite(Lhot, HIGH);	// hot led on
			digitalWrite(RelOn, HIGH);	// power on relay
			delay(DelaySwitch);		// wait for switch water
			digitalWrite(RelOn, LOW);	// power off relay
		}
                Boiler = pressButton(Sboil, Boiler);
		if (Boiler == LOW)	// switch on boiler
		{
			digitalWrite(RelBoil, HIGH);	// switch on boiler
			digitalWrite(Lboil, HIGH);	// switch on led boiler
		}
		else	// switch off boiler
		{
			digitalWrite(RelBoil, LOW);	// switch off boiler
			digitalWrite(Lboil, LOW);	// switch off led boiler
		
		}
	}
	if (AutoStatus == HIGH) 		// switch auto on
	{
		digitalWrite(Lauto, HIGH);		// switch auto led on

	}
	else			// switch auto off
	{
		digitalWrite(Lauto, LOW);		// switch auto led off
	}
}
boolean pressButton(byte nButton, boolean LastState)
{
 boolean Status; //   состояние кнопки

  unsigned int DebDelay = 100;  // задержка мс
  unsigned int LastDebTime;    // предыдущая задержка

delay(3000); 
  Status=digitalRead(nButton);
   Serial.print("pressButton Status =");
 Serial.println(Status);
  Serial.print("nButton=");
  Serial.println(nButton);
    Serial.print("LastState=");
  Serial.println(LastState);
  if (Status != LastState)
    {
      LastDebTime = millis();
      Serial.print("LastDebTime=");
      Serial.println(LastDebTime);

    }
  if ((millis() - LastDebTime) > DebDelay)
  {
    if (Status != LastState)
    {
 
      if (Status == HIGH) 
      {
        Status = !Status;
      }
    }
  }  
return Status;
}
