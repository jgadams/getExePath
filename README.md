This project contains a small header file you can include to discover the path of the currently running executable on Windows platforms. This is similar to /proc/self/exe on Linux systems. It is basically a front end to GetModuleFileName(). You can choose to retrieve the path of the executable file, the directory containing the file, the parent of the directory containing the file, or the drive the file is in.