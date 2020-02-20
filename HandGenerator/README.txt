|-----------------------------------------------|
| Leap Motion Hand Generator module for Slicer  |
|-----------------------------------------------|
| Developed by Leah Groves and Thomas Morphew   |
| Robarts Research Institute, VASST             |
|-----------------------------------------------|

What you need to run this module:
1. 3D Slicer version 4. This works with builds from November 2019 and January 2020.
2. PlusToolkit PlusServer. I don't know exactly which version was used but as long as it supports the Leap Motion sensor it should work.
3. OpenIGTLinkIF module/extension in Slicer. This can be installed through the 3D Slicer extension manager.
4. A Leap Motion Sensor from https://www.ultraleap.com/product/leap-motion-controller/
5. The Leap Motion SDK kit may be necessary. If so, install the (Orion) V4 SDK.
Optional: The "SlicerVirtualReality" module can be used with this module and a head mounted display (virtual reality setup) to see your hand
        in the virtual reality space where it should appear. This can be installed through the 3D Slicer extension manager.

Steps to run the module:
1. Add the "PlusDeviceSet_Server_LeapMotionTracker Right/Left/Both.xml" files that are part of this module to wherever you want them to be 
        stored for the PlusServer to access them.
2. Start the PlusServer with the PlusServerLauncher.exe executable file.
3. Select the directory that you stored the .xml files in step 1 where it says "Device set configuration directory:"
4. Select the file that you wish to run for the configuration under "Device set:". These should have the name
        "PlusServer: LeapMotion Right/Left/Both Hand/Hands". 
5. Make sure the Leap Motion sensor device is connected to your computer (or head mounted display). This is done with a USB cable.
6. Click the "Launch Server" button. Make sure that it boots up correctly, this should be displayed in centre message box.
7. Start 3D Slicer
8. Make sure the module is added via "Application Settings" under the "Edit" tab. Instruction can be found on the Slicer discourse wiki.
9. Select the "Hand Generator" module, it should be located in the "IGT" category.

*WORKING IN THE MODULE*

10. Once all of the above steps are completed, press the "Click to connect" button under the "Parameters" section. This will connect
        the PlusServer to 3D Slicer via the OpenIGTLinkIF module
11. Place your hand(s) within view of the Leap Motion sensor and then press the "Generate Hands" button.
12. Have fun.

*OPTIONAL VIRTUAL REALITY FUNCTIONALITY*
1. Once the hands are rendered (by following the steps above), go to the Virtual Reality module and check the toggle box for the HMD transform.
2. Navigate to the "Data" module and select "Transform hierarchy".
3. Move the "LHG_TrackerToHMD" so that the "VR.HMD" transform is the parent of the "LHG_TrackerToHMD" transform


***KNOWN BUGS***
Sometimes the Leap Motion sensor will not generate the hands despite them being in view. If this happens you can take your hands out of view 
of the Leap Motion sensor for a few seconds and place them back and then press the "Generate ..." again. This problem is due to Leap Motion 
failing to pick up transformational data, moving your hands slightly while in view of the sensor and attempting to regenerate them tends to
resolve this issue.