# Neureset Device Control System
By LeeAnne King, Sara Francis, Knox Kan, Emrehan Sagin, Burak Yildirim

Neureset Device Control System is a project primarily developed in C++ that provides software to control and interact with Neureset devices. The system is designed to manage device operations, facilitate communication between hardware and software components, and ensure precise, reliable device control. The project uses QMake for build configuration and includes a small portion of C code, indicating potential integration with lower-level hardware interfaces or legacy systems. 

## Team Members
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

### Sara Francis

* UML Class diagram
* GUI design
* Treatment logic
* Graphical representation of EEG waveforms
* Traceability Matrix

### Knox Kan

* Use cases
* Sequence Diagram
* Timer implementation
* Progress bar implementation
* Traceability matrix
* Documentation
* Video shooting

### Emrehan Sagin

* Use cases
* State Diagrams
* Battery implementation
* Traceability matrix
* Documentation

### Burak Yildirim

* UML Class diagram
* Traceability Matrix
* Collection EEG waveforms
* Calculation of Baseline
* Sequence Diagram
* Created the Neureset Demo Video
* Write the design decisions for Calculating Baseline and Band Values.

# Instuctions
Before running the code please install the QT Charts module by running `sudo apt install libqt5charts5-dev` in the terminal

The sessionLog.txt file for exporting session logs is being saved to the Build folder, for example `build-Neureset-Desktop-Debug/`

Documentation is in the "Documentation" folder and best viewed with the "Documentation.md" file either on GitHub or Vs Code.

Video going through the sequence diagrams: https://www.youtube.com/watch?v=nU5daDI3iPI 


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
