# Include What You Use

Used for determine what include should be removed (not used) and what include can be used as precompiled headers.  

See [include-what-you-use](https://github.com/include-what-you-use/include-what-you-use)
  
## How to Install

* Clone or download this repository
* Add to paths  
  * `include-what-you-use/linux/iwyu_0.14_clang10/bin`
  * `include-what-you-use/linux/iwyustat`

## How to use

* Turn ON cmake option `CMAKE_EXPORT_COMPILE_COMMANDS`
* Make rebuild project
* Exec script `MuseScore/tools/checkheaders/iwyu.sh path/to/build_dir iwyu_includes.txt` 
   
### Remove unnecessary includes

* Look at `iwyu_includes.txt` and remove what is recommended to be removed.
* Be sure to check the compilation

### Update precompiled headers

* Exec `iwyustat path/to/iwyu_includes.txt`
* Copy output about use `std` and `Qt` to `MuseScore/build/pch/pch.h`

