#include <x86_64/acpi.h>
#include <x86_64/apic.h>
#include <x86_64/paging.h>
#include <x86_64/interrupts.h>
#include <mm/memory.h>
#include <panic.h>

const char *__acpi_Signature = "RSD PTR ";
uint8_t g_Processors[256];
int g_ProcessorCount = 1;
acpi_rsdp_t *acpiDesc;
acpi_rsdt_t *acpiRsdtHeader;
acpi_xsdt_t *acpiXsdtHeader;
acpi_fadt_t *acpiFadt;
lklist_node_t isos;
pci_mcfg_t *pciMcfg;

char acpiOemId[7];

uintptr_t __acpi_GetEntry(uint32_t index)
{
    if (acpiDesc->revision == 2)
        return acpiXsdtHeader->pointers[index];
    else
        return acpiRsdtHeader->pointers[index];
}

void *ACPI_FindTable(const char *str, int index)
{
    int entries = 0;
    if (!acpiDesc)
    {
        return NULL;
    }

    if (acpiDesc->revision == 2)
        entries = (acpiRsdtHeader->table.length - sizeof(acpi_header_t)) / sizeof(uint64_t);
    else
        entries = (acpiRsdtHeader->table.length - sizeof(acpi_header_t)) / sizeof(uint32_t);

    int _index = 0;
    if (memcmp("DSDT", str, 4) == 0)
        return (void *)VM_GetIOMapping(acpiFadt->dsdt);
        
    for (int i = 0; i < entries; i++)
    {
        acpi_header_t *header = (acpi_header_t *)(VM_GetIOMapping(__acpi_GetEntry(i)));
        if (memcmp(header->signature, str, 4) == 0 && _index++ == index)
            return header;
    }
}

void ACPI_Initialize()
{
    for (uintptr_t addr = 0; addr <= 0x7BFF; addr += 16)
    {
        if (memcmp((void *)VM_GetIOMapping(addr), __acpi_Signature, 8) == 0)
        {
            acpiDesc = (acpi_xsdp_t *)(VM_GetIOMapping(addr));
            goto INIT_ACPI_FOUND;
        }
    }

    for (uintptr_t addr = 0x80000; addr <= 0x9FFFF; addr += 16)
    {
        if (memcmp((void *)VM_GetIOMapping(addr), __acpi_Signature, 8) == 0)
        {
            acpiDesc = (acpi_xsdp_t *)(VM_GetIOMapping(addr));
            goto INIT_ACPI_FOUND;
        }
    }

    for (uintptr_t addr = 0xE0000; addr <= 0xFFFFF; addr += 16)
    {
        if (memcmp((void *)VM_GetIOMapping(addr), __acpi_Signature, 8) == 0)
        {
            acpiDesc = (acpi_xsdp_t *)(VM_GetIOMapping(addr));
            goto INIT_ACPI_FOUND;
        }
    }

    CallPanic("[ERR] No ACPI table found.");

INIT_ACPI_FOUND:

    if(acpiDesc->revision == 2)
    {
        acpiRsdtHeader = ((acpi_rsdt_t *) VM_GetIOMapping(((acpi_xsdp_t *) acpiDesc)->xsdtPtr));
        acpiXsdtHeader = ((acpi_xsdt_t *) VM_GetIOMapping(((acpi_xsdp_t *) acpiDesc)->xsdtPtr));
    }
    else
        acpiRsdtHeader = ((acpi_rsdt_t *) VM_GetIOMapping(acpiDesc->rsdtPtr));

    memcpy(acpiOemId, acpiRsdtHeader->table.oemId, 6);
    acpiOemId[6] = 0;

    acpiFadt = (acpi_fadt_t *)(ACPI_FindTable("FACP", 0));

    DisableInterrupts();
    acpi_madt_t *madt = (acpi_madt_t *)(ACPI_FindTable("APIC", 0));
    uintptr_t madtEnd = ((uintptr_t)madt) + madt->header.length;
    uintptr_t madtEntry = ((uintptr_t)madt) + sizeof(acpi_madt_t);

    while (madtEntry < madtEnd)
    {
        madt_entry_t *entry = (madt_entry_t *)(madtEntry);
        switch (entry->type)
        {
        case 0:
        {
            madt_local_t *apicLocal = (madt_local_t *)(entry);
            if (apicLocal->flags & 0x3)
            {
                if (apicLocal->apicId == 0)
                    break;

                g_Processors[g_ProcessorCount++] = apicLocal->apicId;
            }
            break;
        }
        case 1:
        {
            madt_io_t *apicIo = (madt_io_t *)(entry);

            if (!apicIo->gSib)
                IOAPIC_SetBase(apicIo->address);
        }
        case 2:
        {
            madt_iso_t *iso = (madt_iso_t *)(entry);
            lklist_item_t *item = (lklist_item_t *) KernelAllocateObject(sizeof(struct LinkedListItem));
            item->ptr = iso;
            LinkedListAppend(&isos, &item->listnode);
            break;
        }
        case 3:
        {
            break;
        }
        case 4:
        {
            break;
        }
        case 5:
        {
            break;
        }
        default:
            break;
        }

        madtEntry += entry->length;
    }

    pciMcfg = (pci_mcfg_t *)(ACPI_FindTable("MCFG", 0));

    EnableInterrupts();
}