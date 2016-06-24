# cryptsetup-keyscript-raw-device-red-write
Additional scripts allowing to create or write binary key into raw removable disk and use it later in crypttab with custom "keyscript=" option.
Cryptsetup provides tons of option in creating the "keyfile" and use it in own encrypted system however what if all partitions on removable device are already allocated and storing "keyfile" on the partition is not an option. This scientific attempt is to crate an opportunity to utilise last sectors of RAW block device and store key there in binary format. To be extra safe the default block size 512 from the end of the disk is accounted for.

The code is provided without any warranty, any use of the compiled "readkey" or "writekey" is strictly on your own risk !!!
The code quality also may press some doubts hence inspection before "compile and use" is strictly adviced.

Please consult first the cryptsetup manual and create encrypted partition for your platform(OS flavor). Once successfully encrypted and tested using password the following can be attempted at your ow risk (again).
**_Also to note before editing "crypttab" copy your system "initrd.img" to "initrd.img.bak", this way if anything goes wrong you can always use ".bak" initrd file during boot to get to the system and correct mistakes_**:
```bash
sudo cp -aRf /boot/initrd.img-`uname-r` /boot/initrd.img-`uname-r`.bak
```
After adding/modding "crypttab" it is necessary to update "initrd", usually by using (on debian):  <br />
```bash
sudo update-initramfs -u -k `uname -r`  
```
**::readkey.c**  <br />
Simple program to read from raw disk device in linux. Program will attempt to read bytes starting from the end of the
device minus length of the default block size=512, minus keySize. The read bytes then will be printed to the "caller"
i.e. the /boot/key/crypto-usb.sh script which is referenced in /etc/crypttab.  <br />
To compile use:  <br />
```bash
gcc readkey.c -o readkey
```
How to use (please see /boot/key/crypto-usb.sh for sample usage):  <br />
```bash
readkey </path/to/device> <keysize>
```
Or for default keysize 2048
```bash
readkey </path/to/device>
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
When keysize is not provided default size of the key is set to 2048. It is necessary to move keyfile off-site to other secure location after tests proven successfull or deletion of the file  <br />
To verify both written and stored keys are the same "diff" can be used. Diff will print no output if content is identical:
```bash
diff -B <(./readkey </path/to/device> 4096) <(cat .tmpckey)
```
Or for existing key which have bin written to the raw device:
```bash
diff -B <(./readkey </path/to/device> <written keysize>) <(cat /path/to/keyfile)
```
To do:  
  1.) Provide means of using it directly in crypttab as "luks,keyscript=readkey dev keysize"  
  2.) Auto select removable device in readkey.c based on disk ID.  
  3.) Auto encrypt key before write using Disk ID.  
  4.) Integrate with "cryptsetup" as an option into "crypttab" ("luks,usbdev:id=<key size>" or similar approach).  

