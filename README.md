# automatic-flush
An arduino project to flush toilet automatically

# Equipment list (~90$ CAD)

- [Arduino uno](https://amzn.to/2VSbF81)
- [Jumper cables](https://amzn.to/3f8zOP5)
- [Distance sensor](https://amzn.to/3e5MXXN)
- [Glue gun](https://amzn.to/2O0iEaD)
- [Chain link](https://amzn.to/2D8PVOq)
- [Battery pack for Arduino Uno](https://amzn.to/2O2QETG)
- [Cheap servo motor](https://amzn.to/2VScT2K)
- isopropyl alcohol - you will need to fix the placement of the distance sensor and servo motor. Simply dab a cotton swab on the glue and it magically peels off

Optional more powerful motor - needs a separate power source (~40$ CAD more)
- [Servo motor](https://amzn.to/2BAlKj2)
- [Battery pack for Servo motor](https://amzn.to/2ZLTr90)


# Equipment notes
The more expensive servo motor is optional. It's waterproof and has stronger pulling torque. The cheaper motor works fine.


# High level explanation
We use a distance sensor to detect when someone is in front of the toilet. There are many algorithms we can use to detect whether someone is truly using the toilet or just walking by. We send a pulse every second from the distance sensor and check that a treshhold of 5 out of the last 8 seconds had an object within 25cm and 80 cm of the sensor. Once the threshold is hit, we are now in "FLUSH MODE" and wait 5 seconds where nothing is in front of the sensor before flushing. If a person leans over to reach for a magazine, or for the toilet paper but then sits back down, we reset the countdown. 

Sometimes the algorithm will flush too early or won't flush. For this case we added a quick flush mode where a person can hold their hand in front of the distance sensor for two seconds and the toilet will flush.

Feedback is very important in product design. A person using this toilet needs to understand how to use it. The distance sensor has two round input/output sensors that look like eyes. I considered LED lights that would light up when we detected "FLUSH MODE" - but decided on a LCD panel mostly because I wanted to learn how to work with those. The LCD panel is blank when inactive, and has countdowns whenever it detects an object in front of it.

"Flush mode in 4..."
"Toilet in use"
"Flushing in 4..."
"Done"

# Next steps
- There's a lot of product improvements such as having "Quick flush" mode as LCD help text. 
- Stat collection with a clock module to print out the last time toilet was flushed
- WiFi Arduino board for statistics collection for time of day and frequency of toilet use. 
- Different algorithms such as no flushing between 12pm and 6am and queuing up a flush right at 6am to not wake up your partner. 
