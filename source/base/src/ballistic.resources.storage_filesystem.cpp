//
//  ballistic.resources.storage_filesystem.cpp
//  ballistic_graphics
//
//  Created by Luciano da Silva on 7/14/13.
//
//

#include "ballistic.resources.storage_filesystem.h"
#include "ballistic.resources.iloader.h"

#include <fstream>

namespace ballistic {
	namespace resources {
		
		storage_filesystem::~storage_filesystem() {}
		
		bool storage_filesystem::contains (const string & path) {
			//TODO: probably replace this for a more efficient alternative
			return !ifstream (path).fail();
		}
		
		iresource * storage_filesystem::load ( iloader * loader, const string & path, ballistic::resources::stack & stack  ) {
			ifstream file_stream (path, ios::in | ios::binary | ios::ate );
			return loader->load(file_stream, stack);
		}
		
	}
}