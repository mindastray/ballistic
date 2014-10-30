#ifndef	_ballistic_game_h_
#define _ballistic_game_h_

#include "ballistic.containers.entity_container.h"
#include "ballistic.containers.system_container.h"
#include "ballistic.entity.h"
#include "ballistic.message_notifier.h"
#include "ballistic.resource_container.h"
#include "ballistic.system.h"

#include <functional>
#include <map>
#include <queue>
#include <string>

using namespace std;

namespace ballistic {
	
	class game : public entity {
	protected:

		bool _running;

		// game state ---
		system::ballistic_time_t
				_game_start_time,
				_frame_start;

		uint32_t	_frame_id;

		message _m_update;

		property < real >
			* _m_update_game_time,
			* _m_update_frame_time;

		property < uint32_t > 
			* _m_update_frame_count;

	public:

		inline real game_time () const { return *_m_update_game_time; }
		inline real frame_time () const { return *_m_update_frame_time; }
		inline uint32_t frame_count () const { return *_m_update_frame_count; }

		message_notifier global_notifier;

		ballistic::containers::entity_container entities;
		ballistic::containers::system_container systems;

		resource_container resources;

		virtual void initialize ();

		virtual bool is_running ();

		virtual bool frame ();

		virtual void terminate ();

		game ();
		virtual ~game ();

	};

}

#endif