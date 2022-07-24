## Example of a web server using Pico SDK
In my testing it reached 20x performance of the MicroPython implementation from point 3.9.1 of https://datasheets.raspberrypi.com/picow/connecting-to-the-internet-with-pico-w.pdf

### One time setup
```
cp cmake/credentials.cmake.example cmake/credentials.cmake
# <fill in your credentials>
```
### Building
```
./build.sh
```

