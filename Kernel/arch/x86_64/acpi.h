#include <macros.h>

enum AcpiTableNameDefinition
{
    AcpiMultiDescriptionTable = 0
};

typedef struct AcpiHeader
{
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemId[6];
    char oemTableId[8];
    uint32_t oemRevision;
    uint32_t creatorId;
    uint32_t creatorRevision;
} __attribute__((packed)) acpi_header_t;

typedef struct AcpiMadt
{
    acpi_header_t header;
    uint32_t address;
    uint32_t flags;
} __attribute__((packed)) acpi_madt_t;

typedef struct AcpiMadtEntry
{
    uint8_t type;
    uint8_t length;
} __attribute__((packed)) madt_entry_t;

typedef struct MadtLocalApic
{
    madt_entry_t entry;
    uint8_t processorId;
    uint8_t apicId;
    uint32_t flags;
} __attribute__((packed)) madt_local_t;

typedef struct MadtIoApic
{
    madt_entry_t entry;
    uint8_t apicId;
    uint8_t reserved;
    uint32_t address;
    uint32_t gSib;
} __attribute__((packed)) madt_io_t;

typedef struct MadtIso
{
    madt_entry_t entry;
    uint8_t busSource;
    uint8_t irqSource;
    uint32_t gSi;
    uint16_t flags;
} __attribute__((packed)) madt_iso_t;

typedef struct MadtNmi
{
    madt_entry_t entry;
    uint8_t processorId;
    uint16_t flags;
    uint8_t lInt;
} __attribute__((packed)) madt_nmi_t;

struct AcpiGenericAddressStructure
{
    uint8_t address_space;
    uint8_t bit_width;
    uint8_t bit_offset;
    uint8_t access_size;
    uint64_t address;
};

typedef struct AcpiFadt
{
    acpi_header_t header;
    uint32_t firmware_control;
    uint32_t dsdt;

    uint8_t __reserved__0;

    uint8_t ppmp;
    uint16_t sci_interrupt;
    uint32_t smi_cmdport;
    uint8_t acpi_enable;
    uint8_t acpi_disable;
    uint8_t s4bios_req;
    uint8_t pstate_control;
    uint32_t pm1a_event_block;
    uint32_t pm1b_event_block;
    uint32_t pm1a_control_block;
    uint32_t pm1b_control_block;
    uint32_t pm2_control_block;
    uint32_t pmt_timer_block;
    uint32_t gpe0_block;
    uint32_t gpe1_block;
    uint8_t pm1_event_length;
    uint8_t pm1_control_length;
    uint8_t pm2_control_length;
    uint8_t pmt_timer_length;
    uint8_t gpe0_length;
    uint8_t gpe1_length;
    uint8_t gpe1_base;
    uint8_t cstate_control;
    uint16_t worst_c2_latency;
    uint16_t worst_c3_latency;
    uint16_t flush_size;
    uint16_t flush_stride;
    uint8_t duty_offset;
    uint8_t duty_width;
    uint8_t day_alarm;
    uint8_t month_alarm;
    uint8_t century;

    uint16_t boot_arch_flags;
    uint8_t __reserved__1;
    uint32_t flags;

    struct AcpiGenericAddressStructure reset_reg;

    uint8_t reset_value;
    uint8_t __reserved__3[3];

    uint64_t x_firmware_control;
    uint64_t x_dsdt;

    struct AcpiGenericAddressStructure x_pm1a_event_block;
    struct AcpiGenericAddressStructure x_pm1b_event_block;
    struct AcpiGenericAddressStructure x_pm1a_control_block;
    struct AcpiGenericAddressStructure x_pm1b_control_block;
    struct AcpiGenericAddressStructure x_pm2_control_block;
    struct AcpiGenericAddressStructure x_pmt_timer_block;
    struct AcpiGenericAddressStructure x_gpe0_block;
    struct AcpiGenericAddressStructure x_gpe1_block;
} acpi_fadt_t;

typedef struct MadtAddressOverride
{
    madt_entry_t entry;
    uint16_t reserved;
    uint64_t address;
} madt_address_override_t;

struct PciMcfgBaseAddress
{
    uint64_t base;
    uint16_t segment_group_num;
    uint8_t pci_bus_start;
    uint8_t pci_bus_end;
    uint32_t __reserved__;
};

typedef struct PciMcfg
{
    acpi_header_t header;
    uint64_t __reserved__;
    struct PciMcfgBaseAddress bases[];
} pci_mcfg_t;

typedef struct AcpiRootSystemDescPtr
{
    char signature[8];
    uint8_t checksum;
    char oemId[6];
    uint8_t revision;
    uint32_t rsdtPtr;
} __attribute__((packed)) acpi_rsdp_t;

typedef struct AcpiRootSystemDescPtr2
{
    acpi_rsdp_t rsdpPart;
    uint32_t length;
    uint64_t xsdtPtr;
    uint8_t checksumExtended;
    uint8_t reserved[3];
} __attribute__((packed)) acpi_xsdp_t;

typedef struct AcpiSystemDescTable
{
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemId[6];
    char oemTableId[8];
    uint32_t oemRevision;
    uint32_t creatorId;
    uint32_t creatorRevision;
} acpi_sdt_t;

typedef struct AcpiRootSdt
{
    struct AcpiSystemDescTable table;
    uint32_t pointers[]; // (table.length - sizeof(table)) / 4
} acpi_rsdt_t;

typedef struct AcpiExtendedSdt
{
    struct AcpiSystemDescTable table;
    uint64_t pointers[]; // (table.length - sizeof(table)) / 8
} acpi_xsdt_t;

extern uint8_t g_Processors[];
extern int g_ProcessorCount;

void ACPI_Initialize();
void *ACPI_FindTable(const char* str, int index);

void ACPI_SetRsdpTable(acpi_xsdt_t *pointer);
void ACPI_Reset();