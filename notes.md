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

# Nginx source code read notes
## Strategy for high concurrency
Nginx combines multi-processing and Epoll to handle a large amount of connections and requests. 

### Listening sockets
*ngx_conf.h* file records important configurations of Nginx, for example, the number of listening sockets, port number, and the number of worker processes. 

During initialization, *ngx_init_cycle()* combines those important information with the *ngx_cycle_s* structure. *ngx_cycle_s* includes information like *listening* and *connection*. The reason that *ngx_cycle_s* records these information is to make processes share them more conveniently (*cycle* is passed as a parameter).

### Master process
Master process forks several worker processes. It registers callback function of worker process cycle and binds the function with a worker process object <>. If current process is a child process, a worker process cycle would be called. 

Master process waits all the child processes to exit <>. It is because we should free/delete the dynamic memory like cycle, listening and connection after all worker process exit. 

Master process also creates a shared memory for mutex. In Nginx, it implements its own locks, including atomic lock and file lock. Atomic lock is a shared memory among child processes.

### Worker process
In the constructor of worker process, we *new* the read event and write event of connection, and then add those events into Epoll. Each process has its own epollFD. We should write the initialization in the *constructor*, so that the dynamic memory can be freed or deleted by the *destructor*.
