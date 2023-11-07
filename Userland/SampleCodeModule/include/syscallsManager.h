#ifndef SYSCALLSMANAGER_H
#define SYSCALLSMANAGER_H

#include <stdint.h>

//llamados a las syscalls usando int 80h y el id que corresponde a cada rutina de atencion
extern uint64_t sysRead(uint64_t fd, const char * buf, uint64_t len);
extern uint64_t sysWrite(uint64_t fd, const char * buf, uint64_t len);
extern uint64_t sysCustomWrite(uint64_t fd, const char* buf, uint32_t color, uint64_t len);
extern uint64_t sysSetCursorPos(uint64_t x, uint64_t y);
extern uint64_t sysGetTime();
extern uint64_t sysGetScrWidth();
extern uint64_t sysGetScrHeight();
extern uint64_t sysDrawFilledRect(uint32_t color, uint64_t x, uint64_t y, uint64_t width, uint64_t height);
extern uint64_t sysDottedLine(uint32_t color);
extern uint64_t sysVideoRefresh();
extern uint64_t sysClearScreen(uint64_t mode);
extern uint64_t sysPlayBeep();
extern uint64_t sysDrawCustomCharBack(char c, uint64_t color, uint64_t x, uint64_t y);
extern uint64_t sysReadFromBuffer(const char * buf);
extern void call_changeSize(uint64_t newSize);
#endif //SYSCALLSMANAGER_H
