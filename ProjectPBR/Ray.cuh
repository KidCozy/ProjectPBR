#ifndef RAYH
#define RAYH

#include<D3D11.h>
#include<DirectXMath.h>

using namespace DirectX;

	#ifdef __cplusplus
	extern "C" {
	#endif

	class Ray
	{
	private:
		XMFLOAT3 From = XMFLOAT3();
		XMFLOAT3 At = XMFLOAT3();

	public:
		Ray() {}
		_inline Ray(const XMVECTOR& A, const XMVECTOR& B) { XMStoreFloat3(&From, A); XMStoreFloat3(&At, B); }
		~Ray() {}

		_inline XMVECTOR Origin() { return XMLoadFloat3(&From); }
		_inline XMVECTOR Direction() { return XMLoadFloat3(&At); }
		_inline XMVECTOR PointAtParameter(float S) const { 

			XMVECTOR F = XMLoadFloat3(&From);
			XMVECTOR A = XMLoadFloat3(&At);

			return F + S * A;
		}
	
	};
	#ifdef __cplusplus
	}
	#endif

#endif