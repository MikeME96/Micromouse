# Micromouse

## Micromouse Maze-Solving Robot
### Overview

This project is an autonomous Micromouse robot designed to navigate and solve a maze efficiently. The robot was built as part of my Mechanical Engineering degree final project, focusing on robot control, real-time motion systems, and embedded development.
### Project Highlights

* Autonomous maze-solving using the Flood Fill algorithm
* Real-time motion control with a cascade PID controller
* IR sensor-based wall detection for precise navigation
* Quadrature Encoder Interface (QEI) for position tracking
* Embedded system programming on a dsPIC33FJ64GS608 microcontroller
* Wireless debugging and control via UART & Bluetooth (HC-05 module)

### System Components
|Component|	Description|
|:-|:-|
|Microcontroller|	dsPIC33FJ64GS608|
|Motors|	N20 6V 500RPM DC Motors (2x)|
|Sensors|	Sharp GP2Y0A51SK0F IR Sensors (3x)|
|Motor Driver|	TLE9201SG H-Bridge (2x)|
|Communication|	HC-05 Bluetooth UART Module|
|Power|	Rechargeable battery pack|
|Chassis|	3D-printed custom frame|

### Maze-Solving Algorithm

The Flood Fill algorithm assigns values to each cell based on Manhattan distance from the goal. The robot continuously updates the maze map, recalculating the shortest path dynamically.

#### Steps:

  1. Assume no walls and plan the shortest path to the goal.
  2. Navigate while detecting walls with IR sensors.
  3. Update the maze map in real-time and re-calculate the best route.
  4. After mapping, follow the optimized shortest path at higher speed.

### Control System

#### The cascade PID control structure consists of:

  * Outer loop: Position control (PI controller)
  * Inner loop: Velocity control (PID controller)

#### PID tuning process:

  First, the position controller was tuned for minimal steady-state error.
  Next, the velocity controller was tuned for smooth acceleration and stability.

### Movement & Navigation

  * Moving forward: Uses QEI encoders to track distance.
  * Turning: Pre-calculated encoder counts ensure precise 90° and 180° rotations.
  * Wall following & centering: IR sensors adjust speed to maintain correct positioning.
  * Stopping accuracy: The robot slows down near walls to minimize error.

### Challenges & Limitations

#### ✅ Successes:

  * Successfully mapped and solved a 4x4 maze under different configurations.
  * Found the shortest path using real-time updates.

#### ⚠️ Limitations:

  * Requires at least one wall per cell for precise turning.
  * Bluetooth is used to start the robot, which may not meet full competition rules.
  * Performance could improve with gyroscope-based alignment and a lighter LiPo battery.

### Future Improvements

  * Full compliance with competition rules by adding onboard start buttons.
  * PCB redesign for improved space efficiency and direct motor integration.
  * More advanced control strategies to handle turns without relying on walls.

### Project Media  
📹 **First successful run:** [Watch on YouTube](https://www.youtube.com/shorts/OQ9yuvwSb1w)  
📜 **Full project report:** [https://github.com/MikeME96/Micromouse/blob/main/Docs/Project_FInal.pdf]  

