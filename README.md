# Password_Protected_Lock_Project

## Overview
This project implements a **password-protected electronic lock** using a **PIC18F4620 microcontroller**. The system provides a simple and secure method for locking and unlocking a door. The system includes the following components:
- **Keypad** for password entry.
- **LCD Screen** to display feedback (whether the password is correct or wrong).
- **Motor** that simulates door unlocking.
- **Buzzer** to alert when incorrect passwords are entered too many times.
- **Timers** are used to manage precise time delays for lockout periods.

## Features
1. **Password Authentication:**
   - The user enters the password via a keypad.
   - The system compares the input against a predefined password.
   - If the password is correct, the motor unlocks the door.
   - If the password is incorrect, an error message is displayed on the LCD.

2. **Incorrect Password Handling:**
   - If the password is entered incorrectly **3 times**, the buzzer will sound to alert potential unauthorized access.
   - After 3 incorrect attempts, the system locks the user out for **30 seconds**.
   - If the password is entered incorrectly again after the 30-second delay, the lockout time increases to **5 minutes**.
   - The lockout time continues to increase after repeated failed attempts, up to a maximum of **1 hour**.

3. **Timer Modules for Time Delays:**
   - **Timer modules** (such as **Timer0** or **Timer1**) are used to implement precise time delays for the lockout periods.
   - After 3 failed password attempts, a timer manages the **30-second delay** before the user can try again.
   - With repeated incorrect attempts, the timers manage increasing delays (5 minutes, 15 minutes, 30 minutes, 1 hour) to ensure accurate timing for security.

4. **LCD Display:**
   - Displays messages such as "Enter Password," "Correct Password," and "Wrong Password."
   - Provides feedback on the lockout status and time remaining before the user can attempt again.

5. **Motor Control:**
   - The motor represents the physical lock, and it will engage or disengage based on whether the correct password is entered.

6. **Buzzer Alert:**
   - A buzzer will sound when there are repeated incorrect password attempts, signaling a potential security breach.

## Components
- **PIC18F4620 Microcontroller**
- **4x4 Matrix Keypad** for entering the password.
- **20x4 LCD Display** to show status messages.
- **DC Motor or Servo Motor** to simulate the door lock mechanism.
- **Buzzer** to alert when the incorrect password is entered multiple times.
- **Timers (Timer0/Timer1)** to handle delay functions.

## How It Works
1. **Start the System:**
   - The LCD prompts the user to enter the password.
   
2. **Password Input:**
   - The user enters a password using the keypad. The system compares it to the predefined password stored in the microcontroller’s memory.
   
3. **Correct Password:**
   - If the password is correct, the motor engages, unlocking the door, and the LCD displays a success message.
   
4. **Incorrect Password:**
   - If the password is incorrect, an error message is shown on the LCD. After **3 incorrect attempts**, the buzzer sounds, and the system locks out the user for **30 seconds** (handled by the timer module).
   - If incorrect attempts continue, the lockout delay (managed by timers) increases to **5 minutes**, and eventually up to **1 hour**.

## Usage Instructions
1. Power up the system and wait for the LCD to prompt you to enter the password.
2. Use the keypad to input the password.
3. If correct, the motor will unlock the door.
4. If incorrect, the system will alert you after 3 failed attempts with a buzzer and initiate the delay lockout.
5. After the lockout period, you may try again.

## License
This project is open-source and free to use 

## Project Schematic
![image](https://github.com/user-attachments/assets/c24fa4a8-b4b8-4dc1-ace5-7d921f4e201b)


