#!/bin/sh

. /etc/init.d/functions

start () {
    echo -n "Forcing USB OTG to host mode (enabling +5V power rail): "

    devmem 0xe0002170 32 0x600a0067
}

stop() {
    echo -n "Forcing USB OTG to device mode (disabling +5V power rail): "
    devmem 0xe0002170 32 0x600a0007
    return 0
}

case "$1" in
    start)
        start
        echo "OK"
        ;;
    stop)
        stop
        echo "OK"
        ;;
    *)
        echo "Usage: {start|stop}"
        exit 1;
        ;;
esac
exit $?

