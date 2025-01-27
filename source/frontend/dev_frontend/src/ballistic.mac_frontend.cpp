#include "ballistic.mac_frontend.h"

#ifdef BALLISTIC_OS_DARWIN

#include <GLUT/GLUT.h>

namespace ballistic {
	namespace mac_desktop {
		
		void frontend::update_frame(){
			game::instance.frame ();
			glutSwapBuffers();
			glutPostRedisplay ();
		}

		point frontend::get_client_size () { return _window_client_size; }

		frontend::frontend (const point & client_size ) : _window_client_size (client_size) {
		}

		frontend::~frontend () {}

		bool frontend::create () {
			int			dummy_c = 0;
			char *		dummy_v;
			
			glutInit ( &dummy_c, &dummy_v );
			glutInitDisplayMode  (GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_3_2_CORE_PROFILE );
			glutInitWindowSize(_window_client_size.x, _window_client_size.y);
			return true;
		}

		void frontend::show () {
			glutCreateWindow("Ballistic Mac Frontend!");
		}

		void frontend::destroy () {
		}
		
		void frontend::update () {
			
		}

		void frontend::do_event_loop () {
			glutDisplayFunc(update_frame);
			glutMainLoop ();
		}

	}
}
#endif