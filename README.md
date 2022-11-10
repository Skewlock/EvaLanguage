# Eva Language

## Summary:

### Assembler:
- [Usage](#usage)
- [The language](#the-language)
- [To-Do List](#to-do-list)

### Usage:

To use the assembler you'll need to get the code from the github repository. Note that there is already a binary compiled for linux (Kubuntu 22.04) in there but you might have to recompile it anyway. 

Once you have the binary you can write your assembly in a file ending in .evasm (no it's not necessary but it looks good if I start having my own extension didn't it ?)

Once you're done, you can run `./assembler <input_file> <output_file>` and it will generate the binary requested.
Note that the binary is just meant to run in the Eva Virtual Machine, not straight on your computer.
Note that if you haven't created the output file it will create it for you no problems.

### The language:

I'm writing a big documentation aside if you want more details on how the language works. I think I will link it here when I'm dine writing it. Keep in mind that since I'm the only one to work on this project it's a lot of work.

### To-do list:

Here's the things I'm working on and/or want to implements:

#### Currently working on:

- Cleaning up the code (it's really really messy I can do this way better)
- Adding more facilities (labels for example, sections and pseudo-instructions)

#### Planning to add:

- Honestly since this project goes in pair with the Virtual Machine I aim to develop both in parallel so basically if the VM supports I/O this has to do the same and so on
