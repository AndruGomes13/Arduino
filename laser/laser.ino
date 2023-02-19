#define OUTPUT_PIN 2

int laser_status = 0;

void setup(){
    pinMode(OUTPUT_PIN, OUTPUT);
}

void loop(){
    if (laser_status == 0){
        digitalWrite(OUTPUT_PIN, HIGH);
        laser_status = 1;
    } else {
        digitalWrite(OUTPUT_PIN, LOW);
        laser_status = 0;

    }

    delay(1000);


}