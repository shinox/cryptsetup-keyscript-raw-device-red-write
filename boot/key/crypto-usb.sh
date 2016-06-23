#!/bin/sh

echo ">>> Trying to get the key from agreed space <<<" >&2
modprobe usb-storage >/dev/null 2>&1
sleep 4
OPENED=0
disk="/sys/block/sdb"
boot_dir="/boot"
readkey="/boot/key/readkey"
echo ">>> Trying device: $disk <<<" >&2
F=$disk/dev
if [ 0`cat $disk/removable` -eq 1 -a -f $F ]; then
	mkdir -p $boot_dir
	mount /dev/sda1 $boot_dir -t ext2 >&2
	echo ">>> Attempting key extraction <<<" >&2
	if [ -f $readkey ]; then
		# prints key array to the caller
		$readkey 4096
		OPENED=1
	fi
	umount $boot_dir >&2
fi


if [ $OPENED -eq 0 ]; then
	echo "!!! FAILED to find suitable key !!!" >&2
	echo -n ">>> Try to enter your password: " >&2
	read -s -r A
	echo -n "$A"
else
	echo ">>> Success loading key <<<" >&2
fi
