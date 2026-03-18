# ReworkCTF challenge list

## Silkscreen / Solder Mask

These challenges are not electronically gradeable, so they're numbered separately from the others (with a SM- prefix).

### SM-1 (100 points)

Oops, we forgot to label the debug connector! Mark it "JTAG" in white ink.

### SM-2 (100 points)

There's a big hole in the soldermask on the back of the board. Don't ask how it got there, just fix it.

## Surface Conductor

### Challenge 01 (100 points)

A dust speck landed on a photomask and caused a small dot of copper to short the signal trace to the adjacent ground. Clear the short.

### Challenge 02 (200 points)

An etch defect caused two signal traces to short to each other. Clear the short.

### Challenge 08 (100 points)

Somebody dropped a heavy tool on the board and gouged out a short segment from a trace. Reconnect the missing piece.

## Multilayer Conductor / Via

### Challenge 09 (400 points)

We swapped a differential pair for easier layout and didn't realize the SERDES macro didn't have a polarity swap bit. If that wasn't bad enough, it's on an internal layer.

Swap the pair.

(You must do an inner layer circuit edit for this problem. The BGA breakout traces and vias are out of scope.)

### Challenge 10 (300 points)

Somehow we forgot a via between the top and bottom layer. Restore the connection.

### Challenge 11 (400 points)

A zone fill got screwed up and a via from front to back is shorted to the power plane. Clear the short but preserve the intended connection.

### Challenge 15 (100 points)

Somebody forgot a pullup on an open-drain signal. Add a 10K resistor from the trace to the 3V3 rail.

## SMT Component

### Challenge 03 (100 points)

Whoops, one of the LEDs was put on with reversed polarity. Flip it.

### Challenge 04 (200 points)

Place an 10K ohm 0201 resistor on the provided footprint.

### Challenge 05 (300 points)

Place an 10K ohm 01005 resistor on the provided footprint.

### Challenge 06 (200 points)

The intern who made our SOIC flash footprint forgot to put a soldermask aperture on one of the pins! Remove the component, clear the soldermask off the pad, and reconnect the flash.

NOTE: The challenge 6 status LED will not illuminate unless challenge 7 has also been completed.

### Challenge 07 (300 points)

While our previous technician was doing challenge 6, they got too aggressive desoldering and lifted a pad. Install a new pad on the board and restore connectivity to the pin.

NOTE: The challenge 7 status LED will not illuminate unless challenge 6 has also been completed.

## BGA

### Challenge 12 (300 points)

Our solder paste is expired and two balls didn't get good paste coverage so they're not making good contact. Fix it.

(A test point is provided to allow electrical measurements of the test signal)

### Challenge 13 (400 points)

Our BGA breakout generator script had a bug and one of the BGA breakout vias is missing a dogbone trace. Luckily, we discovered the problem BEFORE populating the board, so you have easy access to the BGA site.

### Challenge 14 (500 points)

This is the same exact defect as challenge 13, except somehow it went unnoticed until the board was populated. Have fun.

TODO
* Connect a missing trace to a BGA pad (before populating)
* Add a missing via + dogbone under a BGA (before populating)
* Add a missing via + dogbone under a BGA (after populating)

