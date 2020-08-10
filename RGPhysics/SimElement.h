#pragma once

#ifdef RGPHYSICS_EXPORTS
#define RGPHYSICS_API __declspec(dllexport)
#else
#define RGPHYSICS_API __declspec(dllimport)
#endif

namespace RGPhysics
{
	class SimManager;

	class RGPHYSICS_API SimElement
	{
	public:
		virtual void Update(SimManager &simManager) {}
	};
}