#include "ballistic.graphics.font.h"


namespace ballistic {
	namespace graphics {

		vec2 font::font_uv[256] = {
			{real(0.0)	 , real (0)},
			{real(0.0625), real (0)},
			{real(0.125) , real (0)},
			{real(0.1875), real (0)},
			{real(0.25)  , real (0)},
			{real(0.3125), real (0)},
			{real(0.375) , real (0)},
			{real(0.4375), real (0)},
			{real(0.5)	 , real (0)},
			{real(0.5625), real (0)},
			{real(0.625) , real (0)},
			{real(0.6875), real (0)},
			{real(0.75)  , real (0)},
			{real(0.8125), real (0)},
			{real(0.875) , real (0)},
			{real(0.9375), real (0)},

			{ real(0.0), real (0.0625) },
			{ real(0.0625), real (0.0625) },
			{ real(0.125), real (0.0625) },
			{ real(0.1875), real (0.0625) },
			{ real(0.25), real (0.0625) },
			{ real(0.3125), real (0.0625) },
			{ real(0.375), real (0.0625) },
			{ real(0.4375), real (0.0625) },
			{ real(0.5), real (0.0625) },
			{ real(0.5625), real (0.0625) },
			{ real(0.625), real (0.0625) },
			{ real(0.6875), real (0.0625) },
			{ real(0.75), real (0.0625) },
			{ real(0.8125), real (0.0625) },
			{ real(0.875), real (0.0625) },
			{ real(0.9375), real (0.0625) },

			{ real(0.0), real (0.125) },
			{ real(0.0625), real (0.125) },
			{ real(0.125), real (0.125) },
			{ real(0.1875), real (0.125) },
			{ real(0.25), real (0.125) },
			{ real(0.3125), real (0.125) },
			{ real(0.375), real (0.125) },
			{ real(0.4375), real (0.125) },
			{ real(0.5), real (0.125) },
			{ real(0.5625), real (0.125) },
			{ real(0.625), real (0.125) },
			{ real(0.6875), real (0.125) },
			{ real(0.75), real (0.125) },
			{ real(0.8125), real (0.125) },
			{ real(0.875), real (0.125) },
			{ real(0.9375), real (0.125) },

			{ real(0.0), real(0.1875) },
			{ real(0.0625), real(0.1875) },
			{ real(0.125), real(0.1875) },
			{ real(0.1875), real(0.1875) },
			{ real(0.25), real(0.1875) },
			{ real(0.3125), real(0.1875) },
			{ real(0.375), real(0.1875) },
			{ real(0.4375), real(0.1875) },
			{ real(0.5), real(0.1875) },
			{ real(0.5625), real(0.1875) },
			{ real(0.625), real(0.1875) },
			{ real(0.6875), real(0.1875) },
			{ real(0.75), real(0.1875) },
			{ real(0.8125), real(0.1875) },
			{ real(0.875), real(0.1875) },
			{ real(0.9375), real(0.1875) },

			{ real(0.0), real(0.25) },
			{ real(0.0625), real(0.25) },
			{ real(0.125), real(0.25) },
			{ real(0.1875), real(0.25) },
			{ real(0.25), real(0.25) },
			{ real(0.3125), real(0.25) },
			{ real(0.375), real(0.25) },
			{ real(0.4375), real(0.25) },
			{ real(0.5), real(0.25) },
			{ real(0.5625), real(0.25) },
			{ real(0.625), real(0.25) },
			{ real(0.6875), real(0.25) },
			{ real(0.75), real(0.25) },
			{ real(0.8125), real(0.25) },
			{ real(0.875), real(0.25) },
			{ real(0.9375), real(0.25) },

			{ real(0.0), real(0.3125) },
			{ real(0.0625), real(0.3125) },
			{ real(0.125), real(0.3125) },
			{ real(0.1875), real(0.3125) },
			{ real(0.25), real(0.3125) },
			{ real(0.3125), real(0.3125) },
			{ real(0.375), real(0.3125) },
			{ real(0.4375), real(0.3125) },
			{ real(0.5), real(0.3125) },
			{ real(0.5625), real(0.3125) },
			{ real(0.625), real(0.3125) },
			{ real(0.6875), real(0.3125) },
			{ real(0.75), real(0.3125) },
			{ real(0.8125), real(0.3125) },
			{ real(0.875), real(0.3125) },
			{ real(0.9375), real(0.3125) },

			{ real(0.0), real (0.375) },
			{ real(0.0625), real (0.375) },
			{ real(0.125), real (0.375) },
			{ real(0.1875), real (0.375) },
			{ real(0.25), real (0.375) },
			{ real(0.3125), real (0.375) },
			{ real(0.375), real (0.375) },
			{ real(0.4375), real (0.375) },
			{ real(0.5), real (0.375) },
			{ real(0.5625), real (0.375) },
			{ real(0.625), real (0.375) },
			{ real(0.6875), real (0.375) },
			{ real(0.75), real (0.375) },
			{ real(0.8125), real (0.375) },
			{ real(0.875), real (0.375) },
			{ real(0.9375), real (0.375) },

			{ real(0.0), real (0.4375) },
			{ real(0.0625), real (0.4375) },
			{ real(0.125), real (0.4375) },
			{ real(0.1875), real (0.4375) },
			{ real(0.25), real (0.4375) },
			{ real(0.3125), real (0.4375) },
			{ real(0.375), real (0.4375) },
			{ real(0.4375), real (0.4375) },
			{ real(0.5), real (0.4375) },
			{ real(0.5625), real (0.4375) },
			{ real(0.625), real (0.4375) },
			{ real(0.6875), real (0.4375) },
			{ real(0.75), real (0.4375) },
			{ real(0.8125), real (0.4375) },
			{ real(0.875), real (0.4375) },
			{ real(0.9375), real (0.4375) },

			{ real(0.0), real (0.5) },
			{ real(0.0625), real (0.5) },
			{ real(0.125), real (0.5) },
			{ real(0.1875), real (0.5) },
			{ real(0.25), real (0.5) },
			{ real(0.3125), real (0.5) },
			{ real(0.375), real (0.5) },
			{ real(0.4375), real (0.5) },
			{ real(0.5), real (0.5) },
			{ real(0.5625), real (0.5) },
			{ real(0.625), real (0.5) },
			{ real(0.6875), real (0.5) },
			{ real(0.75), real (0.5) },
			{ real(0.8125), real (0.5) },
			{ real(0.875), real (0.5) },
			{ real(0.9375), real (0.5) },

			{ real(0.0), real (0.5625) },
			{ real(0.0625), real (0.5625) },
			{ real(0.125), real (0.5625) },
			{ real(0.1875), real (0.5625) },
			{ real(0.25), real (0.5625) },
			{ real(0.3125), real (0.5625) },
			{ real(0.375), real (0.5625) },
			{ real(0.4375), real (0.5625) },
			{ real(0.5), real (0.5625) },
			{ real(0.5625), real (0.5625) },
			{ real(0.625), real (0.5625) },
			{ real(0.6875), real (0.5625) },
			{ real(0.75), real (0.5625) },
			{ real(0.8125), real (0.5625) },
			{ real(0.875), real (0.5625) },
			{ real(0.9375), real (0.5625) },

			{ real(0.0), real (0.625) },
			{ real(0.0625), real (0.625) },
			{ real(0.125), real (0.625) },
			{ real(0.1875), real (0.625) },
			{ real(0.25), real (0.625) },
			{ real(0.3125), real (0.625) },
			{ real(0.375), real (0.625) },
			{ real(0.4375), real (0.625) },
			{ real(0.5), real (0.625) },
			{ real(0.5625), real (0.625) },
			{ real(0.625), real (0.625) },
			{ real(0.6875), real (0.625) },
			{ real(0.75), real (0.625) },
			{ real(0.8125), real (0.625) },
			{ real(0.875), real (0.625) },
			{ real(0.9375), real (0.625) },

			{ real(0.0), real (0.6875) },
			{ real(0.0625), real (0.6875) },
			{ real(0.125), real (0.6875) },
			{ real(0.1875), real (0.6875) },
			{ real(0.25), real (0.6875) },
			{ real(0.3125), real (0.6875) },
			{ real(0.375), real (0.6875) },
			{ real(0.4375), real (0.6875) },
			{ real(0.5), real (0.6875) },
			{ real(0.5625), real (0.6875) },
			{ real(0.625), real (0.6875) },
			{ real(0.6875), real (0.6875) },
			{ real(0.75), real (0.6875) },
			{ real(0.8125), real (0.6875) },
			{ real(0.875), real (0.6875) },
			{ real(0.9375), real (0.6875) },

			{ real(0.0), real (0.75) },
			{ real(0.0625), real (0.75) },
			{ real(0.125), real (0.75) },
			{ real(0.1875), real (0.75) },
			{ real(0.25), real (0.75) },
			{ real(0.3125), real (0.75) },
			{ real(0.375), real (0.75) },
			{ real(0.4375), real (0.75) },
			{ real(0.5), real (0.75) },
			{ real(0.5625), real (0.75) },
			{ real(0.625), real (0.75) },
			{ real(0.6875), real (0.75) },
			{ real(0.75), real (0.75) },
			{ real(0.8125), real (0.75) },
			{ real(0.875), real (0.75) },
			{ real(0.9375), real (0.75) },

			{ real(0.0), real (0.8125) },
			{ real(0.0625), real (0.8125) },
			{ real(0.125), real (0.8125) },
			{ real(0.1875), real (0.8125) },
			{ real(0.25), real (0.8125) },
			{ real(0.3125), real (0.8125) },
			{ real(0.375), real (0.8125) },
			{ real(0.4375), real (0.8125) },
			{ real(0.5), real (0.8125) },
			{ real(0.5625), real (0.8125) },
			{ real(0.625), real (0.8125) },
			{ real(0.6875), real (0.8125) },
			{ real(0.75), real (0.8125) },
			{ real(0.8125), real (0.8125) },
			{ real(0.875), real (0.8125) },
			{ real(0.9375), real (0.8125) },

			{ real(0.0), real (0.875) },
			{ real(0.0625), real (0.875) },
			{ real(0.125), real (0.875) },
			{ real(0.1875), real (0.875) },
			{ real(0.25), real (0.875) },
			{ real(0.3125), real (0.875) },
			{ real(0.375), real (0.875) },
			{ real(0.4375), real (0.875) },
			{ real(0.5), real (0.875) },
			{ real(0.5625), real (0.875) },
			{ real(0.625), real (0.875) },
			{ real(0.6875), real (0.875) },
			{ real(0.75), real (0.875) },
			{ real(0.8125), real (0.875) },
			{ real(0.875), real (0.875) },
			{ real(0.9375), real (0.875) },

			{ real(0.0), real (0.9375) },
			{ real(0.0625), real (0.9375) },
			{ real(0.125), real (0.9375) },
			{ real(0.1875), real (0.9375) },
			{ real(0.25), real (0.9375) },
			{ real(0.3125), real (0.9375) },
			{ real(0.375), real (0.9375) },
			{ real(0.4375), real (0.9375) },
			{ real(0.5), real (0.9375) },
			{ real(0.5625), real (0.9375) },
			{ real(0.625), real (0.9375) },
			{ real(0.6875), real (0.9375) },
			{ real(0.75), real (0.9375) },
			{ real(0.8125), real (0.9375) },
			{ real(0.875), real (0.9375) },
			{ real(0.9375), real (0.9375) }
		};

		font::font (const id_t & id) : material (id) {}

	}
}