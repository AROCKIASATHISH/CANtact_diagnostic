
CANtact :

compilation and Flashing steps:

1)First connect jumper inorder to flash.
2)Compile the project by running make command on terminal.
3)After that ,connect usb of stm32 to PC and Flash the generated .bin file to stm32 through usb by running make flash command on terminal.
4)now Device is succesfully programmed.
5)then reconnect the usb of stm32 from PC and remove jumper from board. now normal operation starts.



running procedure:

CANTact diagnostics test 
1.UART TX Test     ----------------(baudrate 9600)
2.UART RX test     ----------------(baudrate 9600)
3.SPI MASTER Test 
4.SPI SLAVE test  
5.I2C MASTER Test  ----------------(frequency 100khz)
6.I2C SLAVE test   ----------------(address 125)
7.CAN TX test      ----------------(speed 500khz , std_id = 0x123)
8.CAN RX test      ----------------(it will accept all packets regardless of packet id)
9.LED test 
10.Header pin test


1)Connect usb of stm32 to PC.
2)First check device file name by entering dmesg command terminal.
3)Connect communication interfaces of two boards.
4)After that 
      1)To view menu, use minicom -D /dev/ttyACM0. then press up/down arrow key.it will display menus. 
        you can select option and run this test by pressing enter. After selecting option corresponding test will run. 
        For confirm test is pass/fail, user can press right and left arrow key..

         * In i2c master test, master device will scan i2c devices on bus. if found it will turn on led.
         * In communication pheriperal tx test except i2c master , transmitter/master device will send led on/off string .
         * In communication pheriperal rx test , receiver/slave will receive led on/off string .
            and control led on board accordingly.
               
         * In LED Test, two led continously blink every one second. 

         * In Header pin Test, all pins are act as output.
               and all connector pins will logic high and low every one second. 
         * in auto diag , each test will run duration of 7 seconds.




