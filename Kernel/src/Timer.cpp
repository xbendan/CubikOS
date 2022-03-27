#include <Timer.h>

static uint64_t currentTick = 0;

void Tick() { currentTick++; }
uint64_t GetTick() { return currentTick++; }