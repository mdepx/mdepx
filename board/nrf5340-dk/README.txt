This is a board code for nRF5340 dual-core ARM Cortex-M33.

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

			app {
				module default;
			};
		};
	};
};
