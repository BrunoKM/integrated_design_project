# Software Requirements
#### Move vehicle between Starting Point, Collection Point, Drop-off Zone and the Recycling Zone:
* Line following
* Map orientation
    *	Line re-alignment after losing contact when going between D1 or D2 and C2.
*	Fail-safety mechanism
####	Read Basket Instructions
*	Position vehicle in front of IR signal transmitter
*	Interpret the signal
####	Collect eggs from the Collection Point C2:
*	Position vehicle in front of turntable
    * Slowly move vehicle forward, until positive reading from the actuation sensor is received.
*	Communicate the desired turntable position using the IR diode.
*	Sort the egg internally:
    *	Classify the egg colour based on IR sensor reading.
    *	Use information about the position of the turntable to determine the egg size
    *	Move actuator accordingly
*	Rinse and repeat the above process
    *	Have flexibility in terms of changing how many extra eggs to pick up for recycling points 
####	Indicate the eggs present in the vehicle with on-board LEDs:
*	Store the data on the eggs present in the vehicle in an internal data structure.
*	Light up LEDs corresponding to the composition.
*	Every time eggs added or removed, refresh the LEDs
####	Drop off eggs at Drop-off Zone
*	Position vehicle over desired basket:
    *	Position vehicle in front of drop zone
    *	Rotate by a desired calibrated angle to position the dispenser over desired basket
####	Drop off eggs at the Recycling Zone
####	Point scoring strategy
*	Will have to be decided once the timing and the consistency thereof of each action is known.
