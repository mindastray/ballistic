#ifndef _ballistic_graphics_graphics_system_h_
#define	_ballistic_graphics_graphics_system_h_

#include <ballistic.base.h>
#include <vector>

#include "ballistic.graphics.idevice.h"
#include "ballistic.graphics.ieffect.h"
#include "ballistic.graphics.camera.h"
#include "ballistic.graphics.render_list.h"

namespace ballistic {
	namespace graphics {

		class rig;

		class graphics_system : public ballistic::isystem {
		private:

			static const uint32_t overlay_offset;

			message
				_render_message;

			idevice *	_device;
			camera	*	_camera;

			render_list _render_list;

			ieffect *	_material_effect;

			// effect constants
			iconstant
				* _c_effect_diffuse,
				* _c_effect_texture,
				* _c_effect_t_model,
				* _c_effect_t_view,
				* _c_effect_t_proj,
				* _c_effect_t_normal,
				* _c_effect_t_mvp,
				* _c_effect_bone_count,
				* _c_effect_t_bones;

			ieffect *	_overlay_effect;

			// overlay constants
			iconstant
				* _c_overlay_diffuse,
				* _c_overlay_texture,
				* _c_overlay_t_model;

			bool _can_render;

			void evaluate_render ();

			void report_render_fail ();

		public:

			graphics_system ();

			virtual id_t id ();

			void device (idevice * dev);
			idevice * device () const;

			void camera (ballistic::graphics::camera * cam);
			const ballistic::graphics::camera * camera () const ;

			void material_effect (ieffect * effect);
			ieffect * material_effect () const;

			void overlay_effect (ieffect * effect);
			ieffect * overlay_effect () const;

			void render();

			virtual void notify ( entity * sender, ballistic::message & message );

			virtual void attach ();
			virtual void detach ();

			// methods
			virtual void push_item (material * material, imesh * mesh, rig_frame_tween * rig, uint8_t layer, const mat4 & transform);
			virtual void push_overlay_item (material * material, imesh * mesh, uint8_t layer, const mat4 & transform);

		};
				  
	}
}

#endif