#!/bin/sh
#

x=x
y=y
ds9=ds9
color=green
size=10
shape=circle
append=0

while [ $# -ge 1 ] ; do
    case $1 in 
	x)	x=$2;		shift	;;
	y)	y=$2;		shift	;;
	shape)	shape=$2;	shift	;;
	size)	size=$2;	shift	;;
	width)  size=$2;	shift	;;
	height) height=$2;	shift	;;
	color)	color=$2	shift	;;
	app*)	append=1		;;
	+)	append=1		;;
	*) ds9=$1
    esac

    shift
done

if [ "$width" = "" ] ; then
	width=$size
fi
if [ "$height" = "" ] ; then
	height=$width
fi

case $shape in 
	box)	size="$width, $height"	;;
esac

if [ $append = 0 ] ; then
    xpaset -p $ds9 regions delete all
fi

  table -h '{	print \"'"$shape"'\", '"$x"', '"$y"', '$size' "# color = \"'$color'\"" }'	\
| xpaset $ds9 regions
