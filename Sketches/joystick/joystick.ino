int x = 0;
int y = 0;
int b = 0;
int i = 0;
String result;

void setup() {
	pinMode(A4, INPUT); // X pin
	pinMode(A5, INPUT); // Y Pin
  pinMode(7, INPUT_PULLUP); //set pin 7 as an input and enable the internal pull-up resistor
	Serial.begin(9600);


  
}
void loop() {
  x = analogRead(A4);	// read X axis value [0..1023]
  y = analogRead(A5);	// read Y axis value [0..1023]
  b = digitalRead(7);	// read Button state [0,1]
  
  // Converting values to strings
  String x_str = String(x);
  String y_str = String(y);
  String b_str = String(b);

  result = x_str + ";" + y_str + ";" + b_str;

  Serial.println(result);
}