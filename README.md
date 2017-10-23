# classProjects
Exploring with Arduino      Class Project Ideas, updated 2017-10-23

This document is an outline of what projects I suggest that you try.
I also suggest that you start with the first 3 projects in order, and
then you can pick and choose which other projects to try.

These projects are intended to introduce and reinforce many concepts,
and I hope that some of the repetition will make it easy to see some
of the same concepts as you apply them to other projects (since I
hope it will helpsome "concrete learners" to learn by doing).

At the time of this update, this file is the README.md file at the root
of a github file tree at https://www.github.com/zonkerharris/classProjects
and I'll be adding Arduino sketches for some projects, and other class
resource information. Some of these will appear as the class progresses
(so that you'll focus on some of the initial projects first, and then
move on to the other topics). 

There is also a wiki, with information and clues about the projects
at http://arduinoclass.pbworks.com for students to reference.

The class was built around the Arduino M0 processor, rather than a 
simpler 326-based board (Arduino UNO, for example), because the M0
CPU is faster, has much more RAM and FLASH memory, and can be used
on a protoyping board easily. Specifically, I picked the Adafruit
"Feather Adalogger M0" because it also has a (LiPo) battery charger
and a MicroSD card socket on the board, which will make it easy to
set up a portable experiment, capture data from a number of sensors,
react to that environment, and log the readings to the SD card.
   https://www.adafruit.com/product/2796#tutorials 
   
The reason I chose the SD card, rather than a wifi-enabled board,
was that students can get cheap or free access to some "cloud services"
while they are still students, but that service will eventually cost 
you money every month. Using the SD card is a fast and easy method to 
pull your data into a computer to visualize the information, and
then analyize it. At that point, you could even use cloud-based
"Business Intelligence" tools to look for correlations across 
multiple data sets. Some of you will be drawn to projects where 
you are taking and logging data, while others may be interested
in learning to analyze and visualize information. You could
partner with other students, and collaborate on projects to
collect and look at data.

Eventually, I hope that you will try ALL of the projects, to
be exposed to many sensors and methods. As you continue your
education, you may be tasked with experiments where you'll need
to take and record measurements, and report the results. Once
you finish these projects, I'm hoping you'll at least think 
"I could do this with an Arduino".

If you prefer to use a classic Arduino board, and using pre-made 
shields, I would suggest that you look into the Arduino Experiment
(ARDX) started in the UK (http://oomlout.com/a/products/ardx/),
and available in the US from Adafruit, SparkFun and Seedstudios.
The project instructions are available free as a PDF, but the
kits have all the parts you'll need to do all of the experiments.
(And each of the experiments introduces a particular aspect of
basic electronics, and basic debugging skills, and starts you
off with working code that you can then modify to see how to
make the sketch perform things they way you would prefer.) The
kits come with an Arduino UNO and a spall protoboard, so you
can then add other shields to expand the functionality.
  ** If you use the UNO, instead of the M0, you'll have
     less memory in the UNO, an dsome of the bigger class
     projects may not fit. The UNO will also run at a slower
     CPU clock rate, and your sketches will appear slower
     than the same program running on the M0.
