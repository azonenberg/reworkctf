# ReworkCTF challenge list

## Silkscreen / Solder Mask

These challenges are not electronically gradeable, so they're numbered separately from the others (with a SM- prefix).

### SM-1 (100 points)

Oops, we forgot to label the debug connector! Mark it "JTAG" in white ink.

### TODO


## Surface Conductor

### Challenge 01 (100 points)

A dust speck landed on a photomask and caused a small dot of copper to short the signal trace to the adjacent ground. Clear the short.

### Challenge 02 (200 points)

An etch defect caused two signal traces to short to each other. Clear the short.

## Component

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

## Conductor

### Challenge 08 (100 points)

Somebody dropped a heavy tool on the board and gouged out a short segment from a trace. Reconnect the missing piece.









Rough summary


* Solder an 0201 passive
* Solder an 01005 passive
* Flip an 0402 LED put on backwards
* Clear soldermask from a SOIC pad
* Connect a missing trace to a BGA pad (before populating)
* Add a missing via + dogbone under a BGA (before populating)
* Add a missing via + dogbone under a BGA (after populating)

