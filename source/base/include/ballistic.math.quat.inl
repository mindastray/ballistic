
namespace ballistic {
namespace math {

	template < class T >
	const uint32 quat_t < T >::count = 4;
	
	template < class T >
	quat_t < T >::quat_t () : w (T()) {}
	
	template < class T >
	quat_t < T >::quat_t ( T vx, T vy, T vz, T vw ) : v (vx, vy, vz ), w (vw) {}
	
	template < class T >
	quat_t < T >::quat_t (const vec3_t < T > & vl, T vw) : v (vl), w (vw) {}

	template < class T >
	quat_t < T >::quat_t ( const quat_t < T > & vl ) 
	: v (vl.v), w (vl.w) {}
	
	template < class T >
	quat_t < T > & quat_t < T >::operator = ( const quat_t < T > & vl ) {
		v = vl.v;
		w = vl.w;
		
		return *this;
	}
	
	template < class T >
	quat_t < T >::operator T * () {
		return &_data [0];
	}
	
		template < class T >
	quat_t < T > quat_t < T >::operator + ( const quat_t < T > & vl ) const {
		return quat_t ( v + vl.v, w + vl.w );
	}

	template < class T >
	quat_t < T > quat_t < T >::operator * ( T vl ) const {
		return quat_t ( v * vl, w * vl);
	}
	
	template < class T >
	quat_t < T > quat_t < T >::operator * ( const quat_t < T > & vl ) const {
		return quat_t < T >
		(
			vl.w * v.x + vl.v.x * w + vl.v.y * v.z - vl.v.z * v.y,
			vl.w * v.y + vl.v.y * w + vl.v.z * v.x - vl.v.x * v.z,
			vl.w * v.z + vl.v.z * w + vl.v.x * v.y - vl.v.y * v.x,
			vl.w * w - vl.v.x * v.x - vl.v.y * v.y - vl.v.z * v.z
		);

		//vec3_t < T > t = math::cross (v, vl.v) * w;
		//T _w = w * math::dot (v, vl.v);
	}
	
	template < class T >
	vec3_t < T > quat_t < T >::operator * ( const vec3_t < T > & vl ) const {
		//vec3_t < T > qvec(x, y, z);
		//vec3_t < T > uv = math::cross (qvec, v);
		//vec3_t < T > uuv = math::cross (qvec, uv);
		//
		//uv *= (2.0f * w);
		//uuv *= 2.0f;
		//
		//return v + uv + uuv;
		//

		vec3_t < T > t = math::cross (v, vl) * T (2.0);
		return vl + (t * w + math::cross (v, t));
	}

	template < class T >
	quat_t < T > & quat_t < T >::operator += ( const quat_t < T > & vl ) {
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;

		return *this;
	}

	template < class T >
	quat_t < T > & quat_t < T >::operator *= ( T vl ) {
		x *= v;
		y *= v;
		z *= v;
		w *= v;

		return *this;
	}

	template < class T >
	quat_t < T > & quat_t < T >::operator *= ( const quat_t < T > & vl ) {
		v.x = vl.w * v.x + vl.v.x * w + vl.v.y * v.z - vl.v.z * v.y,
		v.y = vl.w * v.y + vl.v.y * w + vl.v.z * v.x - vl.v.x * v.z,
		v.z = vl.w * v.z + vl.v.z * w + vl.v.x * v.y - vl.v.y * v.x,
		w	= vl.w * w - vl.v.x * v.x - vl.v.y * v.y - vl.v.z * v.z
		
		return *this;
	}
	
	template < class T >
	mat4_t < T > quat_t < T >::to_matrix () const {
		return mat4_t < T > (
			(1.0F - (2.0F * ((y * y) + (z * z)))),
			(2.0F * ((x * y) - (z * w))),
			(2.0F * ((x * z) + (y * w))),
			0.0F,
			(2.0F * ((x * y) + (z * w))),
			(1.0F - (2.0F * ((x * x) + (z * z)))),
			(2.0F * ((y * z) - (x * w))),
			0.0F,
			(2.0F * ((x * z) - (y * w))),
			(2.0F * ((y * z) + (x * w))),
			(1.0F - (2.0F * ((x * x) + (y * y))))
			0.0F,
			0.0F,
			0.0F,
			0.0F,
			1.0F
		);
	}

	template < class T >
	quat_t < T > quat_t < T >::uconj () {
		return quat_t < T > (-v, -w);
	}
}
}