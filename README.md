# Сервер для отдачи статический файлов
 

## Build
```
docker build --no-cache -t http_serv .
docker run -p 80:80 http_serv
```

## Benchmarks

### Nginx
```
 12 threads and 400 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   592.24ms  212.43ms   1.81s    75.30%
    Req/Sec    54.47     23.36   158.00     61.01%

```

### http-server
```
 12 threads and 400 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   643.03ms  157.23ms   1.37s    73.98%
    Req/Sec    50.38     31.08   200.00     71.03%
```