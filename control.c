
#include "xc.h"
#include "pwm.h"
#include "gpio.h"
#include "qei.h"
#include "control.h"
#include "uart.h"
#include "sensor.h"
#include "timer.h"


int cnt = 0;
int flag = 0;
int i;
float correction = 0.2;
float correction2 = 1;


PID_Controller pos_control_left = {.kp = 1, .ki = 0.1, .kd = 0,
                                    .top_lim = 30, .bot_lim = -30};
PID_Controller pos_control_right = {.kp = 1, .ki = 0.1, .kd = 0,
                                    .top_lim = 30, .bot_lim = -30};

PID_Controller vel_control_left = {.kp = 16, .ki = 0.02, .kd = 4,
                                    .top_lim = 2000, .bot_lim = -2000,
                                    .int_lim = 100,.integral = 0, .last_error = 0};
PID_Controller vel_control_right = {.kp = 15.9, .ki = 0.02, .kd = 4,
                                    .top_lim = 2000, .bot_lim = -2000,
                                    .int_lim = 100, .integral = 0, .last_error = 0};

void short_delay(volatile unsigned int count) {
    while (count--) {
        // Do nothing, just consume time
    }
}
void test_go_straight(int speed) {
    vel_control_left.target = (long)speed;
    vel_control_right.target = (long)speed;
    evaluate_controller(&vel_control_left, current_speed2);
    evaluate_controller(&vel_control_right, current_speed1);
//    sendNameValue("  val L:  ", vel_control_left.value);
//    sendNameValue("  val R:  ", vel_control_right.value);
    drive_motor('L', vel_control_left.value);
    drive_motor('R', vel_control_right.value);
}

char buffer[64];


// PID controller - used in a cascade control configuration
void evaluate_controller(PID_Controller *controller, long int current_control_value) {

    (*controller).error = ((*controller).target - current_control_value) ;
//        sprintf(buffer, "\r value: %ld\t", (*controller).error);
//        send(buffer);
    (*controller).integral = (*controller).integral + (*controller).error;
    
    if ((*controller).integral > (*controller).int_lim) {
        (*controller).integral = (*controller).int_lim;
    }
    if ((*controller).integral < -(*controller).int_lim) {
    (*controller).integral = -(*controller).int_lim;
    }
    
    (*controller).derivative = (*controller).error - (*controller).last_error;
    
    (*controller).last_error = (*controller).error;
    
    (*controller).value = ((*controller).kp * (*controller).error) + 
            ((*controller).ki * (*controller).integral) + 
            ((*controller).kd * (*controller).derivative);
    
//    sprintf(buffer, "\r value: %d\t", (*controller).value);// for debugging
//    send(buffer);
    if ((*controller).value > (*controller).top_lim) {
        (*controller).value = (*controller).top_lim;
    } else if ((*controller).value < (*controller).bot_lim) {
        (*controller).value = (*controller).bot_lim;
    }
 
}


void motor_control() {
    char buffer2[64];
    evaluate_controller(&pos_control_left, (long)(distance1));
    evaluate_controller(&pos_control_right, (long)(distance2));
       
//    sprintf(buffer2, "\r pos1: %d, pos2: %d\t", pos_control_left.value , pos_control_right.value);
//    send(buffer2);
    
    vel_control_left.target = pos_control_left.value;
    vel_control_right.target = pos_control_right.value;
    
//    sprintf(buffer2, "\r vel1: %d, vel2: %d, flag : %d\t", vel_control_left.value , vel_control_right.value, flag);
//    send(buffer2);
    
    if ((abs(pos_control_left.error) < 1000 && abs(pos_control_right.error) < 1000)) {
//        if (distance('f') != -1) {
            
            fast_stop_motor('L');
            fast_stop_motor('R');
            flag = 0;
//        }
    } else 
//    
    if (flag == 1 || flag == 7) {
        int dist_front = distance('f');
        if (frontWall() && dist_front != -1) {
            approach_wall();
            flag = 10;
        }
     else if (flag == 1 && (rightWall() || leftWall())) {
        int dist_left = distance('l');
        if (dist_left == -1) {
            dist_left = 0;
        }
        int dist_right = distance('r');
        if (dist_right == -1) {
            dist_right = 0;
        }
        int dist_diff = dist_left - dist_right;
        if (rightWall() && leftWall()) {
            vel_control_left.target -= (int)(correction*dist_diff);
            vel_control_right.target += (int)(correction*dist_diff);
        } else if (rightWall()) {

            int dist_miss = dist_right - 30;
            vel_control_left.target += (int)(correction2*dist_miss);
            vel_control_right.target -= (int)(correction2*dist_miss);
        } else {
            int dist_miss = dist_left - 30;
            vel_control_left.target -= (int)(correction2*dist_miss);
            vel_control_right.target += (int)(correction2*dist_miss);

        }
    }
    }
    
    evaluate_controller(&vel_control_left, (long)current_speed1);
    evaluate_controller(&vel_control_right, (long)current_speed2);
    
//    drive_motor('L', vel_control_left.value);
//    drive_motor('R', vel_control_right.value);     
}
/////////////////////////////////NEW EXPERIMENTAL FUNCTION////////////////////////////
void evaluate_movement() {
    
    
    // Evaluate the position and speed controllers
    evaluate_controller(&pos_control_left, (long)(distance2));
    evaluate_controller(&pos_control_right, (long)(distance1));
    
    vel_control_left.target =  pos_control_left.value;
    vel_control_right.target = pos_control_right.value;
    
//    sprintf(buffer, "\r left control: %d right control %d \t",pos_control_left.error, pos_control_left.error);// for debugging
//        send(buffer);
//     Check if the target has been reached
    if (flag == 1 || flag == 7) { // keep center line movement based on wall following
        int dist_front = distance('f');
        if (frontWall() && dist_front != -1) {
            approach_wall();
//            
//            flag = 10;
//            drive_motor('L', 0);
//            drive_motor('R', 0);
//            flag = 0;
            sprintf(buffer, "\r retard");// for debugging
            send(buffer);
            
            
        }
    else 
        if ((flag == 1 || flag == 7 || flag == 6)&& (rightWall() || leftWall())) {
        int dist_left = distance('l');
        
        if (dist_left == -1) {
            dist_left = 0;
            
        }
        int dist_right = distance('r');
        if (dist_right == -1) {
            dist_right = 0;
            
        }
        int dist_diff = dist_left - dist_right;
        if (rightWall() && leftWall()) {
            vel_control_left.target -= (long int)(correction*dist_diff);
            vel_control_right.target += (long int)(correction*dist_diff);
            
        } else if (rightWall()) {
            int dist_miss = dist_right - 55;
            vel_control_left.target += (int)(correction2*dist_miss);
            vel_control_right.target -= (int)(correction2*dist_miss);
            
        } else {
            int dist_miss = dist_left - 55;
            vel_control_right.target += (int)(correction2*dist_miss);
            vel_control_left.target -= (int)(correction2*dist_miss);
//            vel_control_right.target += (int)(correction2*dist_miss);
            
        }
    }
    }
    if (flag == 1 && (abs(pos_control_right.error) < 20 && abs(pos_control_right.error) < 20)) { // flag = 1 -> go one cell
        // Stop motors when the target is reached
        drive_motor('R', 0);
        drive_motor('L', 0);
        flag = 0;  // Reset flag to indicate motion is complete
        sprintf(buffer, "\r done flag 1");// for debugging
        send(buffer);
//        sprintf(buffer, "\r left control: %ld right control %ld \t",distance1, distance2);// for debugging
//        send(buffer);
    }
    if (flag == 2 && (abs(pos_control_right.error) < 20 && abs(pos_control_right.error) < 20)) { // flag = 2 -> turn right

        flag = 0;  // Reset flag to indicate motion is complete
        sprintf(buffer, "\r done flag 2");// for debugging
        send(buffer);
//        sprintf(buffer, "\r left control: %ld right control %ld \t",distance1, distance2);// for debugging
//        send(buffer);
    }
    if (flag == 3 && (abs(pos_control_right.error) < 20 && abs(pos_control_right.error) < 20)) { // flag = 3 -> turn left
        
        flag = 0;  // Reset flag to indicate motion is complete
        sprintf(buffer, "\r done flag 3");// for debugging
        send(buffer);
//        sprintf(buffer, "\r left control: %ld right control %ld \t",distance1, distance2);// for debugging
//        send(buffer);
    }
    if (flag == 4 && (abs(pos_control_right.error) < 20 && abs(pos_control_right.error) < 20)) { // flag = 4 -> turn 180
        // Stop motors when the target is reached
        drive_motor('L', 0);
        drive_motor('R', 0);
        

        flag = 0;  // Reset flag to indicate motion is complete
        sprintf(buffer, "\r done flag 4");// for debugging
        send(buffer);
//        sprintf(buffer, "\r left control: %ld right control %ld \t",distance1, distance2);// for debugging
//        send(buffer);
    }

    if (flag == 6 && (abs(pos_control_right.error) < 10 && abs(pos_control_right.error) < 10)) { // flag = 6 -> approach wall
        // Stop motors when the target is reached
//        drive_motor('L', 0);
//        drive_motor('R', 0);
        pos_control_left.target = distance2 + 20*100;
        pos_control_right.target = distance1 + 20*100;
        if ((abs(pos_control_right.error) < 10 && abs(pos_control_right.error) < 10)){
            flag = 0; 
            sprintf(buffer, "\r done flag 6");// for debugging
            send(buffer);
        }

    }
    if (flag == 7 && (abs(pos_control_right.error) < 10 && abs(pos_control_right.error) < 10)) { // flag = 7 -> go x cells
        // Stop motors when the target is reached
        sprintf(buffer, "\r here");// for debugging
        send(buffer);
        drive_motor('L', 0);
        drive_motor('R', 0);
        
        flag = 0;  // Reset flag to indicate motion is complete
        sprintf(buffer, "\r done flag 7");// for debugging
        send(buffer);
//        sprintf(buffer, "\r flag: %d right control %ld \t",flag, distance1);// for debugging
//        send(buffer);
    }
    
    
    
    evaluate_controller(&vel_control_left, (long)current_speed2);
    evaluate_controller(&vel_control_right, (long)current_speed1);
    
    
//    sprintf(buffer, "\r left control: %d right control %d \t",abs(pos_control_left.error), abs(pos_control_right.error));// for debugging
//    send(buffer);
    // Drive the motors
    
    drive_motor('L', vel_control_left.value);
    drive_motor('R', vel_control_right.value);
 
    
}
/////////////////////////////////////Moment Functions///////////////////////////////////////////////////
void go_one_cell() {
    
    
    
    if (flag != 1 && flag != 10 && flag == 0) {
        
        int d = (int)((18)*1000); //in ticks. each cell is 18 cm and the wheels are at 3.25 cm from the back. multiply by 1000 for correct dimensions.
        
        GET_ENCODER_VALUE_1(current_pos1);
        GET_ENCODER_VALUE_2(current_pos2);
//        sprintf(buffer, "\r d: %d\t", d);// for debugging
//        send(buffer);
        pos_control_left.target = (long)(distance2) + d;
        pos_control_right.target = (long)(distance1) + d;

//        }
        flag = 1;
//        sprintf(buffer, "\r flag: %d\t", flag);// for debugging
//        send(buffer);
    }
}

void turn_right() {
    
    if (flag != 2 && flag == 0) {
//        sprintf(buffer, "\r left control: %ld right control %ld \t",distance2, distance1);// for debugging
//        send(buffer);
        int t = 840.5*DISTANCE_PER_CNT;
    
        GET_ENCODER_VALUE_1(current_pos1);
        GET_ENCODER_VALUE_2(current_pos2);


        pos_control_left.target = distance2 + t;
        pos_control_right.target = distance1 - t;

        flag = 2;
    }
}

void turn_left() {
    
    if (flag != 3) {
        
        int t = 840.5*DISTANCE_PER_CNT;

        GET_ENCODER_VALUE_1(current_pos1);
        GET_ENCODER_VALUE_2(current_pos2);

        pos_control_left.target = distance2 - t;
        pos_control_right.target = distance1 + t;

        flag = 3;
    }
}

void turn_180() {
    
    if (flag != 4 && flag == 0) {
        
        int t = 2*840.5*DISTANCE_PER_CNT;

        GET_ENCODER_VALUE_1(current_pos1);
        GET_ENCODER_VALUE_2(current_pos2);

        pos_control_left.target = distance2 + t;
        pos_control_right.target = distance1 - t;

        flag = 4;
    }
}

void stop() {
    
    if (flag != 5) {
        
        GET_ENCODER_VALUE_1(current_pos1);
        GET_ENCODER_VALUE_2(current_pos2);
       
        pos_control_left.target = distance2;
        pos_control_right.target = distance1;


        flag = 5;
    }
}

void approach_wall() {
    
    static int desired_dist = 38;// want to be 4cm from wall
//    sprintf(buffer, "\r flag: %d, front: %d\t", flag, frontWall());// for debugging
//        send(buffer);
    if (flag != 6 && frontWall() == 1) {
        
        GET_ENCODER_VALUE_1(current_pos1);
        GET_ENCODER_VALUE_2(current_pos2);
        
        int miss_value = distance('f') - desired_dist;
        
        int correction_value = (int)(miss_value * 100); //in ticks. multiply by 100 for correct dimensions.
//        sprintf(buffer, "\r correction_value: %d, miss_value: %d\t", correction_value, miss_value);// for debugging
//        send(buffer);        
        pos_control_left.target = distance2 + correction_value;
        pos_control_right.target = distance1 + correction_value;

        flag = 6;
    }    
}

void go_x_cells(int x_cells) {
    
    if (flag != 7 && flag != 10 && flag == 0) {
        
        long int d = (18)*1000; //in ticks. each cell is 18 cm and the wheel are at 3.25 cm from the back. multiply by 1000 for correct dimensions.
//        sprintf(buffer, "\r d: %ld\t", d);// for debugging
//        send(buffer);
        GET_ENCODER_VALUE_1(current_pos1);
        GET_ENCODER_VALUE_2(current_pos2);
        
        pos_control_left.target = (long)(distance2) + (long)(x_cells*d);
        pos_control_right.target = (long)(distance1) + (long)(x_cells*d);

        flag = 7;
    }

}







