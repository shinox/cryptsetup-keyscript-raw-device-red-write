# cryptsetup-keyscript-raw-device-red-write
Cryptsetup suporting additional scripts allowing to create or write key into raw removable disk and use it later in crypttab with custom "keyscript=" option.

The code is provided without any warranty, any use of the compiled readkey or writekey is strictly on your own risk !!!

Please consult first the cryptsetup for your platform(OS flavor). Once successfully encrypted and tested using password following can be attempted at your ow risk.

:readkey.c
  Simple program to read from raw disk device in linux. Program will attempt to read bytes starting from the end of the device minus length of the default block size=512, minus keySize. The read bytes then will be printed to the "caller" i.e. the /boot/key/crypto-usb.sh script which is referenced in /etc/crypttab.
  To compile use: 
    gcc readkey.c -o readkey
  To use (please see /boot/key/crypto-usb.sh for sample usage):
    readkey <keysize>
    
:writekey.c
  Simple program to write existing key information to the raw block device or to generate pseudo random bits and write these to the raw block device as well as to the .tmpckey file which can be then used directly in "cryptsetup".
  To write existing key to device:                                            
    writekey </path/to/keyfile> </path/to/removable/sd*>                    
 To generate and write pseudo random key, key will be saved to temporary file .tmpckey 
    writekey genwrite </path/to/removable/sd*> <keysize in multiples of 512> 
 When keysize is not provided default size is set to 2048.                     
