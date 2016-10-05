/* This program designs an audio application which performs basic additive synthesis using sine waves.
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

Enjoy!!!!

*/


#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Sine.h"
#include<iostream>
#include<string>
#include<stdlib.h>
#include "Smooth.h"
#include<random>
using namespace std;

class MainContentComponent :
	public AudioAppComponent,
	private Slider::Listener, // Listens for the sliding of the frequency and gain sliders
	private ToggleButton::Listener, // Listens for the togglebutton clicks 
	private TextEditor::Listener// Listens for changes in the value of the text editor 
//	private MouseListener  // Listens for events related to mouse movements

	
   
    
{
public:
    MainContentComponent() : gain (0.0), samplingRate(0.0), no_of_sine_waves(0)
    {
		
		initialize_GUI_elements();

        setSize(600, 400); // Sets the initial size of the application 
		nChans = 1;
		setAudioChannels(0, nChans); // no inputs, one output
        // configuring the frequency slider and adding to the main window
           
        
       
    }
    
    ~MainContentComponent()
    {
        shutdownAudio();
    }


	
   // Initializes all the GUI elements for the audio application.
    void initialize_GUI_elements() {
        
        // Text Editor that takes in the number of sine waves from the user
		addAndMakeVisible(input_no_of_sine_waves);
        input_no_of_sine_waves.setText(to_string(max)); // Default value of the number of sine waves is set to the max value
        input_no_of_sine_waves.addListener (this); // Adds the listener function for the specific component ( in this case text editor)
        
        addAndMakeVisible(how_many_sine_waves);
        how_many_sine_waves.setText ("Enter Number of Sine_Waves( Max 5!)", dontSendNotification);
        how_many_sine_waves.attachToComponent(&input_no_of_sine_waves, true);
        
		addAndMakeVisible(confirm_button);
		confirm_button.setButtonText("Confirm");
		confirm_button.addListener(this); 

		no_of_sine_waves= input_no_of_sine_waves.getText().getIntValue(); // Reads number of sine waves from text editor 

		initialize_components_and_variables();

    }
    

	/* 
		This function gets called whenever the mouse moves inside the application.
		It is used in the Robot Music Mode to sense the (X,Y) of the mouse location.
	*/
	void mouseMove(const MouseEvent& event) override {

		if (music_mode==true) {

			for (int i = 0; i < no_of_sine_waves; i++) {

				on_off_label_A[i].setText("Robot " + to_string((i + 1)), dontSendNotification);

			}

			// Sets the frequency for each sine wave/robot according to a mapping function. 
			// An even robot/sine wave gets a separate mapping as compared to an odd robot.
			for (int i = 0; i < no_of_sine_waves; i++) {


				if (i % 2 == 0) {
					frequency_slider_A[i].setVisible(false);
					frequency_slider_A[i].setValue(event.getScreenX() - event.getScreenY() + 261.63*(i+1));
				}

				else {
					frequency_slider_A[i].setVisible(false);
					frequency_slider_A[i].setValue(event.getScreenX() + event.getScreenY()+587.33*(i+1));
				}

			}
		}
	}



	/*
		This function intializes all the components whose size depends on the number of sine waves entered by the  user
		It employs dynamic memory allocation methods. The method waits for the user to enter the number of sine waves and then
		initializes the components.
	*/

    void initialize_components_and_variables() {
		e.wait(7000);
		sine_A = new Sine[no_of_sine_waves];
		frequency_slider_A = new Slider[no_of_sine_waves];
		frequency_label_A = new Label[no_of_sine_waves];
		on_off_label_A = new Label[no_of_sine_waves];
		on_off_button_A = new ToggleButton[no_of_sine_waves];
		on_off_A = new int[no_of_sine_waves];
		smoothing = new Smooth[no_of_sine_waves];
		carrier_freq = new double[no_of_sine_waves];
		
		// Initializes all the On/Off toggle buttons to a zero state(Off)
		for (int i = 0; i < no_of_sine_waves;i++) {

			on_off_A[i] = 0;
		} 
		
		for(int i =0;i<no_of_sine_waves;i++){
		    
		    carrier_freq[i] = 440.0;
		}

        
    }


// Adds the frequency slider, frequency labels, on/off toggle buttons and on/off labels, robot music mode button on the application window
    void frequency_and_on_off_components() {
        
        for( int i = 0; i<no_of_sine_waves ; i++){
            
            addAndMakeVisible(frequency_slider_A[i]);
            frequency_slider_A[i].setRange(50.0,8000.0); // Frequency range (50Hz - 8000 Hz)
            frequency_slider_A[i].setSkewFactorFromMidPoint (500.0);
            frequency_slider_A[i].setValue(261.626*(i+1)); // will also set the default frequency of the sine osc
            frequency_slider_A[i].addListener (this);
            
            
            // configuring frequency label box and adding it to the main window
            addAndMakeVisible(frequency_label_A[i]);
            frequency_label_A[i].setText ("Frequency", dontSendNotification);
            frequency_label_A[i].attachToComponent (&frequency_slider_A[i], true);
            
            
             // configuring on/off button and adding it to the main window
            addAndMakeVisible(on_off_button_A[i]);
            on_off_button_A[i].addListener(this);
            
            
            // configuring on/off label and adding it to the main window
            addAndMakeVisible(on_off_label_A[i]);
            on_off_label_A[i].setText ("On/Off", dontSendNotification);
            on_off_label_A[i].attachToComponent (&on_off_button_A[i], true);
    
        }

		addAndMakeVisible(robot_music_mode);
		robot_music_mode.addListener(this);

    }
       

    // Adds all the visual components for the application on the application window    
    void add_components() {
       
        frequency_and_on_off_components();
        gain_slider_and_labels();
        
    }


    // Adds the gain slider and the gain label on the application window.
    void gain_slider_and_labels() {
        
        // configuring gain slider and adding it to the main window
        
        addAndMakeVisible (gain_slider);
        gain_slider.setRange (0.0, 1.0);
        gain_slider.setValue(0.5); // will alsi set the default gain of the sine osc
        gain_slider.addListener (this);
        
        
        // configuring gain label and adding it to the main window
        addAndMakeVisible(gain_label);
        gain_label.setText ("Gain", dontSendNotification);
        gain_label.attachToComponent (&gain_slider, true);
    }
    

	// This function gets called everytime the application window is resized by the user. 
    void resized() override
    {   
        
        
        // placing the UI elements in the main window
        // getWidth has to be used in case the window is resized by the user
        const int sliderLeft = 80;
        
        no_of_waves.setBounds(sliderLeft, 20, getWidth() - sliderLeft - 20, 20);
        confirm_button.setBounds(300,8, getWidth() - sliderLeft - 300, 20);
		robot_music_mode.setBounds(400, 25, 180,30);
        
		smoothing_button.setBounds(400,25,180,30);
		de_smooth.setBounds(400, 25, 180, 30);
		frequency_and_on_off_bounds(sliderLeft);
        
        gain_slider.setBounds (sliderLeft, 370 , getWidth() - sliderLeft - 20, 20);
        input_no_of_sine_waves.setBounds(3*sliderLeft,10,30,20);
    
    }
    

	// Sets the physical bounds of the frequency slider and label, the on/off button and labels in the application window

	void frequency_and_on_off_bounds(int sliderLeft) {

		for (int i = 0; i < no_of_sine_waves; i++) {

			frequency_slider_A[i].setBounds(sliderLeft, 60 + 65 * i, getWidth() - sliderLeft - 20, 20);
			on_off_button_A[i].setBounds(sliderLeft + 100 * i, 4, 20, 20);

		}

	}

	// Gets called whenever any of the sliders in the application is moved

    void sliderValueChanged (Slider* slider) override {
        
        if (samplingRate > 0.0){
        
        for(int i = 0 ; i < no_of_sine_waves ; i++) {
         
            // Checks if the slider is any of the frequeny sliders
            if (slider == &frequency_slider_A[i]) {
                    
               //   sine_A[i].setFrequency(frequency_slider_A[i].getValue());
				carrier_freq[i] = frequency_slider_A[i].getValue();
				smoothing[i].setSmooth(0.99994);
            } 
            
        }    
         

		// Checks if slider is the gain slider
         if(slider == &gain_slider) {
             
             gain = gain_slider.getValue();
             
         }
           
      }
       
   }
    
    // This function gets called when any button is pressed on the User Interface 
    
    void buttonClicked (Button* button) override
    {
        // turns audio on or off
        
        for(int i = 0 ; i < no_of_sine_waves ; i++) { 
            
            // Checks if the button pressed is any of the on/off toggle buttons( which control the on/off state of the sine waves )
			if (button == &on_off_button_A[i]) {


				if (on_off_button_A[i].getToggleState())
				    

					on_off_A[i]=1;

				else

					on_off_A[i]=0;
			}
        
            }   
            
        
		// Checks of the button clicked is the confirm buton
		if (button == &confirm_button) {

		   no_of_sine_waves = input_no_of_sine_waves.getText().getIntValue(); // Picks up the value from the text editor
		   
		   // Checks if the number of sine waves entered is more than the max value permitted.
		   if (no_of_sine_waves > max) {
			   no_of_sine_waves = 5;
			   input_no_of_sine_waves.setText("5");
		   }

		   // Signals the thread that is waiting to not wait anymore
		    e.signal();
			add_components(); 

			confirm_button.setVisible(false); // Makes the confirm button invisible
			confirm_button.setToggleState(false,dontSendNotification);
			input_no_of_sine_waves.setVisible(false); 
			how_many_sine_waves.setVisible(false);

			addAndMakeVisible(robot_music_mode); //Adds the robot music mode button on the application window
			robot_music_mode.setButtonText("Robot Music Mode");
			robot_music_mode.addListener(this);

		} 

			// Checks if the button clicked is the robot music mode button
		if (button == &robot_music_mode) {
			
			music_mode = true;
			addAndMakeVisible(smoothing_button);
			smoothing_button.setButtonText("Smooth Frequencies");
			smoothing_button.addListener(this);
			robot_music_mode.setVisible(false); // Makes the button that controls the robot music mode invisible 
			de_smooth.setVisible(false);
			

		}
		
		if(button == &smoothing_button) {
		    
		    smooth_mode = true;
			smoothing_button.setVisible(false); // Makes the smoothing button invisible 
			addAndMakeVisible(de_smooth);
			de_smooth.addListener(this); 
			de_smooth.setButtonText("De-Smooth");
			
		    
		}

		if (button == &de_smooth) {

			smooth_mode = false;
			de_smooth.setVisible(false);
			smoothing_button.setVisible(true);
		}
		
         
    }


    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override{
        samplingRate = sampleRate;
        
        for( int i = 0 ; i <no_of_sine_waves ; i++) { 
          
            sine_A[i].setSamplingRate(sampleRate);
            
        }
    }
    

	// Deletes all the components to clear the memory
    void releaseResources() override {
        
		delete[] sine_A;
		delete[] frequency_slider_A;
		delete[] frequency_label_A;
		delete[] on_off_A;
		delete[] on_off_button_A;
		delete[] on_off_label_A;
        
    }    


    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // getting the audio output buffer to be filled
        float* const buffer = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
        
        float temp_buffer = 0.0;
  
        // computing one block
        for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            
            for(int i = 0 ; i < no_of_sine_waves ; i++) {
                
                if(smooth_mode == true) {
				    //smoothing[i].setSmooth(0.9999);
				    sine_A[i].setFrequency(smoothing[i].tick(carrier_freq[i]));
                    // temp buffer which stores the additive synthesis output, which is later assigned to the actual buffer for playing
				    temp_buffer = temp_buffer + sine_A[i].tick()*gain/(i + 1)*on_off_A[i]; //random.nextFloat() * 0.25f*gain/(i + 1) - 0.125f;
               }
               
                else {
                    
                    sine_A[i].setFrequency(carrier_freq[i]);
                    // temp buffer which stores the additive synthesis output, which is later assigned to the actual buffer for playing
				    temp_buffer = temp_buffer + sine_A[i].tick()*gain/(i + 1)*on_off_A[i]; //random.nextFloat() * 0.25f*gain/(i + 1) - 0.125f;
                    
                    
                }
                
            }
            
            
            buffer[sample] = temp_buffer;
            temp_buffer = 0.0;
            
       }
    }
    
    
private:
    // UI Elements

    Slider*frequency_slider_A; // frequency sliders  (control the frequency of sine waves)
	Slider gain_slider,no_of_waves; // gain sliders ( control the gain of the sine waves)
    

    ToggleButton*on_off_button_A; // on/off toggle buttons
    TextButton robot_music_mode; // button to enter the robot music mode
	TextButton confirm_button; // button that is pressed to confirm the number of sine waves
   // Button confirm_button;
   
    TextButton smoothing_button; 
	TextButton de_smooth;

    Label*frequency_label_A,*on_off_label_A; // frequency and on/off labels
    Label how_many_sine_waves,gain_label,confirm_label;
    
    TextEditor input_no_of_sine_waves; // textbox that takes the number of sine_waves as input

    Sine*sine_A; // the sine wave oscillator
	Smooth*smoothing; // Object of Smooth type used to smooth the the process of sudden frequency changes
	double*carrier_freq;
	WaitableEvent e; // Used to make a thread wait while the other is performing tasks
    
	
	Random random;
    // Global Variables
    float gain; // gain for the sine waves
	int*on_off_A; // toggle states for switching on and off the sine waves
 
	int samplingRate, nChans;
	int	no_of_sine_waves; // Variable to store the number of sine waves the user wants to synthesize
	
	
    const int max = 5; // Max number of sine waves for additive synthesis
	bool music_mode = false; // bool that control the on/off the robot music mode
	bool smooth_mode = false; // bool that control the smoothing of frequencies

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
