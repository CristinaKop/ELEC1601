#include "robot.h"

//U = up, D = Down, L = left, R = right
char direction = 'U';
char prev_direction = 'M';
int timer = 0;
int right_timer = 0;
int change = 0;
int count = 0;
int count_timer = 0;
int slow_down_timer_right = 0;
int slow_down_timer_left = 0;
// Sample maze


char state = '1';
char movement = ' ';
char prevMovement = ' ';

// Basic maze 1
void setup_robot(struct Robot *robot){
    robot->x = 270;
    robot->y = 460;
    robot->true_x = 270;
    robot->true_y = 460;
    robot->width = ROBOT_WIDTH;
    robot->height = ROBOT_HEIGHT;
    robot->direction = 0;
    robot->angle = 0;
    robot->currentSpeed = 0;
    robot->crashed = 0;
    robot->auto_mode = 0;

    printf("Press arrow keys to move manually, or enter to move automatically\n\n");
}
// Basic maze 2
//void setup_robot(struct Robot *robot){
//    robot->x = 620;
//    robot->y = 380;
//    robot->true_x = 620;
//    robot->true_y = 380;
//    robot->width = ROBOT_WIDTH;
//    robot->height = ROBOT_HEIGHT;
//    robot->direction = 0;
//    robot->angle = 270;
//    robot->currentSpeed = 0;
//    robot->crashed = 0;
//    robot->auto_mode = 0;
//
//    printf("Press arrow keys to move manually, or enter to move automatically\n\n");
//}

// Basic Maze 3
//void setup_robot(struct Robot *robot){
//    robot->x = 640-10-270;
//    robot->y = 460;
//    robot->true_x = 640-10-270;
//    robot->true_y = 460;
//    robot->width = ROBOT_WIDTH;
//    robot->height = ROBOT_HEIGHT;
//    robot->direction = 0;
//    robot->angle = 0;
//    robot->currentSpeed = 0;
//    robot->crashed = 0;
//    robot->auto_mode = 0;
//
//    printf("Press arrow keys to move manually, or enter to move automatically\n\n");
//}

//// Basic Maze 4
//void setup_robot(struct Robot *robot){
//    robot->x = 0;
//    robot->y = 380;
//    robot->true_x = 0;
//    robot->true_y = 380;
//    robot->width = ROBOT_WIDTH;
//    robot->height = ROBOT_HEIGHT;
//    robot->direction = 0;
//    robot->angle = 90;
//    robot->currentSpeed = 0;
//    robot->crashed = 0;
//    robot->auto_mode = 0;
//
//    printf("Press arrow keys to move manually, or enter to move automatically\n\n");
//}

////Maze 5
//void setup_robot(struct Robot *robot){
//    robot->x = 170;
//    robot->y = 460;
//    robot->true_x = 170;
//    robot->true_y = 460;
//    robot->width = ROBOT_WIDTH;
//    robot->height = ROBOT_HEIGHT;
//    robot->direction = 0;
//    robot->angle = 0;
//    robot->currentSpeed = 0;
//    robot->crashed = 0;
//    robot->auto_mode = 0;
//
//    printf("Press arrow keys to move manually, or enter to move automatically\n\n");
//}

//Maze 6
//void setup_robot(struct Robot *robot){
//    robot->x = 620;
//    robot->y = 40;
//    robot->true_x = 620;
//    robot->true_y = 40;
//    robot->width = ROBOT_WIDTH;
//    robot->height = ROBOT_HEIGHT;
//    robot->direction = 0;
//    robot->angle = 270;
//    robot->currentSpeed = 0;
//    robot->crashed = 0;
//    robot->auto_mode = 0;
//
//    printf("Press arrow keys to move manually, or enter to move automatically\n\n");
//}

////Maze 7
//void setup_robot(struct Robot *robot){
//    robot->x = 640-10-170;
//    robot->y = 460;
//    robot->true_x = 640-10-170;
//    robot->true_y = 460;
//    robot->width = ROBOT_WIDTH;
//    robot->height = ROBOT_HEIGHT;
//    robot->direction = 0;
//    robot->angle = 0;
//    robot->currentSpeed = 0;
//    robot->crashed = 0;
//    robot->auto_mode = 0;
//
//    printf("Press arrow keys to move manually, or enter to move automatically\n\n");
//}

//Maze 8
//void setup_robot(struct Robot *robot){
//    robot->x = 0;
//    robot->y = 40;
//    robot->true_x = 0;
//    robot->true_y = 40;
//    robot->width = ROBOT_WIDTH;
//    robot->height = ROBOT_HEIGHT;
//    robot->direction = 0;
//    robot->angle = 90;
//    robot->currentSpeed = 0;
//    robot->crashed = 0;
//    robot->auto_mode = 0;
//
//    printf("Press arrow keys to move manually, or enter to move automatically\n\n");
//}

int robot_off_screen(struct Robot * robot){
    if(robot->x < 0 || robot-> y < 0){
        return 0;
    }
    if(robot->x > OVERALL_WINDOW_WIDTH || robot->y > OVERALL_WINDOW_HEIGHT){
        return 0;
    }
    return 1;
}

int checkRobotHitWall(struct Robot * robot, struct Wall * wall) {

    int overlap = checkOverlap(robot->x,robot->width,robot->y,robot->height,
                 wall->x,wall->width,wall->y, wall->height);

    return overlap;
}

int checkRobotHitWalls(struct Robot * robot, struct Wall_collection * head) {
   struct Wall_collection *ptr = head;
   int hit = 0;

   while(ptr != NULL) {
      hit = (hit || checkRobotHitWall(robot, &ptr->wall));
      ptr = ptr->next;
   }
   return hit;

}

int checkRobotReachedEnd(struct Robot * robot, int x, int y, int width, int height){

    int overlap = checkOverlap(robot->x,robot->width,robot->y,robot->height,
                 x,width,y,height);

    return overlap;
}

void robotCrash(struct Robot * robot) {
    robot->currentSpeed = 0;
    if (!robot->crashed)
        printf("Ouchies!!!!!\n\nPress space to start again\n");
    robot->crashed = 1;
}

void robotSuccess(struct Robot * robot, int msec) {
    robot->currentSpeed = 0;
    if (!robot->crashed){
        printf("Success!!!!!\n\n");
        printf("Time taken %d seconds %d milliseconds \n", msec/1000, msec%1000);
        printf("Press space to start again\n");
    }
    robot->crashed = 1;
}

int checkRobotSensor(int x, int y, int sensorSensitivityLength, struct Wall * wall)  {
    //viewing_region of sensor is a square of 2 pixels * chosen length of sensitivity
    int overlap = checkOverlap(x,2,y,sensorSensitivityLength,
                 wall->x,wall->width,wall->y, wall->height);

    return overlap;
}

int checkRobotSensorLeftAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store; // ptr and head_store point to the walls
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;
    int sensorSensitivityLength;

    head_store = head; // The pointer head_store is set to the top node (last node that we added)
    robotCentreX = robot->x+ROBOT_WIDTH/2; // calculate the center of the robot based off its width and weight
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;
    sensorSensitivityLength =  floor(SENSOR_VISION/5); // Sensor vision is 30, so 6 ini tally

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        // The coordinates on the screen where the center of the robot is
        xDir = round(robotCentreX+(-ROBOT_WIDTH/2+10)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_WIDTH/2+10)*cos((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle)*PI/180));
        //printf("XDir = %lf, yDir = %lf\n", xDir, yDir);

        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            // if hit is not defined do the second thing
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, &ptr->wall));

            ptr = ptr->next;
        }
        // if hit is defined
        if (hit) // If the sensor is in some range then we return "closeness", 5 ranges
            score = i;
    }

    return score;
}

int checkRobotSensorRightAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store; // ptr and head_store point to the walls
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;
    int sensorSensitivityLength;

    head_store = head; // The pointer head_store is set to the top node (last node that we added)
    robotCentreX = robot->x+ROBOT_WIDTH/2; // calculate the center of the robot based off its width and weight
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;
    sensorSensitivityLength =  floor(SENSOR_VISION/5); // Sensor vision is 30, so 6 ini tally

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        // The coordinates on the screen where the center of the robot is
        xDir = round(robotCentreX+(-ROBOT_WIDTH/2+10)*sin((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_WIDTH/2+10)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle)*PI/180));
        //printf("XDir = %lf, yDir = %lf\n", xDir, yDir);

        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            // if hit is not defined do the second thing
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, &ptr->wall));

            ptr = ptr->next;
        }
        // if hit is defined
        if (hit) // If the sensor is in some range then we return "closeness", 5 ranges
            score = i;
    }

    return score;
}

int checkRobotSensorFrontRightAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store; // ptr and head_store point to the walls
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;
    int sensorSensitivityLength;

    head_store = head; // The pointer head_store is set to the top node (last node that we added)
    robotCentreX = robot->x+ROBOT_WIDTH/2; // calculate the center of the robot based off its width and weight
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;
    sensorSensitivityLength =  floor(SENSOR_VISION/5); // Sensor vision is 30, so 6 ini tally

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        // The coordinates on the screen where the center of the robot is
        xDir = round(robotCentreX+(ROBOT_WIDTH/2-2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle)*PI/180));
        yDir = round(robotCentreY+(ROBOT_WIDTH/2-2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle)*PI/180));
        //printf("XDir = %lf, yDir = %lf\n", xDir, yDir);

        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            // if hit is not defined do the second thing
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, &ptr->wall));

            ptr = ptr->next;
        }
        // if hit is defined
        if (hit) // If the sensor is in some range then we return "closeness", 5 ranges
            score = i;
    }

    return score;
}

int checkRobotSensorFrontLeftAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store; // ptr and head_store point to the walls
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;
    int sensorSensitivityLength =  floor(SENSOR_VISION/5);

    head_store = head; // The pointer head_store is set to the top node (last node that we added)
    robotCentreX = robot->x+ROBOT_WIDTH/2; // calculate the center of the robot based off its width and weight
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;
    sensorSensitivityLength =  floor(SENSOR_VISION/5); // Sensor vision is 30, so 6 ini tally

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        // The coordinates on the screen where the center of the robot is

        xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle)*PI/180));
        //printf("XDir = %lf, yDir = %lf\n", xDir, yDir);

        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            // if hit is not defined do the second thing
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, &ptr->wall));

            ptr = ptr->next;
        }
        // if hit is defined
        if (hit) // If the sensor is in some range then we return "closeness", 5 ranges
            score = i;
    }

    return score;
}


void robotUpdate(struct SDL_Renderer * renderer, struct Robot * robot){
    double xDir, yDir;

    int robotCentreX, robotCentreY, xTR, yTR, xTL, yTL, xBR, yBR, xBL, yBL;
    SDL_SetRenderDrawColor(renderer, 238, 130, 238, 255);

    /*
    //Other Display options:
    // The actual square which the robot is tested against (not so nice visually with turns, but easier
    // to test overlap

    */
    /*
    //Center Line of Robot. Line shows the direction robot is facing
    xDir = -30 * sin(-robot->angle*PI/180);
    yDir = -30 * cos(-robot->angle*PI/180);
    xDirInt = robot->x+ROBOT_WIDTH/2+ (int) xDir;
    yDirInt = robot->y+ROBOT_HEIGHT/2+ (int) yDir;
    SDL_RenderDrawLine(renderer,robot->x+ROBOT_WIDTH/2, robot->y+ROBOT_HEIGHT/2, xDirInt, yDirInt);
    */

    //Rotating Square
    //Vector rotation to work out corners x2 = x1cos(angle)-y1sin(angle), y2 = x1sin(angle)+y1cos(angle)
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;

    // Left
    xDir = round(robotCentreX+(ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xTR = (int) xDir;
    yTR = (int) yDir;

    // Right
    xDir = round(robotCentreX+(ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xBR = (int) xDir;
    yBR = (int) yDir;

    // Front Left
    xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xBL = (int) xDir;
    yBL = (int) yDir;

    // Front right
    xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xTL = (int) xDir;
    yTL = (int) yDir;

    SDL_RenderDrawLine(renderer,xTR, yTR, xBR, yBR);
    SDL_RenderDrawLine(renderer,xBR, yBR, xBL, yBL);
    SDL_RenderDrawLine(renderer,xBL, yBL, xTL, yTL);
    SDL_RenderDrawLine(renderer,xTL, yTL, xTR, yTR);

    int sensor_sensitivity =  floor(SENSOR_VISION/5);
    int i;
    //Left Sensor
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+(-ROBOT_WIDTH/2+10)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*cos((robot->angle)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_WIDTH/2+10)*cos((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*sin((robot->angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, sensor_sensitivity};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

    //Right Sensor
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+(-ROBOT_WIDTH/2+10)*sin((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*cos((robot->angle)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_WIDTH/2+10)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*sin((robot->angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, sensor_sensitivity};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

    //Front Right Sensor
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+(ROBOT_WIDTH/2-2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*sin((robot->angle)*PI/180));
        yDir = round(robotCentreY+(ROBOT_WIDTH/2-2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*cos((robot->angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, sensor_sensitivity};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

    //Front Left Sensor
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*sin((robot->angle)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*cos((robot->angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, sensor_sensitivity};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }
}



void robotMotorMove(struct Robot * robot) {
    double x_offset, y_offset;
    switch(robot->direction){
        case UP :
            robot->currentSpeed += DEFAULT_SPEED_CHANGE;
            if (robot->currentSpeed > 5)
                robot->currentSpeed = 5;
            break;
        case DOWN :
            robot->currentSpeed -= DEFAULT_SPEED_CHANGE;
            if (robot->currentSpeed < -5)
                robot->currentSpeed = -5;
            break;
        case LEFT :
            robot->angle = (robot->angle+360-DEFAULT_ANGLE_CHANGE)%360;
            break;
        case RIGHT :
            robot->angle = (robot->angle+DEFAULT_ANGLE_CHANGE)%360;
            break;
        case BACK :
            robot->currentSpeed = 0;
            break;
    }
    robot->direction = 0;
    x_offset = (-robot->currentSpeed * sin(-robot->angle*PI/180));
    y_offset = (-robot->currentSpeed * cos(-robot->angle*PI/180));

    robot->true_x += x_offset;
    robot->true_y += y_offset;

    x_offset = round(robot->true_x);
    y_offset = round(robot->true_y);

    robot->x = (int) x_offset;
    robot->y = (int) y_offset;
}

// Called when you press enter
void robotAutoMotorMove(struct Robot * robot, int left_sensor, int right_sensor, int front_left_sensor, int front_right_sensor, int *start) {

    printf("L: %d | R: %d | FL: %d| FR: %d\n", left_sensor, right_sensor, front_left_sensor, front_right_sensor);
//    printf("FL: %d\n", front_left_sensor);
    // Calibrate the robot to stick to the wall
    // Turn the robot left 90 degrees incrementally
    if (*start < 4){
        robot -> direction = LEFT;
        *start = *start + 1;

    }
    // Make the robot go straight until the wall is in range 1
    else if(*start == 4){
        // While the robot has not in range of the wall we go straight
        if (front_left_sensor != 3){
          robot->direction = UP;
        }
        // If the robot is range of the wall, turn back forward
        else{
            *start = *start + 1;
            robot->direction = BACK;
        }
    }

    // Turn the robot right so we are facing front again
    else if(*start >= 5 && *start < 8){
        robot -> direction = RIGHT;
        *start = *start + 1;
        printf("Start: %d\n", *start);

    }
    else if (*start == 8){
        *start = 20;
        printf("Start: %d\n", *start);
    }

    // The wall follow code
    else if (*start == 20){

        // Movement code
        // If we need to shift a bit right
        if(movement == 'r' && prevMovement != 'C'){
            printf("Movement is r\n");
            if(timer < 1){
                timer ++;
            }
            else{
                 prevMovement = 'r';
                 movement = 's';
                 timer = 0;
                 robot->direction = LEFT;
                 if (left_sensor == 2){
                    robot->direction = RIGHT;
                 }
            }
        }

        // Full right turn
        else if(movement == 'R' && prevMovement != 'C'){
            printf("Movement is R\n");
            if(timer < 2){
                robot->direction = RIGHT;
                timer ++;
            }
            else{
                 prevMovement = 'R';
                 movement = 's';
                 if (front_right_sensor == 1 && left_sensor == 1){
                   movement = 'r';
                   robot->direction = RIGHT;
                 }
                 timer = 0;
            }
        }


        // Full left turn
        else if(movement == 'L' && prevMovement != 'C'){
            printf("Movement is L\n");
            if (front_left_sensor == front_right_sensor && front_left_sensor > 1) {
                prevMovement = 'L';
                movement = 's';
                timer = 0;
            }
            else if(timer < 2){
                //robot->direction = LEFT;
                timer ++;
            }
            else{
                 prevMovement = 'L';
                 movement = 's';
                 timer = 0;
            }
        }

        // Little left adjust
        else if(movement == 'l' && prevMovement != 'C'){
            printf("Movement is l\n");
            if(front_right_sensor == 3 && prevMovement == 'L'){
                movement = 'R';
                robot->direction = RIGHT;
            }
            else if(left_sensor == 2 && right_sensor == 0 && front_left_sensor == 1 && front_right_sensor == 1 && prevMovement == 'R'){
                movement = 'R';
                robot->direction = RIGHT;
            }
            else if(right_sensor == 1){
                movement = 'r';
                robot->direction = RIGHT;
            }
            else if(left_sensor == 0 && right_sensor == 3 && front_left_sensor == 0 && front_right_sensor == 0){
                //movement = 'L';
                robot->direction = LEFT;
            }
            else if(timer < 1){
                //robot->direction = LEFT;
                timer ++;
            }
            else{
                 prevMovement = 'l';
                 movement = 's';
                 timer = 0;
                 robot->direction = RIGHT;
            }
        }

        // Case 2: The front and right readings are 0
        // Curve correction
        else if ((right_sensor == 0) && (front_left_sensor == 0) && (front_right_sensor == 0) && (left_sensor > 0 && left_sensor <= 4) && prevMovement != 'C'){
            printf("Case 2\n");
            /*if (left_sensor == 3){
               robot->direction = UP;
            }*/
            if (left_sensor == 4){
                // shift right until reading == 3 or 0
                movement = 'r';
                robot->direction = RIGHT;
            }
            else if(left_sensor < 3 && left_sensor > 0){
                movement = 'l';
                robot->direction = LEFT;
            }
            // The robot has shifted a bit from the wall
            else{
                robot->direction = UP;
            }
        }

        // Case 3: Full left turn
        else if (left_sensor == 0 && front_left_sensor == 0 && front_right_sensor == 0 ){
            printf("Case 3\n");
            movement = 'L';
            robot->direction = LEFT;
        }

        // Case 4: Full right turn
        else if (left_sensor == 3 && front_left_sensor == 1 && front_right_sensor == 1){
            printf("Case 4\n");
            movement = 'R';
            robot->direction = RIGHT;
        }

        else if(prevMovement == 'L' && left_sensor == 0 && right_sensor == 0 && front_left_sensor == 2 && front_right_sensor == 2){
            movement = 'R';
            robot->direction = RIGHT;
        }

        // This is for the complex mazes
        else{
            printf("Curve\n");

            if (front_left_sensor > front_right_sensor && front_right_sensor > 0 && left_sensor < 2 && right_sensor < 2) {
                robot -> direction = RIGHT;
            }
            else if (front_left_sensor < front_right_sensor && front_left_sensor > 0 && left_sensor < 2 && right_sensor < 2) {
                robot -> direction = LEFT;
            }
            else if (front_left_sensor == front_right_sensor && front_left_sensor > 1 && left_sensor < 2 && right_sensor < 2) {
                robot -> direction = RIGHT;
            }
            // this is for reflex angle right turns
            else if (front_left_sensor > 0 && front_right_sensor > 0 && left_sensor > 0 && front_left_sensor != front_right_sensor) {
                if (slow_down_timer_right < 7 && slow_down_timer_right >= 0) {
                    robot -> direction = DOWN;
                    slow_down_timer_right++;
                }
                else if (slow_down_timer_right >= 7 && slow_down_timer_right < 11) {
                    slow_down_timer_right++;
                }
                else if (slow_down_timer_right == 11 && slow_down_timer_right == 12) {
                    robot -> direction = RIGHT;
                    slow_down_timer_right++;
                }
                else if (slow_down_timer_right >= 13 && slow_down_timer_right < 15) {
                    robot -> direction = UP;
                    slow_down_timer_right++;
                }
                else if (slow_down_timer_right == 15) {
                    slow_down_timer_right = -1;
                }
                else {
                    robot -> direction = RIGHT;
                }
            }
            else if (front_left_sensor > 0 && front_right_sensor > 0 && right_sensor > 0 && front_left_sensor != front_right_sensor) {
                if (slow_down_timer_left < 7 && slow_down_timer_left >= 0) {
                    robot -> direction = DOWN;
                    slow_down_timer_left++;
                }
                else if (slow_down_timer_left >= 7 && slow_down_timer_right < 11) {
                    slow_down_timer_left++;
                }
                else if (slow_down_timer_left == 11) {
                    robot -> direction = LEFT;
                    slow_down_timer_left++;
                }
                else if (slow_down_timer_left >= 12 && slow_down_timer_left < 14) {
                    robot -> direction = UP;
                    slow_down_timer_left++;
                }
                else if (slow_down_timer_left == 14) {
                    slow_down_timer_left = -1;
                }
                else {
                    robot -> direction = LEFT;
                }
                robot -> direction = LEFT;
            }
            else if (front_left_sensor > 0 && left_sensor > 0) {
                robot -> direction = RIGHT;
            }
            else if (front_right_sensor > 0 && right_sensor > 0) {
                robot -> direction = LEFT;
            }
            else if (front_left_sensor - right_sensor > 0 && right_sensor > 1) {
                robot -> direction = RIGHT;
            }
            else if (front_right_sensor - left_sensor > 0 && left_sensor > 1) {
                robot -> direction = LEFT;
            }
            else if (left_sensor - right_sensor > 0 && right_sensor > 0) {
                robot -> direction = RIGHT;
            }
            else if (right_sensor - left_sensor > 0 && left_sensor > 0) {
                robot -> direction = LEFT;
            }
            else if (left_sensor == right_sensor == front_left_sensor == 0 && front_right_sensor > 0) {
                robot -> direction = LEFT;
            }
            else if (left_sensor == right_sensor == front_right_sensor == 0 && front_left_sensor > 0) {
                robot -> direction = RIGHT;
            }
            else if (left_sensor == 4 && right_sensor == front_left_sensor == front_right_sensor == 0) {
                robot -> direction = RIGHT;
            }
            else if (right_sensor == 4 && left_sensor == front_left_sensor == front_right_sensor == 0) {
                robot -> direction = LEFT;
            }
            else if (left_sensor == right_sensor == front_left_sensor == front_right_sensor == 0) {
                robot -> direction = UP;
            }
            else {
                robot -> direction = UP;
            }

            prevMovement = 'C';

        }

    }

}
