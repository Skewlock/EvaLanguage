# Eva Language

## Summary:

 ### Virtual machine:

 - [Registers](#registers)
 - [Specifications](#specifications)
 - [todo list](#todo-list)

### Registers:

The Eva Virtual Machine are divided in two categories: General purposes registers, and Specific purpose registers.

#### General purposes registers:

General purposes registers are the only one you have access to in your code. The table under recaps everything:

|Number| Name|Purpose|Size
|-|-|-|-|
|0|R0|General|64bits|
|1|R1|General|64bits|
|2|R2|General|64bits|
|3|R3|General|64bits|
|4|R4|General|64bits|
|5|R5|General|64bits|
|6|R6|General|64bits|
|7|R7|General|64bits|
|8|R8|General|64bits|
|9|R9|General|64bits|
|10|R10|General|64bits|
|11|R11|General|64bits|
|12|R12|General|64bits|
|13|RSP|Stack Pointer|64bits|
|14|RLR|Link Register|64bits|
|15|RPC|Program Counter|64bits|


#### Specific registers:
Specific registers are smaller ones with specific use. They are read-only.
The table under recaps everything:
|Name|Purpose|Size|
|-|-|-|
|RSR|Status register|48bits|
|RIR|Instruction Register|48bits|

---

### Specifications:

If you don't change anything the Virtual Machine will use a file named `memory.mem` as RAM. It will basically load the file content in the first bytes in RAM, then fill the rest with 0 (yes it's inefficient but efficiency is not the goal when things do not works). The RAM size is by default 32768 (the minimum size).

The Virtual Machine is a bit configurable, basically here are the changes you can do. Everything is done using flags in the command. 

`-fileName=<file>` changes the filename for the RAM. Basically the RAM is loaded via a file. The file needs to contain a header and the binary of your code. The code must be written at address 0x1000. 

`-ramSize=<number>` changes the RAM size to the number inputted. The number has to be between 32768 and 4294967295. (32kb and 4gb)

Further configurations will be added later. Check [todo list](##todo-list) to see what I wanna add to this project.

### Todo list:

Here's the things I'm working on and/or want to implements:

#### Currently working on:
- fixing all memory leaks
- adding more checks to prevent future bugs or security issues
#### Planning to add:
- Customizable clock speed (for now there isn't even a clock except your CPU's speed)
- GUI (at least better UI, even if it's only CLI)
- Complete hardware support (for now it's basically just RAM and CPU. I'd like to have the ability to display stuff on screen, send serial transmissions, peripherals etc...)

#### Uncertain idea not really a priority:
- QEMU support (I'd love to be ableto run this as a hardware in QEMU)
