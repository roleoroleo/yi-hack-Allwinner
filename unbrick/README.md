## Content

This procedure allows you to unbrick your cam using a backup file (which you did previously).

## How to use

1. Clone this repo on a linux environment.
2. Copy your home partition (mtdblock3.bin) in the folder corresponding to your model.
3. Enter to the unbrick folder
   `cd unbrick`
4. Run the build command to create the unbrick partition:
   
   `./build.sh`
   
5. You will find the file home_XXX.gz in the folder corresponding to your model.
6. Unzip it in the root folder of your cam.
7. Switch on the cam and wait for the cam to come online.

## DISCLAIMER
**NOBODY BUT YOU IS RESPONSIBLE FOR ANY USE OR DAMAGE THIS SOFTWARE MAY CAUSE. THIS IS INTENDED FOR EDUCATIONAL PURPOSES ONLY. USE AT YOUR OWN RISK.**

# Donation
If you like this project, you can buy Roleo a beer :)
[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=JBYXDMR24FW7U&currency_code=EUR&source=url)
