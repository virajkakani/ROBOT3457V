/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       virajkakani                                               */
/*    Created:      7/22/2026, 6:40:36 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/



#include "vex.h"
#include "math_funcs.h"
#include "constants.h"
#include "directions.h"
#include <chrono>
#include <thread>
#include <atomic>

using namespace vex;

gearSetting geaRat = ratio18_1;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

brain Brain; 

bool bl = false;

// Controller
controller Controller(primary);
motor Updownmotor1(PORT19, geaRat, false);
motor Updownmotor2(PORT1, geaRat, true);
motor_group Updownmotor(Updownmotor1, Updownmotor2);
motor Toggle_left(PORT10, geaRat, true);
motor Toggle_right(PORT13, geaRat, true); 
pneumatics Claw(Brain.ThreeWirePort.A); //






//
motor LeftfrontDriveSmart(PORT16, geaRat, true);//
motor RightfrontDriveSmart(PORT18, geaRat, false);
motor LeftbackDriveSmart(PORT17, geaRat, true);
motor RightbackDriveSmart(PORT14, geaRat, false);

vex::motor_group LeftDriveSmart(LeftfrontDriveSmart, LeftbackDriveSmart);
vex::motor_group RightDriveSmart(RightfrontDriveSmart, RightbackDriveSmart);

inertial DrivetrainInertial(PORT21);
smartdrive Drivetrain(LeftDriveSmart, RightDriveSmart, DrivetrainInertial, 319.19, 320, 40, mm, 1);

double to_volt(double value) {
  if (value > 100) value = 100;
  if (value < -100) value = -100;
  return value * 0.12;
}


void pre_auton(void) {
    while (true) {
        Brain.Screen.clearScreen(color::black);
        Brain.Screen.drawRectangle(20, 50, 180, 120, color::red);
        Brain.Screen.printAt(20 + 25, 50 + 65, "RED ALLIANCE");
        Brain.Screen.drawRectangle(280, 50, 180, 120, color::blue);
        Brain.Screen.printAt(280 + 25, 50 + 65, "BLUE ALLIANCE");
        Brain.Screen.setPenWidth(4);
        if (!bl) {
            Brain.Screen.drawRectangle(16, 46, 188, 128, color::transparent); 
        } else {
            Brain.Screen.drawRectangle(276, 46, 188, 128, color::transparent);
        }
        Brain.Screen.setPenWidth(1);

        // 3. READ THE TOUCH SCREEN CLICKS
        if (Brain.Screen.pressing()) {
            int touchX = Brain.Screen.xPosition();
            int touchY = Brain.Screen.yPosition();
            if (touchY > 50 && touchY < 170) {
                if (touchX > 20 && touchX < 200) {
                    bl = false; 
                    wait(250, msec); 
                }

                else if (touchX > 280 && touchX < 460) {
                    bl = true;
                    wait(250, msec); 
                }
            }
        }
        
        wait(50, msec);
    }
  
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/


bool at_toggle = false;
int toggle = 0;
double epsilon = 0.1;

void autonomous(void) {
  // auto pos = START;
  // auto angle_heading = 180;
  // DrivetrainInertial.resetRotation();
  // DrivetrainInertial.resetHeading();
  // std::vector<geo::point> toggle_locations = {
  //   geo::point(2,0),
  //   geo::point(0,2)
  // };
  // size_t i = 0;

  // auto start = pos;
  // auto dest = std::pair<double,double >(geo::__dist(pos, toggle_locations[i]), atan2(pos.y_coord - toggle_locations[i].y_coord, pos.x_coord - toggle_locations[i].x_coord));
  // Drivetrain.setTurnVelocity(40, percent);
  // Drivetrain.turnFor(atan2(2 , 0.75) * 180 / M_PI, degrees);
  // Drivetrain.setDriveVelocity(60, percent);
  // Drivetrain.setTurnVelocity(40, percent);
  // Drivetrain.driveFor(sqrt(5) * 24 , inches);
  // turn(Drivetrain, DrivetrainInertial, 180-atan2(2 , 0.75));
  // auto a = geo::point(2,0);
  // hit_Toggle(Drivetrain, DrivetrainInertial, a, false,bl);
  // Drivetrain.setTurnVelocity(40, percent);
  // Drivetrain.turnFor(-45, degrees);
  // Drivetrain.setDriveVelocity(60, percent);
  // Drivetrain.driveFor(sqrt(5) * 24 , inches);
  // Drivetrain.setTurnVelocity(40, percent); 
  // Drivetrain.turnFor(45, degrees);
  // a = geo::point(0,2);
  // hit_Toggle(Drivetrain, DrivetrainInertial, a, false,bl);

// ================================================================================================================================================================================================

  // go_dir(Drivetrain, DrivetrainInertial, pos, pai, angle_heading);
  // turn(Drivetrain, DrivetrainInertial, ((i == 0) ? 180 : -90) - angle_heading);
  // angle_heading = (i == 0) ? 180 : -90;
  // hit_Toggle(Drivetrain, DrivetrainInertial,toggle_locations[i], (i == 0), bl);
  // i++;
  // pai = std::pair<double,double >(geo::__dist(pos, toggle_locations[i]), atan2(pos.y_coord - toggle_locations[i].y_coord, pos.x_coord - toggle_locations[i].x_coord));
  // go_dir(Drivetrain, DrivetrainInertial, pos, pai, angle_heading);
  // turn(Drivetrain, DrivetrainInertial, ((i == 0) ? 180 : -90) - angle_heading);
  // angle_heading = (i == 0) ? 180 : -90;
  // hit_Toggle(Drivetrain, DrivetrainInertial,toggle_locations[i], (i == 0), bl);
  
    // if (!at_toggle){
    //   auto current_coordinates = get_current_coordinates();
    //   auto current_orientation = get_current_heading();
    //   auto toggle_location = toggle_locations[toggle];
      
    //   double distance = geo::__dist(current_coordinates, toggle_location);
    //   if (distance <= epsilon){
    //     //rotate to the final orientation with the rotation hook facing the toggle
    //     at_toggle = true;
    //   } else {
    //     auto rotation_angle = get_rotation_angle(current_orientation, current_coordinates, toggle_location);
    //     //set velosity max
    //     //rotate to (rotation_angle)
    //     wait(1, msec);
    //   }

    // } else {
    //   auto current_orientation = get_current_heading();
    //   //rotate to point the toggler towards the toggle
    //   //Go back rapidly
    //   //Come forward rapidly
    //   toggle = toggle+1;
    //   at_toggle = false;


    // }


  
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/



void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
   

  ////////////////////////////////////////////////////////////////////////////////

    
  double x = Controller.Axis1.position(percent);
  double y = Controller.Axis3.position(percent);

  LeftDriveSmart.spin(fwd, to_volt(y + x), volt);
  RightDriveSmart.spin(fwd, to_volt(y - x), volt);


  //////////////////////////////////////////////////////////



    // Use the mechanism to move the claw up and down
    if (Controller.ButtonL1.pressing()) {
      Updownmotor.spin(forward, 100, velocityUnits::pct);
    }
    else if(Controller.ButtonL2.pressing()){
      Updownmotor.spin(reverse, 100, velocityUnits::pct);
    }
    else if(Controller.ButtonL2.pressing() && Controller.ButtonL1.pressing()){
      // 67
      Updownmotor.stop();
      Toggle_left.spin(forward, 100, velocityUnits::pct);
    }
    else{
      Updownmotor.stop();
    }
  ///////////////////////////////////////////////////////////////////////////////////////////  



  if (Controller.ButtonR1.pressing()) {
    Claw.open();
  }
  else if (Controller.ButtonR2.pressing()) {
    // Manual reverse
    Claw.close();  
  }
  else if(Controller.ButtonR1.pressing() && Controller.ButtonR2.pressing()){
      // 67
      Toggle_right.spin(forward, 100, velocityUnits::pct);    
  }
  wait(20, msec);
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}