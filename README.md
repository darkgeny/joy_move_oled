# joy_move_oled
convert joystick movements to motor speed driver and show to oled

<h1>Required</h1>

Wemos Lolin32 OLED
L298N motor driver
Joystick analogic 2 axis (JH-D202X-R2_R4-Joystick-5K)
Single button circuit
2 dc motors
12 volts battery

<h1>Joy_Move_Oled Project is used to</h1>

-read the analog data from the joystick
-map it to a speed range for the L298N
-write the data to the OLED display
-and simultaneously move the motors according to the displayed values

I also implemented the joystick calibration mechanism.

![esp32__joystick_command_2_motor__schema](https://github.com/user-attachments/assets/127a4f9f-e368-4243-b4a4-f2f379810d47)
