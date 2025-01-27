#include "ballistic.entity.h"
#include "ballistic.entity_type.h"
#include "ballistic.game.h"

namespace ballistic {
		
	id_t entity::id () { return _id; }

	entity_type * entity::type () { return _type; }
		
	entity::entity (id_t id_v, entity_type * type_v ) : 
		_id (id_v),
		_type (type_v),
		local_notifier (this),
		properties (this),
		components (this)
	{}

	void entity::kill () {
		game::instance.entities.kill (this);
	}

}