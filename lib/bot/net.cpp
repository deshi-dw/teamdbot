#include "net.hpp"

#include "RF24.h"

namespace bot {
namespace net {

static RF24 radio;

bool init(uint32_t pin_ce,
		  uint32_t pin_csn,
		  const char* local,
		  const char* remote) {
	// check local string size.
	const char* local_ptr = local;
	while(local_ptr != nullptr) {
		local_ptr++;
	}
	if(local_ptr - local > 6) {
		return false;
	}

	// check remote string size.
	const char* remote_ptr = remote;
	while(remote_ptr != nullptr) {
		remote_ptr++;
	}
	if(remote_ptr - remote > 6) {
		return false;
	}

	// create and start radio.
	radio = RF24(pin_ce, pin_csn);
	if(! radio.begin()) {
		return false;
	}

	if(! radio.isChipConnected()) {
		return false;
	}

	// configure radio.
	radio.setPALevel(RF24_PA_LOW);
	radio.setDataRate(RF24_250KBPS);

	radio.setAutoAck(true);
	radio.setRetries(5, 5);
	radio.enableDynamicAck();

	radio.setCRCLength(RF24_CRC_8);
	radio.setPayloadSize(32);

	// open pipes.
	radio.openWritingPipe((uint8_t*) remote);
	radio.openReadingPipe(1, (uint8_t*) local);

	// start listening.
	radio.startListening();

	return true;
}

bool send(void* buf, int size, bool ack) {
	// no packets over 32 bytes allowed.
	if(buf == nullptr || size > 32) {
		return false;
	}

	radio.stopListening();

	// write buffer while radio isn't listening. If ack is false, write will not
	// wait for an acknowledgement packet before returning.
	bool acked = radio.write(buf, size, ! ack);

	radio.startListening();

	if(ack) {
		return acked;
	}
	else {
		return true;
	}
}

bool recv(void* buf, int size) {
	if(buf == nullptr || ! radio.available()) {
		return false;
	}

	// TODO handle cases where the buffer is greater than 32 bytes. radio.read()
	//      will take multiple packets from the RX FIFO if there is room in the
	//      buffer. (ie. a size of 64 will read 2 packets)

	if(size < 32) {
		// buffers smaller than 32 bytes will read into a temp buffer first.
		// this is because if we read less than the packets size, the packet
		// will stay in the RX FIFO.
		uint8_t tmp_buf[32];
		radio.read(tmp_buf, sizeof(tmp_buf));

		for(int i = size - 1; i >= 0; i--) {
			((uint8_t*) buf)[i] = tmp_buf[i];
		}
	}
	else {
		// read buffer normally if the buffer is greater than 32.
		radio.read(buf, size);
	}

	return true;
}

bool avaliable() {
	return radio.available();
}

} // namespace net
} // namespace bot
