
#ifndef _ballistic_icomponent_h_
#define _ballistic_icomponent_h_

#include "ballistic.message.h"
#include "ballistic.component_factory.h"

namespace ballistic {

	class entity;
	
	class icomponent {
	public:

		virtual entity & get_entity () const = 0;
		virtual void set_entity ( entity * ent ) = 0;

		virtual ~icomponent ();
			
		virtual void setup () = 0;
			
		virtual void notify ( ballistic::message & message ) = 0;
			
	};
	
	// abstract implementation
	class component : public icomponent {
	private:
		entity * _entity;
	public:
		
		typedef void (*notify_callback)( entity & this_entity, ballistic::message & );
		
		entity & get_entity () const;
		virtual void set_entity ( entity * ent );
		
		component ();
		
		virtual void setup ();
		
		template < class component_t >
		static void define ( const string & name );
		
		template < component::notify_callback callback >
		static void define ( const string & name );
		
	};

	template < class component_t >
	void component::define ( const string & name ) {
		component_factory::define < component_t > (name);
	}

	template < void (*message_handle)( entity & this_entity, ballistic::message & ) >
	class _func_component : public component {
	public:

		virtual inline void notify ( ballistic::message & message );

	};

	template < void (*message_handle)( entity & this_entity, ballistic::message & ) >
	void _func_component < message_handle >::notify ( ballistic::message & message ) {
		message_handle (get_entity (), message);
	}

	template < component::notify_callback callback >
	void component::define ( const string & name ) {
		component_factory::define < _func_component <  callback > > (name);
	}
	
}

#endif