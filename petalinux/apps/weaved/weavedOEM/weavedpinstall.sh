#!/bin/sh  
# Weaved OEM evaluation installer for Xilinx PetaLinux on ARM9
# With user input, launches daemons for ssh/22 and http/80.
# The script weavedstart.sh is created which can be launched again to 
# run the daemons after a power cycle for example.

compatibleDaemon=""
VERSION=ARM_OEM_XILINX_1.1
DIR=/etc/weaved
STARTUPDIR=/etc/init.d
TMPDIR=/tmp

####### Display version and check for root user
displayVersion()
{
    echo "Weaved connection installer Version: $VERSION"
    echo
# check for sudo user at this point
    if [ $(id -u) != 0 ]; then
        echo "Running weavedinstall.sh requires root access." 1>&2
        echo "Please run sudo ./weavedinstall.sh." 1>&2
        exit 1
    fi
}
################## End displayVersion ########

######### find directory to store pid file
findPIDfolder()
{

        if [ -d /var/run ]; then
                PIDFolder=/var/run
        elif [ -d /var ]; then
                PIDFolder=/var
        else
                PIDFolder=$(pwd)
        fi
#       echo "PIDFolder=$PIDFolder"
}

############ makeWeavedFolder (holds the enablement files per connection)
makeWeavedFolder()
{
    if [ -d $DIR ]; then
	echo "$DIR already exists" > /dev/null
    else
	mkdir -p $DIR
    fi
}
###### End of makeWeavedFolder #######
	
######### confirm Install ##########
# parameters
# service name, e.g. SSH or HTTP
# enablement file name, e.g. ssh.xilinx
# startup script/pid file name root, e.g. weavedssh22

confirmInstall()
{
    if ask "Do you want attach Weaved to your $1 server?"; then
        echo
	cp $compatibleDaemon /usr/bin	
        if [ -f $DIR/$2 ]; then
                echo "$1 was previously installed."
                if ask "Do you wish to keep your previous settings?" ; then
                        return 1
                fi
        fi
        echo "Launching $1 daemon..."
	# copy init enablement file for this service
        cp $2 $DIR
	# copy startup script to system folder
	cp $3.sh $STARTUPDIR
        echo "$3.sh start" >> ./weavedstart.sh
        launched=1
        # set -x
        running=$(ps -w | grep "$2" | grep "$compatibleDaemon" | awk '{ print ; }')
#        echo "Running = $running"
        if [ "$running" == "" ]; then    # found active process matching this one
		$STARTUPDIR/$3.sh start
	else
	    echo
	    echo "You have changed the configuration of the Weaved $1 daemon."
	    echo "This daemon is already running.  To make the changes take effect,"
	    echo "you should restart the daemon."
    	    echo "Caution - restarting will close any active connections using this daemon."
	    echo
	    if ask "Do you want to restart the $1 daemon now"; then
		$STARTUPDIR/$3.sh restart
	    fi
        fi
    fi
}

###################################

######### Ask Function #########
ask()
{
    while true; do
        if [ "${2:-}" = "Y" ]; then
            prompt="Y/n"
            default=Y
        elif [ "${2:-}" = "N" ]; then
            prompt="y/N"
            default=N
        else
            prompt="y/n"
            default=
            fi
    # Ask the question
    read -p "$1 [$prompt] " REPLY
    # Default?
    if [ -z "$REPLY" ]; then
        REPLY=$default
    fi
    # Check if the reply is valid
    case "$REPLY" in
    Y*|y*) return 0 ;;
    N*|n*) return 1 ;;
    esac
    done
}
######### End Ask Function #########

###### Beginning of installer

displayVersion
findPIDfolder
makeWeavedFolder

# attempt to execute each installed daemon
# only if output includes "Looks compatible" do we proceed

echo "Checking for compatible Weaved program..."
echo

for file in weavedConnectd*; do
	result="$(./$file -n > /tmp/weavedlog.txt)" 2>/dev/null
	if [ "$(grep passed /tmp/weavedlog.txt | wc | awk '{ print $1; }')" == "3" ]; then
#		echo "-----------------------"
#		echo "$file works. "
		compatibleDaemon=$file 
	fi
done

if [ "$compatibleDaemon" != "" ]; then
	echo "OK"
else
	echo
	echo "Did not find a compatible Weaved binary."
	echo "Please confirm that this device is connected to the internet."
	echo "Please contact Weaved support with the following information:"
	echo
	echo $(uname -a)
	cat /proc/cpuinfo | grep Hardware
	cat /proc/cpuinfo | grep CPU
	echo
#	route
#	traceroute www.weaved.com
	exit
fi

launched=0

if [ -f ./weavedstart.sh ]; then
	rm ./weavedstart.sh
fi

#
# Check if we have installed this before
#
mount /dev/mmcblk0p1 /mnt

if [ -d /mnt/UUID ]
then
#   Restore the UUID files
    mkdir -p /etc/weaved
    cp /mnt/UUID/* /etc/weaved/
else
    echo First Installation of Weaved Connect
fi

echo
confirmInstall SSH Avnet-ssh-22  weavedssh22

echo
confirmInstall HTTP Avnet-http-reverse-proxy-80 weavedhttp80

if [ $launched -eq 1 ]; then
	chmod +x ./weavedstart.sh
	echo
	echo "Now go to your Weaved Account Device List from a computer on the same LAN"
	echo "as this device and register the Weaved attachments you just installed."
fi

if [ -d /mnt/UUID ]
then
#   Start the services
    ./weavedhttp80.sh start
    ./weavedssh22.sh start
else
#   Save the enablement files in persistant storage
    mkdir /mnt/UUID
    cp /etc/weaved/* /mnt/UUID/
fi

umount /mnt
