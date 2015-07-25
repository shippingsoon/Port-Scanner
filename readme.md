Port Scanner
=======

A demonstration of a multithreaded port scanner in several languages. For the sake of brevity some error handling was omitted.


<h4>c-scan:</h4>

To install/run on Linux:<br/>
```
gcc c-scan.c -o bin/c-scan -lpthread
chmod +x bin/c-scan
./bin/c-scan
```

```
Usage: c-scan [-h host] [-t timeout] [-v version]
```
<h4>bash-scan:</h4>

```
Usage: ./bash-scan shippingsoon.com 80 90
```
