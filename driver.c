#pragma warning (disable : 4024 4047 4100)
#include <ntifs.h>
#include <ntddk.h>

//Use DbgView with Capture Kernel on to view output messages

//define IOCTL Codes
#define IO_GET_MODBASE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x666, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_GET_PROCID CTL_CODE(FILE_DEVICE_UNKNOWN, 0x667, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define MEM_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x668, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

PDEVICE_OBJECT pDeviceObject;
UNICODE_STRING dev, dos;
ULONG procId, modBase;


PLOAD_IMAGE_NOTIFY_ROUTINE ImageLoadCallback(PUNICODE_STRING FullImageName, HANDLE ProcessId, PIMAGE_INFO ImageInfo)
{
	//If the image loaded has ac_client.exe in it, change to whatever the fuck you want 
	if (wcsstr(FullImageName->Buffer, L"ac_client.exe"))
	{

		//When assault cube loads
		DbgPrintEx(0, 0, "Assault Cube Loaded");
		procId = ProcessId;
		modBase = ImageInfo->ImageBase;
	}

	return STATUS_SUCCESS;
}

NTSTATUS CreateCall(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	//Create Call Routine
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS CloseCall(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	//Close Call Routine
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS IoControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	//Deal with Control Code and respond accordingly
	NTSTATUS Status = STATUS_UNSUCCESSFUL;
	ULONG BytesIo = 0;

	PIO_STACK_LOCATION Stack = IoGetCurrentIrpStackLocation(Irp);
	ULONG ControlCode = Stack->Parameters.DeviceIoControl.IoControlCode;

	if (ControlCode == IO_GET_MODBASE)
	{
		PULONG Output = (PULONG)Irp->AssociatedIrp.SystemBuffer;
		*Output = modBase;

		Status = STATUS_SUCCESS;
		BytesIo = sizeof(*Output);
	}
	else if (ControlCode == IO_GET_PROCID)
	{
		PULONG Output = (PULONG)Irp->AssociatedIrp.SystemBuffer;
		*Output = procId;

		Status = STATUS_SUCCESS;
		BytesIo = sizeof(*Output);
	}
	Irp->IoStatus.Status = Status;
	Irp->IoStatus.Information = BytesIo;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject)
{
	//Unload Driver Routine
	DbgPrintEx(0, 0, "Driver Unload Called\n");
	PsRemoveLoadImageNotifyRoutine(ImageLoadCallback);

	return STATUS_SUCCESS;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{	
	DbgPrintEx(0, 0, "Driver Loaded\n");
	pDriverObject->DriverUnload = UnloadDriver;
	PsSetLoadImageNotifyRoutine(ImageLoadCallback);

	RtlInitUnicodeString(&dev, L"\\Device\\simpleioctl");
	RtlInitUnicodeString(&dos, L"\\DosDevices\\simpleioctl");

	IoCreateDevice(pDriverObject, 0, &dev, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDeviceObject);
	IoCreateSymbolicLink(&dos, &dev);

	pDriverObject->MajorFunction[IRP_MJ_CREATE] = CreateCall;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = CloseCall;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IoControl;

	pDeviceObject->Flags |= DO_DIRECT_IO;
	pDeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

	return STATUS_SUCCESS;
}
