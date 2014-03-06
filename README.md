Elements
================================

Project in the course TNG022 Modelling and Simulation at Linköping University.

Aim for the project
-------------------------
Using the Navier-Stokes equations to develop a system for visualizing different fluids, e.g. smoke. In the application you should be able to modify the parameter values to see how changing the equation affects the simulation in real-time.

Result
-------------------------
A system that currently can visualize smoke with different density values in 2D (calculations done in 3D, but no volume renderer written during this course), also allowing the user to change the position of the point source via the WASD keys.

About
-------------------------
Originally our plan was to allow the user to modify the fluid simulation via a GUI, using Qt. Even if a graphical userface was indeed created (see folder "qt" for code), we later felt we needed to change to GLFW to improve the developing time for doing the actual math implementation (see folder "final" for code).
Unfortunately it is not possible for you to run the code on your own computer at this time without modifying the code, depending on your setup (OS, graphics card, etc). Since this was a school project, we will probably not maintain the course afterwards.

That being said, we do have images and videos of the app running real-time. For reference, see the articles we based our project on:
* http://prideout.net/blog/?p=58
* http://prideout.net/blog/?p=66
* http://http.developer.nvidia.com/GPUGems/gpugems_ch38.html
* http://http.developer.nvidia.com/GPUGems3/gpugems3_ch30.html
We really appreciate these resources being available online from "The Little Grasshopper" and NVIDIA. Thanks!

Group members
-------------------------
Joakim Deborg - joade361@student.liu.se<br />
Kristofer Janukiewicz - krija286@student.liu.se<br />
Gabriel Baravdish - gabba873@student.liu.se<br />
Marcus Nygren - marny568@student.liu.se<br />
Niklas Andersson - nikan278@student.liu.se<br />
