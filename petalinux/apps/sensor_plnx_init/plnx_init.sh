#!/bin/sh

DEMO_SETUP_SCRIPT=/home/root/demo_setup.sh
ETH1_SETUP_SCRIPT=/home/root/setup_eth1.sh
START_APPS_SCRIPT=/home/root/start_demo_apps.sh

# Show the banner.
echo " "
echo "******************************************************************"
echo "***                                                            ***"
echo "***    Avnet UltraZed SOM + I/O CC IoT Sensor Demo Launcher    ***"
echo "***                                                            ***"
echo "******************************************************************"
#echo "***                                                            ***"
#echo "***                    Loading Kernel Modules                  ***"
#echo "***    Please Press Key to Perform Desired Function Below:     ***"
#echo "***                                                            ***"
#echo "***    'x' - Linux Command Prompt                              ***"
#echo "***                                                            ***"
#echo "***    NOTE: Waiting will AUTO-INITIATE the Sensor Demo Setup  ***"
#echo "***                                                            ***"
#echo "******************************************************************"
echo " "

source ${DEMO_SETUP_SCRIPT}
#source ${ETH1_SETUP_SCRIPT}
#source ${START_APPS_SCRIPT}

#while :
#do
#	answer="c"
#	read -t 5 answer
#	if [ "$answer" = "x" ]
#	then
#		echo " "
#		echo "******************************************************************"
#		echo "*** Exiting to Linux Command Prompt                            ***"
#		echo "******************************************************************"
#		echo " "
#		break
#	else
#		echo " "
#		echo "******************************************************************"
#		echo "*** Auto-Initiating IoT Sensor Demo Setup                      ***"
#		echo "******************************************************************"
#		echo " "
#		source ${DEMO_SETUP_SCRIPT}
#		break
#	fi
#done
