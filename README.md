# Smart-Energy-Meter
Smart alternative to traditional energy meters currently in use. First prototype, a lot of work is to be done. Initial prototype is successful and works accurately for resistive loads.
# Abstract
Measurement of AC/DC current drawn and power consumed by devices using ACS712 sensor and Arduino nano and calculating the energy used during operation by recording the time for which the device was working.
For AC - power can be measured easily once current is calculated since normal household supply voltage in the sockets is 220V - 240V. For DC we will use a voltage divider circuit using resistors or a voltage sensor connected to the Arduino analog pin to measure the voltage.
Then we will send the measured data to a smartphone and a website(thingspeak.com) over WIFI and internet.
Visualization of the recorded data will be in the form of graphs and numeric displays on the website using MATLAB analytics.
Data will also be displayed on an LCD screen connected to the circuit.
The data can also be recorded in excel sheet using MATLAB analytics, after it is sent to the website.
# Scope
Can be used in DC machines to monitor the power generated/used and current drawn in real time and diagnose any fault happening during operation of the machine.
In solar panels to monitor the power generated, the incoming voltage and current.
In homes we can measure the ac current drawn and power consumed by appliances.
Can also be used as smart over-current protection device.
Smart alternative to traditional energy meters
# Components Used
<ul>
  <li>Arduino Nano</li>
  <li>D1 Mini Pro node MCU V2</li>
  <li>ACS712 20A Sensor</li>
  <li>ZMPT101B-sinlge phase AC voltage sensor</li>
  <li>Connecting Wires</li>
  <li>20 SWG Wire</li>
  <li>Multi-meter</li>
  <li>Soldering Iron and Solder Wire</li>
  <li>1kΩ and 2kΩ resistors</li>
  <li>60W, 100W and 200W bulbs</li>
  <li>Red and vlue led</li>
</ul>

# Software stack
<ul>
  <li>Arduino IDE</li>
  <li>MIT App Inventor</li>
  <li>Google Firebase</li>
  <li>Microsoft Excel</li>
  <li>HTML</li>
  <li>Server side framework</li>
</ul>

# Circuit Diagram
<div>
  <img src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/Sketch%20complete_schem.jpg">
</div>

# ACS712 sensor
Works on the principle of hall effect. An output voltage is generated proportional to the current flowing through the sensor. Our system can measure current up to 20A.
<div>
  <img src="https://upload.wikimedia.org/wikipedia/en/1/19/Hall_Effect_Measurement_Setup_for_Electrons.png">
  <img src="http://luckyretail.com/Uploadfile/2015.1.30/036694/036694.jpg" height="184" width="285">
</div>
<ul>
  <li>Low-noise analog signal path</li>
  <li>Sensitivity: 100mV/A</li>
  <li>Measure Current Range: 20A</li>
  <li>Supply Voltage: 4.5V~5.5V DC</li>
</ul>

# ZMPT101B AC Voltage Sensor
ZMPT101B a voltage transform ideal to measure the AC voltage. It has high accuracy, good consistency for voltage and power measurement up to 250V AC. Easy to use and comes with a multi turn trim potentiometer for adjusting the ADC output.
<div>
  <img src="https://www.autobotic.com.my/image/autobotic/image/cache/data/all_product_images/product-9129/4126-3-700x700.jpg">
</div>
<ul>
  <li>Output Signal: Analog 0-5V</li>
  <li>Measure within 250V AC</li>
  <li>Onboard micro-precision voltage transformer</li>
  <li>Operating Voltage: DC 5V-30V</li>
</ul>

# 1st Problem
The alternating current generates a sine wave. The values vary over a period of time from negative to positive. Therefore, the RMS value of the current is considered. Now, the ACS712 sensor needs 5V DC to function properly. The sensor keeps 2.5V as the mid-point and measures the proportional voltage around this mid-point. So, the range varies from 0-5V in the output of the sensor with 2.5V as mid-point, which corresponds to the 0-point.
When we give alternating current as input to the sensor, the sensor measures the proportional analog voltage around the mid-point of 2.5V. So, we get all varying values of the current, this data is useless to us. Suppose we applied 8A as input then changed it to 10A, the change in value won’t reflect in the analog output of the sensor.
<div><img src="https://cdn.instructables.com/ORIG/FNO/T49W/IM3OBJTZ/FNOT49WIM3OBJTZ.jpg?auto=webp&frame=1&fit=bounds&md=1d0e924348a811382f415302bb6e85a1"></div>
Image source - https://www.instructables.com/Simplified-Arduino-AC-Current-Measurement-Using-AC/#:~:text=%20Simplified%20Arduino%20AC%20Current%20Measurement%20Using%20ACS712,6%20Step%206%3A%20Optional%20Sensor%20Calibration%20More%20
Project on Instructables By https://www.instructables.com/member/e024576/

# Solution to 1st problem
To overcome this problem, we tried to measure the standard deviation of input current around the 2.5V midpoint. We took 20 readings per second, then measured their standard deviations and then averaged them. In this way, we measured the fluctuations in the analog output of the sensor.
Then we took 3 blubs of 60W, 100W, and 200W and then connected them to the sensor one by one, to measure the output standard deviations which the sensor was sending to the Arduino. Each bulb was switched on for about a minute and the respective values were recorded. Then we copied the values to an excel sheet. A digital multimeter was connected in series with the bulbs to know the actual RMS currents. Similarly, the current values were noted from the digital multimeter and copied to the excel sheet. In the excel sheet, we plotted a graph between the current and the standard deviations and found the line of best fit. The graph gave us an approximate equation relating to the two data variable.
<div>
  <h2>For ACS712</h2>
  <img src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/pic-5.jpg">
</div>
<div>
  <h2>For ZMPT101B</h2>
  <img src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/voltage.jpg">
</div>

# 2nd problem and it's solution
In our project, we are sending the data through the internet to our database, so that our website and the mobile app can access that data. Arduino as such does not have a WIFI chip inbuilt. So, we had to use a separate WIFI chip (ESP8266 node MCU) in serial communication with the Arduino board, to send the data to the database. Here, one could ask why didn’t we just connect the sensor to the node MCU. The problem was, node MCU provides 3.3V max as output voltage for peripherals but the sensor needed at least 5V DC as input for proper working. Therefore, we used two boards here, Arduino for reading and computing the sensor values, and node MCU for sending data to the database.

# Communication between Arduino and Wemos D1 Mini Pro
The data from the Arduino is sent to the node MCU in JSON format (JavaScript Object Notation) through serial communication. It is a format that uses key-value pairs just like a dictionary. JSON is platform-independent and is lightweight. Hence, communication between Arduino and node MCU becomes faster. Since the system works in real-time, it needs to run fast as possible.

# 3rd problem and it's solution
Connecting an LCD to the circuit, dropped the voltage input to the sensor from 5V to 4.7V DC. Hence, there were significant errors in the data obtained.
So, for now, we have removed the LCD screen from the circuit. To access the output values, we are directly using the serial monitor provided in Arduino IDE.

# Our android app made using MIT App Inventor
<div>
  <img src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/Screenshot_2020-10-21-03-11-37-927_appinventor.ai_routpratik2001.Smart_Current_and_Power_Monitoring_System.jpg">
</div>

# Database for android app
<div>
  <img widht="250" height="250" src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/IMG_20201021_032142.jpg">
  <img widht="250" height="250" src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/IMG_20201021_031154.jpg">
</div>

# Switiching operation for appliance by developing a web app
<div>
  <img src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/IMG_20201021_032308.jpg">
</div>
<div>
  <img src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/IMG_20201021_032253.jpg">
</div><div>
  <img src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/Screenshot_2020-10-21-03-22-45-983_com.android.chrome.jpg">
</div><div>
  <img src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/Screenshot_2020-10-21-03-23-02-206_com.android.chrome.jpg">
</div>

# Circuit Components
<div>
  <img width="150" height="150" src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/IMG_20201020_141151_1.jpg">
  <img width="150" height="150" src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/IMG_20201020_141145.jpg">
  <img width="150" height="150" src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/IMG_20201020_141139.jpg">
  <img width="150" height="150" src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/IMG_20201020_141157.jpg">
  <img width="150" height="150" src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/IMG_20201020_141302_1.jpg">
  <img widht="150" height="150" src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/IMG_20201020_141112.jpg">
</div>

# System under load
<div>
  <img width="150" height="250" src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/IMG_20201021_030337.jpg">
  <img width="150" height="250" src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/IMG_20201021_030535_1.jpg">
  <img width="150" height="250" src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/IMG_20201021_030218.jpg">
  <img width="150" height="250" src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/IMG_20201021_030239.jpg">
  <img widht="150" height="250" src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/IMG_20201021_030148.jpg">
</div>

# Next Steps
As of now the prototype can measure current and power of resistive loads on AC supply accurately as well as DC loads but that is not our goal. We are currently trying to design a circuit to measure power factor, so that we will be able to measure power drawn by inductive and capacitive loads. The final system will be able to measure current draw, power and energy consumption of all types of loads as well as power factor. The system will show the apparent power and the real power. Then we will expand this sytem to three phase systems and loads.
