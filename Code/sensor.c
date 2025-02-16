#include "dma.h"
#include "uart.h"
#include "stdio.h"
#include <stdlib.h>



/** 
 *  returns approximated distance in mm for FRONT sensor
 *  distance 20-90mm in 50mm steps. 
 *  returns -1 for to far
*/
extern long int an1;
extern long int an2;
extern long int an3;
int distance(char sensor){
   long int ret, val;
    if (sensor == 'f')
        val = an1;
    else if(sensor == 'r')
        val = an3;
    else
        val = an2;
        if (44288 < val){
            ret = 20;
        } else {
            if (36672 < val){
                ret = 25;
            } else {
                if (32064 < val){
                    ret = 30;
                } else {
                if (31040 < val ) {
                    ret = 35;
                } else {
                    if (29184 < val ) {
                        ret = 40;
                    } else {
                        if (26944 < val ) {
                            ret = 45;
                        } else {
                            if (24960 < val ) {
                                ret = 50;
                            } else {
                                if (22976 < val ) {
                                    ret = 55;
                                } else {
                                    if (22016 < val ) {
                                        ret = 60;
                                    } else {
                                        if (20800 < val ) {
                                            ret = 65;
                                        } else {
                                            if (18240 < val ) {
                                                ret = 70;
                                            } else {
                                                if (18048 < val){
                                                    ret = 75;
                                                } else {
                                                    if(17920 < val){
                                                        ret = 80;
                                                    } else {
                                                        if (16640 < val){
                                                            ret = 85;
                                                        } else {
                                                            if (15360 < val){
                                                                ret = 90;
                                                            } else {
                                                            ret = -1;
                                                        }
                                                                    }
                                                                }
                                                            }

                                                        }  

                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }   
                }    
                
            
    return ret;
}

/** 
 *  returns 0 for no wall 1 for wall on the right side
*/
int rightWall(){
   long int val = an3;
    if (val < 15360)
        return 0;
    return 1;
}

/** 
 *  returns 0 for no wall 1 for wall on the left side
*/
int leftWall(){
    long int val = an2;
    if (val < 15360)
        return 0;
    return 1;
}

/** 
 *  returns 0 for no wall 1 for wall in front
*/
int frontWall(){
    long int val = an1;
    if (val < 15360)
        return 0;
    return 1;
}



