# PSX Breakout

This is a simple program written in C for the PlayStation 1 (PSX). It is developed and tested in an enumulator but should be able to run on real hardware (not tested).


# Motivation to do this 

My childhood was full of video games, PlayStation and a desire to make video games (especially racing ones). Instead ended up going into the automotive world.

To get back those lost dreams I started an MSc in Computer Science at Bath, but much to my disappointment, most of the content is either real abstract or modern business oriented (backing apps, Java, AI, etc).

So I took the CS50x course because:

- The ad videos seem awesome
- The content seems cool
- C is one of the main languages

I absolutely did not regret it! And learning pointers, and that discipline orientation of C! I actually used some of the lessons from the CS50x course to ace my MSc. So it truly does teach the basics at a VERY good level.

2020 has been a tough year, but I encountered some really cool people on a PSX developers on a discord channel. They thought me about the PSX serial port, and Unirom which could be used to send homebrew programs.
This put me on a shopping spree of buying hardware.

The biggest limitation in PSX development has been getting code compiled since the unofficially available official tool chain is design for old machines using based on an old version of GCC. There was an opportunity to help with this (see next section).

In addition I had always beeen a believer in constant backups of a word file, but I realised code is harder to version control and make backups off... and this is when I realised that git is useful not only for collaborative work but also for managing updating, tracking, back-tracking your own code.
So getting familiar with git usage was one of the hidden learning/movations for me too.

Last but not least... I wanted to impress my 2 kids with showing them that their dad can make a simple PlayStation game!!

### Fork in the road
There was the debate of - learn to work with the Sony SDK, or work with modern tools using newer homebrew SDKs?  
- Homebrew SDKs have are modern, supported but less complete and less tested.
- Original SDK is using old C compilers, and is a bit enigmatic, but used a lot so more of its quirks are known.

Timing / luck had it that someone was trying to parse and convert the libs so that we could use a modern GCC. So I partially started this adventure to support that.


# Game Code
So there are many hello world base codes out there. I partially based things on [Wituz](https://github.com/Wituz/wituz-youtube/blob/master/ps1-game-tutorial/PART%205%20-%20Drawing%20sprites/project/imagekit/images.h)  
Part of his code is based on Orion's code.
I expanded on the functions of Wituz' distribution, fixed some typos.
One big thing to fix was that Wituz put a lot of code in constants.h and then just includes it in the main.c file.
This works, but is misuse of include purpose.
So I make a separate basics.c and basics.h file to compile and link instead.  

## Overal Structure

Wituz uses a lot of Object Oriented style coding such as:  
object1 = transform(object1, parameters);  

I think it may be wasteful on a small machine like this, but I decided to go with it to:
- Develop some more OOP thinking
- See how the PSX will handle it



## Basics.c
This contains almost all of the base functionalities.
#### Image Manipulation
Image createImage(unsigned char imageData[]);  
Image moveImage(Image image, int x, int y);  
void drawImage(Image image);  
Image scaleImage(Image img, int xScale, int yScale);  
Image rotImage(Image img, int rotation);  

#### Basic Primitive Functions (Square/line/colour)
Color createColor(int r, int g, int b);  
Line createLine(Color color, int x1, int y1, int x2, int y2);  
Box createBox(Color color, int x1, int y1, int x2, int y2);  
Line moveLine(Line line, int x1, int y1, int x2, int y2);  
Box moveBox(Box box, int x1, int y1);  
Box transposeBox(Box box, int x1, int y1);  
void drawLine(Line line);  
void drawBox(Box box);  
void drawImage(Image image);  

#### Screen Management
void setScreenMode(int mode);  
void setBackgroundColor(Color color);  
void initializeScreen();  
void initializeDebugFont();  
void initializeOrderingTable();  
void display();  
void clearDisplay();  
void clearVRAM();  


## Main Logic
The program draws the squares according to a mask map called grid. Wherever grid is equal to 1, a square is drawn.
Inputs are done via a controller:  
- Start to make the ball bounce.  
- Left and Right to move the player  

The game tracks the position of the ball's X-Y and checks if it hit up/down/left/right and based on that it changes the velX and velY parameters. Also there is a rotation parameter which is the product of velX and velY.
This is why the rotation direction switches when ball changes direction in 1 of the x/y axis.


# Setting Up My Machine
An imperfect write up to get started.

1) Enable WSL from the powershell  
Enable-WindowsOptionalFeature -Online -FeatureName Microsoft-Windows-Subsystem-Linux

2) Install Ubuntu:  
https://www.microsoft.com/en-us/p/ubuntu-2004-lts/9n6svws3rx71?activetab=pivot:overviewtab

3) Install VSCODE  
Let VSCODE realise you have WSL2 and recommend related package [Remote WSL](https://code.visualstudio.com/blogs/2019/09/03/wsl2 )  
Or direct marketplace [link](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-wsl)

4) g++ (clang installed for other reasons - but unnecessary for this goal)  
sudo apt install -y g++-10  
sudo apt install -y g++-mipsel-linux-gnu  
sudo apt install make  
sudo apt install clang-10

5) Clone recursively (this might have been unnecessary - other [option](https://github.com/JonathanDotCel/helloworld_and_flappycredits))  
[Redux](https://github.com/grumpycoders/pcsx-redux)  
Copied several files (especially crt0.s)
But I also use the redux emulator (which you could compile from this source or download his builds).

6) This is the converted psyq libs  
wget [http://psx.arthus.net/sdk/Psy-Q/psyq-4.7-converted-full.7z](http://psx.arthus.net/sdk/Psy-Q/psyq-4.7-converted-full.7z)  
unzip to /pcsx-redux/src/mips/psyq/


## STUFF TO DO ##
to do debugger - need to re-set it up
to do vscode make - installed but no clue how to use
Install another VS code option
https://marketplace.visualstudio.com/items?itemName=technosophos.vscode-make


# Special Thanks!! #
Credits... there are many people:
Sjefcoder helped debug part of the library and provided me with half a serial cable I needed to get coding.  
Sickle, Nicolas Noble and Danhans42 for the insane patience they had with getting me started.
Nicolas and Sickle helped a lot with getting the SDK working under gcc-mipsel.  
Schnappy, Nicolas, Wituz, Sickle, Danhans42, Lameguy64, and many others helped here and there with coding issues especially those related to PSX development.  
Final thanks to the people that came way before me at reverse engineering a lot of the PSX to have a clue where to begin with.  