## Example of a web server using Pico SDK
In my testing it reached 20x performance of the MicroPython implementation from point 3.9.1 of https://datasheets.raspberrypi.com/picow/connecting-to-the-internet-with-pico-w.pdf

### One time setup
```bash
cp cmake/credentials.cmake.example cmake/credentials.cmake
# <fill in your credentials>
```
### Building
```bash
./build.sh
```

### SSI
Now the example uses Server Side Includes (SSI) to introduce dynamic data into the HTML responses.  
As per documentation:
> SSI-enabled pages must have one of the predefined SSI-enabled file extensions

Which means that no cost should be introduced in non SSI pages
### References
- https://www.nongnu.org/lwip/2_1_x/group__httpd.html
- https://github.com/lwip-tcpip/lwip/tree/master/contrib/examples/httpd
