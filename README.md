# Robot Music Additive Synthesizer

This program designs an audio application which performs basic additive synthesis using sine waves.
The user can control the number of sine waves he/she wants to synthesize together (maximum 5 sine waves)

The application gives the user 2 modes :

1. Manual Mode (By default): The user has the following controls :

	a) User can control the frequency of each sine wave using a frequency slider (Range: 50Hz-8000Hz)

	b) User can control the gain using the gain slider ( Gain slider is common for all sine waves, but gain provided to each sine wave is 
	a different mathematical function. 

	c) User can control which sine wave to switch on/off for the synthesis by controlling the On/Off toggle buttons.


2. Robot Music Mode: Depending on the number of sine waves selected in the beginning , the user gets those many robots to sing.
   It can be solo, duet or a choir performance performed by the robots depending on the on/off toggle buttons states. 

   You can control this robot singing group by the following methods:
	a) By moving the mouse to different locations on the app: A new frequency is generated for each robot for each new mouse location.
		The program maps the X and Y location of the mouse on the screen to a frequency function and the robot/robots sing together as you 
		move your mouse.
    
	b) If you have a touch screen : You can simply tap at different locations of the application and this will generate a discrete effect
		as frequencies change suddenly in this case as compared to a mouse moving event( where the movement is more gradual)
    
Smoothing Add-on!!! : In the robot music mode you have an option to smooth the changes in frequency through a smoothing button and a button to go back from a smoothing mode to a de-smoothing mode. Both modes produce different audio outputs so enjoy playing with it!!

Enjoy Synthesizing!!!!
