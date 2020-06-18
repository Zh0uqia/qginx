# qginx

## Introduction
This is a high performance web server. The purpose of this project is to learn from [Nginx](https://github.com/nginx/nginx) and implement a simpler version similar with it.

## OS
- Ubuntu 18.04

## Development Roadmap
- Version 0: A single-processing and modularized web server. It can process static and dynamic requests. 
- Version 1: A multi-processing web server. It uses Epoll as I/O event notification mechanism, and processes events asynchronously.
Also, I studied [Proxygen](https://github.com/facebook/proxygen), the C++ HTTP abstractions used at Facebook, and learn from its design pattern. 

## Benchmarking
I compared my server with the state-of-the-art, i.e. nginx. Here is the benchmark of 4 threads and 100 connections in 10 seconds.
It is found that my server is slightly faster than Nginx in intensive traffic.

- My server:
```
➜  ~ wrk -t4 -c20 -d10s http://127.0.0.1:8080/index.html
Running 10s test @ http://127.0.0.1:8080/index.html
4 threads and 20 connections
Thread Stats   Avg      Stdev     Max   +/- Stdev
Latency   342.44us   35.68us   2.49ms   77.12%
Req/Sec    14.57k     1.30k   30.13k    90.55%
582868 requests in 10.10s, 126.18MB read
Requests/sec:  57713.89
Transfer/sec:     12.49MB
```

- Nginx:
```
➜  ~ wrk -t4 -c20 -d10s http://127.0.0.1:3200/index.html
Running 10s test @ http://127.0.0.1:3200/index.html
4 threads and 20 connections
Thread Stats   Avg      Stdev     Max   +/- Stdev
Latency   406.54us  135.33us   8.06ms   98.67%
Req/Sec    12.34k   577.08    13.57k    67.82%
495871 requests in 10.10s, 181.57MB read
Requests/sec:  49099.03
Transfer/sec:     17.98MB
```

## Tech blogs
Here are tech blogs related to this project:
[Nginx source code review](https://qycode.me/blog/2020/05/Nginx-notes/)
[Proxygen source code review](https://qycode.me/blog/2020/06/Proxygen-note/)
