#include "mbed.h"
#include "physcom.h"

using namespace physcom;
DigitalOut redLed(p21);
DigitalOut greenLed(p22);
Serial pc(USBTX, USBRX);
AnalogIn passive_light(p20);
Ping Pinger(p11);
M3pi robot;


int main() {

    float light_value;

    //infinite loop that constantly checks environment light levels to turn on Green LED if dark
    while (1) { 
        light_value = passive_light.read();
        wait(0.5);
        if (light_value >= 0.01 && light_value < 0.1) { 
            greenLed = 0;
            redLed = 0;
        }
        else if (light_value >= 0.1) {
            greenLed = 1;
        }
        else if (light_value >= 0.001) {
            redLed = 1;
        }  
    }

    int range;
    while (1) {
        int sensors[5]; //array for all sensors
        robot.calibrated_sensors(sensors); //calibration function

        //Ultrasound Sensor
        Pinger.Send(); 
        wait_ms(300);
        range = Pinger.Read_cm();

        //if sensor senses an object within range, Neville stops
        if (range < 20) {
            robot.activate_motor(0,0);
            robot.activate_motor(1,0);
        }
        //else, Neville proceeds to follow the black path
        else {  
            if (sensors[0] < 150 && sensors[4] < 150) { //both sensors see white
                robot.activate_motor(0,0.1);
                robot.activate_motor(1,0.1);
            }

            else if (sensors[0] > 150) { //left sensor sees black SHOULD GO LEFT
                robot.activate_motor(0,0.0);
                robot.activate_motor(1,0.1);
            }

            else if (sensors[4] > 150) { //right sensor sees black SHOULD GO RIGHT
                robot.activate_motor(0,0.1);
                robot.activate_motor(1,0.0);
            }

            else if (sensors[4] > 150 && sensors[0] > 150) { //both sensors see black at end STOP
                robot.activate_motor(0,0.0);
                robot.activate_motor(1,0.0);
            }

            else if (sensors[4] > 100 && sensors[3] > 100 && sensors[2] > 100 && sensors[1] > 100 && sensors[0] > 100) { // all sensors see value greater than white INCLUDING GREEN LANE
                robot.activate_motor(0,0.3); //SPEEDS UP LEFT MOTOR
                robot.activate_motor(1,0.3); //SPEEDS UP RIGHT MOTOR
            }

            else () { //anything else not met in if statements, Neville stops
                robot.activate_motor(0,0.0);
                robot.activate_motor(1,0.0);
            }
        }
    }
}