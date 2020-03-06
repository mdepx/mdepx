This is an initialization code for nRF5340 dual-core ARM Cortex-M33.

A complete Bluetooth demo application for this CPU could be found here:
https://github.com/machdep/nrf5340

The initialization code split between two directories:
app - application core code
net - network (RADIO) core code

A typical build config entry could look like:

mdepx {
	module board;

	board {
		module nrf5340-dk;

		nrf5340-dk {
			module app;
		};
	};
};
