//ROS
#include "ros.h"
#include "geometry_msgs/Twist.h"
#include <std_msgs/UInt16.h>
#include <ros/time.h>

ros::NodeHandle nh;
geometry_msgs::Twist twist;
ros::Publisher joy_pub("cmd_vel", &twist);

std_msgs::UInt16 button_msg;
ros::Publisher pub_button("buttons", &button_msg);

// timers for the sub-main loop
unsigned long currentMillis;
long previousMillis = 0;    // set up timers
float loopTime = 20;

// values for reading sticks

float stick1;
float stick2;
float stick3;
float stick4;
float stick5;
float stick6;
int but1;
int but2;
int but3;
int but4;
int but1out;
int but2out;
int but3out;
int but4out;
uint16_t combo = 0;

void setup() {

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);

  nh.getHardware()->setBaud(115200);      // set baud rate to 115200
  nh.initNode();              // init ROS
  nh.advertise(joy_pub);      // advertise topic
  nh.advertise(pub_button);   // advertise topic

}

// deadzone function for cheap joysticks

float deadzone(float value) {
     if (value > 50) {
        value = value - 50;
     }
     else if (value < -50) {
      value = value +50;
     }
     else {
      value = 0;
     }
     value = value / 500;   // scale so that we get 0.0 ~ 1.0
     return value;  
}

int invButtons(int value) {
  
    if (value == 0) {
      value = 1;
    }
    else if (value == 1) {
      value = 0;
    }
    return value;
}

// ** Main loop **

void loop() {  

  currentMillis = millis();
        if (currentMillis - previousMillis >= loopTime) {  // run a loop every 10ms          
            previousMillis = currentMillis;          // reset the clock to time it 

            // read sticks and buttons

            but1 = digitalRead(2);
            but2 = digitalRead(3);
            but3 = digitalRead(4);
            but4 = digitalRead(5);

            // invert buttons as they are pull_ip

            but1out = invButtons(but1);
            but2out = invButtons(but2);
            but3out = invButtons(but3);
            but4out = invButtons(but4);

            // combine buttons into single uint16_t value

            combo = 0;    // reset value and add buttons in this loop only

            if (but1out == 1) {
              combo = combo + 1;
            }
            else if (but2out == 1) {
              combo = combo  + 2;
            }
            else if (but3out == 1) {
              combo = combo + 4;
            }
            else if (but4out == 1) {
              combo = combo + 8;
            }
            else {
              combo = 0;      // of no buttons are pressed then combo is zero
            }
          
            stick1 = analogRead(A0);
            stick2 = analogRead(A1);
            stick3 = analogRead(A2);
            stick4 = analogRead(A3);
            stick5 = analogRead(A4);
            stick6 = analogRead(A5);

            // remove offsets
          
            stick1 = stick1 - 78 - 512;
            stick1 = deadzone(stick1);
            stick2 = stick2 - 5 - 512;
            stick2 = deadzone(stick2);
            stick3 = stick3 - 6 - 512;
            stick3 = deadzone(stick3);
            stick4 = stick4 - 4 - 512;
            stick4 = deadzone(stick4);
            stick5 = stick5 + 5 - 512;
            stick5 = deadzone(stick5);
            stick6 = stick6 - 15 - 512;
            stick6 = deadzone(stick6);            

            stick5 = stick5 * -1;   // invert value/direction as required based on wiring
            stick3 = stick3 * -1;

            stick3 = stick3 * 2;    // extra scaling for yaw to mke driving nice

            // *** broadcast cmd_vel twist message **

            // Update the Twist message
            twist.linear.x = stick5;
            twist.linear.y = stick4;
            twist.linear.z = stick6;
        
            twist.angular.x = stick2;
            twist.angular.y = stick1;
            twist.angular.z = stick3;

            joy_pub.publish(&twist);        // make the message ready to publish            

            // *** broadcast buttons ***

            button_msg.data = combo;
            pub_button.publish(&button_msg);  

            nh.spinOnce();                  // make sure we do ROS stuff every loop

        } // end of 10ms loop

} // end of main loop
