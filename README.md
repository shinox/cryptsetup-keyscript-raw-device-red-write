# cryptsetup-keyscript-raw-device-red-write
Cryptsetup suporting additional scripts allowing to create or write key into raw removable disk and use it later in crypttab with custom "keyscript=" option.::

The code is provided without any warranty, any use of the compiled readkey or writekey is strictly on your own risk !!!

Please consult first the cryptsetup for your platform(OS flavor). Once successfully encrypted and tested using password following can be attempted at your ow risk.

:readkey.c  <br />
    Simple program to read from raw disk device in linux. Program will attempt to read bytes starting from the end of the device minus length of the default block size=512, minus keySize. The read bytes then will be printed to the "caller" i.e. the /boot/key/crypto-usb.sh script which is referenced in /etc/crypttab.<br />
    To compile use:  <br />
      gcc readkey.c -o readkey  <br />
    To use (please see /boot/key/crypto-usb.sh for sample usage):  <br />
      readkey "keysize"
    
:writekey.c  <br />
    Simple program to write existing key information to the raw block device or to generate pseudo random bits and write these to the raw block device as well as to the .tmpckey file which can be then used directly in "cryptsetup".  <br />
    To write existing key to device:  <br />                                            
      writekey "/path/to/keyfile"  "/path/to/removable/sd*"  <br />                 
    To generate and write pseudo random key, key will be saved to temporary file .tmpckey  <br />
      writekey genwrite "/path/to/removable/sd*" "keysize in multiples of 512"  <br />
    When keysize is not provided default size is set to 2048.  <br />            
    
