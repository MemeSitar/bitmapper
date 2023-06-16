# Bitmapper  

My small tool for databending bitmaps.  
The initial goals for the tool are:  
- [x] reading the bitmap header
- [ ] stripping (and separately saving) the bitmap header
- [ ] checking the validity (size) of a header-less file and
- [ ] adding back the header

## Notes
The following command compiles the bitmapper for windows: (it's statically linked) `x86_64-w64-mingw32-g++ bitmapper.c`