#ifndef BOT_NET_H
#define BOT_NET_H

#include <stdint.h>

namespace bot {
namespace net {

bool init(uint32_t pin_ce, uint32_t pin_csn, const char* local,
		  const char* remote);

bool send(void* buf, int size, bool ack = false);
bool recv(void* buf, int size);

bool avaliable();

} // namespace net
} // namespace bot

#endif
