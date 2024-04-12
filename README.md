# TeamProject - Team 45

Before running the code please install the QT Charts module by running `sudo apt install libqt5charts5-dev` in the terminal

The sessionLog.txt file for exporting session logs is being saved to the Build folder, for example `build-Neureset-Desktop-Debug/`

Documentation is in the "Documentation" folder and best viewed with the "Documentation.md" file either on GitHub or Vs Code.

Video going through the sequence diagrams: https://www.youtube.com/watch?v=nU5daDI3iPI 

## Team Members and who did what
### Knox Kan 101260592

* Use cases
* Sequence Diagram
* Timer implementation
* Traceability matrix
* Documentation
* Video shooting

### Emrehan Sagin 101223876

* Use cases
* State Diagrams
* Battery implementation
* Traceability matrix
* Documentation

### Sara Francis 101234656

* UML Class diagram
* GUI design
* Treatment logic
* Graphical representation of EEG waveforms
* Traceability Matrix

### LeeAnne King

* UML Class diagram
* GUI design
* Traceability Matrix
* Connection loss between electrodes and the device - 1
* Logging of treatment sessions - 2
* Green, red and blue light for device status - 3
* Documentation File
* Design Decisions for 1, 2 and 3
* Design Patterns

### Burak Yildirim 101233896

* UML Class diagram
* Traceability Matrix
* Collection EEG waveforms
* Calculation of Baseline
* Sequence Diagram
* Created the Neureset Demo Video
* Wrote the desing decisions of Calculating Baseline and Band Values

## Files

Documentation is in the "Documentation" folder and best viewed with the "Documentation.md" file either on GitHub or Vs Code.

### Header Files

Battery.h  
EEGSite.h  
NeuresetController.h  
defs.h  
mainwindow.h

### cpp files

Battery.cpp  
EEGSite.cpp  
NeuresetController.cpp  
main.cpp  
mainwindow.cpp  

### Other Files

mainwindow.ui  
readMe.md  
Neureset.pro.user  
Neureset.pro  
