# ----------------------------------------------------------------------------
#       _____
#      *     *
#     *____   *____
#    * *===*   *==*
#   *___*===*___**  AVNET
#        *======*
#         *====*
# ----------------------------------------------------------------------------
# 
#  This design is the property of Avnet.  Publication of this
#  design is not authorized without written consent from Avnet.
# 
#  Please direct any questions to the UltraZed community support forum:
#     http://www.ultrazed.org/forum
# 
#  Product information is available at:
#     http://www.ultrazed.org/product/ultrazed-EG
# 
#  Disclaimer:
#     Avnet, Inc. makes no warranty for the use of this code or design.
#     This code is provided  "As Is". Avnet, Inc assumes no responsibility for
#     any errors, which may appear in this code, nor does it make a commitment
#     to update the information contained herein. Avnet, Inc specifically
#     disclaims any implied warranties of fitness for a particular purpose.
#                      Copyright(c) 2016 Avnet, Inc.
#                              All rights reserved.
# 
# ----------------------------------------------------------------------------
# 
#  Create Date:         Nov 28, 2016
#  Design Name:         UltraZed + I/O Carrier
#  Module Name:         linux_iic_eeprom_test.sh
#  Project Name:        UltraZed IIC EEPROM Test Script
#  Target Devices:      Xilinx Zynq UltraScale MPSoC
#  Hardware Boards:     UltraZed + I/O Carrier
# 
#  Tool versions:       Xilinx Vivado 2016.2
# 
#  Description:         Test Script for UltraZed IIC EEPROM
# 
#  Dependencies:        
#
#  Revision:            Nov 28, 2016: 1.0 Initial version
# 
# ----------------------------------------------------------------------------
#!/etc/sh

# Sleep interval between tests.
SLEEP_INTERVAL_MICRO=4000
SLEEP_INTERVAL_NORMAL=1

# Information used to execute comparison test between expected results
# and actual read results from target I2C device.
TEST_TEMP_FOLDER=/tmp
TEST_TEMP_GOLDEN=eeprom_golden.txt
TEST_TEMP_TARGET=eeprom_test.txt

# Setup variable to store the test results into.
IIC_EEPROM_TEST_RESULT=-1

# Parse incoming arguments here.

while [[ $# -gt 1 ]]
do
key="$1"

case $key in
    -b|--bus)
    BUS="$2"
    shift # past argument
    ;;
    -s|--slave)
    SLAVE="$2"
    shift # past argument
    ;;
    --default)
    DEFAULT=YES
    ;;
    *)
            # unknown option
    ;;
esac
shift # past argument or value
done
echo "Testing target EEPROM at ${SLAVE} on bus i2c-${BUS}"

# START HERE: Non-boilerplate code above should be contained within 
# functions so that at this point simple high level calls can be made to 
# the bigger blocks above.

# Program the test pattern to the target EEPROM device.
i2cset -y ${BUS} ${SLAVE} 0x00 0x00
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x01 0x01
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x02 0x02
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x03 0x03
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x04 0x04
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x05 0x05
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x06 0x06
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x07 0x07
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x08 0x08
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x09 0x09
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x0A 0x0A
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x0B 0x0B
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x0C 0x0C
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x0D 0x0D
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x0E 0x0E
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x0F 0x0F
usleep ${SLEEP_INTERVAL_MICRO}

# Define the expected output of the i2cdump here:
echo "
 0000|  00 01 02 03 04 05 06 07   08 09 0a 0b 0c 0d 0e 0f 
" > ${TEST_TEMP_FOLDER}/${TEST_TEMP_GOLDEN}

# Capture fresh test results to a target file.
eeprog -xf /dev/i2c-${BUS} ${SLAVE} -8 -r 0x00:0x10 > ${TEST_TEMP_FOLDER}/${TEST_TEMP_TARGET}

sleep ${SLEEP_INTERVAL_NORMAL}
sync

echo "******************************************************************"
echo " "

# Compare the target test results with the 'golden' results.  If the files 
# match identically, the return code from diff is 0 and the test passes.
diff -q ${TEST_TEMP_FOLDER}/${TEST_TEMP_GOLDEN} ${TEST_TEMP_FOLDER}/${TEST_TEMP_TARGET} > /dev/null

# Capture the return code from diff as the test result.
IIC_EEPROM_TEST_RESULT=$?

# Clear the test pattern from the EEPROM to leave no trace of this test.
i2cset -y ${BUS} ${SLAVE} 0x00 0xFF
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x01 0xFF
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x02 0xFF
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x03 0xFF
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x04 0xFF
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x05 0xFF
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x06 0xFF
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x07 0xFF
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x08 0xFF
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x09 0xFF
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x0A 0xFF
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x0B 0xFF
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x0C 0xFF
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x0D 0xFF
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x0E 0xFF
usleep ${SLEEP_INTERVAL_MICRO}
i2cset -y ${BUS} ${SLAVE} 0x0F 0xFF
usleep ${SLEEP_INTERVAL_MICRO}

if [ ${IIC_EEPROM_TEST_RESULT} == "0" ];
then
  echo "I2C EEPROM (${SLAVE}) Test: PASS"
else
  echo "I2C EEPROM (${SLAVE}) Test: FAIL"
fi

echo " "
echo "******************************************************************"
echo " "

# END HERE: All done with the testing.

exit ${IIC_EEPROM_TEST_RESULT}
