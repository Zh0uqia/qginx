# Notes
# Nginx source code reading notes
The best way to understand Nginx is by using *gdb*. Start Nginx and attach to a process to see what is going on.

## Strategy for high concurrency
Nginx combines multi-processing and scalable I/O event notification mechanism to handle a large amount of connections and requests. The IO multiplexing techniques it uses depends on the operating system, for example, it uses *Epoll* module on Linux and *kqueue* on FreeBSD. This blog mainly analyze Nginx on Linux.

*Cycle*, *process*, *event*, *epoll* and *http* are important parts. In the *main()* function, a structure *cycle* is initialized and passed to the master and worker process cycle.
```
int ngx_cdecl
main(int argc, char *const *argv)
{
    ngx_buf_t        *b;
    ngx_log_t        *log;
    ngx_uint_t        i;
    ngx_cycle_t      *cycle, init_cycle;
    ngx_conf_dump_t  *cd;
    ngx_core_conf_t  *ccf;
    
...
// initialize cycle
cycle = ngx_init_cycle(&init_cycle);

...

// begin master process, and master process creates worker processes
if (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_cycle(cycle);

    } else {
        ngx_master_process_cycle(cycle);
    }

    return 0;
}
```

### Ngx_cycle_s (cycle structure) initialization
*ngx_config.h* file records important configurations of Nginx, for example, the number of listening sockets, port number, and the number of worker processes. 

During initialization, *ngx_init_cycle()* combines those important information with the *ngx_cycle_s* structure. *ngx_cycle_s* contains important components like *listening* and *connection pool*. The reason that *ngx_cycle_s* records these information is to make processes share them more conveniently (*cycle* is passed as a parameter).
```
struct ngx_cycle_s {
    void                  ****conf_ctx;
    ngx_pool_t               *pool;

    ...
    
// listening array
    ngx_array_t               listening;
    ngx_array_t               paths;

    ngx_array_t               config_dump;
    ngx_rbtree_t              config_dump_rbtree;
    ngx_rbtree_node_t         config_dump_sentinel;

    ngx_list_t                open_files;
    ngx_list_t                shared_memory;

// number of maximum connections
    ngx_uint_t                connection_n;
    ngx_uint_t                files_n;

// connection pool, read events and write events
    ngx_connection_t         *connections;
    ngx_event_t              *read_events;
    ngx_event_t              *write_events;

    ngx_cycle_t              *old_cycle;

    ...
};
```

### Master process
In *ngx_master_process_cycle(cycle)*, the master process starts worker process and cache manager processes. It registers worker process cycle functions, and then starts a *for* loop. This *for* loop accepts instructions from users, like upgrade, and quit. It can guarantee that it will not exit ealier than the worker processes and leave zombie processes.
```
void
ngx_master_process_cycle(ngx_cycle_t *cycle)
{
    char              *title;
    u_char            *p;
    size_t             size;
    ngx_int_t          i;
    ngx_uint_t         n, sigio;
    sigset_t           set;
    struct itimerval   itv;
    ngx_uint_t         live;
    ngx_msec_t         delay;
    ngx_listening_t   *ls;
    ngx_core_conf_t   *ccf;
    
    ...
    
    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    ngx_start_worker_processes(cycle, ccf->worker_processes,
                               NGX_PROCESS_RESPAWN);
    ngx_start_cache_manager_processes(cycle, 0);

    ngx_new_binary = 0;
    delay = 0;
    sigio = 0;
    live = 1;

    for ( ;; ) {
        if (delay) {
            if (ngx_sigalrm) {
                sigio = 0;
                delay *= 2;
                ngx_sigalrm = 0;
            }
            
            ...
            
            if (!live && (ngx_terminate || ngx_quit)) {
            ngx_master_process_exit(cycle);
        }
        
        ...
        
    }
```

In *ngx_start_worker_processes()*, it calls *ngx_spawn_process()* with times the same as the number of worker processes. In *ngx_spawn_process()*, it registers *ngx_worker_process_cycle* function. *ngx_start_worker_processes()* passes *ngx_worker_process_cycle* as a parameter to *ngx_spawn_process()* which is the function to be called.
```
static void
ngx_start_worker_processes(ngx_cycle_t *cycle, ngx_int_t n, ngx_int_t type)
{
...

for (i = 0; i < n; i++) {

        ngx_spawn_process(cycle, ngx_worker_process_cycle,
                          (void *) (intptr_t) i, "worker process", type);

        ch.pid = ngx_processes[ngx_process_slot].pid;
        ch.slot = ngx_process_slot;
        ch.fd = ngx_processes[ngx_process_slot].channel[0];

        ngx_pass_open_channel(cycle, &ch);
    }
}
```

In *ngx_spawn_process()*, it forks worker processes and calls *proc()* in the child processes. *proc()* here is *ngx_worker_process_cycle*, which is the callback function passed as parameter. 
```
ngx_pid_t
ngx_spawn_process(ngx_cycle_t *cycle, ngx_spawn_proc_pt proc, void *data,
    char *name, ngx_int_t respawn)
{
    pid = fork();

        switch (pid) {

        case -1:
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          "fork() failed while spawning \"%s\"", name);
            ngx_close_channel(ngx_processes[s].channel, cycle->log);
            return NGX_INVALID_PID;

        case 0:
            ngx_parent = ngx_pid;
            ngx_pid = ngx_getpid();
            proc(cycle, data);
            break;

        default:
            break;
        }
        
        ...
        
}
```

### Worker processes
In *ngx_worker_process_cycle()*, two functions are important. *ngx_worker_process_init()* initializes worker process, and *ngx_process_events_and_timers* does the jobs of worker processes. In a worker process cycle, it loops *ngx_process_events_and_timers*.
```
static void
ngx_worker_process_cycle(ngx_cycle_t *cycle, void *data)
{
    ngx_int_t worker = (intptr_t) data;

    ngx_process = NGX_PROCESS_WORKER;
    ngx_worker = worker;

    ngx_worker_process_init(cycle, worker);

    ngx_setproctitle("worker process");

    for ( ;; ) {

        ...
        
        ngx_process_events_and_timers(cycle);
        
        ...
    }
}

```

#### Worker process initialization
During worker process initialization, each module's callback function *init_process()* are called. The most important one is the initialization of event module *ngx_event_process_init()*. 


In *ngx_worker_process_init()*, it calls the *init_process()* of each module. The *event* module is the core module and *ngx_event_process_init()* function is the callback function in the *init_process()* of *event* module. 

First, it checks whether *accept_mutex* is set. If yes, worker processes will try to get the accept mutex before getting a connection. If you use *gdb* to print *ecf*, you will find that *accept_mutex* is set to be **off** by default. According to [Nginx official website](http://nginx.org/en/docs/ngx_core_module.html), *accept_mutex* should not be enabled if your kernel supports the *EPOLLEXCLUSIVE* flag because the recent versions of kernel have solved the thundering herd problem.
```
if (ccf->master && ccf->worker_processes > 1 && ecf->accept_mutex) {
        ngx_use_accept_mutex = 1;
        ngx_accept_mutex_held = 0;
        ngx_accept_mutex_delay = ecf->accept_mutex_delay;

    } else {
        ngx_use_accept_mutex = 0;
    }
```

Then it would initialize the connection pool of each process. 
```
cycle->connections =
        ngx_alloc(sizeof(ngx_connection_t) * cycle->connection_n, cycle->log);
    if (cycle->connections == NULL) {
        return NGX_ERROR;
    }

    c = cycle->connections;

    cycle->read_events = ngx_alloc(sizeof(ngx_event_t) * cycle->connection_n,
                                   cycle->log);

...

cycle->write_events = ngx_alloc(sizeof(ngx_event_t) * cycle->connection_n,
                                    cycle->log);
```
Connection pool is actually a connection "linked list". Each connection has *next* field, which points to the next connection. Each connection is also combined with a read event and write event. 
```
i = cycle->connection_n;
    next = NULL;

    do {
        i--;

        c[i].data = next;
        c[i].read = &cycle->read_events[i];
        c[i].write = &cycle->write_events[i];
        c[i].fd = (ngx_socket_t) -1;

        next = &c[i];
    } while (i);

    cycle->free_connections = next;
    cycle->free_connection_n = cycle->connection_n;
```
The free connection is pointed by *cycle->free_connections* and it always points to the head of linked list. If a connection is freed, it would be inserted to the head of the linked list, and the pointer will point to it.

Also, the handler function of all the read events is set to be *ngx_event_accept*. It means that once a connections comes, *accept()* will be called in *ngx_event_accept()*. The *accept* attribute of read event is also set to be true, which means that it is a listening event.

The last step is to add listening event to the epoll. If *accept_mutex* is set to be true, it will add event later. If *EPOLLEXCLUSIVE* is supported, then we need to set the flag to be *NGX_EXCLUSIVE_EVENT* in *ngx_add_event()*.
```
/* If use accept mutex, will add event later, after getting the accept mutex. */
if (ngx_use_accept_mutex) {
            continue;
        }

#if (NGX_HAVE_EPOLLEXCLUSIVE) // yes

        if ((ngx_event_flags & NGX_USE_EPOLL_EVENT)
            && ccf->worker_processes > 1)
        {
            if (ngx_add_event(rev, NGX_READ_EVENT, NGX_EXCLUSIVE_EVENT)
                == NGX_ERROR)
            {
                return NGX_ERROR;
            }

            continue; // jump line 902
        }

#endif

        if (ngx_add_event(rev, NGX_READ_EVENT, 0) == NGX_ERROR) {
            return NGX_ERROR;
        }

```

#### Process events
##### *accept_mutex* set to be off
The listening event will be placed to epoll of every process. However, it will not lead to the thundering herd problem when new connections come, because the kernal will notify only one process.

For Nginx, the connections of each process are more balanced when *accept_mutex* is off. It is because if *accept_mutex* is on, it is very sensitive to *accept_mutex_delay*. [This post](https://www.ruby-forum.com/t/accpet-mutex-cause-nginx-worker-balance-problem/231326/4) explains it very well. 

Also, in the official [tuning tips](https://www.nginx.com/blog/performance-tuning-tips-tricks/), it suggests to turn off *accept_mutex* when connections volume is low.

> We recommend keeping the default value (off) unless you have extensive knowledge of your app’s performance and the opportunity to test under a variety of conditions, but it can lead to inefficient use of system resources if the volume of new connections is low. Changing the value to on might be beneficial under some high loads.

When adding the exclusive event, EPOLLEXCLUSIVE and EPOLLRDHUP flag can not be used together. 
```
#if (NGX_HAVE_EPOLLEXCLUSIVE && NGX_HAVE_EPOLLRDHUP)
    if (flags & NGX_EXCLUSIVE_EVENT) {
        events &= ~EPOLLRDHUP; // events: 8193->1
    }
#endif
```

##### *accept_mutex* set to be on
Every process has a global variable *ngx_accept_mutex_held*. Every worker process tries to get the mutex lock in each loop. In the *ngx_trylock_accept_mutex()* function, if a process gets the lock, and *ngx_accept_mutex_held* is true, it means that this process also got the lock in last round, and the listening *fd* has already been placed into its epoll, so it does not need to place it again. If *ngx_accept_mutex_held* is false, it would place the listening *fd* into its epoll, and set *ngx_accept_mutex_held* variable to be true. 

If a process did not get the lock, and *ngx_accept_mutex_held* is true, it means that it got the lock and placed the listening *fd* in the last round, so it needs to **remove** *fd* out of its epoll, and set *ngx_accept_mutex_held* to be false. If *ngx_accept_mutex_held* is false, it does not need to add or remove the listening *fd*.  

```
ngx_int_t
ngx_trylock_accept_mutex(ngx_cycle_t *cycle)
{
// If got the lock
    if (ngx_shmtx_trylock(&ngx_accept_mutex)) {

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                       "accept mutex locked");

// If held the lock in the last round, then do nothing
        if (ngx_accept_mutex_held && ngx_accept_events == 0) {
            return NGX_OK;
        }

// If did not hold the lock, then enable it
        if (ngx_enable_accept_events(cycle) == NGX_ERROR) {
            ngx_shmtx_unlock(&ngx_accept_mutex);
            return NGX_ERROR;
        }

        ngx_accept_events = 0;
        ngx_accept_mutex_held = 1;

        return NGX_OK;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                   "accept mutex lock failed: %ui", ngx_accept_mutex_held);

// If did not get the lock in this round, but hold the lock in last round, then release the lock
    if (ngx_accept_mutex_held) {
        if (ngx_disable_accept_events(cycle, 0) == NGX_ERROR) {
            return NGX_ERROR;
        }

        ngx_accept_mutex_held = 0;
    }

    return NGX_OK;
}
```

All of worker processes, no matter it gets the lock or not, will get the ready events from their own epoll. If the ready event is a listening event, it would be placed into *ngx_posted_accept_events*. If the ready event is a read/write event, it would be placed into *ngx_posted_events*. All processes would handle the listening events first. After that, the process holding the lock would unlock mutex. Then, all processes would handle the read/write events. 

The reason to categorize the events is that, if the process holding the mutex lock processes all the events before unlocking, the lock will be held by it for a long time, and other processes cannot get the lock in this period of time. Also, the listening events should be dealt with as soon as possible, so that connections will not block the listening socket too long.


#### Load balancing
Every worker process has its own connection pool. The initialization of connection pool is in *ngx_worker_process_init()*. In *conf.h*, the maximum number of connections *connection_n* is set. The maximum number of connections Nginx can handle as a server is *worker_process * connection_n*. 

If a worker process is handling connections more than *7 * connection_n/8*, then it will not try to get the lock.
```
if (ngx_accept_disabled > 0) {
            ngx_accept_disabled--;

} else {
    if (ngx_trylock_accept_mutex(cycle) == NGX_ERROR) {
        return;
}
```
In the accept event handler *ngx_event_accept()*, every time a new connection is got from the free connection pool, the *ngx_accept_disabled* is reset
```
ngx_accept_disabled = ngx_cycle->connection_n / 8
                              - ngx_cycle->free_connection_n;
```
When *free_connection_n* increases, this value is increasing, and when *free_connection_n = 7/8 * connection_n*, this value will be larger than 0.

The second way Nginx uses for load balancing is that, if a process did not get the lock, it will not be blocked until notification comes from epoll because that might cause it to wait forever. It will set the timeout value of *epoll_wait()* to be *ngx_accept_mutex_delay*.
```
else {
    if (timer == NGX_TIMER_INFINITE
                    || timer > ngx_accept_mutex_delay)
    {
        timer = ngx_accept_mutex_delay;
    }
```

### Edge triggered & level triggered
In Nginx, the *accept* event is level triggered, and the read & write events are edge triggered. 

## Problems and solutions
1. TCP connections handled by each process are not balanced.

2. Some TCP connections from *wrk* cannot be received within timeout.

**Solution:** The *backlog* flag of *listen()* should be big enough (e.g >500). This flag is the queue length of established connections waiting to be accepted.

3. The benchmarking tool *wrk* has socket errors and the browser does not show the content sent by the server.

**Solution:** The header of response should contain *Content-Length* to let the client know how long is the content.

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

```
// check 10 memory blocks with binary
x/10x 0x60400000000010

// check 10 instructions with instruction
x/10i 0x60400000000010
```
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
