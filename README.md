Resources to understand how Vornoi diagrams and Fortune's Algorithm works
- [Voronoi Graphs Explained](https://youtu.be/Y_rMSk_JwJo?si=RfmwaJMDLfAoSrk-)
- [Fortune's Algorithm Overview Video](https://youtu.be/W7h0a89lAno?si=N6Pf0jDzHibcqMsu)
- [How Parabolas Can Help Describe Nature and Business](https://youtu.be/L_joQb12QSE?si=8vmedYfAg5a23FQQ)

General Stuff
- UDP is a communication protocol like HTTP (Determines how data is transferred)
- Protobuff is a message format (Determines the format of the data that is transferred)

Amun
- Internal simulator implementation

Core
- Does the maths for the simulator (vector stuff, velocity calculations)
- Handles the entire time management of the simualtor

Drona
- The strategy module of the simulator. This is where our next actions are determined.
- It has a handle_state function that is called by kuruk
- This handle_state function calls planner.cpp inside voronoi
- planner.cpp consists of the logic we are coding

Kshetra
- Just the QGraphicsView (The graphics that are displayed to us)
- Has the ground, the lines, etc

Kuruk
- The main class that is the parent of everything.
- All other components, like Kshetra, Shunya, Drona, Vishnu, Vyasa are initialized here
- When Kuruk receives signals from Vyasa, it calls Drona's handle_state

Shunya
- Sets up the starting position
- Also has the preset locations when you click on Attack/Defense in the gui

Simulator
- Also the simulator? Idk how this is different from Amun

Varma
- Future visualization library (Currently has nothing)

Vishnu
- Communicates with the referee

Voronoi
- Voronoi path planning

Vyasa
- The intermediary between the backend (simulator) and the frontend.

Yodha
- Defines the blue and yellow bots

