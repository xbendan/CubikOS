namespace Time {
    typedef struct Date{
        uint8_t second;
        uint8_t minute;
        uint8_t hour;
        uint8_t weekday;
        uint8_t day;
        uint8_t month;
        uint16_t year;
    } date_t;

    /**
     * This is the initial method to read the time
     * It reads from the RTC (Real-Time Clock)
     * Don't call this function after the kernel fully started.
     * RTC might be not exactly correct. Call APIC instead.
     */
    Date Now_RTC();

    /**
     * This method read the time from the most stable way
     * It's decided by current environment.
     * Priority (Highest to Lowest):
     * - Internal
     * - APIC
     * - RTC
     */
    Date Now();
}