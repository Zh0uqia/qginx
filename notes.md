# Notes
## Problems and solutions
1. The benchmarking tool *wrk* has socket errors and the browser does not show the content sent by the server.

**Solution:** The header of response should contain *Content-Length* to let the client know how long is the content.

2. Errors like *Segment fault* are raised when *char array* is returned by a function.

**Solution:** *Char array* is stack initialized character array, and it is not dynamic allocated. When a function ends, the stack is recycled when the function ends. The pointer will no longer point to the array. Should always use *malloc()* or *std::string* as return value.



# Segmentfault debug tips
1. Add flag `-g` when compling, and change the size of `core` file. This step should be done again everytime when the machine restarts.
```
$ ulimit -c
$ ulimit -c unlimited
```

2. Open *gdb*, type `bt` to check the call stack by *rbp* and *rsp*. If the return address of functions are overwritten due to overflow, can check the call stack by 
```
$ seg logging on
$ x /2000a $rsp
```
*x* stands for *exam*.

3. If the *.core* file is incomplete, get the code where the program stopped by 
```
$ dmesg | grep a.out
```
Check the file name and line number by converting the address
```
$ addr2line -f -e ~/a.out 000000020052c
```

4. Use *addressSanitizer*
Add macro 
```
$ export ASAN_OPTIONS=symbolize=1
$ export LSAN_OPTIONS=verbosity=1:log_threads=1
```
When compling, add
```
-fPIC -fsanitize=address -fno-omit-frame-pointer -lasan
```
After obtaining the call stack, use *addr2line* to locate the code.


