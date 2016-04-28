#!/bin/sh

# Show the application banner.
echo " "
echo "******************************************************************"
echo "***                                                            ***"
echo "***    Avnet IoT Sensor Fusion Application Build V1.0          ***"
echo "***                                                            ***"
echo "******************************************************************"
echo "***                                                            ***"
echo "***  NOTE: Attach MAX31723 Pmod to connector JA on MZ Carrier  ***"
echo "***                                                            ***"
echo "******************************************************************"
echo "***                                                            ***"
echo "***  NOTE: Connect Ethernet port J1 to port ETH1 on Agate Box  ***"
echo "***                                                            ***"
echo "******************************************************************"
echo " "

# Start the sensor fusion application.
/home/root/linux_temperature_client.elf&


