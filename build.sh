
SOURCEDIRS="./"
SOURCES=""
CSOURCES=""
OBJECTS=""
INCLUDEDIRS="./ include/"
LIBDIRS="./"
LIBS="m GL GLU SDL2 SDL2main stdc++"
flags="-g -DGLEW_STATIC"

for dir in $SOURCEDIRS
do 
	for file in "$dir*.cpp"; do SOURCES="$SOURCES $file"; done
	#for file in "$dir*.c"; do CSOURCES="$CSOURCES $file"; done
done

for s in $INCLUDEDIRS; do flags="$flags -I$s"; done
for file in $SOURCES 
do 
	oname=${file%.*}.o
	echo clang $flags $file -c
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

for d in $LIBDIRS; do flags="$flags -L$d"; done
for l in $LIBS; do flags="$flags -l$l"; done

echo clang $flags $OBJECTS -o space.out
clang $flags $OBJECTS -o space.out
rm $OBJECTS
