# Snap</br>
**Function**: Sorting yellow leaves from the vegetables</br>
**Description**: The system is based on machine vision and robotic control technology for sorting vegetables and 
yellow leaves. First, the camera completes the acquisition of the image data of the obtained RGB color flow image 
segmentation and image processing to obtain the binarization contour vegetables and average color histogram, 
automatic screening algorithm requires sorting through yellow leaves; secondly, for the eligible image depth data stream
is analyzed, considering the pipeline run error, conversion to give the robot to crawl vegetables center; 
finally, PC end PC to calculate the corresponding steering angle control, and finally converted into control 
commands from the calculated value based on the ARM controller servo rotation, to achieve the target of culling. 
Experimental results show that the system can accurately and effectively to achieve recognition of the pipeline yellow leaves of vegetables,
Orientation and reject sorting.</br>
**Video demo**:
This is a [demo](https://youtu.be/lTUlrcDPlNk).
