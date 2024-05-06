# Team61-3004Project

Neurofeedback System Project
===========================================

Creators/Team Members:
Abbas Mohamed         (Student#  101188317)
Ahmed Yussuf          (Student#  101202434)
Jad Hajaig            (Student#  101217761)
Zakaria Mohamoud      (Student# 101189801)
--------

Overview
--------
The Neurofeedback System is designed to manage and conduct neurofeedback sessions using EEG technology. This application provides real-time analysis, session management, and treatment application based on user-specific EEG data. Built using Qt Creator, this software facilitates comprehensive session logging, real-time treatment adjustments, and detailed reports to enhance cognitive therapy and brain function optimization.

Testing Video
--------
Youtube Link: https://www.youtube.com/watch?v=SLCzXniaK3w

Features
--------
- EEG Data Acquisition: Supports real-time data gathering from up to 21 EEG sites.
- Dynamic Analysis: Calculates dominant frequencies and baselines for effective treatment.
- Treatment Protocols: Applies neurofeedback treatment according to the LENS protocol.
- Wave form generation: Apply to display live feed of current EEG site frequency and amplitude for further analysis.
- Session Reporting: Generates detailed reports post-session for review and planning.
- Device Management: Monitors device status, including battery levels and electrode connectivity.

Requirements
-------------
- Qt 5.x or later
- C++11 Standard or later
- Assumed OS here will be Ubuntu but should work on any Linux distribution
- Compatible EEG hardware interface

Installation
------------
1. Clone the repository or download the source code.
2. Install QCharts to your environment using the command `sudo apt-get install libqt5charts5 libqt5charts5-dev` in the terminal window
3. Open Qt Creator and select 'Open Project'.
4. Navigate to the downloaded folder and select the `NeuroProject.pro` file.
5. Configure the project settings as prompted by Qt Creator, especially the kit selection that matches your development environment.

**Alternatively, one can navigate to the Team61-3004Project/src directory and run `qmake` (assuming this is already installed on the system). Then `make` and finally `./NeuroProject` if one does not want to use the Qt creator platform.**

Usage
-----
1. Start Qt Creator and open the Neurofeedback System project.
2. Build the project by selecting 'Build' > 'Build All'.
3. Run the application by selecting 'Run' > 'Start Debugging' or 'Run' > 'Start Without Debugging'.
4. Follow the on-screen instructions to configure user settings and start a session.

**Note with the addition of QChart sometimes the project may not run correctly, if running from the QT Creator make sure to try to build the NeuroProject (a second time)** 

Development
-----------
- **Source Files**: Contain the logic for EEG data handling, session management, and UI interactions.
- **Header Files**: Define the classes and functions used across the project.
- **Form Files (UI)**: Designed using Qt Designer, editable directly from Qt Creator for UI adjustments.

Current Stage (Next steps)
-----------
- Able to run through a session and make the related calculations and adjustments (mostly in console log).
- Current working on display waveform functionality and revamping the UI to be able to display comprehensive session information.

Contributions
-------------
**Jad Hajaig**
- Developed algorithms, workflows, and backend logic for sessions.
- Created project schema, UML class diagrams, UML state diagrams, UML Usecase diagrams, UML Sequence diagrams, and use case documentation.
- Developed GUI components and assisted in refining backend logic
- Implemented session, treatment, and analysis logic within Qt.
- Assisted in textual design doc.

**Abbas Mohamed**
- Implemented session data gathering and main DataModel integration with the UI.
- Developed GUI components and assisted in refining backend logic.
- Created the traceability matrix and provided overall project support.
- Created the textual design documents explaining various implementation choices made

**Zakaria Mohamoud**
- Co-designed session and treatment logic with Jad.
- Implemented the session, treatment, and analysis modules within Qt.
- Assisted in Use Case creation.
- Recorded and uploaded Test Video.

**Ahmed Yussuf**
- Implemented core functionalities related to battery management.
- Developed Wave chart using Qtchart to visually represent node to users functionality.
- Assisted in GUI development and enhanced user interaction features, synchronized the colours to coordinate if contact was lost or is on and if the treatment is in session.

Organization of Submission
--------------------------
The project is organized into several directories to ensure ease of navigation and clarity:

- **Team61-3004Project**: Root directory of the project.
  - **documents**: Contains all UML diagrams and use case documentation.
    - **sequenceDiagram** - subfolder containing all sequence diagrams
    - UMLClassDiagram.png
    - UMLStateDiagram.pdf
    - UMLCaseDiagram.pdf
    - Usecases.pdf
    - tracebilityMatrix.pdf
    - designDoc.pdf (contains textual decisions made in relation to implementation)
  - **src**: Source code of the project, structured as follows:
    - batteryclass.cpp
    - batteryclass.h
    - datamodel.cpp
    - datamodel.h
    - main.cpp
    - mainwindow.cpp
    - mainwindow.h
    - neurofeedbacksystem.cpp
    - neurofeedbacksystem.h
    - sessiondata.cpp
    - sessiondata.h
    - defs.h
    - resourceFile.qrc
    - **pictures** - subfolder containing all the .png photos we used to display the various red/blue/green lights as well as the battery


