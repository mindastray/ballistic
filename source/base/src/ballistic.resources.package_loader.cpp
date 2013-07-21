//
//  ballistic.resources.entity_loader.cpp
//  ballistic_graphics
//
//  Created by Luciano da Silva on 7/18/13.
//
//

#include "ballistic.resources.package_loader.h"
#include "ballistic.resources.entity_info.h"

#include "ballistic.config.h"
#include "ballistic.attribute.h"

#include "ballistic.math.h"

#include "ballistic.resources.istorage.h"
#include "ballistic.resources.component_constructor.h"

#include <tinyxml2.h>

using namespace tinyxml2;

namespace ballistic {
	namespace resources {
		
		package_loader::itype::~itype() {}
		
		class entity_type : public package_loader::itype {
		public:
			
			virtual string get_name () {
				return "entity";
			}
			
			void load_attributes (
								  tinyxml2::XMLElement * element,
								  ballistic::resources::stack & stack,
								  vector < attribute > & attribute_vector
								  ){
				tinyxml2::XMLElement * cursor = element->FirstChildElement ();
				
				while (cursor) {
					attribute_vector.push_back (attribute (string_to_id (cursor->Name ())));
					attribute & at = attribute_vector.back ();
					
					string type = cursor->Attribute ("type");
					string value = cursor->Attribute ("value");
					
					// Convert XML attribute types
					if (type == "int") {
						at = convert_to < int, string > (value);
					} else if (type == "uint") {
						at = convert_to < unsigned int, string > (value);
					} else if (type == "real") {
						at = convert_to < real, string > (value);
					} else if (type == "res") {
						at = stack.get_resource (value);
					} else if (type == "vec3") {
						vec3 v;
						
						stringstream ss (value);
						string trim;
						
						getline (ss, trim, ',');
						v.x = convert_to < real, string > (trim);
						getline (ss, trim, ',');
						v.y = convert_to < real, string > (trim);
						getline (ss, trim, ',');
						v.z = convert_to < real, string > (trim);
						
						at = v;
					} else if (type == "color" || type == "vec4") {
						vec4 v;
						
						stringstream ss (value);
						string trim;
						
						getline (ss, trim, ',');
						v.x = convert_to < real, string > (trim);
						getline (ss, trim, ',');
						v.y = convert_to < real, string > (trim);
						getline (ss, trim, ',');
						v.z = convert_to < real, string > (trim);
						getline (ss, trim, ',');
						v.w = convert_to < real, string > (trim);
						
						at = v;
					} else {
						//TODO: handle properly
						at = value; //as string
					}
					
					cursor = cursor->NextSiblingElement ();
				}
			}
			
			void load_components (
					tinyxml2::XMLElement * element,
					ballistic::resources::stack & stack,
					vector < ballistic::resources::component_info > & component_vector
			) {
				tinyxml2::XMLElement * cursor = element->FirstChildElement();
				
				while (cursor) {
					
					component_vector.push_back(component_info ());
					
					component_info & comp_info = component_vector.back ();
					
					auto ctor = stack.get_resource <icomponent_constructor > (string_to_id(cursor->Name()));
					
					comp_info.set_constructor(ctor);
					
					load_attributes(cursor, stack, comp_info.get_attributes());
					
					
					cursor->NextSiblingElement();
				}
			}
			
			virtual void load_element (tinyxml2::XMLElement * element, ballistic::resources::stack & stack )
			{
				string name = element->Attribute ("name");
				
				entity_info * ctor = new entity_info ();
				
				tinyxml2::XMLElement * cursor = element->FirstChildElement ();
				while (cursor) {
					if (strcmp (cursor->Name (), "attributes") == 0)
						load_attributes (cursor, stack, ctor->get_attributes ());
					
					if (strcmp (cursor->Name (), "components") == 0)
						load_components (cursor, stack, ctor->get_components());
					
					cursor = cursor->NextSiblingElement ();
				}
				
				stack.add_to_level (name, ctor);
			}
			
		};
		
		class package_type : public package_loader::itype {
		public:
			
			virtual string get_name () {
				return "include";
			}
			
			virtual void load_element (
							   tinyxml2::XMLElement * element,
							   ballistic::resources::stack & stack
							   ){
				string source = element->Attribute("source");
				istorage * storage = stack.find_storage(source);
				
				if (storage) {
					package_loader loader;
					storage->load (&loader, source, stack);
				}
			}
		};
		
		bool package_loader::handles (const string & name) {
			if (name.size () < 4)
				return false;
			
			return name.compare (name.size () - 4, 4, ".xml") == 0;
		}
		
		package_loader::package_loader () {
			register_type (new package_type ());
			register_type (new entity_type ());
		}
		
		void package_loader::register_type(ballistic::resources::package_loader::itype *type_inst)
		{
			_registered_types [type_inst->get_name ()] = type_inst;
		}
		
		bool package_loader::load (istream & source, uint32 length, ballistic::resources::stack & stack) {
			
			XMLDocument document;
			
			//TODO: handle these errors properly
			if (document.LoadStream(source, length))
				return false;
			
			auto root = document.FirstChildElement("package");
			
			if (!root)
				return false;
			
			XMLElement * cursor = root->FirstChildElement();
			while (cursor) {
				type_map_t::iterator it = _registered_types.find (cursor->Name());
				
				if (it != _registered_types.end ())
					it->second->load_element(cursor, stack);
				
				cursor = cursor->NextSiblingElement();
			}
			
			return true;
		}
		
	}
}