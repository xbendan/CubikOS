#include <macros.h>

enum AcpiTableNameDefinition
{
    // AcpiMultiDescriptionTable = "MADT"
};

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
} __attribute__((packed)) acpi_rsdp2_t;

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
    uint64_t ptr[]; // (table.length - sizeof(table)) / 4
} acpi_rsdt_t;

typedef struct AcpiExtendedSdt
{
    struct AcpiSystemDescTable table;
    uint64_t ptr[]; // (table.length - sizeof(table)) / 8
} acpi_xsdt_t;

void acpi_load();
void acpi_find_table(const char* str);