#include "ballistic.graphics.overlay_text.h"
#include "ballistic.graphics.common_id.h"
#include "ballistic.graphics.graphics_system.h"

#include "ballistic.graphics.font.h"
#include "ballistic.graphics.material.h"
#include "ballistic.graphics.imesh.h"

namespace ballistic {
	namespace graphics {

		void overlay_text::generate_mesh() {

			std::string text_value = *_overlay_text;

			font * font_inst = *_overlay_font;

			int char_count = text_value.size();

			real * vertex_data = new real[
				20 * char_count // x y z u v * 4
			];

			uint16_t * index_data = new uint16_t[
				6 * char_count
			];

			uint32_t	index = 0;
			uint32_t	index_index = 0;
			uint32_t	vertex_index = 0;
			vec2		pos = { real(0), real(0) };

			const real	ratio = 0.0625;

			for (char c : text_value) {
				vec2 uv_pos = font::font_uv[c];
				vec2 uv_size = font_inst->font_metrics[c];
				real xwidth = uv_size.x / ratio;

				// 0
				vertex_data[index + 0] = pos.x;
				vertex_data[index + 1] = pos.y;
				vertex_data[index + 2] = real(0);
				vertex_data[index + 3] = uv_pos.x;
				vertex_data[index + 4] = uv_pos.y + uv_size.y;
				// 1
				vertex_data[index + 5] = pos.x + xwidth;
				vertex_data[index + 6] = pos.y;
				vertex_data[index + 7] = real(0);
				vertex_data[index + 8] = uv_pos.x + uv_size.x;
				vertex_data[index + 9] = uv_pos.y + uv_size.y;
				// 2
				vertex_data[index + 10] = pos.x + xwidth;
				vertex_data[index + 11] = pos.y + real (1);
				vertex_data[index + 12] = real(0);
				vertex_data[index + 13] = uv_pos.x + uv_size.x;
				vertex_data[index + 14] = uv_pos.y;
				// 3
				vertex_data[index + 15] = pos.x;
				vertex_data[index + 16] = pos.y + real (1);
				vertex_data[index + 17] = real(0);
				vertex_data[index + 18] = uv_pos.x;
				vertex_data[index + 19] = uv_pos.y;

				index_data[index_index + 0] = vertex_index + 0;
				index_data[index_index + 1] = vertex_index + 1;
				index_data[index_index + 2] = vertex_index + 2;
				index_data[index_index + 3] = vertex_index + 0;
				index_data[index_index + 4] = vertex_index + 2;
				index_data[index_index + 5] = vertex_index + 3;

				index += 20;
				index_index += 6;
				vertex_index += 4;

				pos.x += xwidth;
			}

			_mesh->set_data(
				(uint8_t *)vertex_data,
				20 * char_count * sizeof(real),
				index_data,
				6 * char_count * sizeof(uint16_t),
				(mesh_attribute)(mesh_attribute_position | mesh_attribute_uv),
				false
			);
			
			delete [] vertex_data;
			delete [] index_data;

		}

		void overlay_text::require_properties (entity_type * new_type, component_info & info) {
			new_type->properties.require < id_t >(id::graphics::text::font_id, id::null);
			new_type->properties.require < font * >(id::graphics::text::font, nullptr);

			new_type->properties.require < uint8_t >(id::graphics::layer, 0);

			new_type->properties.require < mat4 >(id::transform, mat4());

			new_type->properties.require_notify < std::string > (id::graphics::text::text, "");
		}

		const id_t overlay_text::component_id = ballistic::id::graphics::overlay_text;

		overlay_text::overlay_text ()
			:
			overlay (),
			_overlay_text (nullptr)
		{}

		void overlay_text::setup (entity * parent, ballistic::containers::property_container & parameters) {
			component::setup(parent, parameters);

			game::instance.global_notifier.attach(id::message::render, this);

			_system = dynamic_cast <graphics_system *> (game::instance.systems[ballistic::id::graphics::system]);

			_overlay_font = parent->properties.aquire < font * >(id::graphics::text::font);
			_transform = parent->properties.aquire < mat4 >(id::transform);

			if (!*_overlay_font)
				*_overlay_font = game::instance.resources[
					parent->properties[id::graphics::text::font_id].as < id_t >()
				].as < font >();

			_layer = parent->properties.aquire < uint8_t >(id::graphics::layer);

			_overlay_text = parent->properties.aquire < std::string > (id::graphics::text::text);

			parent->local_notifier.attach (id::message::property_changed, this);

			_mesh = _system->device()->create_mesh(id::null);

			generate_mesh ();
		}

		overlay_text::~overlay_text() {
			delete _mesh;
		}

		void overlay_text::terminate () {
			overlay::terminate ();
			this->parent ()->local_notifier.detach (id::message::render, this);
		}

		void overlay_text::notify (entity * sender, ballistic::message & message) {
			if (message.id () == id::message::property_changed) {
				generate_mesh ();
				return;
			}

			font * font_inst = *_overlay_font;

			if (!font_inst) {
				debug_print ("missing font instance. will not render");
				return;
			}

			if (
				_system &&
				font_inst &&
				_mesh
			){
				_system->push_overlay_item(font_inst, _mesh, *_layer, *_transform);
			} else {
				debug_print ("incomplete overlay text component. will not render!");
			}
		}

	}
}