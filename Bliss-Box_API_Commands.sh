#!/bin/bash

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Copyright 2019 Cray "rezb1t" Elliott

# You can download the latest version of this script from:
# https://github.com/MiSTer-LLAPI/blisSTer_flash
while true;
       do
clear
SCRIPT_PATH="/media/fat/Scripts"
echo "			   The API use here works via USB control transfers. It is important to note"
echo "			that in USB mode the physical ports do not mean anything. The way Software "
echo "			communicates with the ports is by the USB ID. Typically both are set to player 1"
echo "			"
echo "			   Please not that this application uses LIBUSB and so does the MisSTer. So when "
echo "		        any of these comamnds are ran, the device will be reset"
echo "			"
echo "			   If you want the ability to differentiate them, you must set one to player 2."
echo "			This is not require on the MiSTer but in some cases can be handy. To do this: "
echo "			Press and hold the reset port that you want to leave as player 1. "
echo "			Select option m (This sets the other port to player 2). "
echo "			Let go of the reset button. "
echo "			"
echo "			"
echo "			Version 0.1 beta, if the commands fail just try again (this is a known issue)"
echo "			"
echo "			Please make a selection"
echo "			"
echo "			 a) help "
echo "			 b) get port 1 EEPPROM data "
echo "			 c) get port 2 EEPPROM data "
echo "			 d) get port 1 attached controller"
echo "			 e) get port 2 attached controller"
echo "			 f) get port 1 player "
echo "			 g) get port 2 player "
echo "			 h) get port 1 versin major"
echo "			 i) get port 2 version major"
echo "			 j) get port 1 versin minor"
echo "			 k) get port 2 version minor"
echo "			  "
echo "			 l) change  player 1 to player 2"
echo "			 m) change  player 2 to player 1"
echo "			 n) restore defautls on port 1"
echo "			 0) restore defaults on port 2"
echo "			 p) disable all combos on port 1"
echo "			 q) disable all combos on port 2"
echo "			 r) disable autopause on port 1"
echo "			 s) disable autopause on port 2"
echo "			 t) disable hotswap on port 1"
echo "			 u) disable hotswap on port 2"
echo "			 "
echo "			 x) quit" 
echo "			" 

read answer
case $answer in
    a)    $SCRIPT_PATH/Bliss-Box_CLI -help ;;
    b)    $SCRIPT_PATH/Bliss-Box_CLI -p 1 get eeprom_settings;;
    c)    $SCRIPT_PATH/Bliss-Box_CLI -p 2 get eeprom_settings;;
    d)    $SCRIPT_PATH/Bliss-Box_CLI -p 1 get controller;;
    e)    $SCRIPT_PATH/Bliss-Box_CLI -p 2 get controller;;
    f)    $SCRIPT_PATH/Bliss-Box_CLI -p 1 get player;;
    g)    $SCRIPT_PATH/Bliss-Box_CLI -p 2 get player;;
    h)    $SCRIPT_PATH/Bliss-Box_CLI -p 1 get major_ver;;
    i)    $SCRIPT_PATH/Bliss-Box_CLI -p 2 get major_ver;;
    j)    $SCRIPT_PATH/Bliss-Box_CLI -p 1 get minor_ver;;
    k)    $SCRIPT_PATH/Bliss-Box_CLI -p 2 get minor_ver;;
    l)    $SCRIPT_PATH/Bliss-Box_CLI -p 1 set player 1 2 ;;
    m)    $SCRIPT_PATH/Bliss-Box_CLI -p 1 set player 2 1 ;;
    n)    $SCRIPT_PATH/Bliss-Box_CLI -p 1 set modes 255 2;;
    o)    $SCRIPT_PATH/Bliss-Box_CLI -p 2 set modes 255 2;;
    p)    $SCRIPT_PATH/Bliss-Box_CLI -p 1 set modes 32;;
    q)    $SCRIPT_PATH/Bliss-Box_CLI -p 2 set modes 32;;
	r)    $SCRIPT_PATH/Bliss-Box_CLI -p 1 set modes 64;;
    s)    $SCRIPT_PATH/Bliss-Box_CLI -p 2 set modes 64;;
	t)    $SCRIPT_PATH/Bliss-Box_CLI -p 1 set modes 8;;
    u)    $SCRIPT_PATH/Bliss-Box_CLI -p 2 set modes 8;;
    x)    exit 0;;
    *)    exec "/media/fat/Scripts/Bliss-Box_API_Commands.sh";; 
esac
echo " "
echo " "
echo "Press andy key to return to the menu"    
read -rsn1 -p ""
$SCRIPT_PATH/Bliss-Box_CLI -p 1 set reset 
$SCRIPT_PATH/Bliss-Box_CLI -p 2 set reset 
done
