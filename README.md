# qginx

## Introduction
This is a high performance web server. The purpose of this project is to learn about Nginx and implement a simpler version similar with it.

## Development Roadmap
- Version 0: A single-processing and modularized web server. It can process static and dynamic requests. 
- Version 1: A multi-processing web server. It uses Epoll as I/O event notification mechanism, and processes events asynchronously.

## Benchmarking
1. Result of Version 0 (single-processing web server) using *wrk*

Running 10s test @ http://127.0.0.1:8080\
20 threads and 600 connections\
Thread Stats   Avg      Stdev     Max   +/- Stdev\
Latency    12.93ms   55.94ms   1.68s    99.76%\
Req/Sec    91.76     30.30   262.00     87.50%\
4190 requests in 10.08s, 1.10MB read\
Socket errors: connect 0, read 0, write 0, timeout 2\
Requests/sec:    415.50\
Transfer/sec:    111.58KB\

2. Result of Version 1 using *wrk*
