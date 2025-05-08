# joy_move_oled
convert joystick movements to motor speed driver and show to oled

<h1>Required</h1>

Wemos Lolin32 OLED
<ul>
<li>L298N motor driver</li>
<li>Joystick analogic 2 axis (JH-D202X-R2_R4-Joystick-5K)</li>
<li>Single button circuit</li>
<li>2 dc motors</li>
<li>12 volts battery</li>
</ul>

<h1>Joy_Move_Oled Project is used to</h1>

<ul>
<li>read the analog data from the joystick</li>
<li>map it to a speed range for the L298N</li>
<li>write the data to the OLED display</li>
<li>and simultaneously move the motors according to the displayed values</li>
</ul>

I also implemented the joystick calibration mechanism.

<h1>It show on oled display</h1>
<ul>
  <li>A:&lt;speed-motor-a&gt;</li>
  <li>B:&lt;speed-motor-B&gt;</li>
  <li>Direction X:&lt;LEFT/RIGHT&gt;</li>
  <li>Direction Y:&lt;UP/DOWN&gt;</li>
</ul>


![esp32__joystick_command_2_motor__schema](https://github.com/user-attachments/assets/127a4f9f-e368-4243-b4a4-f2f379810d47)
