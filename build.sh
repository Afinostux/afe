
SOURCES="main.cpp"
OBJECTS=""
INCLUDEDIRS="./ include/"
LIBDIRS="./ /usr/lib/"
LIBS="m GL GLU SDL2 SDL2main stdc++"
flags="-std=c++11 -g -fdiagnostics-show-option"
ldflags=""

#for dir in $SOURCEDIRS
#do 
#	for file in "$dir*.cpp"; do SOURCES="$SOURCES $file"; done
#	#for file in "$dir*.c"; do CSOURCES="$CSOURCES $file"; done
#done

for s in $INCLUDEDIRS; do flags="$flags -I$s"; done
for file in $SOURCES 
do 
	oname=${file%.*}.o
	#echo clang $flags $file -c
	clang $flags $file -c
	OBJECTS="$OBJECTS $oname"
done

#for file in $CSOURCES 
#do 
#	oname=${file%.*}.o
#	echo clang $flags $file -c
#	clang $flags $file -c
#	OBJECTS="$OBJECTS $oname"
#done

for d in $LIBDIRS; do ldflags="$ldflags -L$d"; done
for l in $LIBS; do ldflags="$ldflags -l$l"; done

#echo clang $flags $OBJECTS -o space.out
clang $ldflags $OBJECTS -o space.out
rm $OBJECTS
