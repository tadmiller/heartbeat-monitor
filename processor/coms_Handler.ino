
char msgCount = 32;

int sendReading(String data)
{
	Serial.print("C[");
	Serial.print(msgCount);
	Serial.print("]");
	Serial.print(data);
	Serial.println("");

	msgCount += 1;

	if (msgCount >= 127)
		msgCount = 32;

	return 0;
}

void handleComs(int hour, int min, int sec, int reading)
{
	sendReading("B[" + String((char)reading) + "]H[" + String((char)(hour + 32)) + "]M[" + String((char)(min + 32)) + "]S[" + String((char)(sec + 32)) + "]");
}

void sendRGB(int r, int g, int b)
{
	sendReading("R[" + String((char) r) + "]G[" + String((char) g) + "]B[" + String((char) b) + "]");
}