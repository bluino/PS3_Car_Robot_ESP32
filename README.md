# PS3_Car_Robot_ESP32

Therefore, if you want to connect your PS3 controller to the ESP32, you either need to figure out what the Bluetooth MAC address of your PS3 console is and set the ESP32's address to it, or change the MAC address stored in the PS3 controller.

<p dir="auto">Whichever path you choose, you're going to need a tool to read and/or write the currently paired MAC address from the PS3 controller. I used <a href="https://dancingpixelstudios.com/sixaxis-controller/sixaxispairtool/" rel="nofollow">SixaxisPairTool</a> for this, but you can try using <a href="https://github.com/user-none/sixaxispairer">sixaxispairer</a> as well, if open source is important to you.</p>
