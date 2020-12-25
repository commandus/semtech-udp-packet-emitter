#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstring>
#include "utillora.h"
#include "utildate.h"
#include "utilstring.h"
#include "base64/base64.h"

static void encryptPayload(
	std::string &payload,
	unsigned int frameCounter,
	unsigned char direction,
	DEVADDR &devAddr,
	KEY128 &appSKey
)
{
	unsigned char *data = (unsigned char *) payload.c_str();
	unsigned char size = payload.size();
	unsigned char blockA[16];

	// calc number of blocks
	unsigned char numberOfBlocks = size / 16;
	unsigned char incompleteBlockSize = size % 16;
	if (incompleteBlockSize != 0)
		numberOfBlocks++;

	for (int i = 1; i <= numberOfBlocks; i++) {
		blockA[0] = 0x01;
		blockA[1] = 0x00;
		blockA[2] = 0x00;
		blockA[3] = 0x00;
		blockA[4] = 0x00;

		blockA[5] = direction;

		blockA[6] = devAddr[3];
		blockA[7] = devAddr[2];
		blockA[8] = devAddr[1];
		blockA[9] = devAddr[0];

		blockA[10] = (frameCounter & 0x00FF);
		blockA[11] = ((frameCounter >> 8) & 0x00FF);

		blockA[12] = 0x00; // frame counter upper Bytes
		blockA[13] = 0x00;

		blockA[14] = 0x00;

		blockA[15] = i;

		// calculate S
		aesEncrypt(blockA, appSKey);

		// check for last block
		if (i != numberOfBlocks) {
			for (int j = 0; j < 16; j++) {
				*data = *data ^ blockA[j];
				data++;
			}
		} else {
			if (incompleteBlockSize == 0)
				incompleteBlockSize = 16;

			for (int j = 0; j < incompleteBlockSize; j++) {
				*data = *data ^ blockA[j];
				data++;
			}
		}
	}
}

/**
 * 4.3.3 MAC Frame Payload Encryption (FRMPayload)
 * message integrity code
 * B0
 * 1    4       1   4       4            1 1
 * 0x49 0 0 0 0 Dir DevAddr frameCounter 0 Len(msg)
 * cmac = aes128_cmac(NwkSKey, B0 | msg)
 * MIC = cmac[0..3]
 */ 
static uint32_t calculateMIC(
	const std::string &payload,
	unsigned int frameCounter,
	unsigned char direction,
	DEVADDR &devAddr,
	KEY128 &key
)
{
	unsigned char *data = (unsigned char *) payload.c_str();
	unsigned char dataLength = payload.size();
	unsigned char blockB[16];

	unsigned char keyK1[16] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned char keyK2[16] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	unsigned char oldData[16] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned char newData[16] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	unsigned char blockCounter = 0x01;

	// blockB
	blockB[0] = 0x49;
	blockB[1] = 0x00;
	blockB[2] = 0x00;
	blockB[3] = 0x00;
	blockB[4] = 0x00;

	blockB[5] = direction;

	blockB[6] = devAddr[3];
	blockB[7] = devAddr[2];
	blockB[8] = devAddr[1];
	blockB[9] = devAddr[0];

	blockB[10] = (frameCounter & 0x00FF);
	blockB[11] = ((frameCounter >> 8) & 0x00FF);

	blockB[12] = 0x00; // Frame counter upper bytes
	blockB[13] = 0x00;

	blockB[14] = 0x00;
	blockB[15] = dataLength;

	// calc number of blocks and blocksize of last block
	unsigned char numberOfBlocks = dataLength / 16;
	unsigned char incompleteBlockSize = dataLength % 16;

	if (incompleteBlockSize != 0)
		numberOfBlocks++;

	generateKeys(keyK1, keyK2, key);

	// calc on Block B0

	// AES encryption
	aesEncrypt(blockB, key);

	// copy blockB to oldData
	for (int i = 0; i < 16; i++)
	{
		oldData[i] = blockB[i];
	}

	// full calculating until n-1 messsage blocks
	while (blockCounter < numberOfBlocks) {
		// Copy data into array
		for (int i = 0; i < 16; i++) {
			newData[i] = *data;
			data++;
		}

		// XOR with old data
		XOR(newData, oldData);

		//  AES encryption
		aesEncrypt(newData, key);

		// copy newData to oldData
		for (int i = 0; i < 16; i++)
		{
			oldData[i] = newData[i];
		}

		// raise Block counter
		blockCounter++;
	}

	// Perform calculation on last block
	// Check if Datalength is a multiple of 16
	if (incompleteBlockSize == 0) {
		// copy last data into array
		for (int i = 0; i < 16; i++) {
			newData[i] = *data;
			data++;
		}

		// XOR with Key 1
		XOR(newData, keyK1);

		// XOR with old data
		XOR(newData, oldData);

		// last AES routine
		aesEncrypt(newData, key);
	}
	else
	{
		// copy the remaining data and fill the rest
		for (int i = 0; i < 16; i++) {
			if (i < incompleteBlockSize) {
				newData[i] = *data;
				data++;
			}
			if (i == incompleteBlockSize)
				newData[i] = 0x80;
			if (i > incompleteBlockSize)
				newData[i] = 0x00;
		}

		// XOR with Key 2
		XOR(newData, keyK2);

		// XOR with Old data
		XOR(newData, oldData);

		// last AES routine
		aesEncrypt(newData, key);
	}
	uint32_t r;
	memcpy(&r, &newData, 4);
	return r;
}

const std::string DEF_DATA_RATE = "SF7BW125";
const std::string DEF_ECCCODE_RATE = "4/6";
#define DEF_RSSI	-35
#define DEF_LSNR	5.1

rfmMetaData::rfmMetaData() 
	: chan(0), rfch(0), freq(868900000), stat(0), modu(LORA), datr(DEF_DATA_RATE),
	bps(0), codr(DEF_ECCCODE_RATE), rssi(DEF_RSSI), lsnr(DEF_LSNR)
{
	time(&t);			// UTC time of pkt RX, us precision, ISO 8601 'compact' format
}

/**
 * GPS time of pkt RX, number of milliseconds since 06.Jan.1980
 */ 
uint32_t rfmMetaData::tmst() {
	return utc2gps(t);
}

std::string rfmMetaData::modulation() {
	switch (modu)
	{
	case FSK:
		return "FSK";
	default:
		return "LORA";
	}
}

std::string rfmMetaData::frequency() {
	std::stringstream ss;
	int mhz = freq / 1000000; 
	ss << mhz << "." << freq - (mhz * 1000000);
	return ss.str();
}

std::string rfmMetaData::snrratio() {
	std::stringstream ss;
	int n = lsnr; 
	int m = lsnr * 10.0 - n * 10;
	ss << n << "." << m;
	return ss.str();
}

rfmHeader::rfmHeader() {
	memset(&header, 0, sizeof(RFM_HEADER));
}

rfmHeader::rfmHeader(
	const RFM_HEADER &hdr
) {
	memmove(&header, &hdr, sizeof(RFM_HEADER));
	header.macheader = 0x40;
}

rfmHeader::rfmHeader(
	const DEVADDR &addr
) {
	memset(&header, 0, sizeof(RFM_HEADER));
	memcpy(&header.devaddr, &addr, sizeof(DEVADDR));
	header.macheader = 0x40;
}

rfmHeader::rfmHeader(
	const DEVADDR &addr,
	uint16_t frameCounter
) {
	memcpy(&header.devaddr, &addr, sizeof(DEVADDR));
	header.framecountertx = frameCounter;
	header.framecontrol = 0;
	header.frameport = 0;
	header.macheader = 0x40;
}

rfmHeader::rfmHeader(
	const DEVADDR &addr,
	uint16_t frameCounter,
	uint8_t frameControl,
	uint8_t framePort
) {
	header.macheader = 0x40;
	memcpy(&header.devaddr, &addr, sizeof(DEVADDR));
	header.framecountertx = frameCounter;
	header.framecontrol = 0;
	header.frameport = 0;
}

rfmHeader::rfmHeader(
	const std::string &value
) {

}

void rfmHeader::ntoh() {
	*((uint32_t*) &header.devaddr) = ntoh4(*((uint32_t *) &header.devaddr));
	header.framecountertx = ntoh2(header.framecountertx);
}

std::string rfmHeader::toString() {
	ntoh();
	std::string r((const char *) &header, sizeof(RFM_HEADER));
	ntoh();
	return r;
}

semtechUDPPacket::semtechUDPPacket() 
{
	prefix.version = 2;
	prefix.token = 0;
	prefix.tag = 0;

	frameCounter = 0;
	
	memset(&deviceEUI, 0, sizeof(DEVUEI));
	memset(&deviceAddr, 0, sizeof(DEVADDR));
	memset(&nwkSKey, 0, sizeof(KEY128));
	memset(&appSKey, 0, sizeof(KEY128));

	memset(&prefix.mac, 0, sizeof(prefix.mac));
}

std::string jsonPackage(
	const std::string &rfmTxPackage
)
{
	std::stringstream ss;
	std::string s = base64_encode(std::string((const char *) rfmTxPackage.c_str(), rfmTxPackage.size()));
	int ms;
	time_t t = time_ms(ms);
	std::string dt = ltimeString(t, ms, "%FT%T") + "Z";	// "2020-12-16T12:17:00.12345Z";

	ss << "{\"rxpk\":[{ \
	\"time\":\""<< dt << "\", \
	\"tmst\":3512348611, \
	\"chan\":0, \
	\"rfch\":0, \
	\"freq\":868.900000, \
	\"stat\":1, \
	\"modu\":\"LORA\", \
	\"datr\":\"SF7BW125\", \
	\"codr\":\"4/6\", \
	\"rssi\":-35, \
	\"lsnr\":5.1, \
	\"size\":" << s.size() << ", \
	\"data\":\"" << s << "\" \
}]}";
	return ss.str();
}

/**
 * prefix  Gateway-MAC-addr RFM-header
 * 0 1 2 3 4 5 6 7 8 9 A B  C                 DAT mic  
 * 0200000000006cc3743eed46 401111111100000000a0ccfb8e
 *                          401111111100000000
 * VEttttTTaaaaaaaaaaaaaaaa   DEVICEAD
 * Version                  40- unconfirmed uplink 
 *   token                            FC    PO
 *       Tag                            Coun
 */
std::string semtechUDPPacket::serialize2RfmPacket()
{
	std::stringstream ss;
	std::string p(payload);

	// direction of frame is up
	unsigned char direction = 0x00;
	unsigned char frameControl = 0x00;
	unsigned char framePort = 0x01;

	// build radio packet, unconfirmed data up macHeader = 0x40;
	// RFM header 9 bytes
	rfmHeader header(deviceAddr, frameCounter, frameControl, framePort);
	ss << header.toString();

	// load data
	// encrypt data
	encryptPayload(p, frameCounter, direction, deviceAddr, appSKey);
	ss << p;

	std::string rs = ss.str();
	// calc MIC
	uint32_t mic = calculateMIC(rs, frameCounter, direction, deviceAddr, nwkSKey);	// nwkSKey
	// load MIC in package
	// mic = ntoh4(mic);
	ss << std::string((char *) &mic, 4);
	return ss.str();
}

std::string semtechUDPPacket::toString()
{
	std::stringstream ss;
	std::string p(payload);

	// prefix 12 bytes, metadata + payload
	ss << std::string((const char *) &prefix, sizeof(SEMTECH_LORA_PREFIX))
		<< jsonPackage(serialize2RfmPacket());
	return ss.str();
}

void semtechUDPPacket::setGatewayId(
	const std::string &value
) {
	setMAC(prefix.mac, value);
}

void semtechUDPPacket::setDeviceEUI(
	const std::string &value
) {
	setMAC(deviceEUI, value);
}

void semtechUDPPacket::setDeviceAddr(
	const std::string &value
) {
	setAddr(deviceAddr, value);
}

void semtechUDPPacket::setNetworkSessionKey(
	const std::string &value
) {
	setKey(nwkSKey, value);
}

void semtechUDPPacket::setApplicationSessionKey(
	const std::string &value
) {
	setKey(appSKey, value);
}

void semtechUDPPacket::setFrameCounter(
	uint16_t value
) {
	frameCounter = value;
}

/**
 * @brief constructs a LoRaWAN package and sends it
 * @param data pointer to the array of data that will be transmitted
 * @param dataLength bytes to be transmitted
 * @param frameCounterUp  frame counter of upstream frames
 * @param devAddr 4 bytes long device address
 * @param nwkSkey 128 bits network key
 * @param appSkey 128 bits application key
 */
std::string loraDataJson(
	std::string &data, 
	unsigned int frameCounterTx,
	DEVADDR &devAddr,
	KEY128 &nwkSKey,
	KEY128 &appSKey
)
{
	unsigned char i;

	// direction of frame is up
	unsigned char direction = 0x00;

	unsigned char rfmData[64];
	unsigned char rfmPackageLength;

	uint32_t MIC;

	unsigned char frameControl = 0x00;
	unsigned char framePort = 0x01;

	// encrypt data
	encryptPayload(data, frameCounterTx, direction, devAddr, appSKey);

	// build radio packet
	// unconfirmed data up
	unsigned char macHeader = 0x40;

	rfmData[0] = macHeader;

	rfmData[1] = devAddr[3];
	rfmData[2] = devAddr[2];
	rfmData[3] = devAddr[1];
	rfmData[4] = devAddr[0];

	rfmData[5] = frameControl;

	rfmData[6] = (frameCounterTx & 0x00FF);
	rfmData[7] = ((frameCounterTx >> 8) & 0x00FF);

	rfmData[8] = framePort;

	// set current packet length
	rfmPackageLength = 9;

	// load data
	for (i = 0; i < data.size(); i++) {
		rfmData[rfmPackageLength + i] = data[i];
	}

	// Add data Lenth to package length
	rfmPackageLength = rfmPackageLength + data.size();

	// calc MIC
	MIC = calculateMIC(std::string((char *) rfmData, rfmPackageLength), frameCounterTx, direction, devAddr, nwkSKey);

	// load MIC in package
	memcpy(&rfmData + rfmPackageLength, &MIC, 4);

	// add MIC length to RFM package length
	rfmPackageLength = rfmPackageLength + 4;

	// make JSON package
	return jsonPackage(std::string((char *)rfmData, rfmPackageLength));
}

/**
 * @see https://stackoverflow.com/questions/10324/convert-a-hexadecimal-string-to-an-integer-efficiently-in-c/11068850
 */

/** 
 * @brief convert a hexidecimal string to a signed long
 * will not produce or process negative numbers except 
 * to signal error.
 * 
 * @param hex without decoration, case insensitive. 
 * 
 * @return -1 on error, or result (max (sizeof(long)*8)-1 bits)
 */
static int hexdec(unsigned char *value) {
	int r;
	if (!*value)
		return 0;
	if (*value >= '0' && *value <= '9') 
		r = *value - '0';
    else
		if (*value >= 'a' && *value <='f')
			r = *value - 'a' + 10;
    	else
			if (*value >= 'A' && *value <='F')
				r = *value - 'A' + 10; 
	r <<= 4;
	value++;
	if (*value) {
		if (*value >= '0' && *value <= '9') 
			r += *value - '0';
		else
			if (*value >= 'a' && *value <='f')
				r += *value - 'a' + 10;
			else
				if (*value >= 'A' && *value <='F')
					r += *value - 'A' + 10; 
	}
    return r;
}

void setKey(
	KEY128 &value,
	const std::string &strvalue
) {
	if (strvalue.size() < sizeof(KEY128) * 2)
		return;
	unsigned char *s = (unsigned char *) strvalue.c_str();
	for (int i = 0; i < sizeof(KEY128); i++) {
		value[i] = hexdec(s);
		s += 2;
	}
}

void setMAC(
	DEVUEI &value,
	const std::string &strvalue
) {
	memset(value, 0, sizeof(DEVUEI));
	if (strvalue.size() < sizeof(DEVUEI) * 2)
		return;
	unsigned char *s = (unsigned char *) strvalue.c_str();
	for (int i = 0; i < sizeof(DEVUEI); i++) {
		value[i] = hexdec(s);
		s += 2;
	}
}

void setAddr(
	DEVADDR &value,
	const std::string &strvalue
) {
	memset(value, 0, sizeof(DEVADDR));
	if (strvalue.size() < sizeof(DEVADDR) * 2)
		return;
	unsigned char *s = (unsigned char *) strvalue.c_str();
	for (int i = 0; i < sizeof(DEVADDR); i++) {
		value[i] = hexdec(s);
		s += 2;
	}
}

std::string key2string(
	const KEY128 &value
) {
	std::stringstream ss;
	ss << std::hex << std::setw(2) << std::setprecision(2) << std::setfill('0');
	for (int i = 0; i < sizeof(KEY128); i++) {
		ss << (unsigned int)  value[i];
	}
	return ss.str();
}

std::string deviceEui2string(
	const DEVUEI &value
) {
	std::stringstream ss;
	ss << std::hex << std::setw(2) << std::setprecision(2) << std::setfill('0');
	for (int i = 0; i < sizeof(DEVUEI); i++) {
		ss << (unsigned int) value[i];
	}
	return ss.str();
}
