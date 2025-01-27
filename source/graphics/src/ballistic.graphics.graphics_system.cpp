#include "ballistic.graphics.graphics_system.h"
#include "ballistic.graphics.common_id.h"

#include "ballistic.graphics.ieffect.h"
#include "ballistic.graphics.imesh.h"
#include "ballistic.graphics.material.h"
#include "ballistic.graphics.rig.h"

namespace ballistic {
	namespace graphics {

		const uint32_t graphics_system::overlay_offset = 0x01000000;

		void graphics_system::evaluate_render () {
			_can_render =
				_device &&
				_camera &&
				_material_effect &&
				_overlay_effect;
		}

		void graphics_system::report_render_fail () {
			if (!_device)
				debug_error ("graphics device not set! will not render");

			if (!_camera) 
				debug_error ("active camera not set! will not render");

			if (!_material_effect)
				debug_error ("no active material effect! will not render");

			if (!_overlay_effect)
				debug_error ("no active overlay effect! will not render");

		}

		id_t graphics_system::id () {
			return ballistic::id::graphics::system;
		}

		graphics_system::graphics_system () :
			_render_message (id::message::render),

			_device (nullptr),
			_camera (nullptr),
			_material_effect (nullptr),

			_c_effect_diffuse (&null_constant::instance),
			_c_effect_texture (&null_constant::instance),
			_c_effect_t_model (&null_constant::instance),
			_c_effect_t_view (&null_constant::instance),
			_c_effect_t_proj (&null_constant::instance),
			_c_effect_t_normal (&null_constant::instance),
			_c_effect_t_mvp (&null_constant::instance),
			_c_effect_bone_count (&null_constant::instance),
			_c_effect_t_bones (&null_constant::instance),

			_overlay_effect (nullptr),

			_c_overlay_diffuse (&null_constant::instance),
			_c_overlay_texture (&null_constant::instance),
			_c_overlay_t_model (&null_constant::instance),

			_can_render (false)
		{}

		void graphics_system::device ( idevice * dev ) {
			_device = dev;
			evaluate_render ();
		}

		idevice * graphics_system::device () const {
			return _device;
		}

		void graphics_system::camera (ballistic::graphics::camera * cam) {
			_camera = cam;
			evaluate_render ();
		}

		const ballistic::graphics::camera * graphics_system::camera () const {
			return _camera;
		}

		void graphics_system::material_effect (ieffect * effect) {
			_material_effect = effect;

			if (!_material_effect)
				return;

			_c_effect_diffuse = _material_effect->constant (id::graphics::effect::diffuse);
			_c_effect_texture = _material_effect->constant (id::graphics::effect::texture);
			_c_effect_t_model = _material_effect->constant (id::graphics::effect::t_model);
			_c_effect_t_view = _material_effect->constant (id::graphics::effect::t_view);
			_c_effect_t_proj = _material_effect->constant (id::graphics::effect::t_proj);
			_c_effect_t_normal = _material_effect->constant (id::graphics::effect::t_normal);
			_c_effect_t_mvp = _material_effect->constant (id::graphics::effect::t_mvp);
			_c_effect_bone_count = _material_effect->constant (id::graphics::effect::bone_count);
			_c_effect_t_bones = _material_effect->constant (id::graphics::effect::t_bones);

			evaluate_render ();
		}

		ieffect * graphics_system::material_effect () const {
			return _material_effect;
		}

		void graphics_system::overlay_effect (ieffect * effect) {
			_overlay_effect = effect;

			if (!_material_effect)
				return;

			_c_overlay_diffuse = _overlay_effect->constant (id::graphics::effect::diffuse);
			_c_overlay_texture = _overlay_effect->constant (id::graphics::effect::texture);
			_c_overlay_t_model = _overlay_effect->constant (id::graphics::effect::t_model);

			evaluate_render ();
		}

		ieffect * graphics_system::overlay_effect () const {
			return _overlay_effect;
		}

		void graphics_system::render () {
			
			if (!_can_render) {
				report_render_fail ();
				return;
			}

			mat4
				m_v,
				m_vp,
				m_mv,
				m_mvp,
				m_n;

			m_v = _camera->view ();
			m_vp = m_v * _camera->proj();

			// clear before render message to allow for 
			// debug rendering
			_device->clear ();
			_device->begin_frame ();

			// notify entities with visuals
			game::instance.global_notifier.notify(_render_message);

			// sort
			_render_list.sort ();

			_device->alpha_blend (false);

			// render loop ---------------------------
			material * material = nullptr;
			imesh * mesh = nullptr;
			itexture * texture = nullptr;

			bool alpha_blend = false;

			// activate material effect
			_device->activate (_material_effect);

			// least changing properties
			_c_effect_t_view->set_value (m_v);
			_c_effect_t_proj->set_value (_camera->proj ());
				   
			uint32_t render_count = _render_list.size ();
			uint32_t render_index = 0;

			// material loop
			while (render_index < render_count) {

				render_item & item = _render_list [render_index];
				if (item.bucket > overlay_offset)
					break;

				if (item.material != material) {
					material = item.material;
					_c_effect_diffuse->set_value (material->diffuse);
				}

				if (item.material->texture != texture) {
					texture = item.material->texture;
					_device->activate (texture);
					_c_effect_texture->set_value (0);
				}

				if (item.material->opaque != alpha_blend) {
					alpha_blend = true;
					_device->alpha_blend (true);
				}

				if (item.mesh != mesh) {
					mesh = item.mesh;
					_device->activate (mesh);
				}

				// update model / normal matrices
				m_mv = m_v * item.transform;
				m_n = m_mv.transpose ().invert ();
				m_mvp = item.transform * m_vp;

				_c_effect_t_normal->set_value (m_n);
				_c_effect_t_model->set_value (item.transform);
				_c_effect_t_mvp->set_value (m_mvp);

				_c_effect_bone_count->set_value (item.rig->bones.size());
				_c_effect_t_bones->set_value (item.rig->bones);

				// update rig
				_c_effect_bone_count->set_value (item.rig->bones.size ());
				_c_effect_t_bones->set_value (item.rig->bones);
				
				// render the stuffs
				_device->draw_active_mesh ();

				++render_index;
			}

			// overlay render setup
			material = nullptr;
			mesh = nullptr;
			texture = nullptr;

			alpha_blend = false;
			_device->alpha_blend(false);

			// activate material effect
			_device->activate (_overlay_effect);

			// overlay loop
			while (render_index < render_count) {
				render_item & item = _render_list [render_index];

				if (item.material != material) {
					material = item.material;
					_c_overlay_diffuse->set_value (material->diffuse);
				}

				if (item.material->texture != texture) {
					texture = item.material->texture;
					_device->activate (texture);
					_c_overlay_texture->set_value(0);
				}

				if (item.material->opaque != alpha_blend) {
					alpha_blend = true;
					_device->alpha_blend (true);
				}

				if (item.mesh != mesh) {
					mesh = item.mesh;
					_device->activate (mesh);
				}

				// update model
				_c_overlay_t_model->set_value (item.transform);

				// render the stuffs
				_device->draw_active_mesh ();

				++render_index;
			}

			// r-----------------------
			_render_list.clear();

			_device->end_frame ();
			_device->present ();

			_render_list.clear ();
		}

		void graphics_system::notify ( entity * sender, ballistic::message & message ) {
			render ();
		}

		void graphics_system::attach () {
			game::instance.global_notifier.attach (id::message::update, this);
		}

		void graphics_system::detach () {
			game::instance.global_notifier.detach (id::message::update, this);
		}

		void graphics_system::push_item (material * material, imesh * mesh, rig_frame_tween * rig, uint8_t layer, const mat4 & transform) {
			render_item & item = _render_list.reserve_item ();

			item.material = material;
			item.mesh = mesh;
			item.rig = rig;
			item.transform = transform;
			item.layer = layer;

			render_item::set_render_bucket (item, _camera, 0x0);
		}

		void graphics_system::push_overlay_item (material * material, imesh * mesh, uint8_t layer, const mat4 & transform) {
			render_item & item = _render_list.reserve_item ();

			item.material = material;
			item.mesh = mesh;
			item.transform = transform;
			item.layer = layer;

			// 0x01000000 - overlay offset
			render_item::set_render_bucket (item, _camera, overlay_offset);
		}

	}
}