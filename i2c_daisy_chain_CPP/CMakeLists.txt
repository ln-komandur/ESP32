# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS
	    i2c_mast_handler.cpp
	    PCF8574_IO_Expander.cpp         # list the source files of this component
	    PCF8574_device.cpp  
	    main.cpp 
    INCLUDE_DIRS        . # optional, add here public include directories
    PRIV_INCLUDE_DIRS   # optional, add here private include directories
    REQUIRES            # optional, list the public requirements (component names)
    PRIV_REQUIRES       # optional, list the private requirements
)
target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++11)
