#ifndef	_ballistic_mac_desktop_frontend_h_
#define _ballistic_mac_desktop_frontend_h_

#include <ballistic.base.h>

#include <OpenGL/GL.h>

namespace ballistic {
	namespace mac_desktop {

		class frontend : public ballistic::ifrontend {
		private:
			point _window_client_size;
		public:

			virtual point get_client_size ();

			frontend (const point & client_size);
			virtual ~frontend ();

			virtual bool create ();

			virtual void show ();
			virtual void destroy ();
			
			virtual void update ( ballistic::game * game );

		};

	}
}

#endif