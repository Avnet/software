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
#                      Copyright(c) 2017 Avnet, Inc.
#                              All rights reserved.
# 
# ----------------------------------------------------------------------------
# 
#  Create Date:         Mar 09, 2017
#  Design Name:         UltraZed + I/O Carrier
#  Module Name:         sata_performance_test.sh
#  Project Name:        UltraZed I/O Carrier SATA Performance Test Script
#  Target Devices:      Xilinx Zynq UltraScale MPSoC
#  Hardware Boards:     UltraZed + I/O Carrier
# 
#  Tool versions:       Xilinx Vivado 2016.2
# 
#  Description:         Startup Script for UltraZed IOCC SATA Performance
#                       Test design
# 
#  Dependencies:        
#
#  Revision:            Mar 09, 2017: 1.0 Initial version
# 
# ----------------------------------------------------------------------------
#!/etc/sh

# SATA Performance Test version information.
SATA_PERFORMANCE_TEST_VERSION_NUMBER=1.0

# Sleep interval between tests.
SLEEP_INTERVAL_NORMAL=1

# Device file used to dump unneeded data for read performance testing.
NULL_DATA_DESTINATION_FILE=/dev/null

# Device file used to obtain random data for read performance testing.
RANDOM_DATA_SOURCE_FILE=/dev/urandom

# Device file used to obtain fast test data for write performance testing.
ZERO_DATA_SOURCE_FILE=/dev/zero

# Information used to mount the SATA performance test drive.
SATA_TEST_DEVICE=/dev/sda1
SATA_TEST_MOUNT_POINT=/mnt

# Read test files.
SATA_READ_SPEED_SOURCE_FILE=sata_read_dd
SATA_READ_SPEED_SOURCE_1M_FILE=${SATA_READ_SPEED_SOURCE_FILE}_1M
SATA_READ_SPEED_SOURCE_2M_FILE=${SATA_READ_SPEED_SOURCE_FILE}_2M
SATA_READ_SPEED_SOURCE_4M_FILE=${SATA_READ_SPEED_SOURCE_FILE}_4M
SATA_READ_SPEED_SOURCE_8M_FILE=${SATA_READ_SPEED_SOURCE_FILE}_8M
SATA_READ_SPEED_SOURCE_16M_FILE=${SATA_READ_SPEED_SOURCE_FILE}_16M
SATA_READ_SPEED_SOURCE_32M_FILE=${SATA_READ_SPEED_SOURCE_FILE}_32M
SATA_READ_SPEED_SOURCE_64M_FILE=${SATA_READ_SPEED_SOURCE_FILE}_64M
SATA_READ_SPEED_SOURCE_128M_FILE=${SATA_READ_SPEED_SOURCE_FILE}_128M
SATA_READ_SPEED_SOURCE_256M_FILE=${SATA_READ_SPEED_SOURCE_FILE}_256M
SATA_READ_SPEED_SOURCE_512M_FILE=${SATA_READ_SPEED_SOURCE_FILE}_512M
SATA_READ_SPEED_SOURCE_1024M_FILE=${SATA_READ_SPEED_SOURCE_FILE}_1024M
SATA_READ_SPEED_SOURCE_2048M_FILE=${SATA_READ_SPEED_SOURCE_FILE}_2048M
SATA_READ_SPEED_SOURCE_4096M_FILE=${SATA_READ_SPEED_SOURCE_FILE}_4096M
SATA_READ_SPEED_SOURCE_8192M_FILE=${SATA_READ_SPEED_SOURCE_FILE}_8192M
SATA_READ_SPEED_SOURCE_16384M_FILE=${SATA_READ_SPEED_SOURCE_FILE}_16384M
SATA_READ_SPEED_SOURCE_32768M_FILE=${SATA_READ_SPEED_SOURCE_FILE}_32768M
SATA_READ_SPEED_SOURCE_65536M_FILE=${SATA_READ_SPEED_SOURCE_FILE}_65536M

# Write test files.
SATA_WRITE_SPEED_DESTINATION_FILE=sata_write_dd
SATA_WRITE_SPEED_DESTINATION_1M_FILE=${SATA_WRITE_SPEED_DESTINATION_FILE}_1M
SATA_WRITE_SPEED_DESTINATION_2M_FILE=${SATA_WRITE_SPEED_DESTINATION_FILE}_2M
SATA_WRITE_SPEED_DESTINATION_4M_FILE=${SATA_WRITE_SPEED_DESTINATION_FILE}_4M
SATA_WRITE_SPEED_DESTINATION_8M_FILE=${SATA_WRITE_SPEED_DESTINATION_FILE}_8M
SATA_WRITE_SPEED_DESTINATION_16M_FILE=${SATA_WRITE_SPEED_DESTINATION_FILE}_16M
SATA_WRITE_SPEED_DESTINATION_32M_FILE=${SATA_WRITE_SPEED_DESTINATION_FILE}_32M
SATA_WRITE_SPEED_DESTINATION_64M_FILE=${SATA_WRITE_SPEED_DESTINATION_FILE}_64M
SATA_WRITE_SPEED_DESTINATION_128M_FILE=${SATA_WRITE_SPEED_DESTINATION_FILE}_128M
SATA_WRITE_SPEED_DESTINATION_256M_FILE=${SATA_WRITE_SPEED_DESTINATION_FILE}_256M
SATA_WRITE_SPEED_DESTINATION_512M_FILE=${SATA_WRITE_SPEED_DESTINATION_FILE}_512M
SATA_WRITE_SPEED_DESTINATION_1024M_FILE=${SATA_WRITE_SPEED_DESTINATION_FILE}_1024M
SATA_WRITE_SPEED_DESTINATION_2048M_FILE=${SATA_WRITE_SPEED_DESTINATION_FILE}_2048M
SATA_WRITE_SPEED_DESTINATION_4096M_FILE=${SATA_WRITE_SPEED_DESTINATION_FILE}_4096M
SATA_WRITE_SPEED_DESTINATION_8192M_FILE=${SATA_WRITE_SPEED_DESTINATION_FILE}_8192M
SATA_WRITE_SPEED_DESTINATION_16384M_FILE=${SATA_WRITE_SPEED_DESTINATION_FILE}_16384M
SATA_WRITE_SPEED_DESTINATION_32768M_FILE=${SATA_WRITE_SPEED_DESTINATION_FILE}_32768M
SATA_WRITE_SPEED_DESTINATION_65536M_FILE=${SATA_WRITE_SPEED_DESTINATION_FILE}_65536M

# Results files
READ_TEST_RESULT_FILE=/home/root/sata_read_performance_test_results.txt
WRITE_TEST_RESULT_FILE=/home/root/sata_write_performance_test_results.txt

# Mount the SATA drive so that operations can be performed on it.
check_and_mount_sata_test_device ()
{
  if [ -e ${SATA_TEST_DEVICE} ]
  then
    mount ${SATA_TEST_DEVICE} ${SATA_TEST_MOUNT_POINT}
  else
    echo " "
    echo "ERROR: SATA test drive not found at ${SATA_TEST_DEVICE}!"
    echo "make sure that drive is connected and powered on."
    echo " "  
	exit -1
  fi
}

# Checks for the random data source file used for the read performance test.
check_for_sata_read_speed_source_file ()
{
  if [ -e ${SATA_TEST_MOUNT_POINT}/${SATA_READ_SPEED_SOURCE_FILE}_${CURRENT_TEST_BLOCK_COUNT}M ]
  then
    echo "${CURRENT_TEST_BLOCK_COUNT}MB read source file found at ${SATA_TEST_MOUNT_POINT}/${SATA_READ_SPEED_SOURCE_FILE}_${CURRENT_TEST_BLOCK_COUNT}M"
  else
    echo " "
    echo "${CURRENT_TEST_BLOCK_COUNT}MB read source file not found in ${SATA_TEST_MOUNT_POINT}, creating this file..."
    echo "WARNING:  This can take a few minutes!"
    echo " "
	time dd if=${RANDOM_DATA_SOURCE_FILE} of=${SATA_TEST_MOUNT_POINT}/${SATA_READ_SPEED_SOURCE_FILE}_${CURRENT_TEST_BLOCK_COUNT}M bs=1M count=${CURRENT_TEST_BLOCK_COUNT}
  fi
}

# Checks to see if files are available for read speed tests.  If they are 
# not present, then the random data source files must be setup by writing 
# data read from urandom device which can take a few minutes to complete
# for all of the test case files.
check_for_sata_read_speed_source_files ()
{
  # Check to see if the random data source has already been setup on this 
  # system.
  if [ -e ${RANDOM_DATA_SOURCE_FILE} ]
  then
    echo "Random data source file found at ${RANDOM_DATA_SOURCE_FILE}"
  else
    echo " "
    echo "Random data source file not found at ${RANDOM_DATA_SOURCE_FILE}, creating this file..."
    echo " "
    mknod -m 777 ${RANDOM_DATA_SOURCE_FILE} c 1 9 > ${NULL_DATA_DESTINATION_FILE} 2>&1
  fi
  
  # Check for the 1MB read source file.
  CURRENT_TEST_BLOCK_COUNT=1
  check_for_sata_read_speed_source_file

  # Check for the 2MB read source file.
  CURRENT_TEST_BLOCK_COUNT=2
  check_for_sata_read_speed_source_file

  # Check for the 4MB read source file.
  CURRENT_TEST_BLOCK_COUNT=4
  check_for_sata_read_speed_source_file

  # Check for the 8MB read source file.
  CURRENT_TEST_BLOCK_COUNT=8
  check_for_sata_read_speed_source_file

  # Check for the 16MB read source file.
  CURRENT_TEST_BLOCK_COUNT=16
  check_for_sata_read_speed_source_file

  # Check for the 32MB read source file.
  CURRENT_TEST_BLOCK_COUNT=32
  check_for_sata_read_speed_source_file

  # Check for the 64MB read source file.
  CURRENT_TEST_BLOCK_COUNT=64
  check_for_sata_read_speed_source_file

  # Check for the 128MB read source file.
  CURRENT_TEST_BLOCK_COUNT=128
  check_for_sata_read_speed_source_file

  # Check for the 256MB read source file.
  CURRENT_TEST_BLOCK_COUNT=256
  check_for_sata_read_speed_source_file
  
  # Check for the 512MB read source file.
  CURRENT_TEST_BLOCK_COUNT=512
  check_for_sata_read_speed_source_file

  # Check for the 1024MB read source file.
  CURRENT_TEST_BLOCK_COUNT=1024
  check_for_sata_read_speed_source_file
  
  # Check for the 2048MB read source file.
  CURRENT_TEST_BLOCK_COUNT=2048
  check_for_sata_read_speed_source_file
 
#  
# These file sizes are too large for BusyBox dd to use on FAT32.
# 
 
  # Check for the 4096MB read source file.
#  CURRENT_TEST_BLOCK_COUNT=4096
#  check_for_sata_read_speed_source_file

  # Check for the 8192MB read source file.
#  CURRENT_TEST_BLOCK_COUNT=8192
#  check_for_sata_read_speed_source_file
  
  # Check for the 16384MB read source file.
#  CURRENT_TEST_BLOCK_COUNT=16384
#  check_for_sata_read_speed_source_file
  
  # Check for the 32768MB read source file.
#  CURRENT_TEST_BLOCK_COUNT=32768
#  check_for_sata_read_speed_source_file
  
  # Check for the 65536MB read source file.
#  CURRENT_TEST_BLOCK_COUNT=65536
#  check_for_sata_read_speed_source_file
}

# Runs the current read performance test using fixed block size to determine
# how file size affects the transfer rate.  Assumes that both variables
# $CURRENT_TEST_BLOCK_SIZE and $CURRENT_TEST_BLOCK_COUNT have been set before
# this function gets called.
read_fixed_block_size_performance_test ()
{
  # Perform the read test using the system time at the start of the test 
  # and subtract the system time to determine overall execution in 
  # millisecond resolution.
  echo "Reading ${CURRENT_TEST_BLOCK_COUNT} blocks of data using block size of ${CURRENT_TEST_BLOCK_SIZE}..."
  TEST_START_TIME=`date +%s%3N`
  dd if=${SATA_TEST_MOUNT_POINT}/${SATA_READ_SPEED_SOURCE_FILE}_${CURRENT_TEST_BLOCK_COUNT}M of=${NULL_DATA_DESTINATION_FILE} bs=${CURRENT_TEST_BLOCK_SIZE} count=${CURRENT_TEST_BLOCK_COUNT} iflag=direct oflag=dsync status=none
  TEST_END_TIME=`date +%s%3N`
  TEST_RUNTIME=$((TEST_END_TIME - TEST_START_TIME))
  TEST_FILE_SIZE=`stat -c%s ${SATA_TEST_MOUNT_POINT}/${SATA_READ_SPEED_SOURCE_FILE}_${CURRENT_TEST_BLOCK_COUNT}M`
  TEST_FILE_SIZE_MB=$((TEST_FILE_SIZE / 1048576))
  TEST_TRANSFER_RATE=`echo "scale=3; ((${TEST_FILE_SIZE} / 1024) / 1024 ) / ( ${TEST_RUNTIME} / 1000)" | bc -l`
  echo -e "*\t${TEST_FILE_SIZE_MB}\t\t${TEST_RUNTIME}\t\t${TEST_TRANSFER_RATE}\t\t${CURRENT_TEST_BLOCK_SIZE}\t *" >> ${READ_TEST_RESULT_FILE}
}

# Runs the current read performance test using a fixed file size to determine 
# how block size affects the transfer rate.  Assumes that both variables
# $CURRENT_TEST_BLOCK_SIZE and $CURRENT_TEST_BLOCK_COUNT have been set before
# this function gets called.
read_fixed_file_size_performance_test ()
{
  # Perform the read test using the system time at the start of the test 
  # and subtract the system time to determine overall execution in 
  # millisecond resolution.
  echo "Reading ${CURRENT_TEST_BLOCK_COUNT} blocks of data using block size of ${CURRENT_TEST_BLOCK_SIZE}..."
  TEST_START_TIME=`date +%s%3N`
  dd if=${SATA_TEST_MOUNT_POINT}/${SATA_READ_SPEED_SOURCE_FILE}_1024M of=${NULL_DATA_DESTINATION_FILE} bs=${CURRENT_TEST_BLOCK_SIZE} count=${CURRENT_TEST_BLOCK_COUNT} iflag=direct oflag=dsync status=none
  TEST_END_TIME=`date +%s%3N`
  TEST_RUNTIME=$((TEST_END_TIME - TEST_START_TIME))
  TEST_FILE_SIZE=1073741824
  TEST_FILE_SIZE_MB=1024
  TEST_TRANSFER_RATE=`echo "scale=3; ((${TEST_FILE_SIZE} / 1024) / 1024 ) / ( ${TEST_RUNTIME} / 1000)" | bc -l`
  echo -e "*\t${TEST_FILE_SIZE_MB}\t\t${TEST_RUNTIME}\t\t${TEST_TRANSFER_RATE}\t\t${CURRENT_TEST_BLOCK_SIZE}\t *" >> ${READ_TEST_RESULT_FILE}
}

# Runs the current write performance test using fixed block size to determine
# how file size affects the transfer rate.  Assumes that both variables
# $CURRENT_TEST_BLOCK_SIZE and $CURRENT_TEST_BLOCK_COUNT have been set before
# this function gets called.
write_fixed_block_size_performance_test ()
{
  # Perform the write test using the system time at the start of the test 
  # and subtract the system time to determine overall execution in 
  # millisecond resolution.
  echo "Writing ${CURRENT_TEST_BLOCK_COUNT} blocks of data using block size of ${CURRENT_TEST_BLOCK_SIZE}..."
  TEST_START_TIME=`date +%s%3N`
  dd if=${ZERO_DATA_SOURCE_FILE} of=${SATA_TEST_MOUNT_POINT}/${SATA_WRITE_SPEED_DESTINATION_FILE}_${CURRENT_TEST_BLOCK_COUNT}M bs=${CURRENT_TEST_BLOCK_SIZE} count=${CURRENT_TEST_BLOCK_COUNT} oflag=direct status=none
  TEST_END_TIME=`date +%s%3N`
  TEST_RUNTIME=$((TEST_END_TIME - TEST_START_TIME))
  TEST_FILE_SIZE=`stat -c%s ${SATA_TEST_MOUNT_POINT}/${SATA_WRITE_SPEED_DESTINATION_FILE}_${CURRENT_TEST_BLOCK_COUNT}M`
  TEST_FILE_SIZE_MB=$((TEST_FILE_SIZE / 1048576))
  TEST_TRANSFER_RATE=`echo "scale=3; ((${TEST_FILE_SIZE} / 1024) / 1024 ) / ( ${TEST_RUNTIME} / 1000)" | bc -l`
  echo -e "*\t${TEST_FILE_SIZE_MB}\t\t${TEST_RUNTIME}\t\t${TEST_TRANSFER_RATE}\t\t${CURRENT_TEST_BLOCK_SIZE}\t *" >> ${WRITE_TEST_RESULT_FILE}
}

# Runs the current write performance test using a fixed file size to determine 
# how block size affects the transfer rate.  Assumes that both variables
# $CURRENT_TEST_BLOCK_SIZE and $CURRENT_TEST_BLOCK_COUNT have been set before
# this function gets called.
write_fixed_file_size_performance_test ()
{
  # Perform the write test using the system time at the start of the test 
  # and subtract the system time to determine overall execution in 
  # millisecond resolution.
  echo "Writing ${CURRENT_TEST_BLOCK_COUNT} blocks of data using block size of ${CURRENT_TEST_BLOCK_SIZE}..."
  TEST_START_TIME=`date +%s%3N`
  dd if=${ZERO_DATA_SOURCE_FILE} of=${SATA_TEST_MOUNT_POINT}/${SATA_WRITE_SPEED_DESTINATION_FILE}_1024M bs=${CURRENT_TEST_BLOCK_SIZE} count=${CURRENT_TEST_BLOCK_COUNT} oflag=direct status=none
  TEST_END_TIME=`date +%s%3N`
  TEST_RUNTIME=$((TEST_END_TIME - TEST_START_TIME))
  TEST_FILE_SIZE=1073741824
  TEST_FILE_SIZE_MB=1024
  TEST_TRANSFER_RATE=`echo "scale=3; ((${TEST_FILE_SIZE} / 1024) / 1024 ) / ( ${TEST_RUNTIME} / 1000)" | bc -l`
  echo -e "*\t${TEST_FILE_SIZE_MB}\t\t${TEST_RUNTIME}\t\t${TEST_TRANSFER_RATE}\t\t${CURRENT_TEST_BLOCK_SIZE}\t *" >> ${WRITE_TEST_RESULT_FILE}
}

# Runs all of the read performance tests.
launch_all_read_performance_tests ()
{
  CURRENT_TEST_BLOCK_SIZE=1M
  echo "******************************************************************" > ${READ_TEST_RESULT_FILE}
  echo "*                                                                *" >> ${READ_TEST_RESULT_FILE}
  echo "*    SATA Read Performance Test Results - Fixed Block Size Test  *" >> ${READ_TEST_RESULT_FILE}
  echo "*                                                                *" >> ${READ_TEST_RESULT_FILE}
  echo -e "* File Size (MB):\tXfer time (ms):\tRate (MB/s):\tBlock:\t *" >> ${READ_TEST_RESULT_FILE}
  echo "*                                                                *" >> ${READ_TEST_RESULT_FILE}
  echo "******************************************************************" >> ${READ_TEST_RESULT_FILE}
  echo "*                                                                *" >> ${READ_TEST_RESULT_FILE}
  
  # Run the varying file length tests 1MB to 1024MB without changing the block size used.

  # Test with the 1MB read source file.
  CURRENT_TEST_BLOCK_COUNT=1
  read_fixed_block_size_performance_test
  
  # Test with the 2MB read source file.
  CURRENT_TEST_BLOCK_COUNT=2
  read_fixed_block_size_performance_test
  
  # Test with the 4MB read source file.
  CURRENT_TEST_BLOCK_COUNT=4
  read_fixed_block_size_performance_test
  
  # Test with the 8MB read source file.
  CURRENT_TEST_BLOCK_COUNT=8
  read_fixed_block_size_performance_test
  
  # Test with the 16MB read source file.
  CURRENT_TEST_BLOCK_COUNT=16
  read_fixed_block_size_performance_test
  
  # Test with the 32MB read source file.
  CURRENT_TEST_BLOCK_COUNT=32
  read_fixed_block_size_performance_test
  
  # Test with the 64MB read source file.
  CURRENT_TEST_BLOCK_COUNT=64
  read_fixed_block_size_performance_test
  
  # Test with the 128MB read source file.
  CURRENT_TEST_BLOCK_COUNT=128
  read_fixed_block_size_performance_test
  
  # Test with the 256MB read source file.
  CURRENT_TEST_BLOCK_COUNT=256
  read_fixed_block_size_performance_test
  
  # Test with the 512MB read source file.
  CURRENT_TEST_BLOCK_COUNT=512
  read_fixed_block_size_performance_test
  
  # Test with the 1024MB read source file.
  CURRENT_TEST_BLOCK_COUNT=1024
  read_fixed_block_size_performance_test
  
  # Test with the 2048MB read source file.
  CURRENT_TEST_BLOCK_COUNT=2048
  read_fixed_block_size_performance_test

#  
# These file sizes are too large for BusyBox dd to use on FAT32.
# 
  
  # Test with the 4096MB read source file.
#  CURRENT_TEST_BLOCK_COUNT=4096
#  read_fixed_block_size_performance_test

  # Test with the 8192MB read source file.
#  CURRENT_TEST_BLOCK_COUNT=8192
#  read_fixed_block_size_performance_test
  
  # Test with the 16384MB read source file.
#  CURRENT_TEST_BLOCK_COUNT=16384
#  read_fixed_block_size_performance_test
  
  # Test with the 32768MB read source file.
#  CURRENT_TEST_BLOCK_COUNT=32768
#  read_fixed_block_size_performance_test
  
  # Test with the 65536MB read source file.
#  CURRENT_TEST_BLOCK_COUNT=65536
#  read_fixed_block_size_performance_test

  echo "*                                                                *" >> ${READ_TEST_RESULT_FILE}
  echo "******************************************************************" >> ${READ_TEST_RESULT_FILE}

  echo "******************************************************************" >> ${READ_TEST_RESULT_FILE}
  echo "*                                                                *" >> ${READ_TEST_RESULT_FILE}
  echo "*    SATA Read Performance Test Results - Fixed File Size Test   *" >> ${READ_TEST_RESULT_FILE}
  echo "*                                                                *" >> ${READ_TEST_RESULT_FILE}
  echo -e "* File Size (MB):\tXfer time (ms):\tRate (MB/s):\tBlock:\t *" >> ${READ_TEST_RESULT_FILE}
  echo "*                                                                *" >> ${READ_TEST_RESULT_FILE}
  echo "******************************************************************" >> ${READ_TEST_RESULT_FILE}
  echo "*                                                                *" >> ${READ_TEST_RESULT_FILE}
  
  # Run the varying block size tests 1K to 1M without changing the file size used.

  # Test with the 1024MB read source file, block size 1K.
  CURRENT_TEST_BLOCK_SIZE=1K
  CURRENT_TEST_BLOCK_COUNT=1048576
  read_fixed_file_size_performance_test
  
  # Test with the 1024MB read source file, block size 2K.
  CURRENT_TEST_BLOCK_SIZE=2K
  CURRENT_TEST_BLOCK_COUNT=524288
  read_fixed_file_size_performance_test

  # Test with the 1024MB read source file, block size 4K.
  CURRENT_TEST_BLOCK_SIZE=4K
  CURRENT_TEST_BLOCK_COUNT=262144
  read_fixed_file_size_performance_test

  # Test with the 1024MB read source file, block size 8K.
  CURRENT_TEST_BLOCK_SIZE=8K
  CURRENT_TEST_BLOCK_COUNT=131072
  read_fixed_file_size_performance_test

  # Test with the 1024MB read source file, block size 16K.
  CURRENT_TEST_BLOCK_SIZE=16K
  CURRENT_TEST_BLOCK_COUNT=65536
  read_fixed_file_size_performance_test

  # Test with the 1024MB read source file, block size 32K.
  CURRENT_TEST_BLOCK_SIZE=32K
  CURRENT_TEST_BLOCK_COUNT=32768
  read_fixed_file_size_performance_test

  # Test with the 1024MB read source file, block size 64K.
  CURRENT_TEST_BLOCK_SIZE=64K
  CURRENT_TEST_BLOCK_COUNT=16384
  read_fixed_file_size_performance_test

  # Test with the 1024MB read source file, block size 128K.
  CURRENT_TEST_BLOCK_SIZE=128K
  CURRENT_TEST_BLOCK_COUNT=8196
  read_fixed_file_size_performance_test

  # Test with the 1024MB read source file, block size 256K.
  CURRENT_TEST_BLOCK_SIZE=256K
  CURRENT_TEST_BLOCK_COUNT=4096
  read_fixed_file_size_performance_test

  # Test with the 1024MB read source file, block size 512K.
  CURRENT_TEST_BLOCK_SIZE=512K
  CURRENT_TEST_BLOCK_COUNT=2048
  read_fixed_file_size_performance_test
  
  # Test with the 1024MB read source file, block size 1M.
  CURRENT_TEST_BLOCK_SIZE=1M
  CURRENT_TEST_BLOCK_COUNT=1024
  read_fixed_file_size_performance_test
  
  echo "*                                                                *" >> ${READ_TEST_RESULT_FILE}
  echo "******************************************************************" >> ${READ_TEST_RESULT_FILE}
}

# Runs all of the write performance tests.
launch_all_write_performance_tests ()
{
  CURRENT_TEST_BLOCK_SIZE=1M
  echo "******************************************************************" > ${WRITE_TEST_RESULT_FILE}
  echo "*                                                                *" >> ${WRITE_TEST_RESULT_FILE}
  echo "*   SATA Write Performance Test Results - Fixed Block Size Test  *" >> ${WRITE_TEST_RESULT_FILE}
  echo "*                                                                *" >> ${WRITE_TEST_RESULT_FILE}
  echo -e "* File Size (MB):\tXfer time (ms):\tRate (MB/s):\tBlock:\t *" >> ${WRITE_TEST_RESULT_FILE}
  echo "*                                                                *" >> ${WRITE_TEST_RESULT_FILE}
  echo "******************************************************************" >> ${WRITE_TEST_RESULT_FILE}
  echo "*                                                                *" >> ${WRITE_TEST_RESULT_FILE}
  
  # Run the varying file length tests 1MB to 1024MB without changing the block size used.

  # Test with the 1MB write source file.
  CURRENT_TEST_BLOCK_COUNT=1
  write_fixed_block_size_performance_test
  
  # Test with the 2MB write source file.
  CURRENT_TEST_BLOCK_COUNT=2
  write_fixed_block_size_performance_test
  
  # Test with the 4MB write source file.
  CURRENT_TEST_BLOCK_COUNT=4
  write_fixed_block_size_performance_test
  
  # Test with the 8MB write source file.
  CURRENT_TEST_BLOCK_COUNT=8
  write_fixed_block_size_performance_test
  
  # Test with the 16MB write source file.
  CURRENT_TEST_BLOCK_COUNT=16
  write_fixed_block_size_performance_test
  
  # Test with the 32MB write source file.
  CURRENT_TEST_BLOCK_COUNT=32
  write_fixed_block_size_performance_test
  
  # Test with the 64MB write source file.
  CURRENT_TEST_BLOCK_COUNT=64
  write_fixed_block_size_performance_test
  
  # Test with the 128MB write source file.
  CURRENT_TEST_BLOCK_COUNT=128
  write_fixed_block_size_performance_test
  
  # Test with the 256MB write source file.
  CURRENT_TEST_BLOCK_COUNT=256
  write_fixed_block_size_performance_test
  
  # Test with the 512MB write source file.
  CURRENT_TEST_BLOCK_COUNT=512
  write_fixed_block_size_performance_test
  
  # Test with the 1024MB write source file.
  CURRENT_TEST_BLOCK_COUNT=1024
  write_fixed_block_size_performance_test
  
  # Test with the 2048MB write source file.
  CURRENT_TEST_BLOCK_COUNT=2048
  write_fixed_block_size_performance_test

#  
# These file sizes are too large for BusyBox dd to use on FAT32.
# 
  
  # Test with the 4096MB write source file.
#  CURRENT_TEST_BLOCK_COUNT=4096
#  write_fixed_block_size_performance_test

  # Test with the 8192MB write source file.
#  CURRENT_TEST_BLOCK_COUNT=8192
#  write_fixed_block_size_performance_test
  
  # Test with the 16384MB write source file.
#  CURRENT_TEST_BLOCK_COUNT=16384
#  write_fixed_block_size_performance_test
  
  # Test with the 32768MB write source file.
#  CURRENT_TEST_BLOCK_COUNT=32768
#  write_fixed_block_size_performance_test
  
  # Test with the 65536MB write source file.
#  CURRENT_TEST_BLOCK_COUNT=65536
#  write_fixed_block_size_performance_test

  echo "*                                                                *" >> ${WRITE_TEST_RESULT_FILE}
  echo "******************************************************************" >> ${WRITE_TEST_RESULT_FILE}

  echo "******************************************************************" >> ${WRITE_TEST_RESULT_FILE}
  echo "*                                                                *" >> ${WRITE_TEST_RESULT_FILE}
  echo "*   SATA Write Performance Test Results - Fixed File Size Test   *" >> ${WRITE_TEST_RESULT_FILE}
  echo "*                                                                *" >> ${WRITE_TEST_RESULT_FILE}
  echo -e "* File Size (MB):\tXfer time (ms):\tRate (MB/s):\tBlock:\t *" >> ${WRITE_TEST_RESULT_FILE}
  echo "*                                                                *" >> ${WRITE_TEST_RESULT_FILE}
  echo "******************************************************************" >> ${WRITE_TEST_RESULT_FILE}
  echo "*                                                                *" >> ${WRITE_TEST_RESULT_FILE}
  
  # Run the varying block size tests 1K to 1M without changing the file size used.

  # Test with the 1024MB write source file, block size 1K.
  CURRENT_TEST_BLOCK_SIZE=1K
  CURRENT_TEST_BLOCK_COUNT=1048576
  write_fixed_file_size_performance_test
  
  # Test with the 1024MB write source file, block size 2K.
  CURRENT_TEST_BLOCK_SIZE=2K
  CURRENT_TEST_BLOCK_COUNT=524288
  write_fixed_file_size_performance_test

  # Test with the 1024MB write source file, block size 4K.
  CURRENT_TEST_BLOCK_SIZE=4K
  CURRENT_TEST_BLOCK_COUNT=262144
  write_fixed_file_size_performance_test

  # Test with the 1024MB write source file, block size 8K.
  CURRENT_TEST_BLOCK_SIZE=8K
  CURRENT_TEST_BLOCK_COUNT=131072
  write_fixed_file_size_performance_test

  # Test with the 1024MB write source file, block size 16K.
  CURRENT_TEST_BLOCK_SIZE=16K
  CURRENT_TEST_BLOCK_COUNT=65536
  write_fixed_file_size_performance_test

  # Test with the 1024MB write source file, block size 32K.
  CURRENT_TEST_BLOCK_SIZE=32K
  CURRENT_TEST_BLOCK_COUNT=32768
  write_fixed_file_size_performance_test

  # Test with the 1024MB write source file, block size 64K.
  CURRENT_TEST_BLOCK_SIZE=64K
  CURRENT_TEST_BLOCK_COUNT=16384
  write_fixed_file_size_performance_test

  # Test with the 1024MB write source file, block size 128K.
  CURRENT_TEST_BLOCK_SIZE=128K
  CURRENT_TEST_BLOCK_COUNT=8196
  write_fixed_file_size_performance_test

  # Test with the 1024MB write source file, block size 256K.
  CURRENT_TEST_BLOCK_SIZE=256K
  CURRENT_TEST_BLOCK_COUNT=4096
  write_fixed_file_size_performance_test

  # Test with the 1024MB write source file, block size 512K.
  CURRENT_TEST_BLOCK_SIZE=512K
  CURRENT_TEST_BLOCK_COUNT=2048
  write_fixed_file_size_performance_test
  
  # Test with the 1024MB write source file, block size 1M.
  CURRENT_TEST_BLOCK_SIZE=1M
  CURRENT_TEST_BLOCK_COUNT=1024
  write_fixed_file_size_performance_test
  
  echo "*                                                                *" >> ${WRITE_TEST_RESULT_FILE}
  echo "******************************************************************" >> ${WRITE_TEST_RESULT_FILE}
}

# Displays the SATA Performance Test results.
print_sata_performance_test_results ()
{
  echo " "
  
  # Show the results file contents for the read performance tests.
  cat ${READ_TEST_RESULT_FILE}
  
  echo " "
  
  # Show the results file contents for the write performance tests.
  cat ${WRITE_TEST_RESULT_FILE}
}

# Displays the SATA Performance Test start banner.
print_sata_performance_test_start_banner ()
{
  echo " "
  echo "******************************************************************"
  echo "***                                                            ***"                
  echo "*** STARTING SATA PERFORMANCE TEST V${SATA_PERFORMANCE_TEST_VERSION_NUMBER}                        ***"
  echo "***                                                            ***"
  echo "******************************************************************"
  echo " "
}

# Display the "Start" test banner so that the test operator can see that the 
# SATA test is starting.
print_sata_performance_test_start_banner

# Mount the SATA drive used for performance testing.
check_and_mount_sata_test_device

# Check to make sure read source files are available for read tests.
check_for_sata_read_speed_source_files

# Performs all of the read performance tests.
launch_all_read_performance_tests

# Performs all of the write performance tests.
launch_all_write_performance_tests

# Display the test results.
print_sata_performance_test_results

# Finished testing
echo " "
echo "SATA PERFORMANCE TEST COMPLETE!"

# END HERE: All done with the testing.
