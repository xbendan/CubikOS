#include <macros.h>

#define FIS_TYPE_REG_H2D 0x27   // Register FIS - host to device
#define FIS_TYPE_REG_D2H 0x34   // Register FIS - device to host
#define FIS_TYPE_DMA_ACT 0x39   // DMA activate FIS - device to host
#define FIS_TYPE_DMA_SETUP 0x41 // DMA setup FIS - bidirectional
#define FIS_TYPE_DATA 0x46      // Data FIS - bidirectional
#define FIS_TYPE_BIST 0x58      // BIST activate FIS - bidirectional
#define FIS_TYPE_PIO_SETUP 0x5F // PIO setup FIS - device to host
#define FIS_TYPE_DEV_BITS 0xA1  // Set device bits FIS - device to host

struct FisRegHost2Device
{
    uint8_t fis_type; // FIS_TYPE_REG_H2D

    uint8_t pmport : 4;        // Port multiplier
    uint8_t __reserved__0 : 3; // Reserved
    uint8_t c : 1;             // 1: Command, 0: Control

    uint8_t command;  // Command register
    uint8_t featurel; // Feature register, 7:0

    // DWORD 1
    uint8_t lba0;   // LBA low register, 7:0
    uint8_t lba1;   // LBA mid register, 15:8
    uint8_t lba2;   // LBA high register, 23:16
    uint8_t device; // Device register

    // DWORD 2
    uint8_t lba3;     // LBA register, 31:24
    uint8_t lba4;     // LBA register, 39:32
    uint8_t lba5;     // LBA register, 47:40
    uint8_t featureh; // Feature register, 15:8

    // DWORD 3
    uint8_t countl;  // Count register, 7:0
    uint8_t counth;  // Count register, 15:8
    uint8_t icc;     // Isochronous command completion
    uint8_t control; // Control register

    // DWORD 4
    uint8_t __reserved__1[4]; // Reserved
} __attribute__((packed));

struct FisRegDevice2Host
{
    uint8_t fis_type; // FIS_TYPE_REG_D2H

    uint8_t pmport : 4;        // Port multiplier
    uint8_t __reserved__0 : 2; // Reserved
    uint8_t i : 1;             // Interrupt bit
    uint8_t __reserved__1 : 1; // Reserved

    uint8_t status; // Status register
    uint8_t error;  // Error register

    // DWORD 1
    uint8_t lba0;   // LBA low register, 7:0
    uint8_t lba1;   // LBA mid register, 15:8
    uint8_t lba2;   // LBA high register, 23:16
    uint8_t device; // Device register

    // DWORD 2
    uint8_t lba3;          // LBA register, 31:24
    uint8_t lba4;          // LBA register, 39:32
    uint8_t lba5;          // LBA register, 47:40
    uint8_t __reserved__2; // Reserved

    // DWORD 3
    uint8_t countl;           // Count register, 7:0
    uint8_t counth;           // Count register, 15:8
    uint8_t __reserved__3[2]; // Reserved

    // DWORD 4
    uint8_t __reserved__4[4]; // Reserved
} __attribute__((packed));

struct FisDataPack
{
    // DWORD 0
	uint8_t  fis_type;	// FIS_TYPE_DATA
 
	uint8_t  pmport:4;	// Port multiplier
	uint8_t  __reserved__0:4;		// Reserved
 
	uint8_t  __reserved__1[2];	// Reserved
 
	// DWORD 1 ~ N
	uint32_t data[1];	// Payload
} __attribute__((packed));

struct FisPioSetup
{
    // DWORD 0
	uint8_t  fis_type;	// FIS_TYPE_PIO_SETUP
 
	uint8_t  pmport:4;	// Port multiplier
	uint8_t  __reserved__0:1;		// Reserved
	uint8_t  d:1;		// Data transfer direction, 1 - device to host
	uint8_t  i:1;		// Interrupt bit
	uint8_t  __reserved__1:1;
 
	uint8_t  status;		// Status register
	uint8_t  error;		// Error register
 
	// DWORD 1
	uint8_t  lba0;		// LBA low register, 7:0
	uint8_t  lba1;		// LBA mid register, 15:8
	uint8_t  lba2;		// LBA high register, 23:16
	uint8_t  device;		// Device register
 
	// DWORD 2
	uint8_t  lba3;		// LBA register, 31:24
	uint8_t  lba4;		// LBA register, 39:32
	uint8_t  lba5;		// LBA register, 47:40
	uint8_t  __reserved__2;		// Reserved
 
	// DWORD 3
	uint8_t  countl;		// Count register, 7:0
	uint8_t  counth;		// Count register, 15:8
	uint8_t  __reserved__3;		// Reserved
	uint8_t  e_status;	// New value of status register
 
	// DWORD 4
	uint16_t tc;		// Transfer count
	uint8_t  __reserved__4[2];	// Reserved
} __attribute__((packed));