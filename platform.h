#ifndef PLATFORM_H
#define PLATFORM_H 1

#ifdef _MSC_VER
#define	SYSLOG(msg)
#define OPENSYSLOG()
#define CLOSESYSLOG()
#else
#include <syslog.h>
#include <sstream>
#define	SYSLOG(msg) { syslog (LOG_ALERT, "%s", msg); }
#define OPENSYSLOG() { setlogmask (LOG_UPTO(LOG_NOTICE)); openlog("semtech_udp_packet_emitter", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1); }
#define CLOSESYSLOG() closelog();
#endif

#ifdef _MSC_VER
#define ALIGN	__declspec(align(1))
#define PACKED	
#else
#define ALIGN	
#define PACKED	__attribute__((aligned(1), packed))
#endif

#if BYTE_ORDER == BIG_ENDIAN
uint16_t ntoh2(uint16_t v) {
	return v;
}
uint32_t ntoh4(uint32_t v) {
	return v;
}
uint64_t ntoh8(uint32_t v) {
	return v;
}
#else
#define ntoh2 be16toh

#define ntoh4 be32toh

#define ntoh8 be64toh

#endif

#endif
