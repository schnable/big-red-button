# big-red-button
My version of the DL100B Dream Cheeky Generic Controller driver.

- Based on http://blog.opensensors.io/blog/2013/11/25/the-big-red-button/

I've used this udev rule to help simplify the device naming on my raspberry pi:

     KERNEL=="hidraw*", SUBSYSTEM=="hidraw", ATTRS{product}=="DL100B Dream Cheeky Generic Controller", MODE="0664", GROUP="users", SYMLINK+="big-red-button"

