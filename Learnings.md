# **Learnings**

<quote> <i>By the end of this project, we should be able to answer all the following questions without the help of Google</i> </quote>

- ### **What is a program break**

The program break, also known as **brk** or **sbrk** value, is related of course to memory <br>
The program break represent the boundary, the frontier, between the data segment and the heap segment of a process's virtual memory space.
<br>
Of course, if for some reason, your program doesn't need a "heap" to work, the program break then marks the end of the **data segment** <br>
<br>
**Data segment** => section of memory which stores: global & static variables, and initialized data. <br>
**Heap segment** => region of memory used for dynamic memory allocation (malloc family), where the program can request and/or release memory as needed.
<br>
Any time we perform some malloc-kungfu-stuff, we **extend the program break**

That point of program break, acts as a marker, delimiter pointer that specifies the end of the heap. It indicates you the highest address in the process's virtual memory space that has been allocated for the heap. <br>
In other words, any address beyond that "marker" pointer, will shoot you back the famous "Segfault" if you try to access to it, it is considered unallocated or free. <br>

<strong> C language trick time => we can use the sbrk(0) syscall to retrieve the current program break value.</strong>
