#include "utillora.h"
#include "aes-128.h"

unsigned int frameCounterTx = 0x0000;

unsigned char NwkSkey[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char AppSkey[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char DevAddr[4] = {0x00, 0x00, 0x00, 0x00};

// sleep cycles
volatile int sleep_count = 0;
const int sleep_total = 8; // 8*8s 64s sleep time

// LORA_Send_Data(Data, Data_Length, frameCounterTx);

/*
*****************************************************************************************
* Description : Function contstructs a LoRaWAN package and sends it
*
* Arguments   : *Data pointer to the array of data that will be transmitted
*               Data_Length nuber of bytes to be transmitted
*               frameCounterUp  Frame counter of upstream frames
*****************************************************************************************
*/
void LORA_Send_Data(unsigned char *Data, unsigned char Data_Length, unsigned int frameCounterTx)
{
	//Define variables
	unsigned char i;

	//Direction of frame is up
	unsigned char Direction = 0x00;

	unsigned char RFM_Data[64];
	unsigned char RFM_Package_Length;

	unsigned char MIC[4];

	//Unconfirmed data up
	unsigned char Mac_Header = 0x40;

	unsigned char Frame_Control = 0x00;
	unsigned char Frame_Port = 0x01;

	//Encrypt the data
	encryptPayload(Data, Data_Length, frameCounterTx, Direction);

	//Build the Radio Package
	RFM_Data[0] = Mac_Header;

	RFM_Data[1] = DevAddr[3];
	RFM_Data[2] = DevAddr[2];
	RFM_Data[3] = DevAddr[1];
	RFM_Data[4] = DevAddr[0];

	RFM_Data[5] = Frame_Control;

	RFM_Data[6] = (frameCounterTx & 0x00FF);
	RFM_Data[7] = ((frameCounterTx >> 8) & 0x00FF);

	RFM_Data[8] = Frame_Port;

	//Set Current package length
	RFM_Package_Length = 9;

	//Load Data
	for (i = 0; i < Data_Length; i++)
	{
		RFM_Data[RFM_Package_Length + i] = Data[i];
	}

	//Add data Lenth to package length
	RFM_Package_Length = RFM_Package_Length + Data_Length;

	//Calculate MIC
	calculateMIC(RFM_Data, MIC, RFM_Package_Length, frameCounterTx, Direction);

	//Load MIC in package
	for (i = 0; i < 4; i++)
	{
		RFM_Data[i + RFM_Package_Length] = MIC[i];
	}

	//Add MIC length to RFM package length
	RFM_Package_Length = RFM_Package_Length + 4;

	//Send Package
	RFM_Send_Package(RFM_Data, RFM_Package_Length);
}

void encryptPayload(
	const unsigned char *data,
	unsigned char size,
	unsigned int frameCounter,
	unsigned char Direction)
{
	unsigned char i = 0x00;
	unsigned char j;
	unsigned char Number_of_Blocks = 0x00;
	unsigned char Incomplete_Block_Size = 0x00;

	unsigned char Block_A[16];

	// Calculate number of blocks
	Number_of_Blocks = size / 16;
	Incomplete_Block_Size = size % 16;
	if (Incomplete_Block_Size != 0)
	{
		Number_of_Blocks++;
	}

	for (i = 1; i <= Number_of_Blocks; i++)
	{
		Block_A[0] = 0x01;
		Block_A[1] = 0x00;
		Block_A[2] = 0x00;
		Block_A[3] = 0x00;
		Block_A[4] = 0x00;

		Block_A[5] = Direction;

		Block_A[6] = DevAddr[3];
		Block_A[7] = DevAddr[2];
		Block_A[8] = DevAddr[1];
		Block_A[9] = DevAddr[0];

		Block_A[10] = (frameCounter & 0x00FF);
		Block_A[11] = ((frameCounter >> 8) & 0x00FF);

		Block_A[12] = 0x00; //Frame counter upper Bytes
		Block_A[13] = 0x00;

		Block_A[14] = 0x00;

		Block_A[15] = i;

		//Calculate S
		AESEncrypt(Block_A, AppSkey); //original

		//Check for last block
		if (i != Number_of_Blocks)
		{
			for (j = 0; j < 16; j++)
			{
				*data = *data ^ Block_A[j];
				data++;
			}
		}
		else
		{
			if (Incomplete_Block_Size == 0)
			{
				Incomplete_Block_Size = 16;
			}

			for (j = 0; j < Incomplete_Block_Size; j++)
			{
				*data = *data ^ Block_A[j];
				data++;
			}
		}
	}
}

void calculateMIC(unsigned char *Data, unsigned char *Final_MIC, unsigned char Data_Length,
				  unsigned int frameCounter,
				  unsigned char Direction)
{
	unsigned char i;
	unsigned char Block_B[16];

	unsigned char Key_K1[16] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned char Key_K2[16] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	//unsigned char Data_Copy[16];

	unsigned char Old_Data[16] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned char New_Data[16] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	unsigned char Number_of_Blocks = 0x00;
	unsigned char Incomplete_Block_Size = 0x00;
	unsigned char Block_Counter = 0x01;

	//Create Block_B
	Block_B[0] = 0x49;
	Block_B[1] = 0x00;
	Block_B[2] = 0x00;
	Block_B[3] = 0x00;
	Block_B[4] = 0x00;

	Block_B[5] = Direction;

	Block_B[6] = DevAddr[3];
	Block_B[7] = DevAddr[2];
	Block_B[8] = DevAddr[1];
	Block_B[9] = DevAddr[0];

	Block_B[10] = (frameCounter & 0x00FF);
	Block_B[11] = ((frameCounter >> 8) & 0x00FF);

	Block_B[12] = 0x00; //Frame counter upper bytes
	Block_B[13] = 0x00;

	Block_B[14] = 0x00;
	Block_B[15] = Data_Length;

	//Calculate number of Blocks and blocksize of last block
	Number_of_Blocks = Data_Length / 16;
	Incomplete_Block_Size = Data_Length % 16;

	if (Incomplete_Block_Size != 0)
	{
		Number_of_Blocks++;
	}

	generateKeys(Key_K1, Key_K2);

	//Preform Calculation on Block B0

	//Preform AES encryption
	AESEncrypt(Block_B, NwkSkey);

	//Copy Block_B to Old_Data
	for (i = 0; i < 16; i++)
	{
		Old_Data[i] = Block_B[i];
	}

	//Preform full calculating until n-1 messsage blocks
	while (Block_Counter < Number_of_Blocks)
	{
		//Copy data into array
		for (i = 0; i < 16; i++)
		{
			New_Data[i] = *Data;
			Data++;
		}

		//Preform XOR with old data
		XOR(New_Data, Old_Data);

		//Preform AES encryption
		AESEncrypt(New_Data, NwkSkey);

		//Copy New_Data to Old_Data
		for (i = 0; i < 16; i++)
		{
			Old_Data[i] = New_Data[i];
		}

		//Raise Block counter
		Block_Counter++;
	}

	//Perform calculation on last block
	//Check if Datalength is a multiple of 16
	if (Incomplete_Block_Size == 0)
	{
		//Copy last data into array
		for (i = 0; i < 16; i++)
		{
			New_Data[i] = *Data;
			Data++;
		}

		//Preform XOR with Key 1
		XOR(New_Data, Key_K1);

		//Preform XOR with old data
		XOR(New_Data, Old_Data);

		//Preform last AES routine
		// read NwkSkey from PROGMEM
		AESEncrypt(New_Data, NwkSkey);
	}
	else
	{
		//Copy the remaining data and fill the rest
		for (i = 0; i < 16; i++)
		{
			if (i < Incomplete_Block_Size)
			{
				New_Data[i] = *Data;
				Data++;
			}
			if (i == Incomplete_Block_Size)
			{
				New_Data[i] = 0x80;
			}
			if (i > Incomplete_Block_Size)
			{
				New_Data[i] = 0x00;
			}
		}

		//Preform XOR with Key 2
		XOR(New_Data, Key_K2);

		//Preform XOR with Old data
		XOR(New_Data, Old_Data);

		//Preform last AES routine
		AESEncrypt(New_Data, NwkSkey);
	}

	Final_MIC[0] = New_Data[0];
	Final_MIC[1] = New_Data[1];
	Final_MIC[2] = New_Data[2];
	Final_MIC[3] = New_Data[3];
}
