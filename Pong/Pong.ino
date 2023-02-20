#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float dt;

class Ball{
    public:
    float X;
    float Y;
    float V_X;
    float V_Y;
    float vel;
    float angle;
    int radius = 2;
    float X_pred;
    float Y_pred;

    Ball(float default_vel){
        vel = default_vel;
        reset();
    }

    void reset(){
        // TODO: Add initial velocity variation
        X = SCREEN_WIDTH / 2;
        Y = SCREEN_HEIGHT / 2;
        angle = -30 * DEG_TO_RAD;
        set_angle(angle);
    }
    
    void set_angle(float direction){
        V_X = sin(direction) * vel;
        V_Y = cos(direction) * vel;
    }

    void wall_collision(){
        // TODO: add continuous collision method
        if (Y_pred <= radius){
            V_Y *= -1;
            // Y = radius;
        } else if(Y_pred >= SCREEN_HEIGHT - radius){
            V_Y *= -1;
            // Y = SCREEN_HEIGHT - radius;
        }
    }

    int outside_game(){
        if ((X_pred <= radius) or (X_pred >= SCREEN_WIDTH - radius)){
            return 1;
        }else{
            return 0;
        }    
    }

    int step(float dt){
        X_pred = X + V_X * dt;
        Y_pred = Y + V_Y * dt;
        wall_collision();
        if (outside_game()){
            return 1;
        }
        X += V_X * dt;
        Y += V_Y * dt;
        return 0;
    }

    void draw(){
        display.fillCircle(X, Y, radius, WHITE);
    }
};


class Paddle{

    public:
    float Y;
    float X;
    float vel;
    int lenght;
    int width;
    char side;
    int max_Y;
    float Y_pred;
    float max_angle = 75;

    Paddle(int l, int w, char s){
        // Paddle Characteristics
        lenght = l;
        width = w;
        vel = 0;
        side = s;

        // Paddle initial position
        Y = SCREEN_HEIGHT / 2 - lenght / 2;
        if (s == 'L'){
            X = 0;
        } else if (s == 'R'){
            X = SCREEN_WIDTH - width;
        }
        max_Y = SCREEN_HEIGHT - lenght;
    }

    void test(Ball& ball){
        ball.X = random (30, 50);
        ball.Y = random (30, 50);
    }
    int resolve_collision(Ball& ball){
        float hit_location = (ball.Y - Y) / lenght;
        if (hit_location < 0 or hit_location > 1){
            return 0;
        }
        if (side == 'L' and ball.V_X < 0 and ball.X <= width + ball.radius){
            float hit_location_norm = 2 * hit_location - 1;
            float new_angle = (-max_angle * hit_location_norm + 90.0) * DEG_TO_RAD; 
            ball.set_angle(new_angle);
            return 1;
        }
        if (side == 'R' and ball.V_X > 0 and ball.X >= SCREEN_WIDTH - width - ball.radius){
            float hit_location_norm = 2 * hit_location - 1;
            float new_angle = (max_angle * hit_location_norm - 90.0) * DEG_TO_RAD; 
            ball.set_angle(new_angle);
            return 1;
        }
        return 0;
    }

    void step(float dt){
        Y_pred = Y + vel * dt;
        // if ((Y_pred >= 0) and (Y_pred <= max_Y){
        //     Y = Y_pred
        // }

        if (Y_pred < 0){
            Y = 0;
        } else if (Y_pred > max_Y){
            Y = max_Y;
        } else{
            Y = Y_pred;
        }
    }
    void draw(){
        display.fillRect(X, Y, width, lenght, WHITE);
    }
};

void game_over_screen(){
    int restart = 0;
    while (not restart)
    {
        display.setCursor(30, 20);
        display.setTextColor(BLACK, WHITE);
        display.print("Game Over");

        display.display();

        int left = not digitalRead(4);
        int right = not digitalRead(5);
        if (left or right){restart = 1;}
    }
    
}


Paddle L_player(20, 5, 'L');

Paddle R_player(20, 5, 'R');

Ball ball(20.0);

void setup(){
    Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Clear the buffer
    display.clearDisplay();
    display.display();


    pinMode(4, INPUT_PULLUP);
    pinMode(5, INPUT_PULLUP);

}


void loop(){

    // ----------- Handling Inputs ------------
    int left = not digitalRead(4);
    int right = not digitalRead(5);

    if (left){
        L_player.vel = -10;
    } else if (right){
        L_player.vel = 10;
    } else {
        L_player.vel = 0;
    }
    
    // ------------- AI -------------
    float AI_max_speed = 10;
    float AI_center_pos = R_player.Y + R_player.lenght / 2;
    if (AI_center_pos - 3 > ball.Y){
        R_player.vel = -AI_max_speed;
    } else if(AI_center_pos + 3 < ball.Y){
        R_player.vel = AI_max_speed;
    } else {
        R_player.vel = 0;
    }

    // ----------- Stepping simulation ------------

    dt = 0.1;
    L_player.step(dt);
    R_player.step(dt);

    L_player.resolve_collision(ball);
    R_player.resolve_collision(ball);


    if(ball.step(dt)){
        Serial.println("Lost");
        game_over_screen();
        ball.reset();
    };

    
    // Rendering
    display.clearDisplay();

    L_player.draw();
    R_player.draw();
    ball.draw();

    display.display();

    // TODO set FPS
    delay(20);
};