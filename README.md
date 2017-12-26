# GBA Game Prototype

**2nd Year University Project** <br/>
**Semester 1** <br/>
**Programming Low Level Architecture**
 
 For this assignment I was tasked to develop a simple tile based game for the game boy advanced using C and NO$GBA debugger.
 
 This assignment proved to challenge my abilities primarily due to being unable to debug the game using Visual Studio and having no knowledge of ASM with the NO%GBA debugger.
 
 The assignment proved to challenge my abilities to develop for a piece of hardware with strict memory and processing constraints due to the GBA only having a few KB of RAM.
 
 This led to interesting design decisions such as instead of loading in an entire spritesheet into memory, only loading in new sprites when required due to the memory capacity being so resticted.
 
 The Game ended up being a simple 2D Platformer.
 
 **Features:** <br/>
 Player Platformer Character able to move, jump and attack <br/>
 Scrolling Tiled Background <br/>
 Collision Detection <br/>
 Snow Particle System <br/>
 Sprite Animations <br/>
 Respawning <br/>
 
 **Skills Demonstrated:** <br/>
  C Programming Knowledge <br/>
  Const Correctness <br/>
  Various Optimisation Techniques (Memory Pooling, Loop Unwinding etc) <br/>
  Memory Aligned Data Structures (Power of 2 Aligned within reason) <br/>
  Knowledge of Programming for Architecture with Low Memory (Use of 8,16,32 signed/unsigned integers only when necessary) <br/>
  Memory Copying through use of Memcpy or GBA's Direct Memory Access <br/>
  
