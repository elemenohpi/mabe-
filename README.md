
# MABE 

### This is not an official version of [MABE](https://github.com/Hintzelab/MABE/wiki/)

</br>

This version of mabe includes "CoopWorld" which is a world with the goal of evolving cooperation/cooperative behavior. 

</br>

### Setup guide:

In order to run this code, you need to install python3 and all the dependencies required to run the official MABE (Follow the instructions in the mabe wiki from the above link). A shortlist of the required dependencies and how to install them in a linux machine is as follows:

</br>

- You will need a linux machine or a linux-like terminal in order to install dependencies and run this code (different softwares are available for Windows operating systems to enable a linux-like terminal). This link might help: https://www.howtogeek.com/249966/how-to-install-and-use-the-linux-bash-shell-on-windows-10/. If you're using a Mac OS, then although the installation commands may differ, you have access to a terminal (press Command + Space and type terminal and then press the Return key to launch it)

</br>

- python 3.5+ : 

``` $ sudo apt-get install python3.6 ``` This can change considering the python version you intend to install

</br>

- matplotlib, pandas, numpy, and scipy:

``` pip install matplotlib, numpy, pandas, scipy ```

</br>
 In order to compile the code, you need to download the code first. Click on the green button (Clone or download) on top right of this page and select Download as Zip to download the code to your computer.
</br>

In order to compile the code, run the following command in the root directory of mabe (your downloaded folder after unziping it) after downloading it: </br>

```python pythonTools/mbuild.py``` you may want to use "-p X" to parallelize the compile procedure. X is the number of CPU cores you intend to use in order to build/compile the code (usually a good number is one plus the total number of your available CPU cores)

</br>

To run the mabe (after compiling), execute the following command in the root directory of mabe: </br>

``` ./mabe ``` This code will run a basic version of mabe with default settings.

</br> to run CoopWorld you will need to modify the settings_world.cfg file included in the root directory of mabe and run the follwoing command (it's highly adviced to read through all the settings files settings.cfg, settings_world.cfg and settings_organism.cfg before you start your experiments):

``` ./mabe -f settings* ```

</br>
if you are on a PC and compiled the code with visual studio, then you should run this command: </br>

``` x64/Release/mabe.exe -f settings* ```

</br>

CoopWorld is designed and developed by: </br>

Iliya "eLeMeNOhPi" Alavy and Darren Charles Incorvaia 