# automatic-flush
An arduino project to flush toilet automatically

# Summary

This is an Arduino automatic flush toilet project. This is still a work in progress and only has about a dozen flushes of real life production testing. Feel free to contact me with any questions at @michaelminhvo

# Equipment list (~90$ CAD)

- [Arduino uno](https://amzn.to/2VSbF81)
- [Jumper cables](https://amzn.to/3f8zOP5)
- [Distance sensor](https://amzn.to/3e5MXXN)
- [Glue gun](https://amzn.to/2O0iEaD)
- [Chain link](https://amzn.to/2D8PVOq)
- [Battery pack for Arduino Uno](https://www.adafruit.com/product/1566)
- [Cheap servo motor](https://amzn.to/2VScT2K)
- isopropyl alcohol - there will be trial and error for mounting the distance sensor and servo motor. Simply dab a cotton swab with alochol on the glue and it magically peels off.

[Optional] more powerful motor - may need a separate power source depending on your setup (~40$ CAD more)
- [Servo motor](https://amzn.to/2BAlKj2)


# Equipment notes
The more expensive servo motor is optional. It's waterproof and has stronger pulling torque. It sounds better when it turns. However, the cheaper motor works perfectly fine.


# High level explanation
We use a distance sensor to detect when someone is in front of the toilet. There are many algorithms we can use to detect whether someone is truly using the toilet or just walking by. We send a pulse every second from the distance sensor and check that a treshhold of 5 out of the last 8 seconds had an object within 25cm and 80 cm of the sensor. Once the threshold is hit, we are now in "FLUSH MODE" and wait 5 seconds where nothing is in front of the sensor before flushing. If a person leans over to reach for a magazine, or for the toilet paper but then sits back down, we reset the countdown. 

Sometimes the algorithm will flush too early or won't flush. For this case we added a quick flush mode where a person can hold their hand in front of the distance sensor for two seconds and the toilet will flush.

Feedback is very important in product design. A person using this toilet needs to understand how to use it. The distance sensor has two round input/output sensors that look like eyes. There are two prototypes: 1. LED lights that light up when we detect "FLUSH MODE", and blink when it senses possible activity, 2. The LCD panel is blank when inactive, and has countdowns whenever it detects an object in front of it.

"Flush mode in 4..."
"Toilet in use"
"Flushing in 4..."
"Done"

# Battery

1. For prototyping, you can use a 4AA (6V) battery pack or 9V into the DC jack of the arduino. The 9V can power both the LCD and the arduino but will run out of power within hours. The 4AA lasts longer (still testing) but can barely power the LCD.

2. [Recommended] Use a [40$ USB battery pack](https://amzn.to/2ZTqvfk). These have 20,000 mA of power. An arduino uno board takes 10mA and an LCD takes 20mA. 20,000/30/24(hours in a day) gives approximately 27 days of use. Ditching the LCD will give you close to 3 months of usage.

# Code

There are multiple files depending on which project you want to build. 

# Next steps

This is still a work in progress and I will update these docs. 

- There's a lot of product improvements such as having "Quick flush" mode as LCD help text. 
- Stat collection with a clock module to print out the last time toilet was flushed
- WiFi Arduino board for statistics collection for time of day and frequency of toilet use. 
- Different algorithms such as no flushing between 12pm and 6am and queuing up a flush right at 6am to not wake up your partner. 
