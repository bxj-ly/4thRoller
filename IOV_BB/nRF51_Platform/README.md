####Build to image files：

``
Platform/Project/device.uvprojx

Roller: Added I2C support
You need to use the latest nRF pack(3.1.0)
Change nrf_drv_config.h
#define TWI0_ENABLED 0 =====> #define TWI0_ENABLED 1
``

####Build to .lib files:
``
Interface/Project/juma_sdk.uvprojx
``

####Python Building tools:
The SDK will also provide a .lib for user's app to build with. It is automatically generated using Python.

Following the steps below to build the platform, interface and test with Examples

1. run 'python update.py' in 'Interface' folder. 
2. open and build
``
Interface/Project/juma_sdk.uvprojx
``
3. open SDK and build
``
Platform/Project/device.uvprojx
``

####Build Roller
To folder 'Product\Projects\Roller and build app.
Voila!

