#!/bin/bash


RunGame=false
RunGDB=false
RunValgrind=false
ClearCompileOutput=false
ForceFullCompile=false

CompileString="clang++ \
	main.cpp \
	Timer.cpp \
	GamePiece.cpp \
	Tile.cpp \
	Ball.cpp \
	Paddle.cpp \
	Renderer.cpp \
	GameManager.cpp \
	math/Vector2f.cpp \
	math/Rect.cpp  \

	-lSDL2 \
	-lSDL2_ttf \
	-std=c++11 \

	O3 \
	-Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded \
	-Wno-switch-enum -Wno-float-equal -Werror \
	-o DXBall"

RunString="./DXBall -lPlayer Me -rPlayer You -fpsLimit 0 -resolution 1600x900"
GDBString="gdb -ex run --args $RunString"
ValgindString="valgrind \
	--suppressions=valgrind/ignore \
	--tool=memcheck \
	--leak-check=full \
	--leak-resolution=high \
	--error-limit=no \
	--track-origins=yes \
	--undef-value-errors=yes \
	--read-var-info=yes \
	$RunString"
# Completely clean console window
reset

# Let qmake create a makefile
qmake -o Makefile build/DXBall.pro

clear

echo "Building..."

# Try to build the project
if make; then

	# Check arguments
	while getopts ":rvgcf" opt; do
		case $opt in
			r)
				RunGame=true
				RunGDB=false
				RunValgrind=false
				;;
			v)
				RunGame=false
				RunValgrind=true
				RunGDB=false
				;;
			g)
				RunGame=false
				RunGDB=true
				RunValgrind=false
				;;
			c)
				ClearCompileOutput=true
				;;
			f)
				ForceFullCompile=true
				;;
			\?)
				echo "Invalid option: -$OPTARG" >&2
				;;
		esac

	done

	echo "Build succesfull"

	if $ForceFullCompile ; then
		echo "========================================================================"
		echo $CompileString
		$CompileString
	fi

	if $ClearCompileOutput ; then
		clear
	fi

	if $RunGame ; then
		echo -e "\tNormal mode"
		echo -e "\tCommand : " $RunString
		echo "=============================== DX Balll ==============================="
		$RunString
	fi

	if $RunGDB ; then
		echo -e "\tDebug mode"
		echo -e "\tCommand : " $GDBString
		echo "=============================== DX Balll ==============================="
		$GDBString
	fi

	if $RunValgrind ; then
		echo -e "\tValgrind mode"
		echo -e "\tCommand : " $ValgindString
		echo "=============================== DX Balll ==============================="
		$ValgindString
	fi
fi # if make
exit 0
#gdb -ex run ./DXBall
#fi
#./DXBall -lPlayer Ole_Vegard -rPlayer Openent -fpslimit 0