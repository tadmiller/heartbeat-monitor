
//////////
/////////  All Serial Handling Code,
/////////  It's Changeable with the 'outputType' variable
/////////  It's declared at start of code.
/////////

void serialOutput()
{   // Decide How To Output Serial.
    switch(outputType)
    {
        case PROCESSING_VISUALIZER:
            sendDataToSerial('S', Signal);     // goes to sendDataToSerial function
            break;
        case SERIAL_PLOTTER:  // open the Arduino Serial Plotter to visualize these data
            Serial.print(BPM);
            Serial.print(",");
            Serial.print(IBI);
            Serial.print(",");
            Serial.println(Signal);
            break;
        default: 
            break;
    }
}

//  Decides How To OutPut BPM and IBI Data
void serialOutputWhenBeatHappens()
{
    switch(outputType)
    {
        case PROCESSING_VISUALIZER:    // find it here https://github.com/WorldFamousElectronics/PulseSensor_Amped_Processing_Visualizer
            sendDataToSerial('B',BPM);   // send heart rate with a 'B' prefix
            sendDataToSerial('Q',IBI);   // send time between beats with a 'Q' prefix
            break;
        default:
            break;
    }
}

//  Sends Data to Pulse Sensor Processing App, Native Mac App, or Third-party Serial Readers.
void sendDataToSerial(char symbol, int data )
{
    Serial.print(symbol);
    Serial.println(data);
}

void updateCmd()
{
    // Check and see if we can read a command.
    if (Serial.available() > 0)
        cmd = Serial.read();
    
    // If there's a new pulse, and we haven't received a command, and we're not paused from the cmd prompt
    if (QS == true && cmd == 0 && !paused)
    {
        serialOutput();
        BPM = BPM > 99 ? 99 : BPM; // We can't send values higher than 99 since the display is only two numbers.

        //serialOutputWhenBeatHappens();   // A Beat Happened, Output that to serial.
        QS = false;                      // reset the Quantified Self flag for next time

        // Then we write our new BPM to the display
        matrixWrite(BPM);
    }
    else
    {   // 112 == 'p' which means we pause the program state.
        if (cmd == 112)
            paused = true;
        else if (cmd == 114) // 114 = 'r' which means we resume the program state.
            paused = false;
        else if (cmd == 115) // 115 is 's' which means we're going to display the number that comes after
        {
            showValue = 0;
            delay(100);

            // Wait until we receive a number. Then display it.
            while (showValue <= 0 || showValue >= 100)
            {
                showValue = Serial.read();
                delay(30);
            }

            BPM = showValue;
            paused = true;
            matrixWrite(BPM);
        }

        cmd = 0;
    }
}

