#include <macros.h>

enum acpi_table_name_definition
{
    AcpiMultiDescriptionTable = 0 // MADT
};

typedef struct acpi_root_system_desc_pointer
{
    char signature[8];
    uint8_t checksum;
    char oem_id[6];
    uint8_t revision;
    uint32_t rsdt_ptr;
} __attribute__((packed)) acpi_rsdp_t;

typedef struct acpi_root_system_desc_pointer2
{
    acpi_rsdp_t rsdp_part;
    uint32_t length;
    uint64_t xsdt_pointer;
    uint8_t checksum_extended;
    uint8_t reserved[3];
} __attribute__((packed)) acpi_rsdp2_t;

typedef struct acpi_system_desc_table
{
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} acpi_sdt_t;

typedef struct acpi_root_sdt
{
    struct acpi_system_desc_table table;
    uint64_t ptr[];
} acpi_rsdt_t;

typedef struct acpi_extended_sdt
{
    struct acpi_system_desc_table table;
    uint64_t ptr[];
} acpi_xsdt_t;

void acpi_load();
void acpi_find_table(const char* str);