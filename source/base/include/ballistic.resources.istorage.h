//
//  ballistic.resources.istorage.h
//  ballistic_graphics
//
//  Created by Luciano da Silva on 7/14/13.
//
//

#ifndef _ballistic_resources_istorage_h_
#define _ballistic_resources_istorage_h_

#include <iostream>
#include <string>

#include "ballistic.resources.iloader.h"
#include "ballistic.resources.iresource.h"

using namespace std;

namespace ballistic {
	namespace resources {
		
		class istorage {
		public:
	
			virtual ~istorage () = 0;
	
			virtual bool contains ( const string & path ) = 0;
			virtual iresource * load ( iloader * loader, const string & path ) = 0;
	
		};

	}
}

#endif
