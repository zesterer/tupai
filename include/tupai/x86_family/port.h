#include <stddef.h>
#include <stdint.h>

static inline void outb(uint16_t port, uint8_t value)
{
    asm volatile ("outb %0, %1" :: "a"(value), "Nd"(port));
}

static inline void outw(uint16_t port, uint16_t value)
{
    asm volatile ("outw %0, %1" :: "a"(value), "Nd"(port));
}

static inline void outl(uint16_t port, uint32_t value)
{
    asm volatile ("outl %0, %1" :: "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
	uint8_t value;
	asm volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
	return value;
}

static inline uint16_t inw(uint16_t port)
{
	uint16_t value;
	asm volatile ("inw %1, %0" : "=a"(value) : "Nd"(port));
	return value;
}

static inline uint32_t inl(uint16_t port)
{
	uint32_t value;
	asm volatile ("inl %1, %0" : "=a"(value) : "Nd"(port));
	return value;
}
