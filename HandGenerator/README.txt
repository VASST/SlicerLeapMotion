|-----------------------------------------------|
| Leap Motion Hand Generator module for Slicer  |
|-----------------------------------------------|
| Developed by Leah Groves and Thomas Morphew   |
| Robarts Research Institute, VASST             |
|-----------------------------------------------|

What you need to run this module:
1. 3D Slicer version 4. This works with builds from November 2019 and January 2020.
2. PlusToolkit PlusServer. I don't know exactly which version was used but as long as it supports the Leap Motion sensor it should work.
        In the case that it doesn't work, it might be too old a version to read in the data about finger lengths and widths, you can edit 
        the scripted python module for Slicer on the lines that throw errors.
3. OpenIGTLinkIF module/extension in Slicer. This can be installed through the 3D Slicer extension manager.


Steps to run the module:
1. Add the "PlusDeviceSet_Server_LeapMotionTracker Right/Left/Both.xml" files that are part of this module to wherever you want them to be 
        stored for the PlusServer to access them.
2. Start the PlusServer with the PlusServerLauncher.exe executable file.
3. Select the directory that you stored the .xml files in step 1 where it says "Device set configuration directory:"
4. Select the file that you wish to run for the configuration under "Device set:". These should have the name
        "PlusServer: LeapMotion Right/Left/Both Hand/Hands". 
5. Make sure the Leap Motion sensor device is connected to your computer. This is done with a USB cable.
6. Click the "Launch Server" button. Make sure that it boots up correctly.
7. Start 3D Slicer
8. Make sure the module is added via "Application Settings" under the "Edit" tab. Instruction can be found on the Slicer discourse wiki.
9. Select the "LeapMotionHandGenerator" module, it should be located in the "Examples" category unless the file has been edited.

*WORKING IN THE MODULE*

10. Once all of the above steps are completed, press the "Click to connect" button under the "Parameters" section. This will connect
        the PlusServer to 3D Slicer via the OpenIGTLinkIF module
11. Place your hand(s) within view of the Leap Motion sensor and then press the "Generate Hands" button.
12. Have fun.



***KNOWN BUGS***
At the time of making this README file, I (Thomas) know of one fatal bug in the program.
1. DO NOT press the "Generate Hands" button twice. If you do the module fails and no hands will be displayed. This can be fixed by making sure
    the version of PlusServer can properly track the finger length and width data or by removing that feature from the python module.

2. If you press the "Generate Hands" button with no hands in view of the sensor no hand will be generated, even if you put the hands in 
    view after pressing the button. To generate the hands in this case you should restart Slicer, proceed from Step 10 in the "Steps to run 
    the module:" section, and make sure your hand is within view of the Leap Motion sensor.

Note: Hopefully these bugs won't be an issue in the future.

