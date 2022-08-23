#include <x86_64/pci.h>

void PCI_Initialize()
{

}

void PCI_MsiCapSetData(pci_msicap_t *msiCap, uint32_t dat)
{
    if (msiCap->msiControl & PCI_CAP_MSI_CONTROL_64)
    {
        msiCap->data64 = dat;
    }
    else
    {
        msiCap->data = dat;
    }
}

uint32_t PCI_MsiCapGetData(pci_msicap_t *msiCap)
{
    if (msiCap->msiControl & PCI_CAP_MSI_CONTROL_64)
    {
        return msiCap->data64;
    }
    else
    {
        return msiCap->data;
    }
}

void PCI_MsiCapSetAddress(pci_msicap_t *msiCap, int cpu)
{
    msiCap->addressLow = PCI_CAP_MSI_ADDRESS_BASE | (((uint32_t) cpu) << 12);
    if (msiCap->msiControl & PCI_CAP_MSI_CONTROL_64)
    {
        msiCap->addressHigh = 0;
    }
}

void PCI_SetDevice(pci_device_t *info, uint8_t bus, uint8_t slot, uint8_t func);
void PCI_SetDeviceFromInfo(const pci_info_t *info);

void PCI_ReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
uint32_t PCI_ConfigReadDword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
uint16_t PCI_ConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
void PCI_ConfigWriteWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint16_t data);

uint8_t PCI_ConfigReadByte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
void PCI_ConfigWriteByte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint8_t data);

bool PCI_CheckDevice(uint8_t bus, uint8_t device, uint8_t func);
bool PCI_FindDevice(uint16_t deviceID, uint16_t vendorID);
bool PCI_FindGenericDevice(uint16_t classCode, uint16_t subclass);

void PCI_EnumerateDevice(uint16_t deviceID, uint16_t vendorID, void(*func)(const pci_info_t *info));
void PCI_EnumerateGenericDevice(uint8_t classCode, uint8_t subclass, void(*func)(const pci_info_t *info));