## Example of a web server using Pico SDK and lwIP
This example was described in more detail in a blog post: https://krzmaz.github.io/2022-08-15-creating-a-web-server-on-raspberry-pi-pico-w-using-pico-sdk-and-lwip/  
In my testing it reached ~20x performance of the MicroPython implementation from point 3.9.1 of https://datasheets.raspberrypi.com/picow/connecting-to-the-internet-with-pico-w.pdf

If you want to test this example with your html files, just add/edit them in the `src/fs` directory and rebuild the project! 

---
### Dependencies:
- CMake 3.19+
- ARM GNU toolchain
- C++ toolchain for host architecture to build the makefsdata utility

For the first two you can refer to [these instructions](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf#%5B%7B%22num%22%3A39%2C%22gen%22%3A0%7D%2C%7B%22name%22%3A%22XYZ%22%7D%2C115%2C841.89%2Cnull%5D)

---

### One time setup
```bash
cp cmake/credentials.cmake.example cmake/credentials.cmake
# <fill in your credentials>
```
### Building
```bash
./build.sh
```

### Flashing
The built USB Flashing Format file will be located in `build/src/pico_w_webserver.uf2` - just copy it over to the Pico W to flash it!

### SSI
Now the example uses Server Side Includes (SSI) to introduce dynamic data into the HTML responses.  
As per documentation:
> SSI-enabled pages must have one of the predefined SSI-enabled file extensions

Which means that no cost should be introduced in non SSI pages

## FS data
The `my_fsdata.c` file containing the HTML files is generated using [makefsdata](https://github.com/lwip-tcpip/lwip/tree/master/src/apps/http/makefsdata) executable compiled for the host machine.  
It works on Mac in my testing, but the file only has `#ifdef` clauses for `__linux__` and `WIN32`,
so we trick it into compiling by defining `__linux__` on Mac, which is generally a bad idea, but it was
quicker to implement than creating patching logic.
### References
- https://www.nongnu.org/lwip/2_1_x/group__httpd.html
- https://github.com/lwip-tcpip/lwip/tree/master/contrib/examples/httpd
