mkdir -p $1/reactos
mkdir -p $1/reactos/system32
mkdir -p $1/reactos/system32/drivers
mkdir -p $1/reactos/bin
cp ntoskrnl/ntoskrnl.exe $1
cp services/fs/vfat/vfatfsd.sys $1
cp services/dd/ide/ide.sys $1
cp services/dd/keyboard/keyboard.sys $1/reactos/system32/drivers
cp services/dd/blue/blue.sys $1/reactos/system32/drivers
cp apps/shell/shell.exe $1/reactos/system32
cp lib/ntdll/ntdll.dll $1/reactos/system32
cp lib/kernel32/kernel32.dll $1/reactos/system32
cp lib/crtdll/crtdll.dll $1/reactos/system32
cp lib/fmifs/fmifs.dll $1/reactos/system32
cp lib/gdi32/gdi32.dll $1/reactos/system32
cp apps/hello/hello.exe $1/reactos/bin
cp apps/args/args.exe $1/reactos/bin
cp apps/bench/bench-thread.exe $1/reactos/bin
cp apps/cat/cat.exe $1/reactos/bin
cp subsys/smss/smss.exe $1/reactos/system32
cp subsys/win32k/win32k.sys $1/reactos/system32/drivers
