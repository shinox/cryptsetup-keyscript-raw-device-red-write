# cryptsetup-keyscript-raw-device-red-write
Additional scripts allowing to create or write binary key into raw removable disk and use it later in crypttab with custom "keyscript=" option.
Cryptsetup provides tons of option in creating the "keyfile" and use it in own encrypted system however what if all partitions on removable device are already allocated and storing "keyfile" on the partition is not an option. This scientific attempt is to crate an opportunity to utilise last sectors of RAW block device and store key there in binary format. To be extra safe the default block size 512 from the end of the disk is accounted for.

The code is provided without any warranty, any use of the compiled "readkey" or "writekey" is strictly on your own risk !!!
The code quality also may press some doubts hence inspection before "compile and use" is strictly adviced.

Please consult first the cryptsetup for your platform(OS flavor). Once successfully encrypted and tested using password the following can be attempted at your ow risk (again).
**Also to note before adding and editing "crypttab" copy your system "initrd.img" to "initrd.img.bak", this way if anything goes wrong you can always use ".bak" initrd file during boot to get to the system and correct mistakes:
```bash
sudo cp -aRf /boot/initrd.img-`uname-r` /boot/initrd.img-`uname-r`.bak
```
After adding/modding "crypttab" it is necessary to update "initrd", usually by using (on debian):  <br />
```bash
sudo update-initramfs -u -k `uname -r`  
```
If something goes wrong and system refuses to boot restart and in Grub manu choose edit and add ".bak" at the end of "initrd..." file then F-10 (backup is important). 
**::readkey.c**  <br />
Simple program to read from raw disk device in linux. Program will attempt to read bytes starting from the end of the
device minus length of the default block size=512, minus keySize. The read bytes then will be printed to the "caller"
i.e. the /boot/key/crypto-usb.sh script which is referenced in /etc/crypttab.  <br />
To compile use:  <br />
```bash
gcc readkey.c -o readkey  <br />
```

How to use (please see /boot/key/crypto-usb.sh for sample usage):  <br />

```bash
readkey <keysize>  
```

**::writekey.c**  <br />
Simple program to write existing key information to the raw block device or to generate pseudo random bits and write these
to the raw block device as well as to the .tmpckey file which can be then used directly in "cryptsetup".  <br />
To compile use:  <br />
```bash
gcc writekey.c -o writekey
```
To write existing key to device:  <br />
```bash
writekey </path/to/keyfile> </path/to/removable/sd*>
```
To generate and write pseudo random key, key will be saved to temporary file ".tmpckey". Permissions to file will be set as 0600  <br />
```bash
writekey genwrite </path/to/removable/sd*> <keysize in multiples of 512>
```
When keysize is not provided default size is set to 2048.  <br />
