#pragma once
#include <ntddk.h>

#define NTDEVICE_NAME_STRING      L"\\Device\\mydriver"
#define SYMBOLIC_NAME_STRING      L"\\DosDevices\\mydriver"
#define DBG_PREFIX "[MYDRIVER]: "


typedef struct _MYDRIVER_DEVICE_EXTENSION{
	PDRIVER_OBJECT pDriverObject;


}MYDRIVER_DEVICE_EXTENSION, * PMYDRIVER_DEVICE_EXTENSION;

NTSTATUS DriverEntry(
	PDRIVER_OBJECT pDriverObject,
	PDEVICE_OBJECT pDeviceObject
);
NTSTATUS InitDriver(
	PDRIVER_OBJECT pDriverObject,
	PDEVICE_OBJECT pDeviceObject
);
NTSTATUS
MJDispatch(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp
);


VOID
DriverUnload(
	PDRIVER_OBJECT DriverObject
);

extern PMYDRIVER_DEVICE_EXTENSION g_pMyDeviceExtension;