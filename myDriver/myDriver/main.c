#include "main.h"



PMYDRIVER_DEVICE_EXTENSION g_pMyDeviceExtension;


NTSTATUS
MJDispatch(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp
) 
{
	UNREFERENCED_PARAMETER(DeviceObject);
	NTSTATUS status = STATUS_SUCCESS;
	PIO_STACK_LOCATION irpStack;
	

	irpStack = IoGetCurrentIrpStackLocation(Irp);


	switch (irpStack->MajorFunction)
	{
	case IRP_MJ_CREATE:
		KdPrint((DBG_PREFIX" IRP_MJ_CREATE\n"));
		break;
	case IRP_MJ_CLOSE:
		KdPrint((DBG_PREFIX" IRP_MJ_CLOSE\n"));
		break;
	case IRP_MJ_CLEANUP:
		KdPrint((DBG_PREFIX" IRP_MJ_CLEANUP\n"));
		break;
	default:
		break;
	}


	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;

}

VOID
DriverUnload(
	PDRIVER_OBJECT DriverObject
)
{
	KdPrint((DBG_PREFIX" DriverUnload\n"));
	UNICODE_STRING symName;

	RtlInitUnicodeString(&symName, SYMBOLIC_NAME_STRING);
	IoDeleteSymbolicLink(&symName);
	IoDeleteDevice(DriverObject->DeviceObject);

	return;
}


NTSTATUS InitDriver(
	PDRIVER_OBJECT pDriverObject,
	PDEVICE_OBJECT pDeviceObject
)
{


	NTSTATUS status = STATUS_UNSUCCESSFUL;
	UNICODE_STRING deviceName;
	UNICODE_STRING symName;
	int i = 0;
	UNREFERENCED_PARAMETER(pDriverObject);
	UNREFERENCED_PARAMETER(pDeviceObject);

	

	RtlInitUnicodeString(&deviceName, NTDEVICE_NAME_STRING);
	RtlInitUnicodeString(&symName, SYMBOLIC_NAME_STRING);

	status = IoCreateDevice(pDriverObject,
		sizeof(MYDRIVER_DEVICE_EXTENSION),
		&deviceName,
		FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN,
		FALSE,
		&pDeviceObject);
	if (!NT_SUCCESS(status))
	{
		KdPrint((DBG_PREFIX"IoCreateDevice fail %x\n", status));
		return status;
	}

	for (i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		pDriverObject->MajorFunction[i] = MJDispatch;

	}

	pDriverObject->DriverUnload = DriverUnload;
	g_pMyDeviceExtension = pDeviceObject->DeviceExtension ;
	g_pMyDeviceExtension->pDriverObject = pDriverObject;



	status = IoCreateSymbolicLink(&symName, &deviceName);
	if (!NT_SUCCESS(status))
	{
		IoDeleteDevice(pDeviceObject);

		KdPrint((DBG_PREFIX"IoCreateSymbolicLink fail %x\n", status));
		return status;
	}

	pDeviceObject->Flags |= DO_BUFFERED_IO;

	return status;
}
NTSTATUS DriverEntry(
	PDRIVER_OBJECT pDriverObject, 
	PDEVICE_OBJECT pDeviceObject
)
{
	UNREFERENCED_PARAMETER(pDriverObject);
	UNREFERENCED_PARAMETER(pDeviceObject);
	NTSTATUS status = STATUS_UNSUCCESSFUL;

	KdPrint((DBG_PREFIX"------ DriverEntry  hello! --------\n"));

	status = InitDriver(pDriverObject, pDeviceObject);

	if (!NT_SUCCESS(status))
	{

		KdPrint((DBG_PREFIX"------ fail to load  --------\n"));
	}
	KdPrint((DBG_PREFIX"------ success to load  --------\n"));
	return status;

}