# GarageHackSimulation
A simulation of a garage door hack using the Basys MX3 trainer board

This project has multiple features to indicate the simulation of a garage door hack

The LCD contains information about the code. It displays the current mode of the simulation (Set, Attack, Opened),
the amount of bits of the code, the correct code in decimal form, the amount of bits sent in order to hack the code.

Buttons are used to modify the correct garage "code" and begin the attack. Up/Down change the 
number of bits the code is (between 6 and 16 inclusive). Right/Left increment and decrement the code. Note that after
a successful hack the program produces a random number as the next code. The center button will begin the attack.

There are a few ways the program can attack. Switches determine the attack.
  SW0 - Linear Feedback Shift Register
  SW1 - deBruijn Sequence
  SW2 - Random
  SW3 - Binary Search (impractical strategy for attacking garages)
  ELSE- Brute Force
  
Additional Features
  SSD keeps track of the time is takes to hack in hex
