# reworkctf

## What is it?

This is a points-based challenge board intended for practicing of PCB rework and circuit editing skills. This is not a circuit *debug* challenge. The defect for each stage is well documented, your task is to correct it so that the circuit functions properly.

This is also not a learn-to-solder kit. If you're not comfortable populating boards with 0402 passives and larger pitch (0.8mm) BGAs, this is probably not a project you want to undertake until you have the basics down first.

That said, the challenges range from very easy to fairly difficult, so it's totally fine to try some of them then put the board on a shelf for a while until your skills improve. Inner layer circuit edits are among the most challenging rework tasks around, there's a reason they're worth the most points!

This is also intended to be a relatively low-cost board so you can practice without the pressure of reworking a production design with a big FPGA and five-digit price tag. If you destroy a couple of these boards then get the hang of things and your third attempt succeeds, great - you've learned something.

And if the board survives, you've got a STM32H750 dev platform with a UART, two PMODs, a SPI flash, and a lot of LEDs to maybe do something useful with.

IMPORTANT NOTE: Read the challenge list before populating the front side of the PCB. Some of the BGA-site challenges have two versions, an "easy" version which can be done on a bare PCB before populating allowing easy access to the BGA footprint, and a "hard" version which requires you populate the board before doing the rework. Unless you want to do the hard version twice, don't populate the board until you've done the easier ones. This does require doing some of the challenges out of order, unless you want to make two boards and do different challenges on each.

Most notably, the following challenges should be done before populating the BGA:
* Challenge 13

## How do I complete a given challenge?

It's up to you, within reason. If you are able to restore the intended electrical connectivity to the point that the firmware lights up the corresponding "complete" LED and you've followed any restrictions listed in the problem text, you've done it. Please follow the spirit of the challenges, many of them can be "cheated" by jumpering adjacent pins on the MCU together or similar but the point of this board is skill development. If you don't actually fix the intended defect you're not learning.

Almost all challenges are electrically testable by the MCU firmware; those that involve soldermask or silkscreen rework can be graded by visual inspection of the board. If those look good, they probably are.

The [IPC rework guides](https://www.circuitrework.com/guides/guides.html) are a good starting point for general repair techniques.

If you get really stuck on a specific challenge you can message me on Mastodon at[@azonenberg@ioc.exchange](https://ioc.exchange/@azonenberg]) but please make a reasonable attempt to plan a solution first.

## I still have an old rev board, where's the info for that?

The legacy revision 1 challenge, built around a Spartan-3A FPGA, can be found in the "v1" directory and is being kept there in case anybody has an old board and wants to refer to the design. Anyone building the challenge now should use the v2 design which is based on parts that (as of this writing in early 2026) are readily available and inexpensive.

## What tools are required?

A fine point soldering iron, good tweezers, and hot air rework station are a must to attempt any of the challenges. A stereo microscope is strongly recommended.

The silkscreen and solder mask repair challenges require UV or heat cured coating materials and a curing lamp or oven appropriate for the coating chemistry. UV resin allows much more rapid iteration and avoids heat stress on the remainder of the system so we strongly recommend it over heat cure systems.

Surface trace cuts and rerouting require a scalpel or rotary tool/mill with very fine cutters. While the inner layer circuit edits are *possible* to complete with hand scraping or a handheld rotary tool, we strongly recommend use of a 3-axis mill and microscope for the highest grade results and greatest chance of success.

## What are the challenges?

See [the list here](/CHALLENGES.md). Each challenge is marked with silkscreened text on the PCB to make it easy to find.
