#!/bin/sh

FACTORY_TEST_SCRIPT=/home/root/factory_test.sh

# Show the factory test banner.
echo " "
echo "******************************************************************"
echo "***                                                            ***"
echo "***    Avnet PicoZed Petalinux FMC Carrier Build V1.2          ***"
echo "***                                                            ***"
echo "******************************************************************"
echo "***                                                            ***"
echo "***    Please Press Key to Perform Desired Function Below:     ***"
echo "***                                                            ***"
echo "***    'f' - Factory Test                                      ***"
echo "***    'x' - Linux Command Prompt                              ***"
echo "***                                                            ***"
echo "***    NOTE: Waiting will AUTO-INITIATE Factory Test           ***"
echo "***                                                            ***"
echo "******************************************************************"
echo " "

while :
do
	answer="c"
	read -t 5 answer
	if [ "$answer" = "x" ]
	then
		echo " "
		echo "******************************************************************"
		echo "*** Exiting to Linux Command Prompt                            ***"
		echo "******************************************************************"
		echo " "
		break
	elif [ "$answer" = "f" ]
	then
		echo " "
		echo "******************************************************************"
		echo "*** Initiating Factory Test Suite                              ***"
		echo "******************************************************************"
		echo " "
		source ${FACTORY_TEST_SCRIPT}
		break
	else
		echo " "
		echo "******************************************************************"
		echo "*** Auto-Initiating Factory Test Suite                         ***"
		echo "******************************************************************"
		echo " "
		source ${FACTORY_TEST_SCRIPT}
		break
	fi
done
